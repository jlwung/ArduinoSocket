
#include "SignalManager.h"
#include "Debug.h"


SignalManager::SignalManager(RelayManager* rm):pRelayManager(rm)
{
	for (int i = 0; i < SIGNAL_PINNUM_MAX; i++) {
		signals[i].signalId = i;
		signals[i].available = false;
		signals[i].pRelayManager = pRelayManager;
	}
}

void SignalManager::init()
{
	for (int i = 0; i < SIGNAL_PINNUM_MAX; i++) {
		tp.loadSignal(i, &signals[i]);
		signals[i].init();
	}
}

void SignalManager::reset()
{
	signals[0].signalId = 0;
	signals[0].available = true;
	signals[0].signalType = SIGNALTYPE_RESTART;
	signals[0].signalPin = PIN_SIGNAL_FEEDING;
	signals[0].instants[0].relayId = RELAYID1_FILTERUP;
	signals[0].instants[0].available = true;
	signals[0].instants[0].InstantSec = 3;
	signals[0].instants[1].relayId = RELAYID2_FILTERDOWN;
	signals[0].instants[1].available = true;
	signals[0].instants[1].InstantSec = 5;

	signals[1].signalId = 1;
	signals[1].available = true;
	signals[1].signalType = SIGNALTYPE_RESTART;
	signals[1].signalPin = PIN_SIGNAL_TANKLIGHT;
	signals[1].instants[0].relayId = RELAYID3_TANKLIGHT;
	signals[1].instants[0].available = true;
	signals[1].instants[0].InstantSec = 7;
	signals[1].instants[1].available = false;

	signals[2].signalId = 2;
	signals[2].available = true;
	signals[2].signalType = SIGNALTYPE_RESET;
	signals[2].signalPin = PIN_SIGNAL_CUBINETLIGHT;
	signals[2].instants[0].relayId = RELAYID4_CUBINETLIGHT;
	signals[2].instants[0].available = true;
	signals[2].instants[0].InstantSec = 9;
	signals[2].instants[1].available = false;

	signals[3].signalId = 3;
	signals[3].available = true;
	signals[3].signalType = SIGNALTYPE_RESET;
	signals[3].signalPin = PIN_SIGNAL_FILTERUP;
	signals[3].instants[0].relayId = RELAYID1_FILTERUP;
	signals[3].instants[0].available = true;
	signals[3].instants[0].InstantSec = 11;
	signals[3].instants[1].available = false;

	signals[4].signalId = 4;
	signals[4].available = true;
	signals[4].signalType = SIGNALTYPE_RESET;
	signals[4].signalPin = PIN_SIGNAL_FILTERDOWN;
	signals[4].instants[0].relayId = RELAYID2_FILTERDOWN;
	signals[4].instants[0].available = true;
	signals[4].instants[0].InstantSec = 15;
	signals[4].instants[1].available = false;

	for (int i = 0; i < SIGNAL_PINNUM_MAX; i++) {
		signals[i].signalId = i;
		tp.saveSignal(&signals[i]);
		signals[i].init();
	}
}

void SignalManager::onLoop()
{
	for (int i = 0; i < SIGNAL_PINNUM_MAX; i++) {
		signals[i].onLoop();
	}
}

boolean SignalManager::executeRequest(Req * req, Resp * resp)
{
	uint8_t signalId = req->signal.signalId;

	resp->cmdId = req->cmdId;
	resp->cmdType = req->cmdType;
	resp->cmdRst = RST_SUCCESS;

	DPRINT("Signal Command:");
	DPRINTLN(req->cmdType);

	switch (req->cmdType) {
	case REQ_SIGNAL_GET:
		if (!req->hasSignalId())		{
			resp->cmdRst = RST_ERROR_INVALID_SIGNALPARAM;
			break;
		}
		if(signalId >= SIGNAL_PINNUM_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_SIGNALID;
			break;
		}
		resp->signal = signals[signalId];
		DPRINTLN(signals[signalId].toPrintString());
		DPRINTLN(req->signal.toPrintString());
		break;
	case REQ_SIGNAL_SET:
		if (!req->hasSignalId()) {
			resp->cmdRst = RST_ERROR_INVALID_SIGNALPARAM;
			break;
		}
		if (signalId >= SIGNAL_PINNUM_MAX) {
			resp->cmdRst = RST_ERROR_INVALID_SIGNALID;
			break;
		}
		if (!(req->hasInstS0() || req->hasInstS1())) {
			resp->cmdRst = RST_ERROR_INVALID_SIGNALPARAM;
			break;
		}
		if (req->hasInstS0()) {
			signals[signalId].instants[0] = req->signal.instants[0];
		}
		if (req->hasInstS1()) {
			signals[signalId].instants[1] = req->signal.instants[1];
		}
		tp.saveSignal(&(signals[signalId]));
		break;
	case REQ_SIGNAL_RESET:
		reset();
		break;

	case REQ_SIGNAL_SHOWALL:
		showAll();
		break;
	default:
		resp->cmdRst = RST_ERROR_UNKNOWCMD_SIGNAL;
		break;
	}

	return boolean(true);
}

void SignalManager::showAll()
{
	Serial.println("Show all Signal");
	Serial.print("current millis:");
	Serial.println(millis());
	for (int i = 0; i < SIGNAL_PINNUM_MAX; i++) {
		Serial.println(signals[i].toPrintString());
	}
}