#include <Arduino.h>
#include "task.h"
#include "config.h"
#include "DYPlayerESP32.h"

TaskHandle_t audioHandle = nullptr;


void playAudio(void* pvParameters){
    (void)pvParameters;

    /* Make the task execute at a specified frequency */
    const TickType_t xFrequency = configTICK_RATE_HZ / AUDIO_FREQ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    player.stop();
    bool audioEnded = true;
    for(;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        xSemaphoreTake(boardState.mutex, portMAX_DELAY);
        uint8_t currentTrack = boardState.currentTrack;
        bool currentPlayAudio = boardState.playAudio;
        uint8_t currentVolume = boardState.currentVolume;
        xSemaphoreGive(boardState.mutex);
        auto status = player.checkPlayState();
        if(status==DY::PlayState::Stopped){
            audioEnded = true;
            Serial.println("Audio stopped");
        }
        else{
            audioEnded = false;
            Serial.println("Audio playing");
        }
        bool playTrack;
        if(audioEnded and currentPlayAudio){
            player.setVolume(currentVolume);
            Serial.print("Playing Track... ");
            Serial.println(currentTrack);
            player.playSpecified(currentTrack+1);
        }
        else if(!currentPlayAudio){
            player.stop();
        }
        else{
            ;;
        }
    }
}