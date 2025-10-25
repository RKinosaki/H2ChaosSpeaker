#include "task.h"
#include "U8g2lib.h"
#include "config.h"
#include <array>
#include "DYPlayerESP32.h"
#include "track.h"
#include "firstPage.h"

TaskHandle_t displayHandle = nullptr;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);


int alignCentre(const char* t){
  return (u8g2.getDisplayWidth() - u8g2.getStrWidth(t))/2;
}

int alignRight(const char* t){
  return u8g2.getDisplayWidth() - u8g2.getStrWidth(t);
}


void displayDefault(MenuLevel& level){
  u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
  uint8_t glyph;
  boardState.isSelected ? glyph = 0x006F : glyph = 0x0076;
  u8g2.drawGlyph(u8g2.getDisplayWidth()/2-6, 24, glyph);
  
}

void displayTracks(MenuLevel& level){
  u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
  uint8_t glyph;
  boardState.isSelected ? glyph = 0x006F : glyph = 0x0076;
  u8g2.drawGlyph(u8g2.getDisplayWidth()/2-6, u8g2.getDisplayHeight()-16, glyph);
}

void displayVolume(MenuLevel& level){
  u8g2.setFont(u8g2_font_open_iconic_all_2x_t); 
  uint8_t glyph;
  boardState.isSelected ? glyph = 0x006F : glyph = 0x0076;
  u8g2.drawGlyph(0, u8g2.getDisplayHeight()-16, glyph);
}

void displayEyes(MenuLevel& level){
  u8g2.clearBuffer();
  u8g2.drawStr(alignCentre(level.name), 20, level.name);
  int8_t angle = boardState.currentAngle;
  u8g2.setFont(u8g2_font_open_iconic_human_4x_t);
  uint8_t offset = (u8g2.getDisplayWidth()-40)*(angle+90)/180;
  u8g2.drawGlyph(5 + offset, u8g2.getDisplayHeight()/2+30, 0x0040);
}

void displayMenu(){
  const uint8_t xShift = 16;
  xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    int8_t currentTrack = boardState.currentTrack;
    int8_t currentVolume = boardState.currentVolume;
    bool currentSelected = boardState.isSelected;
    bool currentPlayAudio = boardState.playAudio;
  xSemaphoreGive(boardState.mutex);
  u8g2.setFont(u8g2_font_open_iconic_play_2x_t);
  // Volume Symmbol
  if(currentVolume>6){
    u8g2.drawGlyph(xShift, u8g2.getDisplayHeight()-16, 0x004F);
  }
  else if (currentVolume==0){
    u8g2.drawGlyph(xShift, u8g2.getDisplayHeight()-16, 0x0051);
  }
  else{
    u8g2.drawGlyph(xShift, u8g2.getDisplayHeight()-16, 0x0050);
  }

  //Track symbol
  u8g2.drawGlyph(u8g2.getDisplayWidth()/2-6+xShift, u8g2.getDisplayHeight()-16, 0x0040); 

  // Play symbol
  if(currentPlayAudio){
    u8g2.drawGlyph(u8g2.getDisplayWidth()/2-6+xShift, 24, 0x0044);
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      boardState.playAudio = true;  
    xSemaphoreGive(boardState.mutex);
  }
  else{
    u8g2.drawGlyph(u8g2.getDisplayWidth()/2-6+xShift, 24, 0x0045);
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      boardState.playAudio = false;  
    xSemaphoreGive(boardState.mutex);
  }

  u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
  u8g2.drawGlyph(u8g2.getDisplayWidth()/2-10, u8g2.getDisplayHeight()+6, 0x006D);
  

  // Battery symbol

  u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
  int currentVoltage = 100*analogRead(BAT_PIN)/4096;

  if(currentVoltage>30){
    u8g2.drawGlyph(xShift, 24, 0x0049);
  }
  else{
    u8g2.drawGlyph(xShift, 24, 0x0040);
  }

  u8g2.setFont(u8g2_font_spleen6x12_me);
  //Select value
  if(currentPlayAudio){
    u8g2.drawStr(u8g2.getDisplayWidth()/2+18+xShift, 20, "STOP");
  }
  else{
    u8g2.drawStr(u8g2.getDisplayWidth()/2+18+xShift, 20, "PLAY");
  }
  u8g2.setDrawColor(1);

  //Voltage value
  char bufB[4];
  itoa(currentVoltage, bufB, 10);
  u8g2.drawStr(18+xShift, 20, bufB);

  u8g2.setFont(u8g2_font_spleen12x24_me);

  //Volume value
  char bufV[4];
  itoa(currentVolume, bufV, 10);
  u8g2.drawStr(18+xShift, u8g2.getDisplayHeight()-16, bufV);
  // player.setVolume(currentVolume);


  //Track value
  char bufT[4];
  itoa(currentTrack+1, bufT, 10);
  u8g2.drawStr((u8g2.getDisplayWidth()/2)+18+xShift, u8g2.getDisplayHeight()-16, bufT);
  
}

void setupOLED(){
    u8g2.setFont(u8g2_font_spleen12x24_me);
    u8g2.setDrawColor(1);
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.drawBitmap(0, 0, 16, 64, logo_bits);
    u8g2.sendBuffer();
    delay(2000);

}

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
        // Serial.println(boardState.currentLevel->id);
        displayMenu();
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.currentLevel->displayLevel();
        xSemaphoreGive(boardState.mutex);
      u8g2.sendBuffer();
    }
}

