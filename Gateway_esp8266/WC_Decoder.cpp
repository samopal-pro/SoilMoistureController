/**
* Контроллер умного дома. Версия 2.0 ESP8266
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#include "WC_Decoder.h"

WC_Data RC_Data[MAX_SENSORS];
int RC_DataCount = 0;

/**
 * Проверка контрольной суммы пакета
 */
 bool RC_CheckSRC( uint32_t data ){
 // Считаем контрольную сумму полубайтами
  uint8_t src = 0;
  src =  (uint8_t)(data & 0xf);
  src += (uint8_t)(data>>4 & 0xf);
  src += (uint8_t)(data>>8 & 0xf);
  src += (uint8_t)(data>>12 & 0xf);
  src += (uint8_t)(data>>20 & 0xf);
  src += (uint8_t)(data>>24 & 0xf);
  src += (uint8_t)(data>>28 & 0xf);
  uint8_t src1 = (uint8_t)(data>>16 & 0xf);
  if( (src&0xf) != (src1&0xf) ){
     Serial.printf("Bad SRC 0x%lx calc=0x%x decode=0x%x\n",data,src&0xf,src1&0xf);
     return false; 
  }
  return true;
  
 }

/*
 * Находим номер записи в массиве данных по ID датчика или создаем новый
 */
int RC_CountById( uint8_t id ){
   for( int i=0; i<RC_DataCount; i++ ){
// Если найден сенсор в списке    
       if( RC_Data[i].Id == id )return i;
   }
// Если превышено количество сенсоров   
   if( RC_DataCount >= MAX_SENSORS )return -1;
// Создаем новый сенсор  
   RC_Data[RC_DataCount].Id   = id;
   RC_Data[RC_DataCount].Time = 0;
   RC_Clear(RC_DataCount);
   RC_DataCount++;
   return RC_DataCount-1;
}

/**
 * Очистка строки данных
 */
void RC_Clear(int n){
   if( n < 0 || n >= MAX_SENSORS )return;
   for( int i=0; i<MAX_PACKET; i++ ){
//      RC_Data[n].Data[i] = 0;
      RC_Data[n].DataFlag[i] = false;      
   }
   RC_Data[n].Enabled = false;  
}

/**
 * Декодирование сигнала
 */
int RC_Decode( uint32_t data, uint32_t tm1  ){
// Проверка контрольной суммы
  if( RC_CheckSRC( data ) == false )return -1;  
// Декодируем данные  
  uint8_t id    = (uint8_t)(data>>24 &0xff);
  uint8_t count = (uint8_t)(data>>20 &0xf);
  int     val   = (int)(data &0xffff);
// Находим номер в массиве данных  
  int n = RC_CountById(id);
  if( n < 0 )return false;
// Проверка на максимальное число пакетов  
  if( count >= MAX_PACKET )return -1;
// Сохранение данных в массив
  RC_Data[n].Time            = tm1;
  RC_Data[n].Data[count]     = val;
  RC_Data[n].DataFlag[count] = true;
  RC_Data[n].Enabled         = true;
  Serial.printf("Decode: tm=%ld n=%d id=0x%x count=%d val=%d\n",tm1,n,id,count,val);
// Проверка на последний пакет
  if( count == MAX_PACKET-1 )return n;
  return -1;
}




/*
 * Декодируем четыре байта данных
 */
 /*
bool DecodeRC_Packet(uint32_t raw_data){
  
}
*/


