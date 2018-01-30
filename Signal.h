#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#pragma once

#include <Arduino.h>
#include "Debug.h"
#include "RelayManager.h"

#define SIGNALTYPE_UNKNOW		0
#define SIGNALTYPE_RESTART		1
#define SIGNALTYPE_RESET		2

#define SIGNAL_PINNUM_MAX		6
#define SIGNAL_INSTANTNUM_MAX	2

#define DEBOUNCEDELAY			20

#define SIGNAL_BUTTON_PUSHDOWN	LOW
#define SIGNAL_BUTTON_POPUP		HIGH

enum ButtonStatus
{
	Unknow,
	ButtonPushDown,
	ButtonOnDown,
	ButtonPopUp,
	ButtonOnUp
};

class Instant {
public:
	uint8_t relayId;
	uint8_t available;
	unsigned long InstantSec;
	unsigned long InstantEndMillis;		// millis()
	boolean	inInstant;

	Instant();
};


class Signal
{
public:
	uint8_t signalId;
	uint8_t available;
	uint8_t signalType;
	uint8_t signalPin;	//the pin receive the signal
	Instant instants[SIGNAL_INSTANTNUM_MAX];

	int lastButtonState;
	unsigned long LastTimeButtonPushDown;
	unsigned long LastTimeButtonPopUp;
	unsigned long lastTimeDebounce;
	unsigned long InstantStartMillis;	//keeping changing during down

	RelayManager* pRelayManager;

	Signal();
	void init();

	ButtonStatus buttonRead();
	void eventPushDown();
	void eventPopUp();
	void eventOnDown();
	void eventOnUp();

	void onLoop();
	String toPrintString();
	String toRespString();
};


#endif
