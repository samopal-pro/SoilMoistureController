/**
* Контроллер умного дома. Версия 2.0 ESP8266
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#ifndef WS_HTTP_h
#define WS_HTTP_h
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <arduino.h>

extern ESP8266WebServer server;
extern const char DEVICE_ID[];
extern bool isAP;


bool ConnectWiFi(void);
void HTTP_begin(void);
void HTTP_handleRoot(void);
void HTTP_handleConfig(void);
void HTTP_loop();
void WiFi_begin(void);
void Time2Str(char *s,time_t t);
bool SetParamHTTP(int n);
int  HTTP_isAuth();
void HTTP_handleLogin(void);
void HTTP_handleReboot(void);
void HTTP_gotoLogin();
int  HTTP_checkAuth(char *user);
void HTTP_printInput(String &out,const char *label, const char *name, const char *value, int size, int len,bool is_pass=false);
void HTTP_printHeader(String &out,const char *title, uint16_t refresh=0);
void HTTP_printTail(String &out);





#endif
