#include <Arduino.h>
#include "DYPlayerArduino.h"

#define PotRead A0

#define SDA A4
#define SCL A5

// Initialise the player, it defaults to using Serial.
DY::Player player;
bool inputMode = true; //mode = 0 volume, mode = 1 track sets vol on default

void setup() {
  player.begin();
  pinMode(PotRead, INPUT);
}

int readVal(bool mode) {
  float rawVal = analogRead(PotRead);
  if(!mode){ //mode = 0 volume, mode = 1 track
    player.setVolume(int(floor(rawVal*30/1023)));
  }
  else{
    player.playSpecified(floor(rawVal*273/1023));
  }
  
}


void loop() {
  readVal(inputMode);
  // while(button is not pressed){
    delay(10000); //adds delay while the button isn't pressed, making it go indefinitely
  // }


  
  
}
