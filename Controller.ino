#include <SoftwareSerial.h>
#include <FastLED.h>


SoftwareSerial BTSerial(1,0);

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120
#define NUM_LEDS    30
#define DATA_PIN    12
CRGB leds[NUM_LEDS];


byte BRIGHTNESS = 0;
byte RESET_PIN = 5;

typedef enum {
  TURN_ON,
  TURN_OFF,
  RESSET_ARDUINO,
  SET_BRIGHTNESS,
  ONE_COLOR,
  RAINBOW,
  RAINBOW_GLITTER,
  CONFETTI,
  KNIGHRIDER,
  KNIGHRIDER_COLORED,
  BPM,
  JUGGLE,
  FADED,
  BREATHING,
} Pattern;


// Variable storing the current pattern
Pattern currentPattern = TURN_OFF;

// Current led color for selected patterns
long currentColor = -65536;


// INIT METHOD
void setup(){
    turnOffLeds();

    BTSerial.begin(9600);
    initStartingData();
}

// ==== NUMBER DESCRIPTIONS ====
// 0 - turn on leds
// 1 - turn off leds
// 2 - reset Arduino
// 3 - brightness
// 4 - one color
// 5 - rainbow
// 6 - rainbow with glitter
// 7 - confetti
// 8 - knightrider
// 9 - colored knight rider
// 10 - bpm
// 11 - juggle
// 12 - faded
// 13 - breathing
// =============================


// MAIN LOOP
void loop(){
    if(BTSerial.available()){
      String incomingData = readStringFromSerial();
      switch(strToLong(incomingData)){
        
        case 0: // Turn on
          turnOnLeds(currentColor);
          break;

        case 1: // Turn off
          turnOffLeds();
          break;

        case 2: // Reset
          resetArduino();
          break;
      }
    }
}


/////////////// LED METHODS ///////////////

void turnOnLeds(long color){
  if(currentPattern == TURN_OFF){
    color  *= -1;
    for (int i = 0 ; i < NUM_LEDS ; i++) {
      leds[i].setRGB(getRed(color), getGreen(color), getBlue(color));
    }

    for (int i = 1 ; i <= BRIGHTNESS ; i++) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(10);
    }
    currentPattern = ONE_COLOR;
    write("Leds on!");
  }
}

void turnOffLeds(){
    for(int i  = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
    currentPattern = TURN_OFF;
    write("Leds off!");
}


/////////////// MY METHODS ///////////////

// Methods read data from Serial when some information iscomming
String readStringFromSerial(){
    String data = "";
    if(BTSerial.available()){
        data = BTSerial.readStringUntil('\n');
    }
    return data;
}


// Read and set data
void initStartingData(){
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    
    while(!BTSerial.available()){}
    delay(100);

    RESET_PIN = (byte) strToLong(readStringFromSerial());
    initResetPin();

    delay(100);
    BRIGHTNESS = (byte) strToLong(readStringFromSerial());

    delay(100);
    currentColor = strToLong(readStringFromSerial());

    delay(100);
    String turnOnLed = readStringFromSerial();

    if(turnOnLed == "true"){
        turnOnLeds(currentColor);
    }
}

// Init reset pin
void initResetPin(){
    digitalWrite(RESET_PIN,HIGH);
    delay(200);
    pinMode(RESET_PIN, OUTPUT);
}

// Arduino reset method
void resetArduino(){
    write("Resetting the arduino!");
    delay(200);
    digitalWrite(RESET_PIN,LOW);
}

// Write data to device 
void write(String txt){
    BTSerial.println(txt);
}

// ================================= //


/////////////// CONVERT'S METHODS ///////////////

// Function converting string to long
long strToLong(String data){
    return (long) strtol(data.c_str(), NULL, 0);
}

// Retrieve color units from long
unsigned int getRed(long x) {
  return (x & 0x00ff0000) >> 16;
}

unsigned int getGreen(long x) {
  return (x & 0x0000ff00) >> 8;
}

unsigned int getBlue(long x) {
  return (x & 0x000000ff);
}

// ==============================================