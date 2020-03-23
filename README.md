# BlinkLED

This is a small lib for controlling a LED on specific output pin, implemented in non-blocking way.

It has been inspired by JLed and tries to keep close to that's API.
Why am I not using JLED? 

* need to specify PIN at `setup()`, not in constructor; to use BlinkLed inside a class
* didn't get it working yet :( 
* don't need its analog functionality, resulting in smaller memory/flash footprint for this lib



## Usage

Disclaimer: `delay(ms)` is only used to shorten the code for demonstration.

### Initialize

```C
BlinkLed led=BlinkLed();
void setup() {
    led.init(LED_BUILTIN); // LED connected to GND
}
```

### Inverse LED

```C
BlinkLed led=BlinkLed();
#define SOME_PIN_WITH_LED_ATTACHED_TO_VCC 13
void setup() {
    led.init(SOME_PIN_WITH_LED_ATTACHED_TO_VCC, true); // LED connected to VCC
}
```

### Multiple LEDs

```C
BlinkLed led1=BlinkLed();
BlinkLed led2=BlinkLed();
void setup() {
    led1.init(LED_BUILTIN);
    led2.init(12);
}
```

### On/Off

* Switch on and off

```C
void loop() {
    led.setBlink(-1,0).Update();		// LED on
    delay(5000);
    led.setBlink(0,0).Update();			// LED off
    delay(5000);
}
```

* Flashes (indicating activity)
        
```C
void loop() {
    led.setBlink(-1,0).Update();		// LED on, starting a process
    delay(20);							// waiting for something?
    led.setBlink(0,0).Update();			// LED off at start of activity
    delay(5); 							// some activity, e.g. sending IR code
    led.setBlink(-1,0).Update();		// LED on after activity finished
    delay(15); 							// waiting for something more?
    led.setBlink(0,0).Update();			// done, LED finally off
    delay(5000);
}
```
    

### Simple Blink

```C
void setup() {
    //...
    led.setBlink(500,500);				// 1Hz
}
void loop() {
    led.Update();
}
```

## Alternating Blink

* long and short lighting, alternating
 
```C
int blinkIntervals[]={300, 300, 300, 100};
void setup() {
    //...
    led.setBlink(blinkIntervals);
}
void loop() {
    led.Update();
}
```

* Morse code (SOS aka `...---...`, see https://en.wikipedia.org/wiki/Morse_code#Timing)

```C
#define DIT 200		// milliseconds
#define DAH 3*DIT
#define GAP DIT
int blinkIntervals[]={
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
void setup() {
    //...
    led.setBlink(blinkIntervals);
}
void loop() {
    led.Update();
}
```


## Hardware: connecting LED to PIN
* online calculator at https://www.elektronik-kompendium.de/sites/bau/1109111.htm
* dimensioning resistor for LED:
 
    | VCC (chip)           	 | color: red  | color: yellow/green |
    | ---------------------- | :---------: | :-----------------: |
    | 3.3V (ESP8266, ESP32)  |   150 Ohm   |     100 Ohm         |
    | 5V (ATmega*)           |   330 Ohm   |     270 Ohm         |

* detailed considerations (applied ohm's law and such):
    * R = U / I = (Vcc - Uled) / (I=10mA)
    * P = U * I = U * U / R
    * color=red
        * Vcc=5V: (5V-1,6..2,2V)/10mA = 340..280Ohm / power at short circuit of led with 330Ohm: 75mW/16mA
        * Vcc=3.3V: (3.3V-1,6..2,2V)/10mA = 170..110Ohm / power at short circuit of led with 150Ohm: 73mW/22mA
    * color=green/yellow
        * Vcc=5V: (5V-1,9..2,5)/10mA = 310..250Ohm -> power at short circuit of led with 270Ohm: 93mW/19mA
        * Vcc=3.3V: (3.3V-1,9..2,5V)/10mA = 140..80Ohm -> power at short circuit  of led with 100Ohm: 73mW/33mA


## Licensing & Contact
Licensed under LGPL v3.

Email: makerMcl (at) clauss.pro
Please only email me if it is more appropriate than creating an Issue / PR. I will not respond to requests for adding support for particular boards, unless of course you are the creator of the board and would like to cooperate on the project. I will also ignore any emails asking me to tell you how to implement your ideas. However, if you have a private inquiry that you would only apply to you and you would prefer it to be via email, by all means.

## Copyright

Copyright 2020 Matthias Clauß

## TODOs
* implement unit tests
* improve argument checking / error handling for `setBlinkPattern(word*, byte)`
