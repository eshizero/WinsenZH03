/*
* WinsenZH03.cpp

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

#ifndef _WINSENZH03_h
#define _WINSENZH03_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


//#define DEBUGBYTES
//#define DEBUGOTHERS
#define BYTESCOUNT 23 

#define QA false
#define ACTIVE true

class WinsenZH03
{
public:

	WinsenZH03();
	void begin(Stream *ser);
	void setAs(bool active);
	void readContinus();
	int readOnce();
	int sleep();
	int wake();
	float readPM1_0();
	float readPM2_5();
	float readPM10();
	unsigned char buf[BYTESCOUNT];
private:

	int checkValueCon(unsigned char *thebuf, char leng);
	int checkValueMan(unsigned char *thebuf);
	Stream *_s;
	float PM1_0 = 0.0;
	float PM2_5 = 0.0;
	float PM10  = 0.0;

};

#endif