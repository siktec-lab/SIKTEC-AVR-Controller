# SIKTEC-AVR-Controller
Example projects for using and programming the SIKTEC Arduino Controller -

#### Description
Those examples were created as part of several libraries and examples of the AVR Controller Shield by SIKTEC - [Arduino based Controller Shield](http://siktec.net "Arduino based Controller Shield").
SIKTEC AVR Controller is a DIY Shield which extend most Arduino boards to easily integrate several common components which are widely used in any Arduino project who requires human interface.

Our goal was to create a simple well designed shield which integrates widely used components in the best way. The shield enable makers to focus on what really matter – since most of the projects require those capabilities, makers “waste” there time on assembling and integrating common parts on breadboards and proto boards. With this shield you can easily avoid all the hassle and focus on your idea. The shield is fully backed by our code base with simple and advanced examples – No need to google and search every where for the correct drivers and hack your way to make it work. All the code for this shield is well documented and can be easily extended and modified to fit your needs.

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
