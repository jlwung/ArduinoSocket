
#include "Persistence.h"
#include <EEPROM.h>

//EEPROM struct

/**************************
 *       Sys Data         *
 **************************
 *        Timer           *
 **************************
 *       Signal           *
 **************************/

struct SystemStruct {
	uint8_t versionMajor;
	uint8_t versionMinor;
};

union SysData {
	char buffer[100];
	SystemStruct data;
};


//the data structure saved in EEPROM
struct TimerStruct {
    uint8_t  TimerId;
	uint8_t  Available;
    uint8_t  RelayId;
    uint8_t  StartDays; //bit0:Mon, bit1:Tue, ...
    uint8_t  StartHour;
    uint8_t  StartMin;
	uint8_t  StartSec;
    uint8_t  EndHour;
    uint8_t  EndMin;
	uint8_t  EndSec;
    uint8_t  IntermOnSec;
    uint8_t  IntermOffSec;
};

struct InstantStruct {
	uint8_t relayId;
	uint8_t available;
	unsigned long InstantSec;
};

struct SignalStruct {
	uint8_t signalId;
	uint8_t signalPin;	//the pin receive the signal
	uint8_t signalType;
	uint8_t available;
	InstantStruct instants[SIGNAL_INSTANTNUM_MAX];
};


Persistence::Persistence()
{
}

void Persistence::init()
{
	Serial.println("Initializing Persist...");
}

void Persistence::reset()
{
}
  
boolean Persistence::loadTimer(uint8_t id, Timer* t)
{
	TimerStruct ts;
	
	if (id > TIMERID_MAX) return ERROR_INVALID_TIMERID;
	
    EEPROM.get(sizeof(union SysData)+sizeof(struct TimerStruct)*id, ts);
	
	t->TimerId = id;
	t->Available = ts.Available;
	t->RelayId = ts.RelayId;
	t->StartDays = ts.StartDays;
	t->StartHour = ts.StartHour;
	t->StartMin = ts.StartMin;
	t->StartSec = ts.StartSec;
	if (ts.EndHour*10000+ts.EndMin*100+ts.EndSec < ts.StartHour*10000+ts.StartMin*100+ts.StartSec)
	{
		// End time is smaller than start time, swith days to next days - shift left
		t->EndDays = ts.StartDays<<1;
		if(bitRead(t->EndDays, 7))
		{
			bitClear(t->EndDays, 7);
			bitSet(t->EndDays, 0);
		}
	}
	t->EndHour = ts.EndHour;
	t->EndMin = ts.EndMin;
	t->EndSec = ts.EndSec;
	t->IntermOnSec = ts.IntermOnSec;
	t->IntermOffSec = ts.IntermOffSec;
	return true;
}
  
boolean Persistence::saveTimer(Timer* t)
{
	TimerStruct ts;

	if (t->TimerId > TIMERID_MAX) return ERROR_INVALID_TIMERID;
	
	ts.Available = t->Available;
	ts.RelayId = t->RelayId;
	ts.StartDays = t->StartDays;
	ts.StartHour = t->StartHour;
	ts.StartMin = t->StartMin;
	ts.StartSec = t->StartSec;
	ts.EndHour = t->EndHour;
	ts.EndMin = t->EndMin;
	ts.EndSec = t->EndSec;
	ts.IntermOnSec = t->IntermOnSec;
	ts.IntermOffSec = t->IntermOffSec;
	
	EEPROM.put(sizeof(union SysData) + sizeof(struct TimerStruct)*t->TimerId, ts);
	
	return 0;
}
  

boolean Persistence::loadSignal(uint8_t id, Signal* s)
{
	SignalStruct ss;

	if (id > SIGNAL_PINNUM_MAX) 
		return false;

	EEPROM.get(sizeof(union SysData) + sizeof(struct TimerStruct)*TIMERID_MAX + sizeof(struct SignalStruct)*id, ss);

	s->signalId = ss.signalId;
	s->signalPin = ss.signalPin;
	s->signalType = ss.signalType;
	s->available = ss.available;

	s->instants[0].relayId = ss.instants[0].relayId;
	s->instants[0].available = ss.instants[0].available;
	s->instants[0].InstantSec = ss.instants[0].InstantSec;

	s->instants[1].relayId = ss.instants[1].relayId;
	s->instants[1].available = ss.instants[1].available;
	s->instants[1].InstantSec = ss.instants[1].InstantSec;

	return true;
}

boolean Persistence::saveSignal(Signal* s)
{
	SignalStruct ss;

	if (s->signalId > SIGNAL_PINNUM_MAX) 
		return false;

	ss.signalId = s->signalId;
	ss.signalPin = s->signalPin;
	ss.signalType = s->signalType;
	ss.available = s->available;

	ss.instants[0].relayId= s->instants[0].relayId;
	ss.instants[0].available = s->instants[0].available;
	ss.instants[0].InstantSec = s->instants[0].InstantSec ;

	ss.instants[1].relayId = s->instants[1].relayId ;
	ss.instants[1].available = s->instants[1].available;
	ss.instants[1].InstantSec = s->instants[1].InstantSec;

	EEPROM.put(sizeof(union SysData) + sizeof(struct TimerStruct)*TIMERID_MAX + sizeof(struct SignalStruct)*ss.signalId, ss);

	return 0;
}


