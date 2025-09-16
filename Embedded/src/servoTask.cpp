#include "task.h"
#include "config.h"
#include "ESP32Servo.h"

Servo dummyServo, servo0, servo1;
TaskHandle_t servoHandle = nullptr;

void setupServo(){
    ESP32PWM pwm;
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    dummyServo.setPeriodHertz(50);
    servo0.setPeriodHertz(50);
    servo1.setPeriodHertz(50);
    dummyServo.attach(sDummy, servoMinUs, servoMaxUs);
    servo0.attach(S0, servoMinUs, servoMaxUs);
    servo1.attach(S1, servoMinUs, servoMaxUs);
    pwm.attachPin(37, 10000);
}

void servoSweep(){
    for(int pos = 0; pos<=180; pos++){
        servo0.write(pos);
        Serial.println(pos);
    }
    for(int pos = 0; pos<=180; pos++){
        servo1.write(pos);
        Serial.println(pos);
    }
}

void moveServo(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / JOYSTICK_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
      setupServo();
      servoSweep();
    }
}