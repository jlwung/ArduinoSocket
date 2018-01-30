#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#pragma once


// Bluetooth PIN
// RX -> Arduino Digital 2
// TX -> Arduino Digital 3
#define PIN_BLUETOOTH_RX  16
#define PIN_BLUETOOTH_TX  17

// DS1307 PIN
// SDA pin   -> Arduino Digital 4
// SCL pin   -> Arduino Digital 5
#define PIN_DS1307_SDA  20
#define PIN_DS1307_SCL  21

#define PIN_SIGNAL_FEEDING		42
#define PIN_SIGNAL_TANKLIGHT	43
#define PIN_SIGNAL_CUBINETLIGHT	44
#define PIN_SIGNAL_FILTERUP		45
#define PIN_SIGNAL_FILTERDOWN	46

#define PIN_RELAY_FILTERUP      31
#define PIN_RELAY_FILTERDOWN	32
#define PIN_RELAY_TANKLIGHT		33
#define PIN_RELAY_CUBINETLIGHT	34

#define RELAYID1_FILTERUP		0
#define RELAYID2_FILTERDOWN		1
#define RELAYID3_TANKLIGHT		2
#define RELAYID4_CUBINETLIGHT	3

#endif
