# Arduino Leds Controller
Software implementing led strip control.</br>

## Description
It is not an art to go to the store and buy working led strips with pre-programmed patterns. <b>The trick is to make them yourself!</b></br>

That's why I decided to realize it with an Arduino microcontroller, HC-05 bluetooth and WS2812B led strip. 
The whole project is based on simple bluetooth communication between the microcontroller and the mobile app [[@Led App](https://github.com/M4zek/LedApp)].
It has several led patterns and you can easily add your own patterns.

### Patterns
- One color
- Rainbow
- Rainbow with glitter
- Confetti
- Knightrider
- Colored Knightrider
- BPM
- Juggle
- Faded
- Breathing

## Connection Diagram
Example wiring pattern for Arduino, led strip and bluetooth. Of course, the connection can be different, while then it is necessary to change the pin numbers in the code.</br>
<img src="https://github.com/M4zek/ArduinoLedController/blob/master/ss/ConnectionDagram.png" data-canonical-src="https://github.com/M4zek/ArduinoLedController/blob/master/ss/ConnectionDagram.png" width="650" height="550" />

## Compatibility
- [@Led App](https://github.com/M4zek/LedApp)
- Also, you can write your own application based on the controller!
## Authors
- [@M4zek](https://github.com/M4zek)
