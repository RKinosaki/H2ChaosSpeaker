#include "task.h"
#include "U8g2lib.h"
#include "config.h"
#include <array>
#include "DYPlayerESP32.h"
#include "track.h"

TaskHandle_t displayHandle = nullptr;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

int alignCentre(const char* t){
  return (u8g2.getDisplayWidth() - u8g2.getStrWidth(t))/2;
}

int alignRight(const char* t){
  return u8g2.getDisplayWidth() - u8g2.getStrWidth(t);
}

void displayDefault(MenuLevel& level){
    uint8_t currentTrack = boardState.currentTrack;
    uint8_t currentVoltage = boardState.currentVoltage;
    uint8_t currentVolume = boardState.currentVolume;
  xSemaphoreGive(boardState.mutex);
  u8g2.setFont(u8g2_font_open_iconic_play_2x_t);
  // Volume Symmbol
  if(currentVolume>6){
    u8g2.drawGlyph(0, u8g2.getDisplayHeight()-16, 0x004F);
  }
  else if (currentVolume==0){
    u8g2.drawGlyph(0, u8g2.getDisplayHeight()-16, 0x0051);
  }
  else{
    u8g2.drawGlyph(0, u8g2.getDisplayHeight()-16, 0x0050);
  }

  //Track symbol
  u8g2.drawGlyph(u8g2.getDisplayWidth()/2, u8g2.getDisplayHeight()-16, 0x0040); 

  // Play symbol
  if(level.selected){
    u8g2.drawGlyph(u8g2.getDisplayWidth()/2, 24, 0x0044);
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      boardState.playAudio = true;  
    xSemaphoreGive(boardState.mutex);
  }
  else{
    u8g2.drawGlyph(u8g2.getDisplayWidth()/2, 24, 0x0045);
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      boardState.playAudio = false;  
    xSemaphoreGive(boardState.mutex);
  }

  // Battery symbol

  u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
  if(currentVoltage>30){
    u8g2.drawGlyph(0, 24, 0x0049);
  }
  else{
    u8g2.drawGlyph(0, 24, 0x0040);
  }

  u8g2.setFont(u8g2_font_spleen6x12_me);
  u8g2.setDrawColor(0);
  //Select value
  if(level.selected){
    u8g2.drawStr(u8g2.getDisplayWidth()/2+24, 20, "STOP");
  }
  else{
    u8g2.drawStr(u8g2.getDisplayWidth()/2+24, 20, "PLAY");
  }
  u8g2.setDrawColor(1);

    //Voltage value
  char bufB[4];
  itoa(currentVoltage, bufB, 10);
  u8g2.drawStr(24, 20, bufB);

  u8g2.setFont(u8g2_font_spleen12x24_me);

  //Volume value
  char bufV[4];
  itoa(currentVolume, bufV, 10);
  u8g2.drawStr(24, u8g2.getDisplayHeight()-16, bufV);
  // player.setVolume(currentVolume);


  //Track value
  char bufT[4];
  itoa(currentTrack, bufT, 10);
  u8g2.drawStr((u8g2.getDisplayWidth()/2)+24, u8g2.getDisplayHeight()-16, bufT);
  
}

void drawLeftArrow(int xString){
  int offset = 8;
  u8g2.drawTriangle(xString-offset, 24, xString-offset, 0, xString-19, 12);
}

void drawRightArrow(int xString){
  int offset = 8;
  u8g2.drawTriangle(xString + offset, 24, xString+offset, 0, xString+19, 12);
}

void displayVolume(MenuLevel& level){
  uint8_t currentVolume = boardState.currentVolume;
  xSemaphoreGive(boardState.mutex);
  //Title
  int titleXLeft = alignCentre(level.name);
  int titleXRight = titleXLeft + u8g2.getStrWidth(level.name);
  if (level.selected){
    u8g2.setDrawColor(1);
    u8g2.drawBox(0 , 0, u8g2.getDisplayWidth(), 28);

    u8g2.setDrawColor(0);
    u8g2.drawStr(titleXLeft, 20, level.name);
    drawLeftArrow(titleXLeft);
    drawRightArrow(titleXRight);

    u8g2.setDrawColor(1);
  }
  else{
    u8g2.drawStr(titleXLeft, 20, level.name);
    drawLeftArrow(titleXLeft);
    drawRightArrow(titleXRight);
  }
  
  //Volume Bar
  u8g2.setFontMode(1);
  u8g2.setDrawColor(2);
  int boxWidth = currentVolume*u8g2.getDisplayWidth()/level.valueMax;
  u8g2.drawBox(0, 36, boxWidth, 30);
  char buf[4];
  // Serial.println(currentVolume);
  itoa(currentVolume, buf, 10);
  u8g2.drawStr(alignRight(buf), 57, buf);
  player.setVolume(3*currentVolume);
  u8g2.setFontMode(0);

}

void displayTracks(MenuLevel& level){
  uint8_t currentTrack = boardState.currentTrack;
  xSemaphoreGive(boardState.mutex);

  //Title Row
  int titleXLeft = alignCentre(level.name);
  int titleXRight = titleXLeft + u8g2.getStrWidth(level.name);
  if (level.selected){
    u8g2.setDrawColor(1);
    u8g2.drawBox(0 , 0, u8g2.getDisplayWidth(), 28);

    u8g2.setDrawColor(0);
    u8g2.drawStr(titleXLeft, 20, level.name);
    drawLeftArrow(titleXLeft);
    drawRightArrow(titleXRight);

    u8g2.setDrawColor(1);
  }
  else{
    u8g2.drawStr(titleXLeft, 20, level.name);
    drawLeftArrow(titleXLeft);
    drawRightArrow(titleXRight);
  }

  //Track display
  char buf[6];
  itoa(currentTrack, buf, 10);
  strcat(buf, ":");
  u8g2.drawStr(0, 60, buf);
  u8g2.drawStr(alignRight(trackDescription[currentTrack-1]),  60, trackDescription[currentTrack-1]);
}

void displayEyes(MenuLevel& level){
  u8g2.drawStr(alignCentre(level.name), 26, level.name);
}

void setupOLED(){
    u8g2.setFont(u8g2_font_spleen12x24_me);
    u8g2.setDrawColor(1);
    u8g2.begin();
};

void displayOLED(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / DISPLAY_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    setupOLED();
    int taskcount = 0;
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      u8g2.clearBuffer();
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        // Serial.println(boardState.currentLevel->id);
        boardState.currentLevel->displayLevel();
      //xSemaphoreGive(boardState.mutex); this is done within each function so no need
      // Serial.print("Task count: ");
      // Serial.println(taskcount);
      u8g2.sendBuffer();
      // taskcount++;
    }
}

