
#include "Timer.h"

Timer::Timer() {
  this->TimerId = TIMERID_MAX+1;
  this->Available = false;
  inTimer = false;
}

void Timer::reset()
{
	disable();
}

void Timer::enable()
{
	Available = true;
}

void Timer::disable()
{
	Available = false;
}

String Timer::toRespString()
{
	String s;

	s = String("tid=") + String(TimerId);
	s += String("&avl=") + String(Available);
	if (Available) {
		s += String("&rId=") + String(RelayId);
		s += String("&sdays=") + String(StartDays);
		s += String("&stime=") + String(StartHour) + ":" + String(StartMin) + ":" + String(StartSec);
		s += String("&edays=") + String(EndDays);
		s += String("&etime=") + String(EndHour) + ":" + String(EndMin) + ":" + String(EndSec);
		s += String("&interm=") + String(IntermOnSec) + "-" + String(IntermOffSec);
	}
	return s;
}

String Timer::toPrintString()
{
	String s;
	s.reserve(100);

	s += String("TimerId:") + String(TimerId);
	s += String(" RelayId:") + String(RelayId);
	if (Available) {
		s += String(" Start:");
		for (int i = 0; i < 7; i++) {
			s += String(bitRead(StartDays, i));
		}
		s += "-";
		s += String(StartHour) + ":" + String(StartMin) + ":" + String(StartSec);
		s += String(" End:");
		for (int i = 0; i < 7; i++) {
			s += String(bitRead(EndDays, i));
		}
		s += "-";
		s += String(EndHour) + ":" + String(EndMin) + ":" + String(EndSec);
		s += String(" Interm:") + String(IntermOnSec) + "-" + String(IntermOffSec);
		s += String(" inTimer:") + String(inTimer);
	}
	else {
		s += String(" Unavailable!");
	}
	return s;
}

void Timer::toString(char* str, int len)
{
	memset(str, 0, len);

	if (!Available) {
		snprintf(str, len - 1, "ID:%d, Pin:%d Unavailable!",
			TimerId, RelayId);

	}
	else {
		snprintf(str, len - 1, "ID:%d, Pin:%d Start:%xH - %02d:%02d:%02d End:%xH - %02d:%02d:%02d Inter:%d-%d",
			TimerId,
			RelayId,
			StartDays,
			StartHour, StartMin, StartSec,
			EndDays,
			EndHour, EndMin, EndSec,
			IntermOnSec, IntermOffSec);
	}
  return;
}

String Timer::toStringHHMM(uint8_t hour, uint8_t mins)
{
  String s = "";
  if (hour < 10)
    s = s + String("0") + String(hour);
  else
    s = s + String(hour);

  s = s + ":";

  if (mins < 10)
    s = s + String("0") + String(mins);
  else
    s = s + String(mins);

  return s;
}


//set value for Timer
boolean Timer::setTimerId(uint8_t id)
{
	if (id > TIMERID_MAX)
		return false;
	
	TimerId = id;

	return true;
}

boolean Timer::setRelayId(uint8_t  id)
{
	RelayId = id;

	return true;	
}

boolean Timer::setStartDays(uint8_t days)
{

	StartDays = days;
	EndDays = days;

	return true;
}

boolean Timer::setStartDays(char* days)
{
	uint8_t daybit = 0;

	for (int i = 0; i < 7; i++) {
		if (days[i] == '1')
			bitSet(daybit, i);
		else if (days[i] == '0')
			bitClear(daybit, i);
		else
			return false;
	}

	return setStartDays(daybit);
}

boolean Timer::setStartTime(uint8_t hour, uint8_t min, uint8_t sec)
{
  if ((hour>=24) || (min>=60) || (sec>=60))
  return false;

	StartHour = hour;
	StartMin = min;
	StartSec = sec;
	return true;
}

boolean Timer::setEndTime(uint8_t hour, uint8_t min, uint8_t sec)
{
  if ((hour>=24) || (min>=60) || (sec>=60))
    return false;

	EndHour = hour;
	EndMin = min;
	EndSec = sec;
	
	return true;
}

boolean Timer::setIntermSec(uint8_t on, uint8_t off)
{
	IntermOnSec = on;
	IntermOffSec = off;
	return true;
}

boolean Timer::isInsideTimer(Time time)
{
	long sTime, cTime, eTime;
	
	sTime = StartHour*10000+StartMin*100+StartSec;
	eTime = EndHour*10000+EndMin*100+EndSec;
	cTime = time.hour*10000 + time.min*100 + time.min;
	if(sTime < eTime){
		// startTIme < endTime, startTime and endTime are at the same day
		if(bitRead(StartDays, time.dow-1)==1){
			if(sTime <= cTime && cTime <= eTime){
				return true;
			}	
		}
	}
	else{
		if(bitRead(StartDays, time.dow-1)==1){
			if(sTime <= cTime){
				return true;
			}
		}
		uint8_t endDays = StartDays << 1;
		bitWrite(endDays, 0, bitRead(StartDays, 6));
		if(bitRead(endDays, time.dow-1)==1){
			if(cTime <= eTime){
				return true;
			}
		}		
	}
	return false;
}

void Timer::onLoop(const Time currentTime)
{
	if (!Available)
		return;

	if (isInsideTimer(currentTime)) {
		if (inTimer) {
			pRelayManager->insideTimer(RelayId);
		}
		else {
			pRelayManager->beginTimer(RelayId);
		}
		inTimer = true;
	}
	else {
		if (inTimer) {
			pRelayManager->endTimer(RelayId);
		}
		else {
			pRelayManager->outsideTimer(RelayId);
		}
		inTimer = false;
	}
}
