#include <Arduino.h>
#include <Ticker.h>
#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <coos.h>
#include <FS.h>
ADC_MODE(ADC_VCC);

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
#define RAM_SIZE 20 * 1024

// ------------------begin ESP8266'centric----------------------------------
#define FREQUENCY    160                  // valid 80, 160
//
#include "ESP8266WiFi.h"
extern "C" {
  #include "user_interface.h"
}
// ------------------end ESP8266'centric------------------------------------
int voltaje=0;
byte i2c_adress;
byte thiskey;
Ticker timer;
uint16_t cadr_count;
unsigned long timeF,timeR;
int timeCpu = 0,timeGpu = 0,timeSpr = 0,cpuOPS = 0;
byte fps;
volatile uint16_t timers[8];
uint8_t mem[RAM_SIZE] = {0x12,0x00,0x06,0x20,0x99,0x03,0x90,0x00,0x18,0x02,0x11,0x02,0xD4,0x51,0x12,0x00,0x06,0x20,0x97,0x03,0x31,0x01,0x97,0x03,0x12,0x08,0xC1,0x12,0xC2,0x12,0xB1,0x00,0x92,0x00,0x92,0x00,0x01,0x02,0x62,0x03,0x82,0x02,0x31,0x02,0x97,0x03,0x13,0x10,0xA4,0x23,0x82,0x02,0x12,0x50,0x82,0x02,0x12,0x08,0x82,0x02,0x12,0x08,0x82,0x02,0x99,0x00,0x30,0x02,0xA8,0xA0,0x01,0x02,0x40,0x03,0x82,0x02,0x31,0x02,0x97,0x03,0x13,0x10,0xA4,0x23,0x82,0x02,0x12,0x60,0x82,0x02,0x12,0x08,0x82,0x02,0x12,0x08,0x82,0x02,0x99,0x00,0x30,0x02,0xA8,0xA0,0x01,0x02,0x40,0x03,0x82,0x02,0x31,0x02,0x97,0x03,0x13,0x10,0xA4,0x23,0x82,0x02,0x12,0x70,0x82,0x02,0x12,0x08,0x82,0x02,0x12,0x08,0x82,0x02,0x99,0x00,0x30,0x02,0xA8,0xA0,0xA8,0x10,0x97,0x03,0x90,0x00,0x14,0x00,0x12,0x00,0x06,0x20,0x95,0x03,0x31,0x01,0x95,0x03,0x12,0x20,0xC1,0x12,0xC2,0x12,0xB1,0x00,0x92,0x00,0xF0,0x00,0x31,0x01,0x95,0x03,0x01,0x02,0xBE,0x02,0xD5,0x12,0x31,0x01,0x95,0x03,0x12,0x78,0xAD,0x02,0x13,0x78,0xAD,0x03,0xE1,0x23,0x31,0x01,0x95,0x03,0x12,0x02,0x1F,0x02,0xF1,0xF2,0x31,0x01,0x95,0x03,0x12,0x02,0x1F,0x03,0xF1,0xF2,0x31,0x01,0x95,0x03,0x12,0x04,0x13,0x10,0xF1,0x23,0x31,0x01,0x95,0x03,0x12,0x05,0x13,0x10,0xF1,0x23,0xA8,0x10,0x95,0x03,0x90,0x00,0x98,0x00,0x12,0x01,0x82,0x02,0x12,0x04,0x82,0x02,0x01,0x02,0xD0,0x07,0x82,0x02,0x99,0x00,0x3A,0x02,0xA8,0x60,0x12,0xBE,0x82,0x02,0x12,0xDC,0x82,0x02,0x12,0x00,0x82,0x02,0x12,0x09,0x82,0x02,0x99,0x00,0x44,0x02,0xA8,0x80,0x11,0x01,0xD4,0x51,0x11,0x01,0xB1,0x00,0x92,0x00,0x16,0x02,0x11,0x01,0x12,0x02,0xD8,0x12,0x03,0x01,0x99,0x03,0xA8,0x10,0x99,0x03,0x12,0x40,0x82,0x02,0x12,0x40,0x82,0x02,0x03,0x02,0x99,0x03,0x82,0x02,0x99,0x00,0x4E,0x02,0xA8,0x60,0x12,0x00,0x06,0x20,0x95,0x03,0x31,0x01,0x95,0x03,0x12,0x20,0xC1,0x12,0xC2,0x12,0xB1,0x00,0x92,0x00,0xF2,0x01,0x31,0x01,0x95,0x03,0x12,0x06,0x03,0x03,0x99,0x03,0xF1,0x23,0x31,0x01,0x95,0x03,0x12,0x00,0xDC,0x12,0x12,0x02,0xC1,0x12,0xC2,0x12,0xB1,0x00,0x92,0x00,0x88,0x01,0x31,0x01,0x95,0x03,0x12,0x02,0x1F,0x02,0xF1,0xF2,0x31,0x01,0x95,0x03,0x12,0x00,0xDC,0x12,0x12,0x76,0xC1,0x12,0xC2,0x13,0xB1,0x00,0x92,0x00,0xAA,0x01,0x31,0x01,0x95,0x03,0x12,0x00,0x13,0x02,0xA2,0x23,0x1F,0x02,0xF1,0xF2,0x31,0x01,0x95,0x03,0x12,0x01,0xDC,0x12,0x12,0x02,0xC1,0x12,0xC2,0x12,0xB1,0x00,0x92,0x00,0xC8,0x01,0x31,0x01,0x95,0x03,0x12,0x02,0x1F,0x03,0xF1,0xF2,0x31,0x01,0x95,0x03,0x12,0x01,0xDC,0x12,0x12,0x76,0xC1,0x12,0xC2,0x13,0xB1,0x00,0x92,0x00,0xEA,0x01,0x31,0x01,0x95,0x03,0x12,0x00,0x13,0x02,0xA2,0x23,0x1F,0x03,0xF1,0xF2,0xA8,0x10,0x95,0x03,0x90,0x00,0x4E,0x01,0x11,0x04,0x12,0x02,0xD1,0x31,0xD1,0x42,0x01,0x02,0x84,0x03,0x82,0x02,0xD2,0x12,0x82,0x02,0x11,0x04,0x99,0x00,0x58,0x02,0x07,0x21,0xA8,0x40,0x99,0x00,0x26,0x02,0x90,0x00,0x1E,0x01,0x9A,0x00,0x01,0x0F,0x00,0x00,0x06,0xF0,0x9B,0x03,0x99,0x00,0x0A,0x00,0x50,0x00,0xC2,0x16,0xB1,0x00,0x92,0x00,0x26,0x02,0x9A,0x00,0x07,0x10,0x12,0x02,0xA0,0x12,0xD4,0x01,0x9A,0x00,0x07,0x10,0x12,0x02,0xA0,0x12,0xD7,0x01,0x9A,0x00,0x07,0x10,0x12,0x02,0xA0,0x12,0xD7,0x11,0x9A,0x00,0x07,0x10,0x12,0x02,0xA0,0x12,0xD7,0x21,0x9A,0x00,0x07,0x20,0xA0,0x21,0x02,0x22,0x20,0x32,0xB3,0x25,0x92,0x00,0x7C,0x02,0xD1,0x03,0xA8,0x02,0x20,0x32,0x91,0x00,0x60,0x02,0x9A,0x00,0xA8,0x02,0x20,0x32,0x91,0x00,0x60,0x02,0x9A,0x00,0xA8,0x02,0x20,0x32,0xB3,0x25,0x92,0x00,0xA6,0x02,0xA9,0x21,0x64,0x10,0xB3,0x44,0x92,0x00,0xAC,0x02,0xB3,0x49,0x92,0x00,0xAC,0x02,0xB3,0x53,0x92,0x00,0xB8,0x02,0xB3,0x43,0x92,0x00,0xB2,0x02,0x90,0x00,0x60,0x02,0xD1,0x03,0x90,0x00,0x72,0x02,0xD1,0x24,0x90,0x00,0x72,0x02,0xD1,0x04,0x90,0x00,0x72,0x02,0xD1,0x14,0x90,0x00,0x72,0x02,0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x66,0x77,0x77,0x66,0x00,0x00,0x00,0x06,0x77,0x8F,0xC8,0x88,0x60,0x00,0x00,0x67,0x88,0x8F,0xC8,0x88,0x86,0x00,0x06,0x78,0x88,0x8F,0xC8,0x88,0x88,0x60,0x06,0x78,0x88,0x8F,0xC8,0x88,0x88,0x60,0x67,0x88,0x88,0xFC,0xCC,0x88,0x88,0xB6,0x67,0xFF,0xFF,0xCC,0xCC,0xCC,0xCC,0xB6,0x67,0xCC,0xCC,0xCC,0xCC,0xBB,0xBB,0xB6,0x67,0x88,0x88,0xCC,0xCB,0x88,0x88,0xB6,0x06,0x88,0x88,0x8C,0xB8,0x88,0x8B,0x60,0x06,0x88,0x88,0x8C,0xB8,0x88,0x8B,0x60,0x00,0x68,0x88,0x8C,0xB8,0x88,0xB6,0x00,0x00,0x06,0x88,0x8C,0xB8,0xBB,0x60,0x00,0x00,0x00,0x66,0xBB,0xBB,0x66,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x99,0x99,0x99,0x99,0xCC,0xCB,0x9C,0xCC,0xFF,0xCB,0x9F,0xFF,0xFF,0xFB,0x9F,0xFF,0x99,0x99,0x99,0x99,0xCC,0xCC,0xCC,0xB9,0xFF,0xFF,0xFC,0xB9,0xFF,0xFF,0xFF,0xB9,0x00,0x00,0x09,0x99,0x99,0x00,0x09,0xCC,0xB9,0x00,0x09,0xFC,0xB9,0x00,0x09,0xFF,0xB9,0x00,0x99,0x99,0x99,0x99,0xCC,0xCC,0xCC,0xB9,0xFF,0xFF,0xFC,0xB9,0xFF,0xFF,0xFF,0xB9,0x00,0x00,0x20,0x4B,0x45,0x59,0x20,0x50,0x52,0x45,0x53,0x53,0x45,0x44,0x20,0x25,0x44,0x20,0x00};
uint16_t palette[16] = {
    0x0020, 0xE718, 0xB9A8, 0x7DB6, 0x41E9, 0x6D2D, 0x21EC, 0xD5CA,
    0xAC4D, 0x31A7, 0xBB09, 0x3186, 0x73AE, 0x8D4B, 0x3DF9, 0xbdd7
};

uint16_t bgr_to_rgb(uint16_t c){
  return ((c & 0x001f) << 11) + ((c & 0xf800) >> 11) + (c & 0x07e0);
}

unsigned char hexToByte(char h){
  if(h < 48)
    return 48;
  if (h >= 48 && h <= 57) 
    h = map(h, 48, 57, 0, 9);
  else if (h >= 65 && h <= 70) 
    h = map(h, 65, 70, 10, 15);
  else if (h >= 97 && h <= 102) 
    h = map(h, 97, 102, 10, 15);
  return h;
}

void loadFromSerial(){
  char c;
  unsigned char n;
  int16_t j = 0;
  for(int16_t i = 0; i < RAM_SIZE; i++)
    mem[i] = 0;
  while(c != '.'){
    if(Serial.available()){
      c = Serial.read();
      Serial.print(c);
      if(c == '$'){
        n = 48;
        while(n > 15){
          c = Serial.read();
          n = hexToByte(c);
        }
        Serial.print(c);
        mem[j] = n << 4;
        n = 48;
        while(n > 15){
          c = Serial.read();
          n = hexToByte(c);
        }
        Serial.print(c);
        mem[j] += n;
        j++;
      }
    }
  }
  Serial.print(F("load "));
  Serial.print(j);
  Serial.println(F(" byte"));
  Serial.print(F("free heap "));
  Serial.println(system_get_free_heap_size());
  cpuInit();
}

void coos_cpu(void){   
  while(1){
    COOS_DELAY(1);        // 1 ms
    timeR = millis();
    cpuOPS += 1;
    cpuRun(1000);  
    timeCpu += millis() - timeR;
  }
}

void coos_screen(void){
   while(1){
    yield();
    COOS_DELAY(50);        // 50 ms
    timeR = millis();
    clearSpriteScr();
    redrawSprites();
    testSpriteCollision();
    redrawParticles();
    timeSpr += millis() - timeR;
    timeR = millis();
    redrawScreen();
    setRedraw(); 
    timeGpu += millis() - timeR;
    if(millis() - timeF > 1000){
      timeF = millis();
      fps = cadr_count;
      cadr_count = cadr_count % 2;
    }  
  } 
}

void timer_tick(void){
  for(int8_t i = 0; i < 8; i++){
    if(timers[i] >= 1)
      timers[i] --;
  }
}

void coos_key(void){   
  while(1){
    COOS_DELAY(100);        // 100 ms
    getKey(); 
  }
}

void coos_info(void){   
  while(1){
    COOS_DELAY(1000);        // 1000 ms
    voltaje = ESP.getVcc();
    tft.fillRect(0, 0, 32, 80, 0x0000);
    tft.setCursor(1, 0);
    tft.println("fps");
    tft.println(fps);
    tft.println("cpu");
    tft.println(timeCpu, DEC);
    tft.println("gpu");
    tft.println(timeGpu, DEC);
    tft.println("spr");
    tft.println(timeSpr, DEC);
    tft.println("kIPS");
    tft.println(cpuOPS, DEC);
    timeCpu = 0;
    timeGpu = 0;
    timeSpr = 0;
    cpuOPS = 0;
  }
}

void setup() {
  byte menuSelected = 3;
  // ------------------begin ESP8266'centric----------------------------------
  WiFi.forceSleepBegin();                  // turn off ESP8266 RF
  delay(1);                                // give RF section time to shutdown
  system_update_cpu_freq(FREQUENCY);
  // ------------------end ESP8266'centric------------------------------------
  tft.init();            // initialize LCD
  tft.setRotation(1);
  tft.fillScreen(0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xffff);
  Serial.begin (115200);
  Wire.begin(D2, D1);
  geti2cAdress();
  Serial.println(i2c_adress, HEX);
  Serial.println();
  cpuInit();
  //Initialize File System
  if(SPIFFS.begin()){
    Serial.println(F("SPIFFS Initialize....ok"));
    fileList("/");
  }
  else{
    Serial.println(F("SPIFFS Initialization...failed"));
  }
  voltaje = ESP.getVcc();
  randomSeed(ESP.getVcc());
  clearScr();
  setColor(1);
  randomSeed(analogRead(0));
  timer.attach(0.001, timer_tick);
  coos.register_task(coos_cpu); 
  coos.register_task(coos_screen);   
  coos.register_task(coos_key); 
  coos.register_task(coos_info);
  coos.start();                     // init registered tasks
}

void loop() {
  coos.run();  // Coos scheduler
  if(Serial.available()){
    char c = Serial.read();
    Serial.print(c);
    if(c == 'm'){
      loadFromSerial();
      cpuInit();
      return;
    }
    if(c == 'r'){
      ESP.reset();
      return;
    }
  }
}
