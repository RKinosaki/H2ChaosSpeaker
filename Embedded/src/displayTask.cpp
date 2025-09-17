#include "task.h"
#include "U8g2lib.h"
#include "config.h"
#include <array>

TaskHandle_t displayHandle = nullptr;


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

void displayDefault(MenuLevel& level){
  u8g2.drawStr(0, 26, level.name);

}

void displayVolume(MenuLevel& level){
  u8g2.drawStr(0, 26, level.name);
  u8g2.setDrawColor(0);
  u8g2.drawBox(0, 60, 40, 20);
  char buf[4];
  itoa(level.value, buf, 10);
  u8g2.drawStr(40, 60, buf);
}

void displayVoices(MenuLevel& level){
  u8g2.drawStr(0, 26, level.name);
  u8g2.setDrawColor(1);
  char buf[4];
  itoa(level.value, buf, 10);
  u8g2.drawStr(40, 60, buf);
}

void displayEyes(MenuLevel& level){
  u8g2.drawStr(0, 26, level.name);
}

void setupOLED(){
    u8g2.setFont(u8g2_font_spleen16x32_mf);
    u8g2.setDrawColor(1);
    u8g2.begin();
};

void displayOLED(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / DISPLAY_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    setupOLED();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      u8g2.clearBuffer();
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        const char* currentLevel = boardState.currentLevel->name;
        u8g2.setDrawColor(!boardState.currentLevel->selected);
        boardState.currentLevel->displayLevel();
      xSemaphoreGive(boardState.mutex);
      u8g2.sendBuffer();
    }
}

