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

    void (*action)(MenuLevel&);

    // Constructor
    MenuLevel(const char* n, uint8_t i, bool sel = false, uint8_t val = 0, uint8_t vm = 0, void (*act)(MenuLevel&) = nullptr)
        : name(n), id(i), selected(sel), value(val), valueMax(vm), next(nullptr), prev(nullptr), action(act) {}

    void displayLevel(){
        if (action){
            action(*this);
        }
    }
};

//initialise level classes
extern MenuLevel levelDefault;
extern MenuLevel levelVoices;
extern MenuLevel levelVolume;
extern MenuLevel levelEyes;

void displayDefault(MenuLevel& level);
void displayVoices(MenuLevel& level);
void displayVolume(MenuLevel& level);
void displayEyes(MenuLevel& level);




/*<-----    Shared variables    ----->*/
struct sysState{
    uint8_t currentVoltage = 100;
    MenuLevel* currentLevel;
    SemaphoreHandle_t mutex;
};

extern sysState boardState;

#endif

