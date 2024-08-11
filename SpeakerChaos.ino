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

// Initialise the player, it defaults to using Serial.
DY::Player player;
bool inputMode = false; //mode = 0 volume, mode = 1 track sets vol on default

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;); // Don't proceed, loop forever
  }
  player.begin();
  pinMode(PotRead, INPUT);
  display.display();
  delay(10);
  display.clearDisplay();
}

int readVal(bool mode) {
  float rawVal = analogRead(PotRead);
  if(!mode){ //mode = 0 volume, mode = 1 track
    int Vol = map(rawVal, 0, 1023, 0, 30);
    player.playSpecified(1);
    player.setVolume(Vol);
    return Vol;
  }
  else{
    int Track = map(rawVal, 0, 1023, 0, 273);
    player.setVolume(10);
    player.playSpecified(Track);
    return Track;
  } 
  
}

void updateInfo(bool mode){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  if(!mode){
    int rawVal = int(floor(analogRead(PotRead)*30/1023));
    display.println("Vol:" + String(rawVal));
  }
  else{
    float rawVal = analogRead(PotRead);
    int newVal = int(floor(rawVal*273/1023));
    display.println("Track:" + String(newVal));
  }
  
  display.display();
  delay(10);
}


void loop() {
  updateInfo(inputMode);
  readVal(inputMode);
  delay(100);
  updateInfo(inputMode);
  delay(100);
  
  
  // while(button is not pressed){
    //add the display updates here as well, 
    // delay(10000); //adds delay while the button isn't pressed, making it go indefinitely
  // }


  
  
}
