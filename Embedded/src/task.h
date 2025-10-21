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
public:
    const char* name;
    uint8_t id;
    uint8_t valueMax;
    uint8_t step;

    MenuLevel* up;
    MenuLevel* down;
    MenuLevel* right;
    MenuLevel* left;

    void (*action)(MenuLevel&);

    // Constructor
    MenuLevel(const char* n, uint8_t i, bool sel = false,  uint8_t vm = 0, uint8_t s = 1, void (*act)(MenuLevel&) = nullptr)
        : name(n), id(i), valueMax(vm), step(s), up(nullptr), down(nullptr), left(nullptr), right(nullptr), action(act) {}

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
void displayTracks(MenuLevel& level);
void displayVolume(MenuLevel& level);
void displayEyes(MenuLevel& level);

namespace DY{
    class Player;}

extern DY::Player player;




/*<-----    Shared variables    ----->*/
struct sysState{
    uint8_t currentVoltage = 100;
    MenuLevel* currentLevel;
    uint8_t currentTrack = 0;
    uint8_t currentVolume = 3;
    uint8_t currentAngle = 0;
    bool isSelected = false;
    bool playAudio = false;
    bool moveServo = false;
    SemaphoreHandle_t mutex;
};

extern sysState boardState;

#endif

