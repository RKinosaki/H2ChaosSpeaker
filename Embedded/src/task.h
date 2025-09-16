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

class MenuLevel {
public:   // <--- make constructor & members accessible
    const char* name;
    uint8_t id;
    bool selected;
    uint8_t value;
    uint8_t valueMax;

    MenuLevel* next;
    MenuLevel* prev;

    // Constructor
    MenuLevel(const char* n, uint8_t i, bool sel = false, uint8_t val = 0, uint8_t vm = 0)
        : name(n), id(i), selected(sel), value(val), valueMax(vm), next(nullptr), prev(nullptr) {}
};

//initialise level classes
extern MenuLevel levelDefault;
extern MenuLevel levelVoices;
extern MenuLevel levelVolume;
extern MenuLevel levelEyes;




/*<-----    Shared variables    ----->*/
struct sysState{
    MenuLevel* currentLevel = &levelDefault;
    SemaphoreHandle_t mutex;
};

extern sysState boardState;

#endif

