#ifndef _SIGNALMANAGER_H_
#define _SIGNALMANAGER_H_

#include <Arduino.h>
#include "Constant.h"
#include "Command.h"
#include "Signal.h"
#include "Persistence.h"
#include "RelayManager.h"


class SignalManager
{
public:
	Signal signals[SIGNAL_PINNUM_MAX];

	RelayManager* pRelayManager;
	Persistence tp;

	SignalManager(RelayManager* rm);
	void init();
	void reset();
	void onLoop();
	void showAll();

	boolean executeRequest(Req* req, Resp* resp);

private:

};

#endif
