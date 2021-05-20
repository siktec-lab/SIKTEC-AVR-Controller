# SIKTEC-AVR-Controller
Example projects for using and programming the SIKTEC Arduino Controller -

#### Description
Originally it was created as part of several libraries and examples of the AVR Controller Shield by SIKTEC - [Arduino based Controller Shield](http://siktec.net "Arduino based Controller Shield").
This Library is as small as it gets and very well documented - It allows an easy integration of a rotary encoder in an "interrupt way".

#### Table of Contents:
- [Shield Features](#shield-features)
- [Included Examples](#included-examples)
- [Installation](#installation)
- [Dependencies](#dependencies)

## Shield Features
 - Rotary Encoder with push button – Widely used to control menus and interfaces.
 - 2 Programmable Tactile Push Buttons.
 - 1 External Reset Button.
 - 1.8’ Colour TFT screen (ST7735) – Detachable with nylon board spacers.
 - Micro SD card reader.
 - All required headers to connect to the different Arduino Boards.

## Included Examples:
 - **Image Reader Example**: Turns the arduino boards into an image reader which reads all images from a MicroSD into a menu controlled by the rotary encoder. This example is great to learn how to glue all the capabilities together:
    - Building a menu system.
    - Dynamically reading files from an SD card.
	- Drawing graphics on a TFT.
	- Using Interrupts.
	- Drawing Bitmaps.
	- Using a Rotary Encoder as a menu control.

- **Blink Delay Example**: Blinks the onboard LED, Controlled by the encoder and switches to manipulate the delay in various ways. This Example is great resource to learn how:
	- Use interrupts (Raising and Change).
	- drawing text on the attached TFT screen and refreshing it.
	- drawing a bitmap from program memory.
	- Using a Rotary Encoder.

## Installation:
You can install the examples via one of the following:
1. Arduino library manager: Search for "SIKTEC AVR Controller" and click install  (Examples will be added to the examples menu).
2. Download the repositories as a ZIP file and install it on the Arduino IDE by:
	`Sketch -> Include library -> Add .ZIP Library.`
3. Download the library and include extract all files - Than you can create a project folder and copy the example files you want to compile".
**NOTE**: Those examples has several dependencies if you install them manually or download the files make sure you manually installs all the dependencies.

## Dependencies
 - [SIKTEC_Rotary](https://github.com/siktec-lab/SIK-Rotary-Encoder "SIKTEC Rotary Library"),
 - [SIKTEC_SDExplorer](https://github.com/siktec-lab/SIKTEC-SdExplore "SIKTEC_SDExplorer"),
 - [GFX Library,](https://github.com/adafruit/Adafruit-GFX-Library "Adafruit GFX Library,")
 - [ST7735 Driver](https://github.com/adafruit/Adafruit-ST7735-Library "Adafruit_ST7735"),
 - [Adafruit ImageReader](https://github.com/adafruit/Adafruit_ImageReader "Adafruit ImageReader")