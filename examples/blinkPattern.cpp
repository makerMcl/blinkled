#include <Arduino.h>
#include "blinkLed.h"

BlinkLed *bl = new BlinkLed();
word bp[] = {100, 100, 200, 100, 200, 300};

void setup()
{
    bl->init(LED_BUILTIN, true);
    bl->setBlinkPattern(bp, sizeof(bp) / sizeof(bp[0]));
}

void loop() {
    bl->update();
}