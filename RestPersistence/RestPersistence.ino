/*
 Name:		RestPersistence.ino
 Created:	1/26/2018 9:31:48 AM
 Author:	Home
*/

//#include <SoftwareSerial.h>
#include "Arduino.h"
#include "TimerManager.h"
#include "SignalManager.h"

RelayManager rm;

TimerManager tm(&rm);

SignalManager sm(&rm);

void setup() {

	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	tm.reset();
	sm.reset();
	Serial.println("Reset is done.");

	tm.init();
	sm.init();
	Serial.println("Initialization is done.");

	tm.showAll();
	sm.showAll();

}

void loop() {

}


