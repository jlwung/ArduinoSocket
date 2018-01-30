
#include "Communication.h"


Communication::Communication(TimerManager* tm, SignalManager* sm) :BT(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX)
{
	strSerial = "";
	strBT = "";
	this->tm = tm;
	this->sm = sm;
}

void Communication::init()
{
	BT.begin(9600);

	Serial.println("Bluetooth is ready.");
}


int Communication::readStrFromBT()
{
	int num = 0;

	while (BT.available())
	{
		// get the new byte:
		int inChar = BT.read();

		//break if read faile
		if (inChar < 0)
			return num;

		// add it to the inputString:
		strBT += (char)inChar;
		num++;

		// if the incoming character is command end character, break
		if (inChar == '#') {
			return num;
		}

	}
	return num;
}

int Communication::readStrFromSerial()
{
	int num = 0;
	while (Serial.available())
	{
		// get the new byte:
		int inChar = Serial.read();

		//break if read faile
		if (inChar < 0)
			return num;

		// add it to the inputString:
		strSerial += (char)inChar;
		num++;

		// if the incoming character is command end character, break
		if (inChar == '#') {
			return num;
		}
	}
	return num;
}

void Communication::onLoop()
{
/*
	while (BT.available())
	{
		int num = readCmdStrFromBT();

		if (isCmdStr(strCmd))
		{
			Serial.println("Receive command from bluetooth:" + strCmd);
			strResp = executeCmdStr(strCmd.c_str);

			BT.print(strResp);
			Serial.println("Send response to bluetooth:" + strResp);
		}
		else
		{
			Serial.println("received wrong command from bluetooth:" + strCmd);
			return strCmd;
		}
	}
	*/

		
	if(readStrFromSerial() > 0)
	{
		if (isCmdStr(&strSerial))
		{
			EPRINT("\nCommand from serial:");
			EPRINTLN(strSerial);

			strSerial.remove(strSerial.length() - 1, 1);

			String strResp = executeCmdStr(&strSerial);

			EPRINT("Response to serial:");
			EPRINTLN(strResp);

			strSerial.remove(0);
		}
		else
		{
			EPRINT("String from serial:");
			EPRINTLN(strSerial);
		}
	}
}


String Communication::executeCmdStr(String* strReq)
{
	Req req;
	Resp resp;

	resp.cmdRst = RST_SUCCESS;

	DPRINT("strReq:[");
	DPRINT(*strReq);
	DPRINTLN("]");

	resp.cmdRst = req.buildFromString(strReq);
	if (resp.cmdRst != RST_SUCCESS) {
		EPRINT("Failed to parse Cmd string!");
		EPRINTLN(resp.cmdRst);
	}
	else {
		//execute request
		if (req.isTimeReq() || req.isTimerReq())
			tm->executeRequest(&req, &resp);
		else if (req.isSignalReq())
			sm->executeRequest(&req, &resp);
		else {
			EPRINTLN("Unknow request cmdType:");
			EPRINTLN(req.toPrintString());
		}

	}

	return (resp.getRespString());
}

boolean Communication::isCmdStr(const String* str)
{
	return (str->endsWith("#"));
}

