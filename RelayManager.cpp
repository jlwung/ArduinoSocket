
#include "Relaymanager.h"
#include "Debug.h"

RelayManager::RelayManager()
{
	for (int i = 0; i < RELAYID_MAX; i++) {
		relays[i].relayId = i;
	}
}

void RelayManager::init()
{
	relays[0].relayId = RELAYID1_FILTERUP;
	relays[0].relayPin = PIN_RELAY_FILTERUP;
	relays[0].available = true;
	relays[0].relayType = RELAYTYPE_ON_BOTHOFF;

	relays[1].relayId = RELAYID2_FILTERDOWN;
	relays[1].relayPin = PIN_RELAY_FILTERDOWN;
	relays[1].available = true;
	relays[1].relayType = RELAYTYPE_ON_BOTHOFF;

	relays[2].relayId = RELAYID3_TANKLIGHT;
	relays[2].relayPin = PIN_RELAY_TANKLIGHT;
	relays[2].available = true;
	relays[2].relayType = RELAYTYPE_OFF_BOTHON;

	relays[3].relayId = RELAYID4_CUBINETLIGHT;
	relays[3].relayPin = PIN_RELAY_CUBINETLIGHT;
	relays[3].available = true;
	relays[3].relayType = RELAYTYPE_OFF_BOTHON;

	for(int i=0; i<RELAYID_MAX; i++){
		relays[i].init();
	}
}

int RelayManager::getIndex(uint8_t relayId)
{
	for (int i = 0; i < RELAYID_MAX; i++) {
		if (relays[i].relayId == relayId) {
			return i;
		}
	}

	return -1;
}

void RelayManager::beginTimer(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;
	
	relays[index].beginTimer();
}

void RelayManager::insideTimer(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;

	relays[index].insideTimer();
}

void RelayManager::endTimer(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;

	relays[index].endTimer();
}

void RelayManager::outsideTimer(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;
	
	relays[index].outsideTimer();
}

void RelayManager::beginCountdown(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;

	relays[index].beginCountdown();

}

void RelayManager::insideCountdown(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if(!relays[index].available)
		return;
	
	relays[index].insideCountdown();
	
}

void RelayManager::endCountdown(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;

	relays[index].endCountdown();

}

void RelayManager::outsideCountdown(uint8_t relayId)
{
	int index = getIndex(relayId);

	if (index == -1) return;

	if (!relays[index].available)
		return;
	
	relays[index].outsideCountdown();
}
