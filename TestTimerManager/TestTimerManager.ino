
#include <DS1307.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
//#include "Communication.h"
#include "TimerManager.h"
#include "Timer.h"

RelayManager rm;

TimerManager tm(&rm);

void setup() {

	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	tm.init();

	//Serial.println("Initing time...");
	//tm.setDate(2018, 1, 24);
	//tm.setDay(3);
	//tm.setTime(21, 55, 43);

	delay(100);
	tm.showAll();

}

void loop() {


	delay(1000);
}


