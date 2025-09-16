#include "task.h"
#include "config.h"

TaskHandle_t joystickHandle = nullptr;



void controlJoystick(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
      boardState.joyX = analogRead(X_PIN);
      boardState.joyY = analogRead(Y_PIN);
      boardState.joyZ = digitalRead(Z_PIN);
      xSemaphoreGive(boardState.mutex);
    }
}