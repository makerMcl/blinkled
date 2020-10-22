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
typedef enum
{
    ACTIVE_LOW,
    ACTIVE_HIGH,
    TRISTATE_LOW = 0x10,
    TRISTATE_HIGH = 0x11,
} blinktype_t;

/**
 * Tristate = two LEDs at one GPIO pin: one against Vcc, one against GND.
 * If using tristate mode, you need to specify which one shall be 
 * activated at the activation methods.
 */
class BlinkLed
{
private:
    uint8_t _pin = NOT_A_PIN;
    blinktype_t _activeOn;
    uint8_t _nextIndex = BLINKLED_INDEX_HALTED;
    blinkDuration_t *_blinkTiming = nullptr;
    uint8_t _timingLength = 2;
    uint64_t _lastUpdate = 0;
    blinkDuration_t _blinkTiming2[2] = {0, 0}; // for simple blinking, to avoid memory fragmentation with multiple calls of setBlink(int, int)

    void startBlink(const uint8_t numPhases);
    void ledOn();
    void ledOff();

public:
    void init()
    {
        init(NOT_A_PIN);
    }
    /** Single led at given pin, active on HIGH. */
    void init(const uint8_t pin)
    {
        init(pin, ACTIVE_HIGH);
    }

    /**
     * Initializes blinking led at the given pin.
     * 
     * If singular led (not tristate), provide value
     * <li>ACTIVE_LOW if single led is active at LOW</li>
     * <li>ACTIVE_HIGH if single led is active on HIGH</li>
     * for argument activeType.
     * 
     * @param activeType ACTIVE_LOW if single led is active at LOW,
     *                   ACTIVE_HIGH if active on HIGH,
     *                   otherwise TRISTATE_*
     */
    void init(const uint8_t pin, const blinktype_t activeType);

    /**
     * Sets blink code, alternating between <i>millisOn</i> active led and <i>millisOff</i> inactive led.
     * @param millisOn 0 is off
     * @param millisOff 0 is on forever
     */
    BlinkLed *setBlink(const blinkDuration_t millisOn, const blinkDuration_t millisOff);
    /** 
     * Immediately switches any led off.
     * Stops blinking. 
     */
    BlinkLed *off();
    /** 
     * Immediately switches led on.
     * In tristate mode, the led indicated by last on(blinktype_t)/init() method is activated.
     * Stops blinking.
     */
    BlinkLed *on();
    /** 
     * Immediately switches led on.
     * In tristate mode, the active led is specified by the tristate mode given.
     * Stops blinking.
     */
    BlinkLed *on(blinktype_t type);

    BlinkLed *blink(blinktype_t type, const blinkDuration_t millisOn, const blinkDuration_t millisOff);
    BlinkLed *blinkPattern4(blinktype_t type, blinkDuration_t millis[4]);
    BlinkLed *blinkPattern(blinktype_t type, blinkDuration_t millis[4], const uint8_t length);

    /**
     * Sets special blink pattern with 2 alternating active phases.
     * Times in milliseconds duration.
     * index 0 - off
     * index 1 - 1st active phase
     * index 2 - off
     * index 3 - 2nd active phase
     */
    BlinkLed *setBlinkPattern4(blinkDuration_t millis[4]);

    BlinkLed *setBlinkPattern(blinkDuration_t *blinkTiming, const uint8_t length);

    /** Update Status of LED. */
    void update();
};
#endif
