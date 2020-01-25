#include <Wire.h>
#include <MS5611.h>
#include <SoftwareSerial.h>


const int analogInPin0 = A0;
const int analogInPin1 = A1; 
const int analogInPin2 = A2;

int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;


MS5611 ms5611;
double referencePressure;


byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9];


void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);
  
  Serial.println("Initialize MS5611 Sensor");

  while(!ms5611.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }
  referencePressure = ms5611.readPressure();
  checkSettings();
}
void checkSettings()
{
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

  
void loop() {
  //---------------------MQ---------------------------

  sensorValue0 = analogRead(analogInPin0); //считываем значения с датчика
  sensorValue1 = analogRead(analogInPin1); //считываем значения с датчика
  sensorValue2 = analogRead(analogInPin2); //считываем значения с датчика
  
  Serial.print("MQ2= ");
  Serial.println(sensorValue0);
  Serial.print("MQ135= ");
  Serial.println(sensorValue1);
  Serial.print("CO MQ7= ");
  Serial.println(sensorValue2);
  Serial.println("---------------------------");
  //---------------------MQ---------------------------
//---------------------pressue---------------------------
  // Read true temperature & Pressure
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();

  Serial.println("--");

  Serial.print("realTemp = ");
  Serial.print(realTemperature);
  Serial.println(" *C");

  Serial.print("realPressure = ");
  Serial.print(realPressure);
  Serial.println(" Pa");
  
  Serial.print("realPressure = ");
  Serial.print(realPressure/133);
  Serial.println(" mmHg Art.");
  
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
    Serial.println("CRC error: " + String(crc) + " / "+ String(response[8]));
    Serial.println("Sensor CRC error");
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    unsigned int ppm = (256*responseHigh) + responseLow;
    Serial.println("CO2: " + String(ppm) + " ppm"); 
  }
//---------------------CO2---------------------------
  delay(1000);
}
