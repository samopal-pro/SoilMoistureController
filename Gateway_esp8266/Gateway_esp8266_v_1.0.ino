/**
* Контроллер умного дома. Версия 2.0 ESP8266
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "WC_RCSwitch.h"
//#include <Time.h>
#include <EEPROM.h>
//#include <arduino.h>

#include "WC_EEPROM.h"
#include "WC_HTTP.h"
#include "WC_NTP.h"
#include "WC_Decoder.h"

uint32_t data, data_save = 0;
uint32_t ms0 = 0, ms1 = 0;
uint32_t tm         = 0;
uint32_t t_cur      = 0;    
long   t_correct     = 0;
uint16_t err_count  = 0;

int SensorsCount;



RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  Serial.println("WSP8266 RC315/433 gateway start...");
// Инициализация EEPROM
   EC_begin();  
   EC_read();
// Подключаемся к WiFi  
  WiFi_begin();
  delay(2000);
// Старт внутреннего WEB-сервера  
  HTTP_begin(); 
   if( !isAP ){
// Инициализация UDP клиента для NTP
      NTP_begin();   
// Инициализация RC приемника  
      delay(2000);
      Serial.println("Init RC receiver ..."); 
      
      mySwitch.enableReceive(2);  // Receiver on inerrupt 0 => that is pin #2
   }
   Serial.println("Init complete");
}

void loop() {
  uint32_t ms = millis();
  char buf[100];
  char val[16];
  t_cur       = ms/1000;
  if( !isAP ){
   
     if (mySwitch.available()) {
    
       data = mySwitch.getReceivedValue();
    
       if (data != 0) {
         if( data != data_save ){
           tm    = t_cur + t_correct;
           int n = RC_Decode(data,tm);
           if( n >= 0 ){
               mySwitch.disableReceive();
               Serial.printf("Send to server\n");

               if( !SetParamHTTP(n) )err_count++;
               RC_Clear(n);  
               mySwitch.enableReceive(2);
           }
           data_save = data;
         }
       }

       mySwitch.resetAvailable();
     }
     
  }
  if( err_count>=3 )ESP.reset();
// HTTP сервер

  if(  ms < ms0 || (ms - ms0) > 3000 ){
     ms0 = ms;
     mySwitch.disableReceive();
     HTTP_loop();
     mySwitch.enableReceive(2);
  }

// Каждые 600 секунд считываем время в интернете 
   if( !isAP && (ms < ms1 || (ms - ms1) > 600000 || ms1 == 0 ) ){
       err_count++;
       mySwitch.disableReceive();
       uint32_t t = GetNTP();
       if( t!=0 ){
          ms1       = ms;
          err_count = 0;
          t_correct = t - t_cur;
       }
       mySwitch.enableReceive(2);
       Serial.printf("Get NTP time. Error = %d\n",err_count);
   }
  

 // delay(200);

}
