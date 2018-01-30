
#include "Relay.h"
#include "Debug.h"

void Relay::init()
{
	if (!available)
		return;

	pinMode(relayPin, OUTPUT);
	switch (relayType) {
	case RELAYTYPE_ON_SIGNALREVS:
	case RELAYTYPE_ON_TIMEROFF:
	case RELAYTYPE_ON_SIGNALOFF:
	case RELAYTYPE_ON_BOTHOFF:
		turnOn();
		break;
	case RELAYTYPE_OFF_SIGNALREVS:
	case RELAYTYPE_OFF_TIMERON:
	case RELAYTYPE_OFF_SIGNALON:
	case RELAYTYPE_OFF_BOTHON:
		turnOff();
		break;
	}
}

void Relay::turnOn()
{
	digitalWrite(relayPin, LOW);
	currentStatus = RelayOn;
}

void Relay::turnOff()
{
	digitalWrite(relayPin, HIGH);
	currentStatus = RelayOff;
}

void Relay::beginTimer()
{
	IPRINT("beginTimer() - RelayId:");
	IPRINT(relayId);
	IPRINT(" relayPin:");
	IPRINTLN(relayPin);

	insideTimerFlag = true;
	switch (relayType) {
	case RELAYTYPE_ON_TIMEROFF:
	case RELAYTYPE_ON_BOTHOFF:
		turnOff();
		break;
	case RELAYTYPE_OFF_TIMERON:
	case RELAYTYPE_OFF_BOTHON:
		turnOn();
		break;
	}
}

void Relay::insideTimer()
{
	DPRINT("insideTimer() - RelayId:");
	DPRINT(relayId);
	DPRINT(" relayPin:");
	DPRINTLN(relayPin);

	insideTimerFlag = true;
	switch (relayType) {
	case RELAYTYPE_ON_TIMEROFF:
	case RELAYTYPE_ON_BOTHOFF:
		turnOff();
		break;
	case RELAYTYPE_OFF_TIMERON:
	case RELAYTYPE_OFF_BOTHON:
		turnOn();
		break;
	}
}

void Relay::endTimer()
{
	IPRINT("endTimer() - RelayId:");
	IPRINT(relayId);
	IPRINT(" relayPin:");
	IPRINTLN(relayPin);


	insideTimerFlag = false;
	switch (relayType) {
	case RELAYTYPE_ON_TIMEROFF:
		turnOn();
		break;
	case RELAYTYPE_OFF_TIMERON:
		turnOff();
		break;
	case RELAYTYPE_ON_BOTHOFF:
		if (insideCountdownFlag)
			turnOff();
		else
			turnOn();
		break;
	case RELAYTYPE_OFF_BOTHON:
		if (insideCountdownFlag)
			turnOn();
		else
			turnOff();
		break;
	}
}

void Relay::outsideTimer()
{
	DPRINT("outsideTimer() - RelayId:");
	DPRINT(relayId);
	DPRINT(" relayPin:");
	DPRINTLN(relayPin);

	insideTimerFlag = false;
	switch (relayType) {
	case RELAYTYPE_ON_TIMEROFF:
		turnOn();
		break;
	case RELAYTYPE_OFF_TIMERON:
		turnOff();
		break;
	case RELAYTYPE_ON_BOTHOFF:
		if (insideCountdownFlag)
			turnOff();
		else
			turnOn();
		break;
	case RELAYTYPE_OFF_BOTHON:
		if (insideCountdownFlag)
			turnOn();
		else
			turnOff();
		break;
	}
}


void Relay::beginCountdown()
{
	IPRINT("beginCountdown() - RelayId:");
	IPRINT(relayId);
	IPRINT(" relayPin:");
	IPRINTLN(relayPin);


	insideCountdownFlag = true;
	switch (relayType) {
	case RELAYTYPE_ON_SIGNALREVS:
	case RELAYTYPE_OFF_SIGNALREVS:
		if (currentStatus == RelayOff) {
			turnOn();
		}
		else{
			turnOff();
		}
		break;
	case RELAYTYPE_ON_SIGNALOFF:
	case RELAYTYPE_ON_BOTHOFF:
		turnOff();
		break;
	case RELAYTYPE_OFF_SIGNALON:
	case RELAYTYPE_OFF_BOTHON:
		turnOn();
		break;
	default:
		break;
	}
}


void Relay::insideCountdown()
{
	DPRINT("insideCountdown() - RelayId:");
	DPRINT(relayId);
	DPRINT(" relayPin:");
	DPRINTLN(relayPin);

	insideCountdownFlag = true;
}

void Relay::endCountdown()
{
	IPRINT("endCountdown() - RelayId:");
	IPRINT(relayId);
	IPRINT(" relayPin:");
	IPRINTLN(relayPin);


	insideCountdownFlag = false;
	switch (relayType) {
	case RELAYTYPE_ON_SIGNALOFF:
		turnOn();
		break;
	case RELAYTYPE_OFF_SIGNALON:
		turnOff();
		break;
	case RELAYTYPE_ON_BOTHOFF:
		if (insideTimerFlag)
			turnOff();
		else
			turnOn();
		break;
	case RELAYTYPE_OFF_BOTHON:
		if (insideTimerFlag)
			turnOn();
		else
			turnOff();
		break;
	}
}

void Relay::outsideCountdown()
{
	DPRINT("outsideCountdown() - RelayId:");
	DPRINT(relayId);
	DPRINT(" relayPin:");
	DPRINTLN(relayPin);

	insideCountdownFlag = false;
}
