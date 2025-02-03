#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define X_PIN 8
#define Y_PIN 7
#define Z_PIN 9

float x, y, z = 0;
float prevX, prevY, prevZ;
int levelIdx = 0;
int deadzone = 100;
String levelNames[3] = {"Default", "Volume", "Voices"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(X_PIN,INPUT);
  pinMode(Y_PIN,INPUT);
  pinMode(Z_PIN,INPUT_PULLUP);
  Serial.println("Starting...");

  
}


float inDeadZone(float prevValue, float value, int deadzone){ //checks if the value is outside the deadzone, in which case the value is updated
  float JSVal;
  if((value>prevValue + deadzone)||(value<prevValue - deadzone)){ //checks if the value is outside the deadzone
    JSVal = value;
  }
  else{ //if it is inside the deadzone, it will keep the previous value
    JSVal = prevValue;
  }
  return JSVal;
}

char JSReadDirection(float prevValue, float value, int deadzone, bool axis){//axis True = x direction, False = y direction
  char JSDirection;
  if((value>prevValue + deadzone)&& axis){ //checks if the value is outside the deadzone
    JSDirection = 'r';
  }
  else if ((value>prevValue + deadzone)&& !axis){
    JSDirection = 'u';
  }
  else if ((value<prevValue - deadzone)&& axis){
    JSDirection = 'l';
  }
  else if ((value<prevValue - deadzone)&& !axis){
    JSDirection = 'd';
  }
  else{ //if it is inside the deadzone, it will keep the previous value
    JSDirection = 'n';
  }
  return JSDirection;
}

void changeMenuLevel(char direction){
  // Serial.println(direction);
  // Serial.println("Before change: " + levelNames[levelIdx]);
  if (direction == 'r'){
    levelIdx == 2 ? levelIdx = 0 : levelIdx += 1; 
    delay(250);
  }
  else if (direction == 'l'){
    levelIdx == 0 ? levelIdx = 2: levelIdx -= 1; 
    delay(250);
  }
  // Serial.println("After change: " + levelNames[levelIdx]);
}

void loop() {
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  prevX = x;
  prevY = y;
  prevZ = z;
  //Reads the current values //Deadzone calculation, check inDeadZone function
  x = inDeadZone(prevX, analogRead(X_PIN), deadzone);
  y = inDeadZone(prevY, analogRead(Y_PIN), deadzone);
  z = digitalRead(Z_PIN);
  
  // Serial.println(levelNames[levelIdx]);
  changeMenuLevel(JSReadDirection(prevX, x, deadzone, true));

  // Serial.print(">levelIdx:");
  // Serial.println(levelIdx);

  
  // Serial.print(">x:");
  // Serial.println(x);
  // Serial.print(">y:");
  // Serial.println(y);
  // Serial.print(">z:");
  // Serial.println(z);
  delayMicroseconds(500);

}