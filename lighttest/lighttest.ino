#include <FastLED.h>
#include "palettes.h"
#include <AccelStepper.h>


//stepper params
//************
//adjust for string length
const int COW_RAISE = 3600;
//************
const int DIR = 4;
const int READY_PIN = 2;
const int STEP = 15;
const int maxSpeed = 300000;
const int acceleration = 250;

#define LED_PIN 5
#define NUM_LEDS 36   //total LEDS
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define TOP_LED_CT 6
#define BEAMLIGHTS 10

#define motorInterfaceType 1
//set up steper motor
AccelStepper motor(motorInterfaceType, STEP, DIR);

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedPalette_p PROGMEM;
extern const TProgmemPalette16 myBluePalette_p PROGMEM;
extern const TProgmemPalette16 myGreenPalette_p PROGMEM;

bool bounce = false;
bool isBeaming = true;
uint8_t startIndex = 0;
int lightDelay = 300;
int randomNum = random(0, 32);
int limiter = 115;

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Sunset_Real_gp,
  es_rivendell_15_gp,
  es_ocean_breeze_036_gp,
  rgi_15_gp,
  retro2_16_gp,
  Analogous_1_gp,
  es_pinksplash_08_gp,
  Coral_reef_gp,
  es_ocean_breeze_068_gp,
  es_pinksplash_07_gp,
  es_vintage_01_gp,
  departure_gp,
  es_landscape_64_gp,
  es_landscape_33_gp,
  rainbowsherbet_gp,
  gr65_hult_gp,
  gr64_hult_gp,
  GMT_drywet_gp,
  ib_jul01_gp,
  es_vintage_57_gp,
  ib15_gp,
  Fuschia_7_gp,
  es_emerald_dragon_08_gp,
  lava_gp,
  fire_gp,
  Colorfull_gp,
  Magenta_Evening_gp,
  Pink_Purple_gp,
  es_autumn_19_gp,
  BlacK_Blue_Magenta_White_gp,
  BlacK_Magenta_Red_gp,
  BlacK_Red_Magenta_Yellow_gp,
  Blue_Cyan_Yellow_gp
};



void setup() {

  Serial.begin(115200);
  pinMode(READY_PIN, OUTPUT);
  delay(3000);  // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = myRedPalette_p;
  currentBlending = LINEARBLEND;

  delay(5);
  motor.setMaxSpeed(maxSpeed);
  motor.setAcceleration(acceleration);
  motor.setCurrentPosition(0);

  digitalWrite(READY_PIN, HIGH);
}


void loop() {


  if (bounce || isBeaming) {
    startIndex = startIndex + 1;
  } else {
    startIndex = startIndex - 1;
  }

  if (startIndex == 255) {
    bounce = false;
  } else if (startIndex == limiter) {
    bounce = true;
  }




  if (startIndex == limiter) {
    randomNum = random(0, 32);
  }


  fadeUpBeam(startIndex);

  FastLED.show();
  // FastLED.delay(0);

  //stepper runner;
  motor.run();
}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {

  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += 6;
  }
}

void FillLEDsUpload(uint8_t colorIndex) {

  FastLED.setBrightness(BRIGHTNESS);
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += 2;
  }
}

void topLights(CRGB color) {
  for (int i = 0; i < TOP_LED_CT; ++i) {
    leds[i] = color;
  }
  // FastLED.show();
}

void fadeUpBeam(uint8_t count) {
  uint8_t secondHand = (millis() / 1000) % 60;

  static uint8_t beamToggle = 0;
  if (secondHand >= 30 && secondHand <= 48) {  //beam me up light
    Serial.println("Beam Me Up!!");
    FillLEDsUpload(startIndex);
    for (int i = 0; i <= BEAMLIGHTS; i++) {
      if (i == 1)
        leds[NUM_LEDS - i] = randomColor(secondHand);

      leds[NUM_LEDS - i] = CRGB::White;
    }

    topLights(randomColor(secondHand));
    lightDelay = 0;
    isBeaming = true;
    currentPalette = myBluePalette_p;
    // motor.setAcceleration(acceleration);  //raise cow slow
    motor.moveTo(COW_RAISE);
    beamToggle = 1;
  } else {  //waiting light
    FillLEDsFromPaletteColors(startIndex);
    for (int i = 0; i <= BEAMLIGHTS; i++) {
      if (i == 1)
        leds[NUM_LEDS - 1] = randomColor(secondHand);

      leds[NUM_LEDS - i] = CRGB::Black;
    }

    isBeaming = false;
    lightDelay = 10;
    currentPalette = myRedPalette_p;
    randomPalette(secondHand);
    if (beamToggle == 1) {
      Serial.println("Lower ME!!");
      // motor.setAcceleration(acceleration);  //lower faster
      motor.moveTo(0);
      beamToggle = 2;
    }
  }

  FastLED.show();
}



CRGB randomColor(uint8_t secondHand) {

  if (secondHand >= 30 && secondHand <= 32) {
    return CRGB::Green;
  } else if (secondHand > 32 && secondHand <= 38) {
    return CRGB::Blue;
  } else if (secondHand > 38 && secondHand <= 42) {
    return CRGB::Yellow;
  } else if (secondHand >= 42) {
    return CRGB::Red;
  }
}

void randomPalette(uint8_t secondHand) {

  if (secondHand < 10) {
    currentPalette = myGreenPalette_p;
  } else if (secondHand >= 10 && secondHand <= 15) {
    currentPalette = gGradientPalettes[randomNum];
  } else if (secondHand > 15 && secondHand <= 20) {
    currentPalette = gGradientPalettes[randomNum];
  } else if (secondHand > 20 && secondHand <= 30) {
    currentPalette = gGradientPalettes[randomNum];
  } else if (secondHand >= 45 && secondHand <= 50) {
    currentPalette = gGradientPalettes[randomNum];
  }
}
