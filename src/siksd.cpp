
#include "siksd.h"  

namespace SIKtec {
    
    SdFat SdExplorer::SD;

    /*
    * Constructor: initiate the Sd card wrapper Class with some required definitions
    */
    SdExplorer::SdExplorer(bool useSerial) {
        this->debugSerial = useSerial;
    }

    /*
    * initialize:
    */
    bool SdExplorer::initialize(uint8_t sd_cs_pin, uint8_t limitSpiMHz) {
        if (this->debugSerial) 
            Serial.print(F("Initializing filesystem..."));
        if(!SdExplorer::SD.begin(sd_cs_pin, SD_SCK_MHZ(limitSpiMHz))) { // Breakouts require 10 MHz limit due to longer wires
            if (this->debugSerial) 
                Serial.println(F("SD initialization failed."));
            return false;
        }
        Serial.println(F(" Filesystem OK!"));
        return true;
    }
    /*
    * setRoot:
    */
    bool SdExplorer::setRoot() {
        return this->setRoot("/");
    }
    bool SdExplorer::setRoot(const char* path) {
        if (SdExplorer::SD.chdir(path)) {
            this->mainDirectory.open(path);
            if (this->debugSerial) Serial.println(F("Loaded root."));
            return true;
        }
        if (this->debugSerial) Serial.println(F("Failed to load root."));
        return false;
    }

    /*
    * printDirectory:
    */
    uint8_t SdExplorer::scanFiles(const char* ext) {
        this->loadedFiles = 0;
        if (!this->mainDirectory.isOpen()) {
            if (this->debugSerial) 
                Serial.println(F("Main directory not available."));
        } else {
            if (this->debugSerial)
                Serial.println(F("Root Directory opened."));
            FatFile candid;
            char nameBuff[SIKSD_MAX_FILES_NAME];
            memset(nameBuff, ' ', SIKSD_MAX_FILES_NAME);
            this->mainDirectory.rewind();
            while (candid.openNext(&this->mainDirectory, O_RDONLY)) {
                if (candid.isFile() && !candid.isHidden()) {
                    candid.getName(nameBuff, SIKSD_MAX_FILES_NAME);
                    if (this->fileExtensionIs(nameBuff, ext)) {
                        this->files[this->loadedFiles++] = candid.dirIndex();
                        if (this->debugSerial) {
                            Serial.print(nameBuff);
                            Serial.print(F("  - Index: "));
                            Serial.println(candid.dirIndex());
                        }
                    }
                }
                candid.close();
            }
        }
        return this->loadedFiles;
    }
    /*
    * listRootDirectory:
    */
    bool SdExplorer::fileExtensionIs(const char* filename, const char* ext) {
        uint8_t name_len = strlen(filename);
        uint8_t ext_len = strlen(ext);
        return (name_len >= ext_len) && (0 == strcmp(filename + (name_len - ext_len), ext));
    }
    void SdExplorer::getFullFileNameByIndex(char* nameBuff , const int16_t index) {
        FatFile candid;
        if (index < this->loadedFiles && index >= 0) {
            if (candid.open(&this->mainDirectory, this->files[index], O_RDONLY)) {
                candid.getName(nameBuff, SIKSD_MAX_FILES_NAME);
                candid.close();
            } else {
                nameBuff[0] = '\0';
            }
        }
    }
    void SdExplorer::getFilesAroundCurrent(uint8_t index, char* menunames, size_t bufferSize, size_t maxlength, size_t shift) {
        if (this->loadedFiles < 1) return;
        uint8_t rows = shift * 2 + 1;
        int8_t loop_index = index - shift;
        FatFile candid;
        char nameBuff[maxlength] = {' '};
             nameBuff[maxlength - 1] = '\0';
        for (uint8_t i = 0; i < rows; i++, loop_index++) {
            if (loop_index < this->loadedFiles && loop_index >= 0) {
                if (candid.open(&this->mainDirectory, this->files[loop_index], O_RDONLY)) {
                    candid.getName(nameBuff, maxlength);
                    candid.close();
                } else {
                    nameBuff[0] = '\0';
                }
            } else {
                nameBuff[0] = '\0';
            }
            memcpy((menunames + bufferSize * i), nameBuff, sizeof(nameBuff));
            nameBuff[maxlength-1] = '\0';
            // Serial.print("File at loop_index: ");
            // Serial.print(loop_index);
            // Serial.print( " -> ");
            // Serial.println(nameBuff);
        }
    }
    /*
    * listRootDirectory:
    */
    void SdExplorer::listRootDirectory() {
        SdExplorer::SD.ls();
    }
}
