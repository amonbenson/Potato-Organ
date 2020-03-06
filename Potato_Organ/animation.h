#ifndef ANIMATION_H
#define ANIMATION_H

#include <FastLED.h>

#include "constants.h"


#define INTRO_ANIMATION_NUM_FRAMES 23
#define INTRO_ANIMATION_NUM_COLORS 28

const int introAnimation_frames[] = { 20, 12, 13, 5, 6, 7, 1, 8, 9, 14, 15, 21, 22, 16, 17, 10, 2, 0, 3, 4, 11, 18, 19, 23 };
const CRGB introAnimation_colors[] = {
  CRGB(25, 0, 0),
  CRGB(50, 0, 0),
  CRGB(75, 25, 0),
  CRGB(100, 50, 0),
  CRGB(125, 75, 25),
  CRGB(150, 100, 50),
  CRGB(175, 125, 75),
  CRGB(200, 150, 100),
  CRGB(225, 175, 125),
  CRGB(255, 200, 150),
  CRGB(255, 225, 175),
  CRGB(255, 255, 200),
  CRGB(255, 255, 225),
  CRGB(255, 255, 255),
  CRGB(225, 255, 255),
  CRGB(200, 255, 255),
  CRGB(175, 225, 255),
  CRGB(150, 200, 255),
  CRGB(125, 175, 225),
  CRGB(100, 150, 200),
  CRGB(75, 125, 175),
  CRGB(50, 100, 150),
  CRGB(25, 75, 125),
  CRGB(0, 50, 100),
  CRGB(0, 25, 75),
  CRGB(0, 0, 50),
  CRGB(0, 0, 25),
  CRGB(0, 0, 0)
};
int introAnimation_currentFrame = 0;

void introAnimation_update(CRGB leds[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    int colorId = introAnimation_currentFrame - introAnimation_frames[i];
    if (colorId < 0 || colorId >= INTRO_ANIMATION_NUM_COLORS) continue;

    leds[ledOrder[i]] = introAnimation_colors[colorId];
  }

  introAnimation_currentFrame++;
}

#endif