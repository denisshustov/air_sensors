//#include <Wire.h>
#include <MS5611.h>
//#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
//#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include "DHT.h"
#include <TroykaMQ.h>

OneWire ds(6);

const int chipSelect = 7;

MQ2 mq2(A0);
MQ7 mq7(A2);
MQ135 mq135(A1);

unsigned long mq7Value = 0;

MS5611 ms5611;
//double referencePressure;


byte cmd[9] = { 0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79 };
unsigned char response[9];
int error = 0;//1 - card, 2 - barometer, 3 - time SetTime, 4 - time circuitry, 5 - CO2, 6 - open log file, 7 - humid 

DHT dht(4, DHT11);


#define pwmPin 13


unsigned int readCO2PWM() {
	unsigned long th, ppm_pwm = 0;
	do {
		th = pulseIn(pwmPin, HIGH, 1004000) / 1000;
		ppm_pwm = 5000 * (th - 2) / (th + 1004 - th - 4);
	} while (th == 0);
	//Serial.print("PPM PWM: ");
	//Serial.println(ppm_pwm);
	return ppm_pwm;
}
void showAlarm() {
	Serial.println("ERROR:");
	Serial.println(error);
	//while (1);
}

void setup() {
	Serial.begin(9600);
	Serial1.begin(9600);
	while (!Serial); // wait for serial
	while (!Serial1); // wait for serial


	//---------------setTime--------------------------
	//setTime(hr, min, sec, day, month, yr);	
	//setTime(20, 25, 00, 21, 04, 2020);
	//RTC.set(now());
	//---------------mqCalibrate --------------------------
	mq7.cycleHeat();
	mq2.calibrate();//9.57
	mq135.calibrate();
	//---------------mqCalibrate --------------------------
	pinMode(pwmPin, INPUT);

	//Serial.println("Serial ok");
	//Serial1.println("Serial1 ok");

	if (!SD.begin(chipSelect)) {
		//Serial.println("Card failed, or not present");
		error = 1;
		//showAlarm();!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	while (!ms5611.begin())
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



void loop() {
	//String dataString = "";//TIME;LPG;Methane;Smoke;Hydrogen;CO2;CarbonMonoxide;pressue;temp1;CO2;temp2;humid;temp3;
	char resultData[100];
	//---------------------TIME---------------------------

	tmElements_t tm;
	if (RTC.read(tm)) {
		/*dataString = String(tm.Hour) + ":" + String(tm.Minute) + ":" + String(tm.Second) + " " +
			String(tm.Day) + "/" + String(tm.Month) + "/" + String(tmYearToCalendar(tm.Year)) + ";";*/
		sprintf(resultData, "%u:%u:%u %u/%u/%u;", tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tmYearToCalendar(tm.Year));
		//Serial.println(resultData);
	}
	else {

		if (RTC.chipPresent()) {
			//Serial.println("The DS1307 is stopped.  Please run the SetTime");
			error = 3;
		}
		else {
			error = 4;
			//Serial.println("DS1307 read error!  Please check the circuitry.");
		}
		showAlarm();
	}
	//---------------------TIME---------------------------



	//---------------------MQ---------------------------

	if (!mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
		mq7.calibrate(1);
		mq7.cycleHeat();
	}
	if (mq7.isCalibrated() && mq7.atHeatCycleEnd()) {
		mq7Value = mq7.readCarbonMonoxide();
		delay(100);
		mq7.cycleHeat();
	}

	/*dataString += String(mq2.readLPG()) + ";" + String(mq2.readMethane()) + ";" + String(mq2.readSmoke()) + ";" + String(mq2.readHydrogen()) + "" +
		";" + String(mq135.readCO2()) + ";" + String(mq7Value) + ";";*/

		/*sprintf(resultData + strlen(resultData), "%6ld;%6ld;%6ld;%6ld;%6ld;%6ld;",
			mq2.readLPG(), mq2.readMethane(), mq2.readSmoke(), mq2.readHydrogen(), mq135.readCO2(), mq7Value);*/
	dtostrf(mq2.readLPG(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(mq2.readMethane(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(mq2.readSmoke(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(mq2.readHydrogen(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(mq135.readCO2(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(mq7Value, 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");

	//---------------------MQ---------------------------

	//---------------------pressue---------------------------
	// Read true temperature & Pressure
	//double realTemperature = ms5611.readTemperature();
	long realPressure = ms5611.readPressure();

	//dataString += String(realPressure / 133) + ";" + String(realTemperature) + ";";
	//sprintf(resultData + strlen(resultData), "%6ld;%6ld;", (realPressure / 133), long(ms5611.readTemperature()));
	dtostrf((realPressure / 133), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(ms5611.readTemperature(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");

	//---------------------pressue---------------------------
	//---------------------CO2---------------------------
	  /*Serial1.write(cmd, 9);
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
	  }*/

	  //dataString += String(readCO2PWM()) + ";";
	  //sprintf(resultData + strlen(resultData), "%d;", readCO2PWM());
	dtostrf(readCO2PWM(), 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	//---------------------CO2---------------------------

	//---------------------TEMP2---------------------------
	byte data[2];

	ds.reset();
	ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
	ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память

	//delay(1000); // Микросхема измеряет температуру, а мы ждем.  

	ds.reset(); // Теперь готовимся получить значение измеренной температуры
	ds.write(0xCC);
	ds.write(0xBE); // Просим передать нам значение регистров со значением температуры
	// Получаем и считываем ответ
	resultData[0] = ds.read(); // Читаем младший байт значения температуры
	resultData[1] = ds.read();

	dtostrf(((resultData[1] << 8) | resultData[0]) * 0.0625, 2, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");

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

	//dataString += String(h) + ";" + String(t) + ";";
	//sprintf(resultData + strlen(resultData), "%f;%f", h, t);

	dtostrf(h, 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	dtostrf(t, 4, 2, &resultData[strlen(resultData)]);
	strcat(resultData, ";");
	//---------------------HUMID---------------------------
	Serial.println(resultData);

	//---------------------CD CARD---------------------------

	File dataFile = SD.open("datalog.txt", FILE_WRITE);

	if (dataFile) {
		dataFile.println(resultData);
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
