#include "task.h"
#include "config.h"
#include "EEPROM.h"

TaskHandle_t joystickHandle = nullptr;

char JSReadDirection(int joyX, int joyY, int joyZ){
    char JSDirection;
    joyX -= 2048;
    joyY -= 2048;
    if (joyZ == 0){
      return 'p';
    }
    else if(abs(joyX)>abs(joyY)){
        if(joyX> deadzone){
            JSDirection = 'r';
        }
        else if(joyX< -deadzone){
            JSDirection = 'l';
        }
        else{
            JSDirection = 'n';
        }
    }
    
    else{
        if(joyY > deadzone){
            JSDirection = 'd';
        }
        else if(joyY<- deadzone){
            JSDirection = 'u';
        }
        else{
            JSDirection = 'n';
        }
    }
   
    // Serial.println(JSDirection);
    return JSDirection;
}

char SerialReadDirection(){
  if(Serial.available()){
    String command = Serial.readStringUntil('\n');
    Serial.print("Character read as: ");
    Serial.println(command[0]);
    return command[0];
  }
  else{
    return 'n';
  }
}


void changeMenuLevel(char direction, uint8_t id){
  if (direction == 'r'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    if (boardState.currentLevel->right != nullptr){
      boardState.currentLevel = boardState.currentLevel->right;
    }
    else{
      boardState.currentLevel = boardState.currentLevel;
    }
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'l'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    if (boardState.currentLevel->left != nullptr){
      boardState.currentLevel = boardState.currentLevel->left;
    }
    else{
      boardState.currentLevel = boardState.currentLevel;
    }
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'u'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    if (boardState.currentLevel->up != nullptr){
      boardState.currentLevel = boardState.currentLevel->up;
    }
    else{
      boardState.currentLevel = boardState.currentLevel;
    }
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'd'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    if (boardState.currentLevel->down != nullptr){
      boardState.currentLevel = boardState.currentLevel->down;
    }
    else{
      boardState.currentLevel = boardState.currentLevel;
    }
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'p'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    boardState.isSelected = true;
    if (id==0){
      boardState.playAudio = true;
    }
    else if(id==3){
      boardState.moveServo = true;
    }
    xSemaphoreGive(boardState.mutex);
    Serial.println("Selected");
  }
  else{
    ;
  }
}

void changeValueLevel(char direction, uint8_t id){
    uint8_t currentValue;
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    if(id == 1){
      currentValue = boardState.currentVolume;
    }
    else if (id==2){
      currentValue = boardState.currentTrack;
    }
    else if (id==3){
      currentValue = boardState.currentAngle;
    }
    else{
      ;
    }
    uint8_t maxValue = boardState.currentLevel->valueMax;
    uint8_t step = boardState.currentLevel->step;
    Serial.println(currentValue);
    xSemaphoreGive(boardState.mutex);
    if (direction == 'r'){
        currentValue == maxValue ? currentValue = 0 : currentValue += step;
        Serial.print("Current value: ");
        Serial.println(currentValue);
    }
    else if (direction == 'l'){
        currentValue == 0 ? currentValue = maxValue : currentValue -= step;
        Serial.print("Current value: ");
        Serial.println(currentValue);
    }
    else if (direction == 'p'){
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.isSelected = false;
        
        switch (id)
        {
        case 0:
          boardState.playAudio = false;
          break;
        
        case 1:
          EEPROM.put(id, boardState.currentVolume);
          EEPROM.commit();
        
        case 2:
          EEPROM.put(id, boardState.currentTrack);
          EEPROM.commit();
        case 3:
          boardState.moveServo = false;
        default:
          break;
        }
        xSemaphoreGive(boardState.mutex);
        Serial.println("Unselected");
    }
    else{
        ;
    }
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      if(id == 1){
        boardState.currentVolume = currentValue;
      }
      else if (id==2){
        boardState.currentTrack = currentValue;
      }
      else if(id==3){
        boardState.currentAngle = currentValue;
      }
      else{
        ;
      }
    xSemaphoreGive(boardState.mutex);
}


void controlJoystick(void* pvParameters){
    (void)pvParameters;
    bool lockThread = false;
    uint8_t threadCounter = 0;
    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      if (!lockThread){
        int joyX = analogRead(X_PIN);
        int joyY = analogRead(Y_PIN);
        int joyZ = digitalRead(Z_PIN);
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        const bool isSelected = boardState.isSelected;
        uint8_t id = boardState.currentLevel->id;
        xSemaphoreGive(boardState.mutex);
        if(isSelected){
          changeValueLevel(JSReadDirection(joyX, joyY, joyZ), id);
        }
        else{
          changeMenuLevel(JSReadDirection(joyX, joyY, joyZ), id);
        }
        lockThread = true;
      }
      else{
        threadCounter++;
        if (threadCounter >= LOCK_DURATION){
          lockThread = false;
        }
        else{
          lockThread = true;
        }
      }
      
    }
}