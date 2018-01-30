/*
 Name:		TestDS1307.ino
 Created:	1/22/2018 10:03:21 PM
 Author:	Home
*/

#include <DS1307.h>

DS1307 rtc(4, 5);

char str[200];

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	rtc.begin();
	rtc.halt(false);
	Time t = rtc.getTime();

	Serial.println(t.year);
	strcpy(str, rtc.getDateStr(FORMAT_LONG, FORMAT_BIGENDIAN, '/'));
	Serial.println(str);
	strcpy(str, rtc.getDOWStr(FORMAT_LONG));
	Serial.println(str);
	strcpy(str, rtc.getTimeStr(FORMAT_LONG));
	Serial.println(str);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
