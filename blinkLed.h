/*
 * Copyright 2020 Matthias Clauﬂ
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
#ifdef UNIT_TEST
#include "ArduinoMock.h"
#else
#include "Arduino.h"
#endif

#ifndef BLINK_LED_H
#define BLINK_LED_H

#define BLINKLED_INDEX_HALTED 255
typedef uint16_t blinkDuration_t;


class BlinkLed
{
private:
    uint8_t _pin = NOT_A_PIN;
    uint8_t _activeOn;
    uint8_t _nextIndex = BLINKLED_INDEX_HALTED;
    blinkDuration_t *_blinkTiming=nullptr;
    uint8_t _timingLength = 2;
    uint64_t _lastUpdate = 0;
    blinkDuration_t _blinkTiming2[2] = {0, 0}; // for simple blinking, to avoid memory fragmentation with multiple calls of setBlink(int, int)

    void startBlink(const uint8_t numPhases);

public:
    void init()
    {
        init(NOT_A_PIN);
    }
    void init(const uint8_t pin)
    {
        init(pin, false);
    }

    /**
     * Initializes blinking led at the given pin.
     * @param activeOnLow true if led is active at LOW, otherwise false (active on HIGH)
     */
    void init(const uint8_t pin, const bool activeOnLow);

    /**
     * Sets blink code, alternating between <i>millisOn</i> active led and <i>millisOff</i> inactive led.
     * @param millisOn 0 is off
     * @param millisOff 0 is on forever
     */
    BlinkLed *setBlink(const blinkDuration_t millisOn, const blinkDuration_t millisOff);
    BlinkLed *off();
    BlinkLed *on();

    /**
     * Sets special blink pattern with 2 alternating active phases.
     * Times in milliseconds duration.
     * index 0 - off
     * index 1 - 1st active phase
     * index 2 - off
     * index 3 - 2nd active phase
     */
    BlinkLed *setBlinkPattern4(blinkDuration_t millis[4]);

    BlinkLed *setBlinkPattern(blinkDuration_t *blinkTiming, uint8_t length);

    /** Update Status of LED. */
    void update();
};
#endif
