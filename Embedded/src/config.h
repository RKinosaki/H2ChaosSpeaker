#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// === TASKS === //
#define HARDWARE_TIMER_PRESCALER 80

//DisplayData Task
#define DISPLAY_DATA true
#define DISPLAY_FREQ 100
#define DISPLAY_PRIORITY 1

#define BAT_PIN 2

//Joystick Task
#define READ_JOYSTICK true
#define JOYSTICK_FREQ 10
#define JOYSTICK_PRIORITY 2
#define LOCK_DURATION 8

//Servo task
#define MOVE_SERVO true
#define SERVO_FREQ .2
#define SERVO_PRIORITY 3

#define deadzone 200

//Audio task
#define PLAY_AUDIO true
#define AUDIO_FREQ 2
#define AUDIO_PRIORITY 4

//PINS
#define SCL 9
#define SDA 8

#define X_PIN 4
#define Y_PIN 5
#define Z_PIN 6

#define S0 12
#define S1 11
#define sDummy 10
#define servoMinUs 1000
#define servoMaxUs 2000


#endif