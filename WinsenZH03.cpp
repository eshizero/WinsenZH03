/*
* WinsenZH03.h

* Esta libreria permite leer el sensor WinsenZH03, creada 
* en base al codigo de DFrobot https://www.dfrobot.com/wiki/index.php/PM2.5_laser_dust_sensor_SKU:SEN0177
*
* Tomas Leiva
* Nov 10,2017
* https://github.com/eshizero/WinsenZH03
* V0.4
*
* MIT License
*
* Copyright (c) 2017 eshizero
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "WinsenZH03.h"


WinsenZH03::WinsenZH03()
{
	_s = NULL;
}

void WinsenZH03::begin(Stream *ser)
{
	_s = ser;
	_s->setTimeout(2000);
}

void WinsenZH03::setAs(bool active)
{
	byte setConfig[] = { 0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46 };//QA config
	byte response[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	if (active) {
		setConfig[3] = 0x40;//Continus config
		setConfig[8] = 0x47;
#ifdef DEBUGOTHERS
		Serial.println(F("Continus mode"));
#endif
	}
#ifdef DEBUGOTHERS
	else
	{
		Serial.println(F("QA mode"));
	}
#endif
	_s->write(setConfig, sizeof(setConfig));
		// Wait for the response
	delay(1500);
	while (_s->available() > 0) {
		byte c = _s->read();//Clear 
		//Serial.print(c);
	}
}

void WinsenZH03::readContinus()
{
	if (_s->find(0x42)) {    //start to read when detect 0x42


		_s->readBytes(buf, BYTESCOUNT);

		if (buf[0] == 0x4d) {
#ifdef DEBUGBYTES
			for (int i = 0; i < BYTESCOUNT; i++)
			{
				Serial.print(buf[i], HEX); Serial.print(" ");
			}
			Serial.print("\r\n");
#endif
			if (checkValueCon(buf, BYTESCOUNT))
			{
				this->PM1_0 = (buf[2] << 8) + buf[3];
				this->PM2_5 = (buf[4] << 8) + buf[5];
				this->PM10 = (buf[6] << 8) + buf[7];
			}
			

		}
	}
}

int WinsenZH03::readOnce()
{
	int once_flag = 0;
	static byte petitionn[] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };// Petition to get a single result
	static byte measure[8] = { 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };// Space for the response	
	_s->write(petitionn, sizeof(petitionn));
	delay(1500);
	// read

	if (_s->available() > 0) {
		_s->readBytes(measure, 9);
	}

	#ifdef DEBUGBYTES

	for (int i = 0; i < 9; i++)
	{
		//Serial.print(i);
		Serial.print(" ");
		Serial.print(measure[i], HEX);
		//Serial.print("|");

	}

	#endif

	if (measure[1] == 0x86)
	{
		if (checkValueMan(measure))
		{

			this->PM1_0 = (measure[2] << 8) + measure[3];
			this->PM2_5 = (measure[4] << 8) + measure[5];
			this->PM10 = (measure[6] << 8) + measure[7];
			once_flag = 1;
#ifdef DEBUGOTHERS
			Serial.println(F("check ok"));
#endif
		}
		else
		{
#ifdef DEBUGOTHERS
			Serial.println(F("check fails"));
#endif
			once_flag = 0;
		}



#ifdef DEBUGOTHERS
		Serial.print(F("PM1_0:"));
		Serial.print(PM1_0);
		Serial.print(F(" PM2_5:"));
		Serial.print(PM2_5);
		Serial.print(F(" PM10:"));
		Serial.println(PM10);
#endif

	}
	delay(100);
	return once_flag;
	}


int WinsenZH03::checkValueCon(unsigned char *thebuf, char leng)
{
	int receiveflag = 0;
	int receiveSum = 0;

	for (int i = 0; i < (leng - 2); i++) {
		receiveSum = receiveSum + thebuf[i];
	}
	receiveSum = receiveSum + 0x42;

	if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1]))  //check the serial data 
	{
#ifdef DEBUGBYTES
		Serial.println(F("Check continuous ok"));
#endif


		receiveSum = 0;
		receiveflag = 1;
	}
	


	return receiveflag;
}

int WinsenZH03::checkValueMan(unsigned char *thebuf)
{
	int receiveflag = 0;
	int receiveSum = 0;

	for (int i = 1; i < 8; i++) {
		receiveSum = receiveSum + thebuf[i];
		//Serial.print(thebuf[i],HEX); Serial.print(" ");
	}
#ifdef DEBUGOTHERS
	Serial.print((byte)(~receiveSum + 1), HEX);
	Serial.print(F("<->"));
	Serial.println(thebuf[8], HEX);
#endif
	if (((byte)(~receiveSum + 1)) == thebuf[8])  //check the serial data 
	{
#ifdef DEBUGOTHERS
		Serial.println("F(Check manual ok"));
#endif
		receiveSum = 0;
		receiveflag = 1;
	}
	return receiveflag;
}

float WinsenZH03::readPM1_0()
{
	//int PM01Val;
	//PM01Val = (measure[2] << 8) + measure[3]; //count PM1.0 value of the air detector module
	//return PM01Val;
	return this->PM1_0;
}

float WinsenZH03::readPM2_5()
{
	//int PM2_5Val;
	//PM2_5Val = (measure[4] << 8) + measure[5];//count PM2.5 value of the air detector module
	//return PM2_5Val;
	return this->PM2_5;
}

float WinsenZH03::readPM10()
{
	//int PM10Val;
	//PM10Val = (measure[6] << 8) + measure[7]; //count PM10 value of the air detector module  
	//return PM10Val;
	return this->PM10;
}


int WinsenZH03::sleep()
{
	
		byte setSleep[] = { 0xFF, 0x01, 0xA7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x57};//duerme
		_s->write(setSleep, sizeof(setSleep));
		return slewarespo();
	}
	
int WinsenZH03::wake()
{
	
		byte setWake[] = { 0xFF, 0x01, 0xA7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58};//despierta
		_s->write(setWake, sizeof(setWake));
		return slewarespo();
	}
	
int WinsenZH03::slewarespo()
{
			byte response[9] = { 0xFF, 0xA7, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58 };	
				delay(1500);
				while (_s->available() > 0) {
			byte c = _s->read();//Clear 
		}
				if (c==response){
			
			return 1;
		}else{
			return 0;
		}
		
		
	}
	
