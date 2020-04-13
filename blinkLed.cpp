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

// LED permanently on
BlinkLed *BlinkLed::on()
{
    _nextIndex = BLINKLED_INDEX_HALTED;
    if (NOT_A_PIN != _pin)
    {
        digitalWrite(_pin, _activeOn);
    }
    return this;
}

// LED permanently off
BlinkLed *BlinkLed::off()
{
    _nextIndex = BLINKLED_INDEX_HALTED;
    if (NOT_A_PIN != _pin)
    {
        digitalWrite(_pin, !_activeOn);
    }
    return this;
}

BlinkLed *BlinkLed::setBlink(const word millisOn, const word millisOff)
{
    if (0 == millisOn)
    {
        return off();
    }
    else if (0 == millisOff)
    {
        return on();
    }
    else if (2 != _timingLength || _blinkTiming[0] != millisOff || _blinkTiming2[1] != millisOn)
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
        bool unmodified = (length == _timingLength);
        int i = 0;
        while (unmodified && i < length)
        {
            unmodified &= (blinkTiming[i] == _blinkTiming[i]);
            ++i;
        }
        if (!unmodified)
        {
            _blinkTiming = blinkTiming;
            startBlink(length);
        }
        return this;
    }
    else
    {
        // invalid argument, force an error when chained!
        return nullptr;
    }
}

void BlinkLed::startBlink(const byte numPhases)
{
    _timingLength = numPhases;
    _nextIndex = 1; // start with active phase
    _lastUpdate = 0;
    update();
}

void BlinkLed::update()
{
    if (NOT_A_PIN != _pin && BLINKLED_INDEX_HALTED != _nextIndex)
    {
        const unsigned long now = millis();
        if (now - _lastUpdate > _blinkTiming[_nextIndex])
        {
            //Serial << "pin " << _pin << "=" << ((_nextIndex + 1) % 2) << ": delta=" << (now - _lastUpdate) << ", " << endl;
            ++_nextIndex;
            if (_nextIndex >= _timingLength)
            {
                _nextIndex = 0;
            }
            digitalWrite(_pin, _nextIndex % 2 ? _activeOn : !_activeOn);
            _lastUpdate = now;
        }
    }
}
