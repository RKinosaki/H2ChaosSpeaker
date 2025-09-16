#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "task.h"
#include "config.h"
#include "Wire.h"

sysState boardState;


float x, y, z = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  pinMode(Z_PIN, INPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);

  Serial.println("Starting...");

  boardState.mutex = xSemaphoreCreateMutex();

  //Create Tasks

  #if DISPLAY_DATA
    xTaskCreate(
      displayOLED,        //Function Name
      "OLED",              //Text Name
      5000,               //Stack size (bytes)
      NULL,               //Parameters
      DISPLAY_PRIORITY,       // Priority
      &displayHandle        // Pointer
    );
  #endif

  #if READ_JOYSTICK
    xTaskCreate(
      controlJoystick,        //Function Name
      "Joystick",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      JOYSTICK_PRIORITY,       // Priority
      &joystickHandle        // Pointer
    );
  #endif

  #if MOVE_SERVO
    xTaskCreate(
      moveServo,        //Function Name
      "Servo",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      SERVO_PRIORITY,       // Priority
      &servoHandle       // Pointer
    );
  #endif

   #if PLAY_AUDIO
    xTaskCreate(
      playAudio,        //Function Name
      "Audio",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      AUDIO_PRIORITY,       // Priority
      &audioHandle       // Pointer
    );
  #endif
}

void loop() {
  Serial.println("Starting Loop...");
  vTaskDelay(pdMS_TO_TICKS(1000));
}