#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <arduino.h>
#include <SoftwareSerial.h>
#include "Constant.h"
#include "Debug.h"
#include "TimerManager.h"
#include "SignalManager.h"
#include "Communication.h"

#define BUFFER_LEN		200

class Communication
{
private:
	SoftwareSerial BT;

	String strSerial;
	String strBT;

	TimerManager* tm;
	SignalManager* sm;

public:
	Communication(TimerManager* tm, SignalManager* sm);

	void init();
	void onLoop();

private:

	int readStrFromBT();
	int readStrFromSerial();

	boolean isCmdStr(const String*);

	String executeCmdStr(String* strReq);

};

#endif
