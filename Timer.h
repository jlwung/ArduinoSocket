#ifndef _TIMER_H_
#define _TIMER_H_

#include <arduino.h>
#include "DS1307.h"
#include "RelayManager.h"

#define TIMERID_MAX 10

#define TIMER_ENABLE  1
#define TIMER_DISABLE  0

#define MON_BIT	bit(0)
#define TUE_BIT bit(1)


class Timer
{
public:
	Timer();
	void toString(char *, int);
	String toRespString();
	String toPrintString();

	uint8_t  TimerId;
	uint8_t  Available;	//0: unavailable  1:available
	uint8_t  RelayId;
	uint8_t  StartDays; //bit0:Mon, bit1:Tue, ...
	uint8_t  StartHour;
	uint8_t  StartMin;
	uint8_t  StartSec;
	uint8_t  EndDays;
	uint8_t  EndHour;
	uint8_t  EndMin;
	uint8_t  EndSec;
	uint8_t  IntermOnSec;	//0:always on; >0:intermittent on
	uint8_t  IntermOffSec;

	boolean  inTimer;

public:
	void reset();
	void enable();
	void disable();
	boolean setTimerId(uint8_t id);
	boolean setRelayId(uint8_t  id);
	boolean setStartDays(uint8_t days);
	boolean setStartDays(char* days);
	boolean setStartTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setEndTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setIntermSec(uint8_t on, uint8_t off);
	boolean isInsideTimer(Time t);

	RelayManager* pRelayManager;

	void onLoop(const Time time);

private:
	String toStringHHMM(uint8_t hour, uint8_t mins);
};

#endif 

