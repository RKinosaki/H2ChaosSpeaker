#include "task.h"
#include "U8g2lib.h"
#include "config.h"
#include <array>

TaskHandle_t displayHandle = nullptr;


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

void setupOLED(){
    u8g2.setFont(u8g2_font_spleen16x32_mf);
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
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        const char* currentLevel = boardState.currentLevel->name;
      xSemaphoreGive(boardState.mutex);
      u8g2.clearBuffer();
      u8g2.drawStr(0, 26, "LevelID:");
      u8g2.drawStr(0, 60, currentLevel);
      u8g2.sendBuffer();
    }
}

