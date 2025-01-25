#define BLYNK_TEMPLATE_ID "TMPL6Dfy9ZZOe"
#define BLYNK_TEMPLATE_NAME "Giam sat nhip tim"
#define BLYNK_AUTH_TOKEN "fh0_f6c15KVkCTPdUwjpEWs7pfrBsuoM"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "FPT - Nguyen An"; //Tên Wifi
char pass[] = "0962698659"; //Mật khẩu Wifi
WidgetLED led1(V0);
unsigned long timeS=millis();
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
PulseOximeter pox;
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#include <Ticker.h>
Ticker timer;

void setup(){
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println("SSD1306 allocation failed");
    //for(;;);
  }
  display.setFont(&FreeSerif9pt7b);
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(20,10);             
  display.println("Welcome to");
  display.setCursor(0,30);             
  display.println("Do an 2");            
  display.display();
  delay(2000); 
  if (!pox.begin()) {
      Serial.println("FAILED");
      for(;;);
  } else {
      Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
  timer.attach_ms(100, update);
}

void loop(){
  Blynk.run();
  if(millis()-timeS>1000){
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    display.clearDisplay(); 
    display.setCursor(0,12);  
    display.print("HeartR:");
    int heatRate = pox.getHeartRate();
    int spo2 = pox.getSpO2();
    display.print(pox.getHeartRate(),0); 
    display.println(" bpm");
    display.setCursor(0,29);
    display.print("SpO2  : ");
    display.print(pox.getSpO2());
    display.println(" %");
    display.display();

    if (led1.getValue()) {
      led1.off();
      //Serial.println("LED on V0: off");
    } else {
      led1.on();
      //Serial.println("LED on V0: on");
    }
    Blynk.virtualWrite(V1,heatRate);
    Blynk.virtualWrite(V2,spo2);
    timeS=millis();
  }
}

void update(){
  pox.update();
}
void onBeatDetected()
{
    Serial.println("Beat!");
}
