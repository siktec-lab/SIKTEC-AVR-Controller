/******************************************************************************/
// Created by: Shlomo Hassid.
// Release Version : 1.0.3
// Creation Date: 2021-04-27
// Copyright 2021, SIKTEC.
/******************************************************************************/
/*****************************      Changelog       ****************************
version:
    1.0.3
        -> Handles and show messages when SD card is not detected or no images found.
        -> 
    1.0.2
        -> initial release - Example to showcase siktec Arduino Controller board.
        -> Based on Adafruit GFX + ImageReader.
        -> Populates a menu on the attached TFT 1.8' 
        -> Controll by the Rotary encoder Using SIKtec::Rotary lib
        -> Scans A SD card for BMP images and show them on the menu.
        -> Draws selected images on the TFT.
        -> Encoder SW draws an image -> another push clears the screen. 
*******************************************************************************/
/***********************  Assets / Learning More  *****************************
  1. A good tool for picking colors: http://www.barth-dev.de/online/rgb565-color-picker/
  2. For textSize in depth reffer to: https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives#characters-and-text-2002798-24
  3. For fonts and sizes reffer to: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
  4. CHANGE interrupts Overview: https://github.com/EnviroDIY/Arduino-SDI-12/wiki/2b.-Overview-of-Interrupts
*******************************************************************************/
/***************************   Dependencies   *********************************
 * Adafruit_GFX
 * Adafruit_ST7735
 * Adafruit_ImageReader
 * SIKTEC_Rotary
 * SIKTEC_SDExplorer
*******************************************************************************/
/******************************************************************************/
/**** REQUIRED / LIBRARIES / INCLUDES *****************************************/
/******************************************************************************/
#include <Arduino.h>
#include <SPI.h>
// Used by this example code for graphics drawing - also required by ImageReader.
#include <Adafruit_GFX.h>   
// Hardware-specific library To interface with the TFT         
#include <Adafruit_ST7735.h>        
// Used by this Example for reading and drawing a BMP file without exhausting memory
#include <Adafruit_ImageReader.h>   
// SIKtec::Rotary - a generic lib to easily interface with a rotary encoder - using interrupts.
#include <SIKTEC_Rotary.h> 
// Image in progmem to draw on TFT
#include "sik_logo.h"    
// SIKtec::SdExplorer - a Wrapper for doing some operations on the SD card.
#include "siksd.h"     
 

/***************************************************************************/
/**** BASE DEFINITIONS *****************************************************/
/***************************************************************************/

//Output To Serial Also:
#define CONT_OUTPUT_SERIAL true

//Define Attached pins:
#define USE_SD_CARD                     // Defined to indicate GFX we are using data from SD card
#define CONT_ROTARY_ENCODER_A   2       //PD2, Arduino PIN 2  -> Interrupt pin
#define CONT_ROTARY_ENCODER_B   3       //PD3, PIN 3  -> Interrupt pin
#define CONT_ROTARY_ENCODER_SW  4       //PD4, Arduino PIN 4  -> Digital pin for switch button
#define CONT_BUT_0              PIN_A0  // Arduino PIN 14 -> Analog 0  --- NOT USED IN THIS EXAMPLE --- 
#define CONT_BUT_1              PIN_A1  // Arduino PIN 15 -> Analog 1  --- NOT USED IN THIS EXAMPLE --- 
#define CONT_TFT_RST            PIN_A2  // Arduino PIN 16 -> Analog 2
#define CONT_TFT_CS             10      //PB2, Arduino PIN 10 -> SS / Chip Select
#define CONT_TFT_DC             8       //PB0, Arduino PIN 8  -> Data Command control pin
#define CONT_SD_CS              9       //PB1, Arduino PIN 9  -> SD card Chip select
#define CONT_LED_PIN            5       //PD5, Arduino PIN 5  -> Attached to Shield onboard LED --- NOT USED IN THIS EXAMPLE --- 

//SD Card definitions:
#define CONT_SD_LIMIT_MHZ 50 //50 MHz limit the SPI interface - use at 10 or 50

//TFT Size:
#define CONT_TFT_WIDTH  160 //pixels
#define CONT_TFT_HEIGHT 128 //pixels

//Menu Definition:
#define CONT_NUM_MENU_ROWS      3       // always odd number smallest is 3;
#define CONT_MAX_CHR_MENU_ROW   24      // Absolute max is 24 for 1.8' tft with base font at size 1;
enum TftScreens { Main };               // Defined static screens

//Colors Definition:
#define CONT_BASE_BACKGROUND            ST7735_BLACK    // Base black color 0x0000
#define CONT_HEADER_LOGO_COLOR          0xF640          // SIKTEC orange
#define CONT_HEADER_TEXT_COLOR          ST7735_WHITE    // Base white color
#define CONT_MENU_ROW_BG                0x2945          // Dark grey
#define CONT_MENU_TEXT_COLOR            0x7BCF          // Light gray
#define CONT_MENU_ROW_SELECTED_BG       0x4228          // Light gray
#define CONT_MENU_SELECTED_TEXT_COLOR   0xFE68          // Light orange

/***************************************************************************/
/**** INTIATE OBJECTS / LIBRARIES / GLOBAL VARIABLES ***********************/
/***************************************************************************/

SIKtec::RotaryState pointTo;                                    // Rotary Class instance.
SIKtec::SdExplorer SikSd(CONT_OUTPUT_SERIAL);                   // True / False enables serial output for debugging.
Adafruit_ImageReader imageReader(SIKtec::SdExplorer::SD);       //ImageReader object, pass in SD filesys
Adafruit_ST7735 tft(CONT_TFT_CS, CONT_TFT_DC, CONT_TFT_RST);    // The TFT class - takes pin configuration defined in the declare_example.h file.

// A char buffer variable to later store a full file name to load and draw:
char imageToLoadNameBuff[SIKSD_MAX_FILES_NAME];

// A 2d string buffer to hold the short name of files and print on the screen:
char menuFiles[CONT_NUM_MENU_ROWS][CONT_MAX_CHR_MENU_ROW] = {" ","No Images Found!"," "};

// A simple structure to hold and use Target image dimensions.
struct ImageSize { 
    int32_t width = 0; 
    int32_t height = 0; 
} imageToLoadSize;  

// Holds the current image selected from the menu - This index is altered by the interrupt.
// ROTARY_COUNTER_TYPE is defined by the Rotary Lib -> to override it define it before the include in a header file
ROTARY_COUNTER_TYPE loadImage = -1;

// Simple flag to force a redraw of the static screen parts (checked in the loop):
bool loadMainScreen;

//Intiate the Rotary encoder helper class:
SIKtec::Rotary rotary = SIKtec::Rotary(
    CONT_ROTARY_ENCODER_A, 
    CONT_ROTARY_ENCODER_B, 
    CONT_ROTARY_ENCODER_SW,
    false // Circular mode? -> infinite loop counting.
);

/***************************************************************************/
/**** Functions ************************************************************/
/***************************************************************************/

/*! initTFT
    @brief  Tft initialization:
    @param  bg          RGB565 - color of the background
    @param  rotation    The screen rotation in degrees 0 - 360
    @note bg color - see Asset 1.
*/
bool initTFT(const uint16_t bg = 0x0000, const uint8_t rotation = 0) {
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(bg);
    tft.setRotation(rotation);
    delay(500);
    return true;
}

/*! ISR `Rotary push Button`
    @brief  ISR to handle rotary CHANGE interupt of the button switch
    @note   The CHANEG interrupt is set in the setup function
    @note   The CHANEG interrupt is set in the setup function
*/
ISR (PCINT2_vect) {
    if ((millis() - rotary.lastDebounceTime) > SIKtec::Rotary::debounceDelay && digitalRead(CONT_ROTARY_ENCODER_SW) == LOW) {
        cli(); // Disable interrupts
        if (CONT_OUTPUT_SERIAL) Serial.println(F("Rotary Push"));
        rotary.lastDebounceTime = millis(); // We update the time for denouncing software side.
        //Set load image flag:
        if (loadImage == -1) { // Indicates we are in menu mode
            //get current state of the encoder:
            SIKtec::RotaryState currnet = rotary.readState();
            //Disable the encoder - We don't want to do anything until next rotary push:
            rotary.disable();
            loadImage = currnet.pos;
            //Indicates a change has occurred - will be caught in the loop:
            rotary.changed = true;
        } else { // Indicates we are in image loaded mode
            //Enable the encoder - We are now exiting the Image draw mode:
            rotary.enable();
            loadImage = -1;
            loadMainScreen = true; // Will set rotary changed after draw in the main loop.
        }
        sei(); // Enable interrupts
    }
}

/*! customCallback
    @brief  Example of a user based callback attached to the rotary
    @param  state the current state of the rotary encoder 
*/
void customCallback(SIKtec::RotaryState state) {
    if (CONT_OUTPUT_SERIAL) {
        Serial.print(F(" Rotary State -> Delta: "));
        Serial.print(state.delta);
        Serial.print(F(" Previous: "));
        Serial.print(state.prev);
        Serial.print(F(" Current: "));
        Serial.println(state.pos);
    }
}

/*! printScreens
    @brief  A handy function to render all predefined screen static output
    @param  _tft The TFT Driver passed by refernce.
    @param  screen an enum value which indiacate which screen to print 
*/
bool printScreens(Adafruit_ST7735& _tft, const enum TftScreens screen) {
  switch (screen) {
    case TftScreens::Main:
      _tft.fillScreen(CONT_BASE_BACKGROUND);
      _tft.drawBitmap(5,5, bitmap_logo, 20, 33, CONT_HEADER_LOGO_COLOR); // Draw a bitmap from memory
      _tft.setCursor(30, 30);
      _tft.setTextColor(CONT_HEADER_TEXT_COLOR);
      _tft.setTextSize(1);
      _tft.println("Image Reader Example");
      rotary.changed = true;
    break;
    default: return false;
  }
  return true;
}

/*! printMenu
    @brief  Prints the menu containing the current file names.
    @param  _tft        The TFT Driver passed by refernce.
    @param  cur         The current selected file index -> from rotatry encoder.
    @param  colX        The staring position X
    @param  rowY        The staring position Y
    @param  padX        Pads the row, in Pixels X.
    @param  padY        Pads the row, in Pixels Y.
    @param  textSize    The text size multiplier.
    @note for the 1.8' keep the text size small (1) it is too small to handle file names in a larger format
    @note textSize in depth - see Asset 2
    @note Font sizes - see Asset 3
*/
void printMenu(
    Adafruit_ST7735& _tft, 
    const ROTARY_COUNTER_TYPE cur, 
    const uint8_t colX, 
    uint8_t rowY, 
    const uint8_t padX = 4, 
    const uint8_t padY = 9, 
    const uint8_t textSize = 1
) {
    uint8_t baseCharWidth = 6 * textSize,  // Base Adafruit character width is 6
            baseCharHeight = 8 * textSize; // Base Adafruit character height is 8
    uint8_t maxStringLength = (uint8_t)((CONT_TFT_WIDTH - (padX * 2)) / baseCharWidth - 1); // After padding - We calculate this to avoid line breaks
    uint8_t menuRowsShift = (CONT_NUM_MENU_ROWS - 1) / 2; // Calculates how many rows are before and after the selected one.
    uint8_t rowHeight = padY * 2 + baseCharHeight; // The Row calculated height.
    //Loads files names into the menu buffer:
    SikSd.getFilesAroundCurrent(cur, (char*)menuFiles, CONT_MAX_CHR_MENU_ROW, maxStringLength, menuRowsShift);
    _tft.setTextSize(textSize);
    for (uint8_t i = 0; i < CONT_NUM_MENU_ROWS; i++) {
        if (i == menuRowsShift) { 
            // Selected row - set color schemes: 
            _tft.setTextColor(CONT_MENU_SELECTED_TEXT_COLOR);
            _tft.fillRect(colX, rowY, CONT_TFT_WIDTH, rowHeight, CONT_MENU_ROW_SELECTED_BG);
        }
        else {
            // Prev or After row - set color schemes: 
            tft.setTextColor(CONT_MENU_TEXT_COLOR);
            _tft.fillRect(colX, rowY, CONT_TFT_WIDTH, rowHeight, CONT_MENU_ROW_BG);
        }
        // Print the file name padded: 
        _tft.setCursor(colX + padX, rowY + padY);
        _tft.println(menuFiles[i]);
        rowY += rowHeight;
    }
}

/*! drawSelectedImage
    @brief  draws a CMP image on the TFT image is selected by its index.
    @param  _tft        The TFT Driver passed by refernce.
    @param  loadImage   The current selected file index -> from rotatry encoder.
*/
void drawSelectedImage(Adafruit_ST7735& _tft, const ROTARY_COUNTER_TYPE loadImage) {
    int8_t padx, pady;
    ImageReturnCode stat;
    SikSd.getFullFileNameByIndex(imageToLoadNameBuff, loadImage);
    stat = imageReader.bmpDimensions(imageToLoadNameBuff, &imageToLoadSize.width, &imageToLoadSize.height);
    if (stat == IMAGE_SUCCESS) {
        padx = CONT_TFT_WIDTH > imageToLoadSize.width ? (CONT_TFT_WIDTH - imageToLoadSize.width)  / 2: 0;
        pady = CONT_TFT_HEIGHT > imageToLoadSize.height ? (CONT_TFT_HEIGHT - imageToLoadSize.height) / 2: 0;
        stat = imageReader.drawBMP(imageToLoadNameBuff, tft, padx, pady);
    } else {
        if (CONT_OUTPUT_SERIAL) {
            Serial.print(F("Can't load file: "));
            Serial.println(imageToLoadNameBuff);
            imageReader.printStatus(stat);
        }
    }
}

/***************************************************************************/
/**** MAIN LOGIC - Setup + Main Loop ***************************************/
/***************************************************************************/

/*! setup
    @brief  run once before the main loop
*/
void setup()   {
    // Enable PIE2 change interupt - important fo SW button of Rotary: 
    // @note: For understanding and learning more about CHANGE interrupts
    // Visit Asset 4.
    PCICR |= 0B00000100;    // The register that enables change interrupt of the group pins
    PCMSK2 |= 0B00010000;   // The register that enables change interrupt of the group - Enable D4

    //Status from image-reading functions
    ImageReturnCode stat;

    //Initiate serial port for debuging:
    Serial.begin(9600);
    #if !defined(ESP32)
        while (!Serial) { ; }
    #endif

    //Load filesystem:
    uint8_t foundFiles = 0;
    //Initialize sd card:
    if(!SikSd.initialize(CONT_SD_CS, CONT_SD_LIMIT_MHZ)) {
        //Sd card failed or not present
        memcpy(menuFiles[((CONT_NUM_MENU_ROWS - 1) / 2)], "SD card not detected", 21);
        rotary.activate(false);
    } else {
        //Set Folder of SD card:
        SikSd.setRoot();
        //Scan directory for BMP files:
        foundFiles = SikSd.scanFiles(".bmp");
        if (foundFiles > 0) {
            Serial.print(F("Found Images: "));
            Serial.println(foundFiles);
        } else {
            Serial.print(F("No Images found: "));
            Serial.println(foundFiles);
            rotary.activate(false);
        }
    }

    //Set Rotary range - According to the number of images found on sd card
    rotary.setRange({0, foundFiles - 1, 1});
    rotary.setPos(0); 
    rotary.attachInterupts(
        []{ rotary.interA(); }, // void lambda function that calls the default rotary interrupt function
        []{ rotary.interB(); }  // void lambda function that calls the default rotary interrupt function
    );

    //Attach an additional user defined callback:
    rotary.setCallback(customCallback);

    //Initiate TFT with the required rotation:
    initTFT(CONT_BASE_BACKGROUND, 45);
    //Flag to draw Static Screen:
    loadMainScreen = true;
}

void loop() {
    
    //Checks flags and states to draw the correct view on TFT:
    if (loadMainScreen) {
        
        //Main screen - Mostly LOGO + HEADER + BG
        loadMainScreen = false;
        printScreens(tft, TftScreens::Main);

    } else if (rotary.changed && loadImage == -1) { 
        
        //Rotary has changed draw the menu:
        pointTo = rotary.readState(); 
        printMenu(tft, pointTo.pos, 0, 45);

    } else if (rotary.changed && loadImage != -1) {

        //Rotary has changed (SW push) draw the selected image:
        rotary.changed = false;
        drawSelectedImage(tft, loadImage);

    }
}