#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "task.h"
#include "config.h"
#include "Wire.h"
#include "DYPlayerESP32.h"


sysState boardState;


float x, y, z = 0;

MenuLevel levelDefault("Default", 0);
MenuLevel levelVolume("Volume", 1, false, 10);
MenuLevel levelVoices("Voices", 2, false, 20);
MenuLevel levelEyes("Eyes", 3);
DY::Player player(UART_NUM_0, TX, RX);

void setupDisplayFunctions(){
  levelDefault.action = displayDefault;
  levelVoices.action = displayTracks;
  levelVolume.action = displayVolume;
  levelEyes.action = displayEyes;
}

void setupFSMRelations(){
    levelDefault.next = &levelVolume;
    levelDefault.prev = &levelEyes;

    levelVolume.next = &levelVoices;
    levelVolume.prev = &levelDefault;

    levelVoices.next = &levelEyes;
    levelVoices.prev = &levelVolume;

    levelEyes.next = &levelDefault;
    levelEyes.prev = &levelVoices;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  setupDisplayFunctions();
  setupFSMRelations();
  pinMode(Z_PIN, INPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);

  Serial.println("Starting...");

  boardState.currentLevel = &levelDefault;
  boardState.mutex = xSemaphoreCreateMutex();

  //Create Tasks

  #if DISPLAY_DATA
    BaseType_t res = xTaskCreate(
      displayOLED,        //Function Name
      "OLED",              //Text Name
      5000,               //Stack size (bytes)
      NULL,               //Parameters
      DISPLAY_PRIORITY,       // Priority
      &displayHandle        // Pointer
    );
    Serial.printf("Task created: %d\n", res);
  #endif

  #if READ_JOYSTICK
     res = xTaskCreate(
      controlJoystick,        //Function Name
      "Joystick",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      JOYSTICK_PRIORITY,       // Priority
      &joystickHandle        // Pointer
    );
    Serial.printf("Task created: %d\n", res);
  #endif

  #if MOVE_SERVO
    res = xTaskCreate(
      moveServo,        //Function Name
      "Servo",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      SERVO_PRIORITY,       // Priority
      &servoHandle       // Pointer
    );
    Serial.printf("Task created: %d\n", res);
  #endif

   #if PLAY_AUDIO
    res = xTaskCreate(
      playAudio,        //Function Name
      "Audio",              //Text Name
      2500,               //Stack size (bytes)
      NULL,               //Parameters
      AUDIO_PRIORITY,       // Priority
      &audioHandle       // Pointer
    );
    Serial.printf("Task created: %d\n", res);
  #endif
}

void loop() {
  Serial.println("Starting Loop...");
  vTaskDelay(pdMS_TO_TICKS(1000));
}