/*
   receive all the timer command, and then
   1. maintain all timers in memory
   2, receive timer command and check the avilibity of timer command
   3, update the timer in memory
   4, persist timer to EEPROM
   5, onLoop() is called by loop() to set relay

*/

#ifndef _C_TIMERMANAGER_H_
#define _C_TIMERMANAGER_H_

#pragma once


#include <Arduino.h>
#include <DS1307.h>
#include "Constant.h"
#include "Command.h"
#include "Persistence.h"
#include "RelayManager.h"
#include "Debug.h"

class TimerManager
{
	public:
	TimerManager(RelayManager* rm);
	
	void init();
	void reset();
	void onLoop();

	boolean setTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setDate(uint16_t year, uint8_t mon, uint8_t date);
	boolean setDay(uint8_t);
	Time getTime();
	String getTimeStr();

	Timer* getTimer(uint8_t id);

	int allocTimerId();
	boolean enableTimer(uint8_t timerId, uint8_t relayId);
	boolean disableTimer(uint8_t id);
	boolean setTimerDays(uint8_t id, uint8_t days);
	boolean setTimerDays(uint8_t id, char* days);
	boolean setTimerStartTime(uint8_t id, uint8_t hour, uint8_t min);
	boolean setTimerEndTime(uint8_t id, uint8_t hour, uint8_t min);
	boolean setTimerInterm(uint8_t id, uint8_t on, uint8_t off);
	boolean setTimerAlways(uint8_t id);


	void showAll();
  
	boolean executeRequest(Req* req, Resp* resp);
	
	private:
	unsigned long lastLoopMillis;
	Timer timers[TIMERID_MAX];
	Persistence tp;
	DS1307 rtc;
	RelayManager* rm;
};

#endif
