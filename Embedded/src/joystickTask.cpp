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


void changeMenuLevel(char direction){
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
    xSemaphoreGive(boardState.mutex);
    Serial.println("Selected");
  }
  else{
    ;
  }
}

void changeValueLevel(char direction){
    xSemaphoreTake(boardState.mutex, portMAX_DELAY);
    uint8_t currentValue = boardState.currentLevel->value;
    uint8_t maxValue = boardState.currentLevel->valueMax;
    xSemaphoreGive(boardState.mutex);
    if (direction == 'r'){
        currentValue == maxValue ? currentValue = 0 : currentValue+=1;
        Serial.print("Current value: ");
        Serial.println(currentValue);
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.currentLevel->value = currentValue;
        xSemaphoreGive(boardState.mutex);
    }
    else if (direction == 'l'){
        currentValue == 0 ? currentValue = maxValue : currentValue-=1;
        Serial.print("Current value: ");
        Serial.println(currentValue);
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.currentLevel->value = currentValue;
        xSemaphoreGive(boardState.mutex);
    }
    else if (direction == 'p'){
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        boardState.currentLevel->selected = false;
        xSemaphoreGive(boardState.mutex);
        Serial.println("Unselected");
    }
    else{
        ;
    }
}


void controlJoystick(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      uint8_t joyX = analogRead(X_PIN);
      uint8_t joyY = analogRead(Y_PIN);
      uint8_t joyZ = digitalRead(Z_PIN);
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      const bool isSelected = boardState.currentLevel->selected;
      xSemaphoreGive(boardState.mutex);
      if(isSelected){
        changeValueLevel(SerialReadDirection());
      }
      else{
        changeMenuLevel(SerialReadDirection());
      }
    }
}