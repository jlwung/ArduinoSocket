#ifndef _RELAYMANAGER_H_
#define _RELAYMANAGER_H_

#include "Relay.h"
#include "Constant.h"


class RelayManager{
	public:
	Relay relays[RELAYID_MAX];
	
	public:
  RelayManager();
	void init();
	
	// called by Timer
	void beginTimer(uint8_t relayId);
	void insideTimer(uint8_t relayId);
	void endTimer(uint8_t relayId);
	void outsideTimer(uint8_t relayId);
	
	// called by signal Countdown
	void beginCountdown(uint8_t relayId);
	void insideCountdown(uint8_t relayId);
	void endCountdown(uint8_t relayId);
	void outsideCountdown(uint8_t relayId);
	
	private:
	int getIndex(uint8_t relayPinId);
};

#endif
