/*
 * Copyright 2020 Matthias Clauß
 */
/**
 * Demo for BlinkLed lib, sending the morse code for S-O-S continually.
 */
#include <Arduino.h>
#include "blinkLed.h"

#if (defined(ESP32) || defined(ESP8266)) && !defined(LED_BUILTIN)
#define LED_BUILTIN 2       // built-in led on ESP32 DevKit v1
#endif

#define DIT 200		// milliseconds
#define DAH 3*DIT
#define GAP DIT

word blinkIntervals[18]={
    DIT*6,		// word space
    DIT,
    GAP,
    DIT,
    GAP,
    DIT,			
    GAP*3,		// letter space
    DAH,
    GAP,
    DAH,
    GAP,
    DAH,
    GAP*3,		// letter space
    DIT,
    GAP,
    DIT,
    GAP,
    DIT
};

BlinkLed led=BlinkLed();
void setup() {
    led.init(LED_BUILTIN);
    led.setBlinkPattern(blinkIntervals, 18);
}
void loop() {
    led.update();
}
