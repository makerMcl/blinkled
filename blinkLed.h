/*
 * Copyright 2020 Matthias Clauß
 * 
 * This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Arduino.h>
#ifndef BLINK_LED_H
#define BLINK_LED_H

#define BLINKLED_INDEX_HALTED 255
class BlinkLed
{
private:
    byte _pin = NOT_A_PIN;
    byte _activeOn;
    byte _nextIndex = BLINKLED_INDEX_HALTED;
    word *_blinkTiming;
    byte _timingLength = 2;
    unsigned long _nextUpdate = 0;
    word _blinkTiming2[2] = {0, 0}; // for simple blinking, to avoid memory defragmentation with multiple calls of setBlink(int, int)

    void startBlink(const byte numPhases);

public:
    void init()
    {
        init(255);
    }
    void init(const byte pin)
    {
        init(pin, false);
    }

    /**
     * Initiazes blinking led at the given pin.
     * @param activeOnLow true if led is active at LOW, otherwise false (active on HIGH)
     */
    void init(const byte pin, const bool activeOnLow);

    /**
     * Sets blink code, alternating between <i>millisOn</i> active led and <i>millisOff</i> inactive led.
     * @param millisOn 0 is off, -1 is on forever
     */
    BlinkLed *setBlink(const word millisOn, const word millisOff);

    /**
     * Sets special blink pattern with 2 alternating active phases.
     * Times in milliseconds duration.
     * index 0 - off
     * index 1 - 1st active phase
     * index 2 - off
     * index 3 - 2nd active phase
     */
    BlinkLed *setBlinkPattern4(word millis[4]);

    BlinkLed *setBlinkPattern(word *blinkTiming, byte length);

    /** Update Status of led. */
    void update();
};

#endif
