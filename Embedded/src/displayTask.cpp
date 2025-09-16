#include "task.h"
#include "U8g2lib.h"
#include "config.h"

TaskHandle_t displayHandle = nullptr;


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);
const char* levelNames[4] = {"Default", "Volume", "Voices", "Eyes"};
const int levelLength = sizeof(levelNames)/sizeof(levelNames[0]);

int alignLeft(const char* text){
    return 0;
}

int alignCentre(const char* text){
    return (u8g2.getDisplayWidth()-u8g2.getUTF8Width(text))/2;
}

int alignRight(const char* text){
    return u8g2.getDisplayWidth()-u8g2.getUTF8Width(text);
}

void setupOLED(){
    u8g2.setFont(u8g2_font_spleen16x32_mf);
    u8g2.begin();
};

char JSReadDirection(){
    char JSDirection;
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    uint8_t xValue = boardState.joyX;
    uint8_t yValue = boardState.joyY;
    xSemaphoreGive(boardState.mutex);
    if(abs(xValue)>abs(yValue)){
        if(xValue>deadzone){
            JSDirection = 'r';
        }
        else if(xValue<-deadzone){
            JSDirection = 'l';
        }
        else{
            JSDirection = 'n';
        }
    }
    else{
        if(yValue>deadzone){
            JSDirection = 'u';
        }
        else if(yValue<-deadzone){
            JSDirection = 'd';
        }
        else{
            JSDirection = 'n';
        }
    }
    return JSDirection;
}

char SerialReadDirection(){
    String command = Serial.readStringUntil('\n');
    Serial.print("Character read as: ");
    Serial.println(command[0]);
    return command[0];
}

void changeMenuLevel(char direction){
  // Serial.println(direction);
  // Serial.println("Before change: " + levelNames[levelIdx]);
  xSemaphoreTake(boardState.mutex, portMAX_DELAY);
  //   Serial.println(levelLength);
  int levelIdx = boardState.levelID;
  //   Serial.println(levelIdx);
  xSemaphoreGive(boardState.mutex);
  if (direction == 'r'){
    (levelIdx == levelLength-1) ? levelIdx = 0 : levelIdx += 1; 
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  else if (direction == 'l'){
    (levelIdx == 0) ? levelIdx = levelLength-1 : levelIdx -= 1; 
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  else{
    levelIdx = levelIdx;
  }
  xSemaphoreTake(boardState.mutex, portMAX_DELAY);
  boardState.levelID = levelIdx;
  xSemaphoreGive(boardState.mutex);
  // Serial.println("After change: " + levelNames[levelIdx]);
}

void displayOLED(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / DISPLAY_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    setupOLED();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      changeMenuLevel(SerialReadDirection());
      u8g2.clearBuffer();
      u8g2.drawStr(0, 26, "LevelID:");
      u8g2.drawStr(0, 60, levelNames[boardState.levelID]);
      u8g2.sendBuffer();
    }
}

