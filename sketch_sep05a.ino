#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#define NUM_LEDS 100
#define DATA_PIN            6
#define BUTTON_MEET         9  // Button for Meetings
#define BUTTON_WHITE        8  // Button for just white
#define BUTTON_BRIGHT       5  // Button for reset or anything
#define ENCODERA            4
#define ENCODERB            3
#define DELAY               20  // Delay per loop in ms
#define BACKLOG             33
#define WORKING             41
#define DONE                14
CRGB real_leds[NUM_LEDS];
CRGB* leds = real_leds;
uint32_t last_button = 0;
int user_mode = 0;

void setup()
{
  pinMode(DATA_PIN, OUTPUT); 
  pinMode(BUTTON_MEET, INPUT_PULLUP);
  pinMode(BUTTON_WHITE, INPUT_PULLUP);
  pinMode(BUTTON_BRIGHT, INPUT_PULLUP);
  pinMode(ENCODERA, INPUT_PULLUP);
  pinMode(ENCODERB, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  last_button = millis();
  Serial.begin(9600);
}

void loop()
{
  int BRIGHTNESS=255;
  boolean ENCODERALAST=LOW;
  boolean n = LOW;
  if( check_meeting() ){
    for( int i = 0; i < NUM_LEDS; i++ ) {
        if (i<BACKLOG){
          leds[i] = CRGB(255, 0, 0);
          
        }
        if (i<(BACKLOG+WORKING) && i=>BACKLOG){
          leds[i] = CRGB(255, 255, 0);
          Serial.print("blub");
        }
        if (i<(BACKLOG+WORKING+DONE) && i>=(WORKING+BACKLOG)){
          leds[i] = CRGB(0, 128, 0);
        } 
    }
  }
  if (check_white()){
    fill_solid(leds, NUM_LEDS, CRGB(0xFFFFFF));
  }

  if (check_bright()){
    delay(250);
    while (!check_bright()){
      n = digitalRead(ENCODERA);
      if ((ENCODERALAST == LOW) && (n == HIGH)) {
        if (digitalRead(ENCODERB) == LOW) {
          BRIGHTNESS= BRIGHTNESS-5;
          /**if (BRIGHTNESS<0){
            BRIGHTNESS=0;
          }**/
          FastLED.setBrightness(BRIGHTNESS);
        } else {
          BRIGHTNESS=BRIGHTNESS+5;
          /**if (BRIGHTNESS>255){
            BRIGHTNESS=255;
          }**/
          FastLED.setBrightness(BRIGHTNESS);
        }
      }
    ENCODERALAST = n;
    FastLED.show();
    }
  }
  FastLED.show();
}


boolean check_meeting() {
  boolean ret = false;
  if( digitalRead(BUTTON_MEET) == LOW ) {
    if( millis() - last_button > 50 ) {
      ret = true;
    }
    last_button = millis();
  }
  return ret;
}

boolean check_white() {
  boolean ret = false;
  if( digitalRead(BUTTON_WHITE) == LOW ) {
    if( millis() - last_button > 50 ) {
      ret = true;
    }
    last_button = millis();
  }
  return ret;
}

boolean check_bright() {
  boolean ret = false;
  if( digitalRead(BUTTON_BRIGHT) == LOW ) {
    if( millis() - last_button > 50 ) {
      ret = true;
    }
    last_button = millis();
  }
  return ret;
}

