#include <Wire.h>
#include <MS5611.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include "DHT.h"

#define DHTPIN 4   
#define DHTTYPE DHT11   // DHT 11

OneWire ds(6);

const int chipSelect =7;


const int analogInPin0 = A0;
const int analogInPin1 = A1; 
const int analogInPin2 = A2;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;


MS5611 ms5611;
//double referencePressure;


byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9];
int error = 0;//1 - card, 2 - barometer, 3 - time SetTime, 4 - time circuitry, 5 - CO2, 6 - open log file, 7 - humid 


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);
  while (!Serial) ; // wait for serial
  while (!Serial1) ; // wait for serial
  
  //Serial.println("Serial ok");
  //Serial1.println("Serial1 ok");
   
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    error = 1;
    //showAlarm();!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }

  while(!ms5611.begin())
  {
    //Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    error = 2;
    showAlarm();
    //delay(500);
  }
  //Serial.println("ms5611 ok!");
  //referencePressure = ms5611.readPressure();
  //ms5611.getOversampling();

  dht.begin();  
  //Serial.println("dht ok!");
  
}

void showAlarm() {
  Serial.println("ERROR:");
  Serial.println(error);
  //while (1);
}

void loop() {
  String dataString = "";//TIME;MQ2;MQ135;MQ7;pressue;temp1;CO2;temp2;humid;temp3;

  //---------------------TIME---------------------------

  tmElements_t tm;
  if (RTC.read(tm)) {
    dataString = String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second) + " " +
     String(tm.Day) + "/" + String(tm.Month) + "/" + String(tmYearToCalendar(tm.Year)) + ";";
  } else {
    
    if (RTC.chipPresent()) {
      //Serial.println("The DS1307 is stopped.  Please run the SetTime");
      error = 3;
    } else {
      error = 4;
      //Serial.println("DS1307 read error!  Please check the circuitry.");
    }
    showAlarm();
  }
//---------------------TIME---------------------------



  //---------------------MQ---------------------------

  sensorValue0 = analogRead(analogInPin0);
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  dataString += String(sensorValue0) + ";" +String(sensorValue1) + ";" +String(sensorValue2) + ";";
  //---------------------MQ---------------------------
//---------------------pressue---------------------------
  // Read true temperature & Pressure
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();

  dataString += String(realPressure/133) + ";" + String(realTemperature) + ";";
  
//---------------------pressue---------------------------
//---------------------CO2---------------------------
  Serial1.write(cmd, 9);
  memset(response, 0, 9);
  Serial1.readBytes(response, 9);
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc+=response[i];
  crc = 255 - crc;
  crc++;
 
  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
    error = 5;
    showAlarm();
    //Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
    //Serial.println("Sensor CRC error");
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    unsigned int ppm = (256*responseHigh) + responseLow;
    //Serial.println("CO2: " + String(ppm) + " ppm");
    dataString += String(ppm) + ";";
  }
//---------------------CO2---------------------------

//---------------------TEMP2---------------------------
  byte data[2]; // Место для значения температуры
  
  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
  
  //delay(1000); // Микросхема измеряет температуру, а мы ждем.  
  
  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC); 
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read();
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;  
  dataString += String(temperature) + ";";


//---------------------TEMP2---------------------------

//---------------------HUMID---------------------------

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    
    error = 7;
    showAlarm();
    //return;
  }

  dataString += String(h) + ";" + String(t) + ";";
//---------------------HUMID---------------------------
  Serial.println(dataString);
  
//---------------------CD CARD---------------------------
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();

    //Serial.println(dataString);
  }
  else {
    error = 6;
    showAlarm();
    //Serial.println("error opening datalog.txt");
  }

//---------------------CD CARD---------------------------

  delay(10000);
}
