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
#include <Streaming.h>
#include "blinkLed.h"

void BlinkLed::init(byte pin, bool activeOnLow)
{
    _pin = pin;
    _activeOn = activeOnLow ? LOW : HIGH;
    if (_pin != BLINKLED_INDEX_HALTED)
    {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, !_activeOn);
    }
}

BlinkLed *BlinkLed::setBlink(const word millisOn, const word millisOff)
{
    if (0 == millisOn)
    {
        _nextIndex = BLINKLED_INDEX_HALTED;
        if (NOT_A_PIN != _pin)
        {
            digitalWrite(_pin, !_activeOn);
        }
    }
    else
    {
        _blinkTiming2[0] = millisOff;
        _blinkTiming2[1] = millisOn;
        _blinkTiming = _blinkTiming2;
        startBlink(2);
    }
    return this;
}

BlinkLed *BlinkLed::setBlinkPattern4(word millis[4])
{
    _blinkTiming = millis;
    startBlink(4);
    return this;
}

BlinkLed *BlinkLed::setBlinkPattern(word *blinkTiming, const byte length)
{
    if (0 == (length % 2))
    {
        _blinkTiming = blinkTiming;
        startBlink(length);
        return this;
    }
    else
    {
        // invalid argument, force an error!
        return nullptr;
    }
}

void BlinkLed::startBlink(const byte numPhases)
{
    _timingLength = numPhases;
    _nextIndex = 1; // start with active phase
    _nextUpdate = 0;
    update();
}

void BlinkLed::update()
{
    if (NOT_A_PIN != _pin && BLINKLED_INDEX_HALTED != _nextIndex)
    {
        const unsigned long now = millis();
        if (((long)now - _nextUpdate) >= 0)
        {
            digitalWrite(_pin, _nextIndex % 2 ? _activeOn : !_activeOn);
            _nextUpdate = now + _blinkTiming[_nextIndex];
            ++_nextIndex;
            if (_nextIndex >= _timingLength)
            {
                _nextIndex = 0;
            }
        }
    }
}