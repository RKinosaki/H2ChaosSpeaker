#include "task.h"
#include "config.h"
#include "ESP32Servo.h"

Servo servo0, servo1;
ESP32PWM pwm;
TaskHandle_t servoHandle = nullptr;

void setupServo(){
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servo0.setPeriodHertz(50);
    servo1.setPeriodHertz(50);
    
}

void servoSweep(uint8_t pos){
    servo0.attach(S0, servoMinUs, servoMaxUs);
    servo1.attach(S1, servoMinUs, servoMaxUs);
    pwm.attachPin(37, 10000);
    servo0.write(pos);
    servo1.write(pos);
    Serial.println(pos);
    vTaskDelay(pdMS_TO_TICKS(200));
    servo0.detach();
    servo1.detach();
    pwm.detachPin(S1);
}

void moveServo(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    setupServo();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        uint8_t pos = boardState.currentAngle;
        bool moveServo = boardState.moveServo;
      xSemaphoreGive(boardState.mutex);
      if(moveServo){
        servoSweep(pos);
      }
      
    }
}