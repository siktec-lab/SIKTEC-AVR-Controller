#if defined(__AVR__)
    #include <avr/pgmspace.h> 
#elif defined(__PIC32MX__)
    #define PROGMEM
#elif defined(__arm__)
    #define PROGMEM
#endif
#pragma once

/***************************************************************************/
/**** Graphics to be used **************************************************/
/***************************************************************************/
#define bitmap_logo_BMPWIDTH  100
#define bitmap_logo_BMPHEIGHT 33
//const unsigned char bitmap_logo[] PROGMEM = {
const uint8_t bitmap_logo[] PROGMEM = {
  0B00000001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // .......###..............................................................................................
  0B00000011,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......####..............................................................................................
  0B00000011,0B11100000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......#####.............................................................................................
  0B00000011,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......####..............................................................................................
  0B00000001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // .......###..............................................................................................
  0B11110001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ####...###..............................................................................................
  0B11110001,0B11100000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ####...####.............................................................................................
  0B11111001,0B11100000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // #####..####.............................................................................................
  0B11111100,0B11110000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ######..####............................................................................................
  0B00111110,0B01111000,0B00000000,0B01111110,0B00000011,0B10000111,0B10000011,0B10001111,0B11111111,0B00011111,0B11111000,0B00011111,0B10000000, // ..#####..####............######.......###....####.....###...############...##########......######.......
  0B00011110,0B00111100,0B00000001,0B11111111,0B00000011,0B10000111,0B10000111,0B10001111,0B11111111,0B00111111,0B11111000,0B00111111,0B11000000, // ...####...####.........#########......###....####....####...############..###########.....########......
  0B00001111,0B00111110,0B00000001,0B11111111,0B10000011,0B10000111,0B10001111,0B00001111,0B11111111,0B00111111,0B11111000,0B01111111,0B11100000, // ....####..#####........##########.....###....####...####....############..###########....##########.....
  0B00000111,0B10011111,0B00000011,0B11000011,0B10000011,0B10000111,0B10001110,0B00000000,0B11110000,0B00111100,0B00000000,0B01110000,0B11110000, // .....####..#####......####....###.....###....####...###.........####......####...........###....####....
  0B01100111,0B11001111,0B00000011,0B10000000,0B00000011,0B10000111,0B10011100,0B00000000,0B11110000,0B00111100,0B00000000,0B11110000,0B01110000, // .##..#####..####......###.............###....####..###..........####......####..........####.....###....
  0B11110011,0B11000111,0B10000011,0B11000000,0B00000011,0B10000111,0B10111100,0B00000000,0B11110000,0B00111100,0B00000000,0B11100000,0B01110000, // ####..####...####.....####............###....####.####..........####......####..........###......###....
  0B11110001,0B11000011,0B11000011,0B11111100,0B00000011,0B10000111,0B10111100,0B00000000,0B11110000,0B00111100,0B00000000,0B11100000,0B00000000, // ####...###....####....########........###....####.####..........####......####..........###.............
  0B11110001,0B11000011,0B11100001,0B11111111,0B00000011,0B10000111,0B11111110,0B00000000,0B11110000,0B00111111,0B11110000,0B11100000,0B00000000, // ####...###....#####....#########......###....##########.........####......##########....###.............
  0B11110011,0B11000011,0B11000000,0B11111111,0B10000011,0B10000111,0B11111110,0B00000000,0B11110000,0B00111111,0B11110000,0B11100000,0B00000000, // ####..####....####......#########.....###....##########.........####......##########....###.............
  0B11110011,0B11100111,0B10000000,0B00011111,0B11000011,0B10000111,0B11101110,0B00000000,0B11110000,0B00111100,0B00000000,0B11100000,0B00000000, // ####..#####..####..........#######....###....######.###.........####......####..........###.............
  0B11110011,0B11001111,0B00000000,0B00000011,0B11000011,0B10000111,0B11001111,0B00000000,0B11110000,0B00111100,0B00000000,0B11100000,0B00100000, // ####..####..####..............####....###....#####..####........####......####..........###.......#.....
  0B11111001,0B10011110,0B00000000,0B00000001,0B11000011,0B10000111,0B10000111,0B00000000,0B11110000,0B00111100,0B00000000,0B11110000,0B01110000, // #####..##..####................###....###....####....###........####......####..........####.....###....
  0B01111100,0B00111110,0B00000111,0B10000001,0B11000011,0B10000111,0B10000111,0B10000000,0B11110000,0B00111100,0B00000000,0B11110000,0B11110000, // .#####....#####......####......###....###....####....####.......####......####..........####....####....
  0B00111110,0B01111100,0B00000011,0B11000011,0B11000011,0B10000111,0B10000111,0B10000000,0B11110000,0B00111111,0B11110000,0B01111001,0B11100000, // ..#####..#####........####....####....###....####....####.......####......##########.....####..####.....
  0B00011111,0B11111000,0B00000011,0B11111111,0B10000011,0B10000111,0B10000011,0B10000000,0B11110000,0B00111111,0B11111000,0B00111111,0B11100000, // ...##########.........###########.....###....####.....###.......####......###########.....#########.....
  0B00001111,0B11110000,0B00000000,0B11111111,0B00000011,0B10000111,0B10000011,0B11000000,0B11110000,0B00111111,0B11111000,0B00011111,0B11000000, // ....########............########......###....####.....####......####......###########......#######......
  0B00000111,0B11100000,0B00000000,0B00011000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000110,0B00000000, // .....######................##................................................................##.........
  0B00000011,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......####..............................................................................................
  0B00000001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // .......###..............................................................................................
  0B00000001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // .......###..............................................................................................
  0B00000011,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......####..............................................................................................
  0B00000011,0B11100000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......#####.............................................................................................
  0B00000011,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000, // ......####..............................................................................................
  0B00000001,0B11000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000,0B00000000  // .......###..............................................................................................
};