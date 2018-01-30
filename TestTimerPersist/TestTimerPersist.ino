/*
 Name:		TestTimerPersist.ino
 Created:	1/22/2018 8:10:33 PM
 Author:	Home
*/
#include <EEPROM.h>
#include "TimerPersist.h"

TimerPersist tp;
Timer t;
char str[200];

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	t.enable();
	t.setTimerId(0);
	t.setRelayId(10);
	t.setStartDays(3);
	t.setStartTime(12, 23, 34);
	t.setEndTime(23, 34, 45);

	tp.saveTimer(&t);

	t.setTimerId(1);
	t.setRelayId(20);
	t.setStartDays(4);
	t.setStartTime(11, 22, 33);
	t.setEndTime(22, 33, 44);

	tp.saveTimer(&t);
}

// the loop function runs over and over again until power down or reset
void loop() {
	tp.loadTimer(0, &t);

	t.toString(str, 200);
	Serial.println(str);

	tp.loadTimer(1, &t);
	t.toString(str, 200);
	Serial.println(str);
	
	delay(1000);
}
