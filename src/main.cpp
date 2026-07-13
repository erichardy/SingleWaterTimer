#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "esp_log.h"


#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "rotaryEncoderEH.h"
Encoder encoder(PIN_A, PIN_B);

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WIRE Wire
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

int minutes;
int seconds;

void disp(int val){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(val);
    display.display();
}

void ms2MinSec(long ms, int *minutes, int *secondes) {
    *minutes = ms / 60000;
    *secondes = (ms % 60000) / 1000;
}

void dispTime(int minutes, int seconds, int nbMinutes){
    char sTime[5];
    char head[14];
    sprintf(head, "Total : %2d mn", nbMinutes);
    sprintf(sTime, "%2d:%02d", minutes, seconds);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1.5);
    display.print(head);
    display.setTextSize(3);
    display.setCursor(10, 12);
    display.print(sTime);
    display.display();
    delay(100);
    
}

void setup() {
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_B, INPUT_PULLUP);
    pinMode(PIN_SW, INPUT_PULLUP);
    encoder.write(VALUE_INIT);
    Serial.begin (115200);
    delay(100);
    // anciennePosition = encoder.read();
    Serial.println("Encoder READY !!!");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    display.display();
    delay(100);
    display.setRotation(2); // vertical flip
    display.setTextSize(2);

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.display();
    // dispTime(3 , 3);
}

void loop() {
    int nbMinutes;
    long lastVal = 99;
    long val = encoder.read();
    nbMinutes = val / 4;
    disp(nbMinutes);
    while(1) {
        if (val != lastVal) {
            val = constrain(val, MIN_VAL, MAX_VAL);
            encoder.write(val);
            nbMinutes = val / 4;
            // Serial.print(nbMinutes);
            // Serial.print(" ");
            // Serial.println(val);
            disp(nbMinutes);
        }
        lastVal = val ;
        bool clic = digitalRead(PIN_SW);
        if (! clic) break ;
        val = encoder.read();
        delay(50);
    }
    // print small sized total duration
    
    // Serial.print("valeur : ");
    // Serial.println(val);
    // Serial.println(millis());
    long durationMs = nbMinutes * 60000 ; // duree en milli-secondes
    long start = millis(); // début du décompte
    long current = millis(); // instant T
    long spent = current - start; // temps écoulé
    long remain = durationMs - spent; // le temps qui reste
    while (current < (start + durationMs)) {
        current = millis();
        long spent = current - start;
        long remain = durationMs - spent;
        ms2MinSec(remain, &minutes, &seconds);
        dispTime(minutes, seconds, nbMinutes);
        delay(300);
    }
}