
#include "TimerManager.h"

TimerManager::TimerManager(RelayManager* rm) : rtc(PIN_DS1307_SDA, PIN_DS1307_SCL), rm(rm)
{
	lastLoopMillis = 0;
	for (int i = 0; i < TIMERID_MAX; i++) {
		timers[i].TimerId = i;
		timers[i].pRelayManager = rm;
	}
}

void TimerManager::init()
{
	// Set the clock to run-mode
	Serial.println("Initializing DS1307 ...");
	rtc.begin();
	rtc.halt(false);

	// load timers
	Serial.println("Loading timer from EEPROM ...");
	for (int i = 0; i < TIMERID_MAX; i++)
	{
		tp.loadTimer(i, &timers[i]);
	}

}

void TimerManager::reset()
{
	for (int i = 0; i < TIMERID_MAX; i++)
	{
		timers[i].reset();
		tp.saveTimer(&timers[i]);
	}
}

void TimerManager::onLoop()
{
	Time currentTime = getTime();
	for (int i = 0; i < TIMERID_MAX; i++) {
		if (!timers[i].Available)
			continue;

		timers[i].onLoop(currentTime);
	}
	return;
}

boolean TimerManager::setTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	if ((hour >= 24) || (min >= 60) || (sec >= 60))
		return false;

	rtc.setTime(hour, min, sec);
	return true;
}

boolean TimerManager::setDate(uint16_t year, uint8_t mon, uint8_t date)
{
	if (year < 2000 || year > 3000 || mon < 1 || mon > 12 || date < 1 || date > 31)
		return false;

	rtc.setDate(date, mon, year);
	return true;
}

boolean TimerManager::setDay(uint8_t day)
{
	if (day > B1111111)
		return false;

	rtc.setDOW(day);
	return true;
}

Time TimerManager::getTime()
{
	return rtc.getTime();
}

String TimerManager::getTimeStr()
{
	String s = String(rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/'));
	  s += String(" ");
	  s += String(rtc.getDOWStr(FORMAT_LONG));
	  s += " ";
	  s += String(rtc.getTimeStr(FORMAT_LONG));
	return (s);
}

Timer* TimerManager::getTimer(uint8_t id)
{
	if (id > TIMERID_MAX)
		return NULL;

	return &(timers[id]);
}

int TimerManager::allocTimerId()
{
	for (int i = 0; i < TIMERID_MAX; i++)
	{
		if (!timers[i].Available)
			return i;
	}
	return -1;
}


boolean TimerManager::enableTimer(uint8_t timerId, uint8_t relayId)
{
	if (timerId > TIMERID_MAX)
		return false;

	timers[timerId].Available = TIMER_ENABLE;
	timers[timerId].RelayId = relayId;
	return true;
}

boolean TimerManager::disableTimer(uint8_t id)
{
	if (id > TIMERID_MAX)
		return false;

	timers[id].Available = TIMER_DISABLE;
	return true;
}

boolean TimerManager::setTimerInterm(uint8_t id, uint8_t on, uint8_t off)
{
	if (id > TIMERID_MAX)
		return false;

	if (on == 0 || off == 0)
		return false;

	timers[id].IntermOnSec = on;
	timers[id].IntermOffSec = off;

	return true;
}

boolean TimerManager::setTimerAlways(uint8_t id)
{
	if (id > TIMERID_MAX)
		return false;

	timers[id].IntermOnSec = 0;
	timers[id].IntermOffSec = 0;
	return true;
}

boolean TimerManager::setTimerStartTime(uint8_t id, uint8_t hour, uint8_t min)
{
	if (id > TIMERID_MAX)
		return false;

	timers[id].StartHour = hour;
	timers[id].StartMin = min;
	return true;
}

boolean TimerManager::setTimerEndTime(uint8_t id, uint8_t hour, uint8_t min)
{
	if (id > TIMERID_MAX)
		return false;

	timers[id].EndHour = hour;
	timers[id].EndMin = min;
	return true;
}

boolean TimerManager::setTimerDays(uint8_t id, uint8_t days)
{
	if (id > TIMERID_MAX)
		return false;

	return timers[id].setStartDays(days);
}

boolean TimerManager::setTimerDays(uint8_t id, char* days)
{
	if (id > TIMERID_MAX || strlen(days) != 7)
		return false;

	return timers[id].setStartDays(days);
}

void TimerManager::showAll()
{
	char str[200] = { 0 };

	snprintf(str, 200, "\nShow all\nTime:\n%s %s %s\nTimer:",
		rtc.getDateStr(FORMAT_LONG, FORMAT_BIGENDIAN, '/'),
		rtc.getDOWStr(FORMAT_LONG),
		rtc.getTimeStr(FORMAT_LONG));
	EPRINTLN(str);

	for (int i = 0; i < TIMERID_MAX; i++)
	{
			EPRINTLN(timers[i].toPrintString());
	}
	EPRINTLN("");
}

boolean TimerManager::executeRequest(Req* req, Resp* resp)
{
	resp->cmdId = req->cmdId;
	resp->cmdType = req->cmdType;
	resp->cmdRst = RST_SUCCESS;
	uint8_t timerId = req->timer.TimerId;


	DPRINT("Timer Command:");
	DPRINTLN(req->cmdType);

	switch (req->cmdType) {
	case REQ_TIME_GET:
		resp->time = rtc.getTime();
		break;

	case REQ_TIME_SET:
		if(req->hasTime())
			rtc.setTime(req->time.hour, req->time.min, req->time.sec);
		if (req->hasDate()) {
			rtc.setDate(req->time.date, req->time.mon, req->time.year);
		}
		if(req->hasDay())
			rtc.setDOW(req->time.dow);
		break;

	case REQ_TIMER_CREATE:
		if (!req->hasTimerId()) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERPARAM;
			return false;
		}
		if (timerId >= TIMERID_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERID;
			return false;
		}
		if (timers[timerId].Available) {
			resp->cmdRst = RST_ERROR_TIMER_ALREADYEXIST;
			break;
		}
		if (!req->hasTimerId() || 
			!req->hasRelayId() || 
			!req->hasStartDays() || 
			!req->hasStartTime() || 
			!req->hasEndTime()) {
			resp->cmdRst= RST_ERROR_INVALID_TIMER;
			break;
		}
		req->timer.Available = true;
		timers[timerId] = req->timer;
		tp.saveTimer(&(timers[timerId]));
		break;

	case REQ_TIMER_GET:
		if (!req->hasTimerId()) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERPARAM;
			return false;
		}
		if (timerId >= TIMERID_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERID;
			return false;
		}
		resp->timer = timers[timerId];
		break;

	case REQ_TIMER_UPDATE:
		if (!req->hasTimerId()) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERPARAM;
			return false;
		}
		if (timerId >= TIMERID_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERID;
			return false;
		}
		if (req->hasRelayId()) {
			if (!timers[timerId].setRelayId(req->timer.RelayId)) {
				resp->cmdRst = RST_ERROR_EXECUTE;
				break;
			}
		}
		if (req->hasStartDays()) {
			if (!timers[timerId].setStartDays(req->timer.StartDays)) {
				resp->cmdRst = RST_ERROR_EXECUTE;
				break;
			}
		}
		if (req->hasStartTime()) {
			if (!timers[timerId].setStartTime(req->timer.StartHour, req->timer.StartMin, req->timer.StartSec)) {
				resp->cmdRst = RST_ERROR_EXECUTE;
				break;
			}
		}
		if (req->hasEndTime()) {
			if (!timers[timerId].setEndTime(req->timer.EndHour, req->timer.EndMin, req->timer.EndSec)) {
				resp->cmdRst = RST_ERROR_EXECUTE;
				break;
			}
		}
		if (req->hasIntermSec()) {
			if (!timers[timerId].setIntermSec(req->timer.IntermOnSec, req->timer.IntermOnSec)) {
				resp->cmdRst = RST_ERROR_EXECUTE;
				break;
			}
		}
		tp.saveTimer(&(timers[timerId]));
		break;

	case REQ_TIMER_DISABLE:
		if (!req->hasTimerId()) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERPARAM;
			return false;
		}
		if (timerId >= TIMERID_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_TIMERID;
			return false;
		}
		if (disableTimer(timerId) != true)
			resp->cmdRst = RST_ERROR_EXECUTE;
		break;
	case REQ_TIMER_RESETALL:
		reset();
		break;

	case REQ_TIMER_SHOWALL:
		showAll();
		break;

	default:
		resp->cmdRst = RST_ERROR_UNKNOWCMD_TIMER;
		return false;

	}

	return true;
}


