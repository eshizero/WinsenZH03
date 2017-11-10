/*
WinsenZH03.h - This library allows you to set and read the ZH03 Winsen Sensor module.
Created by Tomas Leiva , Nov 10, 2017.
*/




#include <WinsenZH03.h>
WinsenZH03 sensorZH;


void setup() {
	Serial.begin(9600);  //UART to pc
	Serial1.begin(9600); //UART to sensor
	sensorZH.begin(&Serial1);//configure the serial that the sensor will use
	sensorZH.setAs(QA);//configure the sensor QA mode 
}

void loop() {

		sensorZH.readOnce();//Every time a sample is taken, this function should be set, only in QA.
		Serial.print("PM1.0: ");
		Serial.print(sensorZH.readPM1_0());//
		Serial.println("  ug/m3");
		Serial.print("PM2.5: ");
		Serial.print(sensorZH.readPM2_5());
		Serial.println("  ug/m3");
		Serial.print("PM10: ");
		Serial.print(sensorZH.readPM10());
		Serial.println("  ug/m3");
		delay(1000);

}




