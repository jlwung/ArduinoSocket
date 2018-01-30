#include "Signal.h"


Instant::Instant()
{
	available = false;
	relayId = 0;
	InstantSec = 0;
	InstantEndMillis = 0;
	inInstant = false;
}


Signal::Signal()
{
	unsigned long ms = millis();
	LastTimeButtonPushDown = ms - 2;
	LastTimeButtonPopUp = ms - 1;
	lastTimeDebounce = ms;
	lastButtonState = SIGNAL_BUTTON_POPUP;
	available = false;

	signalId = SIGNAL_PINNUM_MAX + 1;
	signalType = SIGNALTYPE_UNKNOW;
}

void Signal::init()
{
	if(available)
		pinMode(signalPin, INPUT_PULLUP);
}

ButtonStatus Signal::buttonRead() {

	if (!available)
		return Unknow;

	int reading = digitalRead(signalPin);

	if (reading == lastButtonState) {
		lastTimeDebounce = millis();
		if (reading == SIGNAL_BUTTON_PUSHDOWN)
			return ButtonOnDown;
		else
			return ButtonOnUp;
	}
	else {
		if ((millis() - lastTimeDebounce) > DEBOUNCEDELAY) {
			//读取值变化且过了防抖动时间，确认状态变化
			IPRINTLN("button status changed.");
			lastButtonState = reading;
			lastTimeDebounce = millis();
			if (reading == SIGNAL_BUTTON_PUSHDOWN) {
				LastTimeButtonPushDown = lastTimeDebounce;
				return ButtonPushDown;
			}
			else {
				LastTimeButtonPopUp = lastTimeDebounce;
				return ButtonPopUp;
			}
		}
		else {
			//读取值变化但在防抖动时间内，保持原状态，等待下次检查
			if (lastButtonState == SIGNAL_BUTTON_PUSHDOWN)
				return ButtonOnDown;
			else
				return ButtonOnUp;
		}
	}
}

void Signal::eventPushDown()
{
	IPRINT("push down, pin:");
	IPRINTLN(signalPin);
	boolean inInstant = false;
	InstantStartMillis = millis();
	if (signalType == SIGNALTYPE_RESTART) {
		for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
			if (!instants[i].available) {
				continue;
			}
			instants[i].InstantEndMillis = InstantStartMillis + instants[i].InstantSec * 1000;
			instants[i].inInstant = true;
			pRelayManager->beginCountdown(instants[i].relayId);
		}
	}
	else if (signalType == SIGNALTYPE_RESET) {
		for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
			if (!instants[i].available) {
				continue;
			}

			inInstant |= instants[i].inInstant;
		}

		if (inInstant) {
			//reset instants if any instant
			for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
				if (!instants[i].available || !instants[i].inInstant) {
					continue;
				}

				instants[i].InstantEndMillis = InstantStartMillis;
				instants[i].inInstant = false;
				pRelayManager->endCountdown(instants[i].relayId);
			}
		}
		else {
			//restart all instants if no instant
			for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
				if (!instants[i].available) {
					continue;
				}

				instants[i].InstantEndMillis = InstantStartMillis + instants[i].InstantSec * 1000;
				instants[i].inInstant = true;
				pRelayManager->beginCountdown(instants[i].relayId);
			}
		}
	}
}

void Signal::eventOnDown()
{
	InstantStartMillis = millis();
	for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
		if (instants[i].available) {
			if (signalType == SIGNALTYPE_RESTART) {
				instants[i].InstantEndMillis = InstantStartMillis + instants[i].InstantSec * 1000;
				instants[i].inInstant = true;
				pRelayManager->insideCountdown(instants[i].relayId);
			}
			else if (signalType == SIGNALTYPE_RESET) {
				if(instants[i].inInstant)
					pRelayManager->insideCountdown(instants[i].relayId);
				else
					pRelayManager->outsideCountdown(instants[i].relayId);
			}
		}
	}
}


void Signal::eventPopUp()
{
	IPRINT("pop up, pin:");
	IPRINTLN(signalPin);
	InstantStartMillis = millis();
	for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
		if (instants[i].available) {
			if (signalType == SIGNALTYPE_RESTART) {
				instants[i].InstantEndMillis = InstantStartMillis + instants[i].InstantSec * 1000;
				instants[i].inInstant = true;
				pRelayManager->insideCountdown(instants[i].relayId);
			}
			else if (signalType == SIGNALTYPE_RESET) {
				if (instants[i].inInstant)
					pRelayManager->insideCountdown(instants[i].relayId);
				else
					pRelayManager->outsideCountdown(instants[i].relayId);
			}
		}
	}
}

void Signal::eventOnUp()
{
	unsigned long ms = millis();

	for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
		if (!instants[i].available)
			continue;

		if (instants[i].inInstant == false) {
			pRelayManager->outsideCountdown(instants[i].relayId);
		}
		else {
			if (InstantStartMillis < instants[i].InstantEndMillis) {
				if (ms >= InstantStartMillis && ms < instants[i].InstantEndMillis)
					pRelayManager->insideCountdown(instants[i].relayId);
				else {
					IPRINT("Clean instant timer for relay ");
					IPRINTLN(instants[i].relayId);
					instants[i].inInstant = false;
					pRelayManager->endCountdown(instants[i].relayId);
				}
			}
			else {
				if (ms >= InstantStartMillis || ms < instants[i].InstantEndMillis)
					pRelayManager->insideCountdown(instants[i].relayId);
				else {
					IPRINT("Clean instant timer for relay ");
					IPRINTLN(instants[i].relayId);
					instants[i].inInstant = false;
					pRelayManager->endCountdown(instants[i].relayId);
				}
			}
		}
	}
}

void Signal::onLoop()
{
	if (!available)
		return;

	ButtonStatus buttonStatus = buttonRead();
	switch (buttonStatus) {
	case ButtonPushDown:
		eventPushDown();
		break;
	case ButtonOnDown:
		eventOnDown();
		break;
	case ButtonPopUp:
		eventPopUp();
		break;
	case ButtonOnUp:
		eventOnUp();
		break;
	default:
		break;
	}
}

String Signal::toRespString()
{
	String s;

	s = String("sId=") + String(signalId);
	s += String("&spin=") + String(signalPin);
	s += String("&styp=") + String(signalType);
	s += String("&avl=") + String(available);
	for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
		s += String("&inst=")+ String(i);
		s += "-" + String(instants[i].available);
		s += "-" + String(instants[i].relayId);
		s += "-" + String(instants[i].InstantSec);
	}
	return s;
}

String Signal::toPrintString()
{
	String str;

	str.reserve(100);

	str += "Signal Id:";
	str += signalId;
	str += " signalPin:";
	str += signalPin;
	str += " signalType:";
	str += signalType;
	if (!available) {
		str += " Unavailable!";
		return str;
	}

	str += " buttonStat:";
	str += lastButtonState;

	for (int i = 0; i < SIGNAL_INSTANTNUM_MAX; i++) {
		str += "\n    inst[";
		str += i;
		str += "]:";
		if (instants[i].available) {
			str += "relayId:";
			str += instants[i].relayId;
			str += " interval:";
			str += instants[i].InstantSec;
			str += " endMillis:";
			str += instants[i].InstantEndMillis;
		}
		else {
			str += "Unavailable";
		}
	}
	return str;
}
