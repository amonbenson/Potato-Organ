#ifndef CONSTANTS_H
#define CONSTANTS_H


/*
SENSOR LAYOUT (0 = mprLower, pin 0; 23 = mprUpper, pin 11)
  13  15      18  20  22
12  14  16  17  19  21  23
   1   3   5   6   8  10
 0   2   4   5   7   9  11

LED LAYOUT (0 = start of led strip, 23 = end of led strip)
   1   0       6   7   8
 2   3   4   5  11  10   9
  22  23      12  13  14
21  20  19  18  17  16  15
*/


// #define DEBUG // Serial debug instead of midi
#define DEBUG_BAUDRATE 115200 // Baud rate for serial debug
#define MIDI_BAUDRATE 31250 // Baud rate for midi messages

#define MPR_LOWER_ADDR 0x5A // ADD unconnected
#define MPR_UPPER_ADDR 0x5B // ADD tied to 3V3

#define NUM_SENSORS 24

#define ROOT_NOTE 60 // C4 (middle C)

#define LED_PIN 3
#define NUM_LEDS NUM_SENSORS


#define AUTOLIGHT // Automaitcally light up the pressed sensor without any midi input


#define VELOCITY_NOTE_ON 100
#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90


const int ledOrder[] = {
  21, 22, 20, 23, 19, 18, 12, 17, 13, 16, 14, 15, // lower octave
  2, 1, 3, 0, 4, 5, 6, 11, 7, 10, 8, 9 // upper octave
};


#endif