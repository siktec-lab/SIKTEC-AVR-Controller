/******************************************************************************/
// Created by: Shlomo Hassid.
// Release Version : 1.0.6
// Creation Date: 2021-04-27
// Copyright 2021, SIKTEC.
/******************************************************************************/
/*****************************      Changelog       ****************************
version:
    1.0.6
        -> Bumped to match library version
    1.0.3
        -> Bumped after changes were made to other examples.
    1.0.2
        -> initial release - Example to showcase siktec Arduino Controller board.
        -> Based on Adafruit GFX + SIKTEC Rotary Library.
        -> Populates a visual output on the attached TFT 1.8' 
        -> Controll by the Rotary encoder Using SIKtec::Rotary lib
        -> Delay controlled by the rotary stepping up an down the delay interval.
        -> Rotary switch sets the delay to MAX / MIN of the rage.
        -> Controller Buttons (0, 1) Inc / Decrement by X10 the delay. 
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
 * SIKTEC_Rotary
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
// SIKtec::Rotary - a generic lib to easily interface with a rotary encoder - using interrupts.
#include <SIKTEC_Rotary.h> 
// Image in progmem to draw on TFT
#include "sik_logo_100_33.h"    


/***************************************************************************/
/**** BASE DEFINITIONS *****************************************************/
/***************************************************************************/

//Output To Serial Also:
#define CONT_OUTPUT_SERIAL true

//Define Attached pins:
#define CONT_ROTARY_ENCODER_A   2       //PD2, Arduino PIN 2  -> Interrupt pin
#define CONT_ROTARY_ENCODER_B   3       //PD3, PIN 3  -> Interrupt pin
#define CONT_ROTARY_ENCODER_SW  4       //PD4, Arduino PIN 4  -> Digital pin for switch button
#define CONT_BUT_0              PIN_A0  // Arduino PIN 14 -> Analog 0 
#define CONT_BUT_1              PIN_A1  // Arduino PIN 15 -> Analog 1 
#define CONT_TFT_RST            PIN_A2  // Arduino PIN 16 -> Analog 2
#define CONT_TFT_CS             10      //PB2, Arduino PIN 10 -> SS / Chip Select
#define CONT_TFT_DC             8       //PB0, Arduino PIN 8  -> Data Command control pin
#define CONT_SD_CS              9       //PB1, Arduino PIN 9  -> SD card Chip select --- NOT USED IN THIS EXAMPLE --- 
#define CONT_LED_PIN            5       //PD5, Arduino PIN 5  -> Attached to Shield onboard LED

//SD Card definitions --- NOT USED IN THIS EXAMPLE --- :
#define CONT_SD_LIMIT_MHZ 50 //50 MHz limit the SPI interface - use at 10 or 50

//TFT Size:
#define CONT_TFT_WIDTH  160 //pixels
#define CONT_TFT_HEIGHT 128 //pixels

//Rotary starting position:
#define STARTING_INTERVAL 500

//Screens Definition:
enum TftScreens { Main };               // Defined static screens

//Colors Definition:
#define CONT_BASE_BACKGROUND            ST7735_BLACK    // Base black color 0x0000
#define CONT_HEADER_LOGO_COLOR          0xF640          // SIKTEC orange
#define CONT_HEADER_TEXT_COLOR          ST7735_WHITE    // Base white color
#define CONT_MENU_ROW_BG                0x2945          // Dark grey
#define CONT_MENU_TEXT_COLOR            0x7BCF          // Light gray
#define CONT_MENU_ROW_SELECTED_BG       0x4228          // Light gray
#define CONT_MENU_SELECTED_TEXT_COLOR   0xFE68          // Light orange


//Rotary global variables 
volatile unsigned long lastPushButton = 0;

//Led state:
int ledState = LOW;         // The LED state that is being toggled in the loop

//Those Variables are for handling the delay:
unsigned long previousMillis = 0;

//Rotary range and pos:
SIKtec::RotaryState interval;
enum IntervalLimits { MAX = 1200, MIN = 20, STEP = 10 };

//Display position and states
const uint8_t counter_col = 40;
const uint8_t counter_row = 90;
char counter_str[4];

//Intiate the TFT:
Adafruit_ST7735 tft(CONT_TFT_CS, CONT_TFT_DC, CONT_TFT_RST);

//Intiate the Rotary encoder helper class:
SIKtec::Rotary rotary = SIKtec::Rotary(
    CONT_ROTARY_ENCODER_A, 
    CONT_ROTARY_ENCODER_B, 
    CONT_ROTARY_ENCODER_SW,
    false, // Circular mode? -> infinite loop counting.
    {IntervalLimits::MIN, IntervalLimits::MAX, IntervalLimits::STEP}
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
ISR (PCINT2_vect) 
{
    if ((millis() - rotary.lastDebounceTime) > SIKtec::Rotary::debounceDelay && digitalRead(CONT_ROTARY_ENCODER_SW) == LOW) {
        cli();
        Serial.println(F("Rotary Push"));
        rotary.lastDebounceTime = millis();
        SIKtec::RotaryState currnet = rotary.readState();
        if (IntervalLimits::MAX - currnet.pos > currnet.pos - IntervalLimits::MIN) {
            rotary.setPos(IntervalLimits::MAX);
        } else {
            rotary.setPos(IntervalLimits::MIN);
        }
        sei();
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
bool printScreens( Adafruit_ST7735& _tft, const enum TftScreens screen) {
  switch (screen) {
    case TftScreens::Main:
      _tft.drawBitmap(29,10, bitmap_logo, 100, 33, 0xF640); // Draw a bitmap from memory
      _tft.setCursor(3, 59);
      _tft.setTextColor(ST7735_WHITE);
      _tft.setTextSize(2);
      _tft.println(F("-BLINK DELAY-"));
    break;
    default: return false;
  }
  return true;
}

void printCounter() {
  printCounter(tft, interval.pos, counter_col, counter_row);
}

void printCounter(Adafruit_ST7735& _tft, const int _counter, int16_t col, const int16_t row) {
  memset(counter_str, ' ', sizeof(counter_str));
  sprintf(counter_str, "%-4d", _counter);
  _tft.setCursor(col, row);
  _tft.setTextSize(3);
  _tft.setTextColor(0xFBCF, ST77XX_BLACK);
  _tft.print(counter_str);
  _tft.setTextSize(2);
  _tft.setCursor(_tft.getCursorX(), _tft.getCursorY() + 7);
  _tft.println("ms");
}

/***************************************************************************/
/**** MAIN LOGIC - Setup + Main Loop ***************************************/
/***************************************************************************/

void setup()   {
    
    //Enable PIE2 change int:
    PCICR |= 0B00000100;
    PCMSK2 |= 0B00010000; // Enable D4

    //Set Programmable Led:
    pinMode(CONT_LED_PIN, OUTPUT);
    pinMode(CONT_BUT_0, INPUT);
    pinMode(CONT_BUT_1, INPUT);

    digitalWrite(CONT_LED_PIN, LOW);

    //Initiate serial port for debuging:
    Serial.begin(9600);
    while (!Serial) { ; }

    //Initiate hardware controls:
    //Rotary + integrated push button - Pin modes are handled in the class
    rotary.attachInterupts(
        []{ rotary.interA(); }, // void lambda function that calls the Rotary interA
        []{ rotary.interB(); } // void lambda function that calls the Rotary interA
    );

    rotary.setPos(STARTING_INTERVAL); // Sets default position of the rotary
    rotary.setCallback(customCallback);

    initTFT(ST7735_BLACK, 45);
    
    //Draw Static Screen - Mainly handles all graphics wi:
    printScreens(tft, TftScreens::Main);

}

void loop() {
    //Current timestamp:
    unsigned long currentMillis = millis();
    
    //Check if rotation happend:
    if (rotary.changed) {
        //Update current interval which is the rotary position
        interval = rotary.readState(); 
        printCounter();
    }

    //Blink led without delaying:
    if (currentMillis - previousMillis >= interval.pos) {
        previousMillis = currentMillis;
        ledState = ledState ? LOW : HIGH;
        digitalWrite(CONT_LED_PIN, ledState);
    }

    //Check if buttons pushed:
    if ((currentMillis - lastPushButton) > SIKtec::Rotary::debounceDelay) {
        if (digitalRead(CONT_BUT_0) == LOW) {
            lastPushButton = currentMillis;
            printf(Serial, "Button A0 Pushed\n");
            rotary.stepUp(10, true);
        } else if (digitalRead(CONT_BUT_1) == LOW) {
            lastPushButton = currentMillis;
            printf(Serial, "Button A1 Pushed\n");
            rotary.stepDown(10, true);
        }
    }
}
/*
 * Simple printf for writing to an Arduino serial port.  Allows specifying Serial..Serial3.
 * 
 * const HardwareSerial&, the serial port to use (Serial..Serial3)
 * const char* fmt, the formatting string followed by the data to be formatted
 * 
 * int d = 65;
 * float f = 123.4567;
 * char* str = "Hello";
 * serial_printf(Serial, "<fmt>", d);
 * 
 * Example:
 *   serial_printf(Serial, "Sensor %d is %o and reads %1f\n", d, d, f) will
 *   output "Sensor 65 is on and reads 123.5" to the serial port.
 * 
 * Formatting strings <fmt>
 * %B    - binary (d = 0b1000001)
 * %b    - binary (d = 1000001)  
 * %c    - character (s = H)
 * %d/%i - integer (d = 65)\
 * %f    - float (f = 123.45)
 * %3f   - float (f = 123.346) three decimal places specified by %3.
 * %o    - boolean on/off (d = On)
 * %s    - char* string (s = Hello)
 * %X    - hexidecimal (d = 0x41)
 * %x    - hexidecimal (d = 41)
 * %%    - escaped percent ("%")
 */
void printf(HardwareSerial& serial, const char* fmt, ...) {
        va_list argv;
    va_start(argv, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            // Look for specification of number of decimal places
            int places = 2;
            if (fmt[i+1] >= '0' && fmt[i+1] <= '9') {
                places = fmt[i+1] - '0';
                i++;
            }
            
            switch (fmt[++i]) {
                case 'B':
                    serial.print("0b"); // Fall through intended
                case 'b':
                    serial.print(va_arg(argv, int), BIN);
                    break;
                case 'c': 
                    serial.print((char) va_arg(argv, int));
                    break;
                case 'd': 
                case 'i':
                    serial.print(va_arg(argv, int), DEC);
                    break;
                case 'f': 
                    serial.print(va_arg(argv, double), places);
                    break;
                case 'l': 
                    serial.print(va_arg(argv, long), DEC);
                    break;
                case 'o':
                    serial.print(va_arg(argv, int) == 0 ? "off" : "on");
                    break;
                case 's': 
                    serial.print(va_arg(argv, const char*));
                    break;
                case 'X':
                    serial.print("0x"); // Fall through intended
                case 'x':
                    serial.print(va_arg(argv, int), HEX);
                    break;
                case '%': 
                    serial.print(fmt[i]);
                    break;
                default:
                    serial.print("?");
                    break;
            }
        } else {
            serial.print(fmt[i]);
        }
    }
    va_end(argv);
}