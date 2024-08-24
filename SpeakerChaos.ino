#include <Arduino.h>
#include "DYPlayerArduino.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PotRead A0
#define PlayToggle 10
#define VolTrackToggle 5

// Initialise the player, it defaults to using Serial.
DY::Player player;
bool inputMode = true; //mode = 0 volume, mode = 1 track sets vol on default
int vol = 10;
int track = 1;
int currPot;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;); // Don't proceed, loop forever
  }
  player.begin();
  pinMode(PotRead, INPUT);
  pinMode(PlayToggle, INPUT_PULLUP);
  pinMode(VolTrackToggle, INPUT_PULLUP);
  display.display();
  delay(10);
  display.clearDisplay();
}

int readVal(bool mode, int currVol, int currTrack) {
  float rawVal = analogRead(PotRead);
  if(!mode){ //mode = 0 volume, mode = 1 track
    int Vol = map(rawVal, 0, 1023, 0, 30);
    player.playSpecified(currTrack);
    player.setVolume(Vol);
    return Vol;
  }
  else{
    int Track = map(rawVal, 0, 1023, 0, 273);
    player.setVolume(currVol);
    player.playSpecified(Track);
    return Track;
  } 
  
}

void updateInfo(int mode, int Vol, int Track){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(mode ? 1:2);
  display.setCursor(0, 0);
  display.println("Vol:" + String(Vol));
  display.setTextSize(mode ? 2:1);
  display.setCursor(0, (mode ? 16 : 20));
  display.println("Track:" + String(Track));
  display.display();
  delay(10);
}


void loop() {
  updateInfo(inputMode, vol, track);
  delay(100);
  if(!digitalRead(VolTrackToggle)){
    currPot = analogRead(PotRead);
    inputMode = !inputMode;
  }
  if(currPot<analogRead(PotRead)-40 || analogRead(PotRead)+40<currPot){
    if(inputMode){
      track = readVal(inputMode, vol, track);
    }
    else{
      vol = readVal(inputMode, vol, track);
    }
  }  
  while(!digitalRead(PlayToggle)){
    player.setVolume(vol);
    delay(1000); //adds delay while the button isn't pressed, making it go indefinitely
  }


  
  
}
