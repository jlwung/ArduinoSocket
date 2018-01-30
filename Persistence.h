#ifndef _C_TIMERPERSIST_H_
#define _C_TIMERPERSIST_H_
#pragma once

#include <Arduino.h>
#include "Timer.h"
#include "Signal.h"

#define ERROR_INVALID_TIMERID	1
#define ERROR_INVALID_SIGNALID	2
#define ERROR_INVALID_DATA		3

class Persistence
{
  public:
	  Persistence();
	
	void init();
    void reset();
    boolean loadTimer(uint8_t id, Timer* t);
	boolean saveTimer(Timer* t);

	boolean loadSignal(uint8_t id, Signal* s);
	boolean saveSignal(Signal* s);
	
};

#endif

