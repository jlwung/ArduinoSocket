
#include <DS1307.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include "Communication.h"
#include "TimerManager.h"
#include "SignalManager.h"

RelayManager rm;

TimerManager tm(&rm);

SignalManager sm(&rm);

//Init controler
Communication comm(&tm, &sm);

unsigned long lastLoopMillis, currentLoopMillis;

int i;
void setup() {

	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

	rm.init();
	tm.init();
	sm.init();
	comm.init();

	lastLoopMillis = 0;

	Serial.println("Initialization is done.");
}

void loop() {

	// read command from serial port or bluetooth and execute the command
	comm.onLoop();

	// receive signal and inform RelayManager
	sm.onLoop();

	// check timer one by one and inform RelayManager
	currentLoopMillis = millis();
	if (currentLoopMillis - lastLoopMillis > 900) {
		lastLoopMillis = currentLoopMillis;
		tm.onLoop();
	}

}


