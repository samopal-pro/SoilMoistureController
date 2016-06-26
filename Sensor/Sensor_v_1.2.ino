/**
* Контроллер умного дома. 
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <RCSwitch.h>
#include <LowPower.h>


#define PIN_DS   16
#define PIN_RC   9
//#define PIN_RC   10

#define PIN_VCC  A1
#define PIN_SOIL A0
#define SLEEP_LOOP 120
#define COUNT_REPEAT 2
#define DEBUG  1

uint8_t SENSOR_ID = 0x15;





OneWire oneWire(PIN_DS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;


RCSwitch mySwitch = RCSwitch();

uint16_t count = 0;
bool is_ds = false;

/**
* Функция определения напряжения питания устройства
*/
static int vccRead(byte us =250) {
  ADMUX = 1<<REFS0; // опорное напряжение - Vcc
  ADMUX |= 0x0E;    // объект измерения - внутренний источник
                    // стабилизированного напряжения 1.1В
  delayMicroseconds(us);
 
  ADCSRA |= 1<<ADSC;         // запуск АЦ-преобразования
  while(ADCSRA & (1<<ADSC)); // и ожидание его завершения
  word x = ADC;
//  return x ? (1100L * 1023) / x : -1;
  return x ? (1100L * 1212) / x : -1;
}


uint32_t RC_encoder(uint8_t id, uint8_t count,int val){
// Считаем контрольную сумму полубайтами
  uint8_t src = 0;
  src = (uint8_t)(val & 0xf);
  src += (uint8_t)(val>>4 & 0xf);
  src += (uint8_t)(val>>8 & 0xf);
  src += (uint8_t)(val>>12 & 0xf);
  src += 0xf & count;
  src += 0xf & id;
  src += id >>4 &0xf;
// Формируем четыре байта данных   
  uint32_t data = (uint32_t)(id&0xff) <<24 | 
     (uint32_t)(count&0xf)<<20 |
     (uint32_t)(src&0xf)<<16|
     (uint32_t)(val&0xffff);
  return data;   
 
  
  
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Starting ...");

// Инициализация датчика температуры
   Serial.print("Locating devices...");
#endif   
   sensors.begin();
   int n = sensors.getDeviceCount();   
#ifdef DEBUG
   Serial.print("Found ");
   Serial.print(n, DEC);
   Serial.println(" DS18B20.");
#endif   
   sensors.getAddress(insideThermometer, 0);
   sensors.setResolution(insideThermometer, 9);
   if( n > 0 )is_ds = true;

  
// Инициализация передатчика 
  mySwitch.enableTransmit(PIN_RC);
  
}

void loop() {
  uint32_t data = 0;
// Переводим аналоговые входы в режим АЦП
///  analogReference(INTERNAL);  
  int vcc = vccRead(50);
  delay(50);
///  analogReference(DEFAULT);  
  int a1 = analogRead(PIN_SOIL);
  
// Посылаем счетчик циклов измерения
  data = RC_encoder(SENSOR_ID,0,count);
#ifdef DEBUG   
  Serial.print("Count=");
  Serial.print(count);
  Serial.print(" 0x");
  Serial.println(data,HEX);
#endif  
  for( int i=0; i<COUNT_REPEAT;i++)mySwitch.send( data, 32); 
  
// Посылаем значение АЦП A0      
  data = RC_encoder(SENSOR_ID,1,vcc);
#ifdef DEBUG
   Serial.print("Vcc=");
  Serial.print(vcc);
  Serial.print(" 0x");
  Serial.println(data,HEX);
#endif  
  for( int i=0; i<COUNT_REPEAT;i++)mySwitch.send( data, 32); 

// Считываем и посылаем температуру  
  if( is_ds ){
     sensors.requestTemperatures();
     float Temp = sensors.getTempC(insideThermometer);
     data = RC_encoder(SENSOR_ID,2,0xffff&(int)(Temp*10) );
#ifdef DEBUG
     Serial.print("Temp=");
     Serial.print(Temp,1);
     Serial.print(" 0x");
     Serial.println(data,HEX);
#endif     
     for( int i=0; i<COUNT_REPEAT;i++)mySwitch.send( data, 32); 
    
  }
   
// Посылаем значение АЦП A1     
  data = RC_encoder(SENSOR_ID,3,a1);
#ifdef DEBUG
  Serial.print("Soil=");
  Serial.print(a1);
  Serial.print(" 0x");
  Serial.println(data,HEX);
#endif  
  for( int i=0; i<COUNT_REPEAT;i++)mySwitch.send( data, 32); 
  count++;

#ifdef DEBUG 
  Serial.println("");
#endif  
  delay(100);

// Засыпаем на SLEEP_LOOP циклов по 8 сек 
//   delay(5000);
  for( int i=0; i<SLEEP_LOOP; i++)LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

}




