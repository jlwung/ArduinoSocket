/*
 Name:		TestSignal.ino
 Created:	1/23/2018 2:00:06 PM
 Author:	jianlwa
*/
#include <Arduino.h>
#include "RelayManager.h"
#include "SignalManager.h"

RelayManager rm;

SignalManager sm = SignalManager(&rm);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	rm.init();
	sm.init();
}

// the loop function runs over and over again until power down or reset
void loop() {
	sm.onLoop();
}
