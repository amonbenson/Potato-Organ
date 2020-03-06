#include <Adafruit_MPR121.h>
#include <FastLED.h>

#include "constants.h"
#include "palette.h"
#include "animation.h"


Adafruit_MPR121 mprLower = Adafruit_MPR121();
Adafruit_MPR121 mprUpper = Adafruit_MPR121();

CRGB leds[NUM_LEDS];

uint32_t bitpointer = 1;
uint32_t currtouched = 0;
uint32_t lasttouched = 0;


void touch(int i);
void release(int i);
void handleNoteOn(uint8_t pitch, uint8_t velociry);
void sendMidi(uint8_t cmd, uint8_t data1, uint8_t data2);
void recvMidi();
void fail();


void setup() {
  #ifdef DEBUG
    // Setup the debug connection
    Serial.begin(DEBUG_BAUDRATE);
  #else
    // Setup the midi serial
    Serial.begin(MIDI_BAUDRATE);
  #endif
  while (!Serial) delay(10);

  // Setup FastLED
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  // Setup both mpr boards
  if (!mprLower.begin(MPR_LOWER_ADDR)) {
    Serial.println("ERROR: MPR board for lower octave not found on address 0x5A.");
    fail();
  }
  if (!mprUpper.begin(MPR_UPPER_ADDR)) {
    Serial.println("ERROR: MPR board for upper octave not found on address 0x5B.");
    fail();
  }

  #ifdef DEBUG
    Serial.println("Potato Organ by Schlegel Flegel.");
  #endif

  // Release all notes
  for (int i = 0; i < NUM_SENSORS; i++) {
    release(i);
  }
}

void loop() {
  // Read MIDI messages and invoke callbacks
  #ifndef DEBUG
    recvMidi();
  #endif

  // Update the intro animation
  if (introAnimation_currentFrame <= INTRO_ANIMATION_NUM_FRAMES + INTRO_ANIMATION_NUM_COLORS) {
    EVERY_N_MILLISECONDS(50) {
      introAnimation_update(leds);
    }
  }

  // Autolight
  else {
    #ifdef AUTOLIGHT
      bitpointer = 1;
      int j;
      for (int i = 0; i < NUM_SENSORS; i++) {
        j = ledOrder[i];
        if (currtouched & bitpointer) leds[j] = CRGB(255, 163, 0);
        else {
          leds[j].fadeToBlackBy(250);
        }
        bitpointer = bitpointer << 1;
      }
    #endif
  }

  // Set the current touched bitmask
  currtouched = (uint32_t) mprLower.touched() << 12 | mprUpper.touched();

  // Check each sensor
  bitpointer = 1;
  for (int i = 0; i < NUM_SENSORS; i++) {
    // Pin changed state to touched
    if ((currtouched & bitpointer) && !(lasttouched & bitpointer)) {
      touch(i);
    }

    // Pin changed state to released
    if (!(currtouched & bitpointer) && (lasttouched & bitpointer)) {
      release(i);
    }

    // Move the pointer one to the left
    bitpointer = bitpointer << 1;
  }

  // Set the touched flag
  lasttouched = currtouched;

  // Update the leds
  FastLED.show();
}

void touch(int i) {
  #ifdef DEBUG
    Serial.print("Send: NoteOn: Pitch = ");
    Serial.print(ROOT_NOTE + i);
    Serial.println(", Velocity = 127");
  #else
    // Send a note on
    sendMidi(MIDI_NOTE_ON, ROOT_NOTE + i, VELOCITY_NOTE_ON);
  #endif
}

void release(int i) {
  #ifdef DEBUG
    Serial.print("Send: NoteOff: Pitch = ");
    Serial.print(ROOT_NOTE + i);
    Serial.println(", Velocity = 127");
  #else
    // Send a note off
    sendMidi(MIDI_NOTE_OFF, ROOT_NOTE + i, 127);
  #endif
}

void handleNoteOn(uint8_t pitch, uint8_t velocity) {
  // Check if the message is in bounds
  if (pitch < ROOT_NOTE || pitch >= ROOT_NOTE + NUM_SENSORS) return;

  // Get the led index and light it up
  int i = ledOrder[pitch - ROOT_NOTE];
  leds[i] = palette[velocity];
}

void sendMidi(uint8_t cmd, uint8_t data1, uint8_t data2) {
  // Send a midi short message (3 bytes)
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void recvMidi() {
  while (Serial.available()) {
    // Receive a short message (3 bytes)
    uint8_t cmd = Serial.read();
    uint8_t data1 = Serial.read();
    uint8_t data2 = Serial.read();

    // Check the message type
    switch (cmd) {
      case MIDI_NOTE_ON:
        #ifdef DEBUG
          Serial.print("Recv: NoteOn: Pitch = ");
          Serial.print(data1);
          Serial.print(", Velocity = ");
          Serial.println(data2);
        #endif
        handleNoteOn(data1, data2);
        break;
      case MIDI_NOTE_OFF:
        #ifdef DEBUG
          Serial.print("Recv: NoteOff: Pitch = ");
          Serial.print(data1);
          Serial.print(", Velocity = ");
          Serial.println(data2);
        #endif
        handleNoteOn(data1, 0);
        break;
      default:
        #ifdef DEBUG
          Serial.print("Recv: Unknown: Cmd = ");
          Serial.print(cmd);
          Serial.print(", Data1 = ");
          Serial.print(data1);
          Serial.print(", Data2 = ");
          Serial.println(data2);
        #endif
        break;
    }
  }
}

void fail() {
  // Someone fucked up
  Serial.println("--- Cannot proceed program execution! ---");

  // Blink red and hold
  while (1) {
    for (int i = 0; i < 2; i++) {
      fill_solid(leds, NUM_LEDS, i ? CRGB::Red : CRGB::Black);
      FastLED.show();
      delay(500);
    }
  }
}
