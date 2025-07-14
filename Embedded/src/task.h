#ifndef TASK_H
#define TASK_H
#include <arduino.h>

/*<------Task Handles------->*/

extern TaskHandle_t joystickHandle;
extern TaskHandle_t displayHandle;
extern TaskHandle_t servoHandle;
extern TaskHandle_t audioHandle;

/*<------Task Functions------->*/
void displayOLED(void *pvParameter);
void controlJoystick(void *pvParameter);
void moveServo(void *pvParameter);
void playAudio(void *pvParameter);


/*<-----    Shared variables    ----->*/
struct sysState{
    uint8_t levelID = 0;
    uint8_t joyX = 0;
    uint8_t joyY = 0;
    uint8_t joyZ = 0;
    bool selectedVolume = false;
    bool selectedDefault = true;
    bool selectedVoices = false;
    bool selectedEyes = false;
    uint16_t track = 0;
    uint8_t volume = 0;
    SemaphoreHandle_t mutex;
};

extern sysState boardState;

#endif

