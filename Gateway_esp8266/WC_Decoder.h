/**
* Контроллер умного дома. Версия 2.0 ESP8266
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#ifndef WC_DECODER_h
#define WC_DECODER_h
#include <arduino.h>

#define MAX_SENSORS 32
#define MAX_PACKET  4

struct WC_packet {
   uint8_t  Id;    //Первый байт - ID сенсора
   uint8_t  Count; //Первый полубайт второго байта - номер пакета
   uint8_t  SRC;   //Второй полубайт второго байта - контрольная сумма
   uint16_t Data; //Третий и четвериый байт - данные 
};

struct WC_Data{
  uint8_t  Id;
  uint32_t Time;
  int Data[MAX_PACKET];
  bool DataFlag[MAX_PACKET];
  bool Enabled;
};


bool RC_CheckSRC( uint32_t data );
int  RC_Decode( uint32_t data, uint32_t tm1 );
void RC_Clear(int n);
int  RC_CountById( uint8_t id );

extern WC_Data RC_Data[];
extern int RC_DataCount;


#endif
