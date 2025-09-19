#include "task.h"
#include "config.h"

TaskHandle_t joystickHandle = nullptr;

char JSReadDirection(uint8_t joyX, uint8_t joyY, uint8_t joyZ){
    char JSDirection;
    if(abs(joyX)>abs(joyY)){
        if(joyX>deadzone){
            JSDirection = 'r';
        }
        else if(joyX<-deadzone){
            JSDirection = 'l';
        }
        else{
            JSDirection = 'n';
        }
    }
    else{
        if(joyY>deadzone){
            JSDirection = 'u';
        }
        else if(joyY<-deadzone){
            JSDirection = 'd';
        }
        else{
            JSDirection = 'n';
        }
    }
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
    boardState.currentLevel = boardState.currentLevel->next;
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'l'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    boardState.currentLevel = boardState.currentLevel->prev;
    xSemaphoreGive(boardState.mutex);
  }
  else if (direction == 'p'){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    boardState.currentLevel->selected = true;
    if (id==1 or id==2){
      boardState.playAudio = true;
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
    else{
      ;
    }
    uint8_t maxValue = boardState.currentLevel->valueMax;
    xSemaphoreGive(boardState.mutex);
    if (direction == 'r'){
        currentValue == maxValue ? currentValue = 0 : currentValue+=1;
        Serial.print("Current value: ");
        Serial.println(currentValue);
    }
    else if (direction == 'l'){
        currentValue == 0 ? currentValue = maxValue : currentValue-=1;
        Serial.print("Current value: ");
        Serial.println(currentValue);
    }
    else if (direction == 'p'){
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.currentLevel->selected = false;
        xSemaphoreGive(boardState.mutex);
        if(id == 1 or id==2){
          boardState.playAudio = false;
        }
        else{
          ;
        }
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
      else{
        ;
      }
    xSemaphoreGive(boardState.mutex);
}


void controlJoystick(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Serial.println("Doing JS Task...");
      uint8_t joyX = analogRead(X_PIN);
      uint8_t joyY = analogRead(Y_PIN);
      uint8_t joyZ = digitalRead(Z_PIN);
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      const bool isSelected = boardState.currentLevel->selected;
      uint8_t id = boardState.currentLevel->id;
      xSemaphoreGive(boardState.mutex);
      if(isSelected){
        changeValueLevel(SerialReadDirection(), id);
      }
      else{
        changeMenuLevel(SerialReadDirection(), id);
      }
    }
}