/******************************************************************************/
// Created by: Shlomo Hassid.
// Release Version : 1.0.6
// Creation Date: 2021-04-18
// Copyright 2021, SIKTEC / SIKDEV.
/******************************************************************************/
/*****************************      Changelog       ****************************
version 1.0.6:
    ->initial, supports callbacks, circular + Push button support
    ->Debounce integrated/
*******************************************************************************/

#ifndef _SIKTECSDCARDINTERFACE_h
#define _SIKTECSDCARDINTERFACE_h

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>                // SD card & FAT filesystem library

namespace SIKtec {

    /***************************************************************************/
    /**** Some Defintions - ****************************************************/
    /***************************************************************************/
    // Don't edit - Most of those can be set by the constructor - or define them 
    // prior to the class include.
    #ifndef SIKSD_LOAD_MAX_FILES
        // Comment it:
        #define SIKSD_LOAD_MAX_FILES  20 
    #endif

    #ifndef SIKSD_MAX_FILES_NAME
        // Comment it:
        #define SIKSD_MAX_FILES_NAME  50 
    #endif




    /***************************************************************************/
    /**** Types to be used *****************************************************/
    /***************************************************************************/
    /* 
    * RotaryRange: Struct for defining counter range.
    */
    typedef struct 
    {

        //ROTARY_COUNTER_TYPE min; // Minimum included

    } SdFilesPreview;

    /***************************************************************************/
    /**** SikRot CLASS *********************************************************/
    /***************************************************************************/
    class SdExplorer {
        
    private:

        
        
    public:

        //Whether we want to show procedures as Serial Output:
        bool debugSerial;

        //SD card filesystem:
        static SdFat SD; 

        //Loaded directory:
        FatFile mainDirectory;

        //Loaded files indexes:
        uint16_t files[SIKSD_LOAD_MAX_FILES] = {0};
        uint8_t loadedFiles = 0;
    
        /*
        * Constructor: initiate the Rotary Class with some required definitions
        * ----------------------------
        *   returns: self 
        */
        SdExplorer(bool useSerial = false);

        /*
        * initialize:
        * ----------------------------
        * 
        * 
        */
        bool initialize(uint8_t sd_cs_pin, uint8_t limitSpiMHz = 10);
        /*
        * setRoot:
        * ----------------------------
        * 
        * 
        */
        bool setRoot(/* base root */);
        bool setRoot(const char* path);

        /*
        * setRoot:
        * ----------------------------
        * 
        * 
        */
        uint8_t scanFiles(const char* ext);

        /*
        * fileExtensionIs:
        * ----------------------------
        * 
        * 
        */
        bool fileExtensionIs(const char* filename, const char* ext);

        /*
        * getFullFileNameByIndex:
        * ----------------------------
        * 
        * 
        */
        void getFullFileNameByIndex(char* nameBuff , const int16_t index);

        /*
        * getFilesAroundCurrent:
        * ----------------------------
        * 
        * 
        */
        void getFilesAroundCurrent(uint8_t index, char* menunames, size_t bufferSize, size_t maxlength, size_t shift = 1);

        /*
        * listRootDirectory:
        * ----------------------------
        * 
        * 
        */
        void listRootDirectory();

    };

}

#endif
