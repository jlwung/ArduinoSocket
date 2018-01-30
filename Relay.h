#ifndef _RELAY_H_
#define _RELAY_H_
#pragma once

#include <arduino.h>
#include "Constant.h"

enum RelayStatus {
	RelayOff,
	RelayOn
};

#define RELAYID_MAX	6

#define RELAYTYPE_ON_SIGNALREVS			1
#define RELAYTYPE_ON_TIMEROFF			2
#define RELAYTYPE_ON_SIGNALOFF			3
#define RELAYTYPE_ON_BOTHOFF			5

#define RELAYTYPE_OFF_SIGNALREVS		11 //�ϵ紦�ڶϿ����޶�ʱ�����յ��źź�״̬��ת
#define RELAYTYPE_OFF_TIMERON			12 //�ϵ紦�ڶϿ�����ʱ���ڼ䴦�ڽ�ͨ���յ��źź��޶���
#define RELAYTYPE_OFF_SIGNALON			13 //�ϵ紦�ڶϿ����޶�ʱ�����յ��źź��ͨһ��ʱ��
#define RELAYTYPE_OFF_BOTHON			15 //�ϵ紦�ڶϿ�����ʱ���ڼ䴦�ڽ�ͨ���յ��źź��ͨһ��ʱ��

class Relay {
public:
	uint8_t relayId;
	uint8_t relayPin;
	boolean available;
	uint8_t relayType;
	RelayStatus currentStatus;
	boolean insideTimerFlag;
	boolean insideCountdownFlag;

public:
	void init();
	void turnOn();
	void turnOff();

	void beginTimer();
	void insideTimer();
	void endTimer();
	void outsideTimer();

	void beginCountdown();
	void insideCountdown();
	void endCountdown();
	void outsideCountdown();
};

#endif
