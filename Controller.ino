#include <SoftwareSerial.h>
#include <FastLED.h>


SoftwareSerial BTSerial(1,0);

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120
#define NUM_LEDS    30
#define DATA_PIN    12
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];


byte BRIGHTNESS = 0;
byte RESET_PIN = 5;
uint8_t gHue = 0;

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
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    turnOffLeds();

    BTSerial.begin(9600);
    initStartingData();
}

// ==== NUMBER DESCRIPTIONS ====
// 0 - turn on leds
// 1 - turn off leds
// 2 - reset Arduino
// 3 - brightness
// 4 - one color !
// 5 - rainbow !
// 6 - rainbow with glitter !
// 7 - confetti !
// 8 - knightrider !
// 9 - colored knight rider !
// 10 - bpm !
// 11 - juggle !
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

        case 3: // Brightness
          setBrightness();
          break;

        case 4: // One color
          write("Running a pattern ONE COLOR");
          oneColor(readColorFromDevice());
          break;

        case 5: // Rainbow
          write("Running a pattern RAINBOW");
          rainbow();
          break;

        case 6: // Rainbow with glitter
          write("Running a pattern RAINBOW WITH GLITTER");
          rainbowWithGlitter();
          break;

        case 7: // Confetti
          write("Running a pattern CONFETTI");
          confetti();
          break;

        case 8: // Knightrider
          write("Running a pattern KNIGHTRIDER");
          knightRider();
          break;

        case 9: // Colored knightrider
          write("Running a pattern COLORED KNIGHTRIDER");
          coloredKnightRider();
          break;

        case 10: // BPM
          write("Running a pattern BPM");
          bpm();
          break;

        case 11: // JUGGLE
          write("Running a pattern JUGGLE");
          juggle();
          break;

        case 12: // FADED
          write("Running a pattern FADED");
          faded();
          break;

        case 13: // BREATHING
          write("Running a pattern BREATHING");
          breathing(readColorFromDevice());
          break;

        default:
          write("Unknown operation!");
        break;
      }
    }
}


/////////////// LED METHODS (PATTERNS) ///////////////
void turnOnLeds(long color){
  if(currentPattern == TURN_OFF){
    for (int i = 0 ; i < NUM_LEDS ; i++) {
      leds[i].setRGB(getRed(color), getGreen(color), getBlue(color));
    }

    for (int i = 1 ; i <= BRIGHTNESS ; i++) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(10);
    }
    currentPattern = ONE_COLOR;
    currentColor = color;
    write("Leds on!");
  }
}

void turnOffLeds(){
    for(int i  = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(50);
    }
    currentPattern = TURN_OFF;
    write("Leds off!");
}

void oneColor(long color) {
  int r = getRed(color);
  int g = getGreen(color);
  int b = getBlue(color);
  for (int i = 0; i <= NUM_LEDS / 2; i++) {
    leds[i].setRGB(r, g, b);
    leds[NUM_LEDS - i].setRGB(r, g, b);
    FastLED.show();
    delay(100);
  }
  currentPattern = ONE_COLOR;
  currentColor = color;
}

void setBrightness(){
  BRIGHTNESS = ((byte) strToLong(readStringFromSerial())) * 2.5;
  if(FastLED.getBrightness() > BRIGHTNESS){
      for(int i = FastLED.getBrightness(); i >= BRIGHTNESS; i--){
        FastLED.setBrightness(i);
        FastLED.show();
        delay(25);
      }
  } else {
      for(int i = FastLED.getBrightness(); i <= BRIGHTNESS; i++){
        FastLED.setBrightness(i);
        FastLED.show();
        delay(25);
      }
  }
  write("The brightness has been set!");
  startPattern();
}

void rainbow() {
  currentPattern = RAINBOW;
  while (!BTSerial.available()) {
    showAndDelay();
    fill_rainbow( leds, NUM_LEDS, gHue, 10);
  }
}

void rainbowWithGlitter() {
  currentPattern = RAINBOW_GLITTER;
  while (!BTSerial.available()) {
    showAndDelay();
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
    addGlitter(80);
  }
}

void confetti() {
  currentPattern = CONFETTI;
  while (!BTSerial.available()) {
    showAndDelay();
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
  }
}

void coloredKnightRider() {
  currentPattern = KNIGHRIDER_COLORED;
  while (!BTSerial.available()) {
    showAndDelay();
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
    leds[pos] += CHSV(gHue, 255, 192);
  }
}

void knightRider() {
  currentPattern = KNIGHRIDER;
  while (!BTSerial.available()) {
    showAndDelay();
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
    leds[pos] += CHSV(255, 255, 192);
  }
}

void bpm() {
  currentPattern = BPM;
  while (!BTSerial.available()) {
    showAndDelay();
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { 
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
  }
}

void juggle() {
  currentPattern = JUGGLE;
  while (!BTSerial.available()) {
    showAndDelay();
    fadeToBlackBy( leds, NUM_LEDS, 20);
    uint8_t dothue = 0;
    for ( int i = 0; i < 8; i++) {
      leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }
}

void faded() {
  currentPattern = FADED;
  while (!BTSerial.available()) {
    static uint8_t hue = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue++, 255, 255);
      FastLED.show();
      fadeall();
      delay(20);
    }

    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
      leds[i] = CHSV(hue++, 255, 255);
      FastLED.show();
      fadeall();
      delay(20);
    }
  }
}

void breathing(long color) {
  unsigned int r = getRed(color);
  unsigned int g = getGreen(color);
  unsigned int b = getBlue(color);

  currentPattern = BREATHING;
  currentColor = color;

  for (int i = 0; i <= NUM_LEDS / 2; i++) {
    leds[i].setRGB(r, g, b);
    leds[NUM_LEDS - i].setRGB(r, g, b);
    FastLED.show();
    delay(50);
  }

  while (!Serial.available()) {
    for (int i = FastLED.getBrightness(); i < 256; i++) {
      FastLED.setBrightness(i);
      FastLED.show();
      if (BTSerial.available()) break;
      delay(10);
    }
    for (int i = FastLED.getBrightness(); i >= 0; i--) {
      FastLED.setBrightness(i);
      FastLED.show();
      if (BTSerial.available()) break;
      delay(10);
    }
  }
}


void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
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
    while(!BTSerial.available()){}
    delay(100);

    RESET_PIN = (byte) strToLong(readStringFromSerial());
    initResetPin();

    delay(100);
    BRIGHTNESS = ((byte) strToLong(readStringFromSerial())) * 2.5;

    delay(100);
    currentColor = strToLong(readStringFromSerial()) * -1;

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


void showAndDelay() {
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
}
//

long readColorFromDevice(){
  return strToLong(readStringFromSerial()) * -1;
}

void startPattern(){
 switch(currentPattern){
    case ONE_COLOR:
      oneColor(currentColor);
      break;

    case RAINBOW:
      rainbow();
      break;

    case RAINBOW_GLITTER:
      rainbowWithGlitter();
      break;

    case CONFETTI:
      confetti();
      break;

    case KNIGHRIDER:
      knightRider();
      break;

    case KNIGHRIDER_COLORED:
      coloredKnightRider();
      break;

    case BPM:
      bpm();
      break;

    case JUGGLE:
      juggle();
      break;

    case FADED:
      faded();
      break; 

    case BREATHING:
      breathing(currentColor);
      break;
 }
}
// =========================================== //


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