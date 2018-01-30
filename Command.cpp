#include "Command.h"

Req::Req() {
	cmdId = 0;
	cmdType = CMD_UNKNOW;
	paramBits = 0;
}

int Req::splitString(String& str, char delimiter, String strAry[], int sizeAry)
{
	int idxAry = 0;
	int idxDelim;

	//check if
	//1, the first and last char are not spliter
	//2. not over the idxAry size
	//3. no double spliter together

	while (str.length()) {
		idxDelim = str.indexOf(delimiter);
		// if no delimiter, return
		if (idxDelim == -1) {
			strAry[idxAry] = str;
			return (idxAry + 1);
		}
		// if the first char is delimiter, remove and continue
		if (idxDelim == 0) {
			str.remove(0, 1);
			continue;
		}
		strAry[idxAry++] = str.substring(0, idxDelim);
		str.remove(0, idxDelim + 1);
	}

	return (idxAry+1);
}

int Req::buildFromString(String * reqStr) 
{
	int numParams, numPairs, numValues;
	String params[20], pairs[2], values[3];

	DPRINT("Parse:["); 
	DPRINT(*reqStr); 
	DPRINTLN("]");

	numParams = splitString(*reqStr, '&', params, 20);

	DPRINT("param num:"); DPRINTLN(numParams);

	for (int idx = 0; idx < numParams; idx++) {

		numPairs = splitString(params[idx], '=', pairs, 2);
		if (numPairs != 2) {
			DPRINT("Failed to parse params:["); DPRINT(params[idx]); DPRINTLN("]!");
			return RST_ERROR_PARSE_PAIRS;
		}

		DPRINT(idx); DPRINT(" - Key:["); DPRINT(pairs[0]); DPRINT("] Value:["); DPRINT(pairs[1]); DPRINTLN("]");

		//Header parameters
		//Id=123
		if (pairs[0].equalsIgnoreCase("Id")) {
			if (!setCmdId(pairs[1].toInt())) {
				return RST_ERROR_PARSE_ID;
			}
		}
		//cmd=11
		else if (pairs[0].equalsIgnoreCase("cmd")) {
			if (!setCmdType(pairs[1].toInt())) {
				return RST_ERROR_SET_CMD;
			}
		}

		//Time parameters
		//date=2019-12-23
		else if (pairs[0].equalsIgnoreCase("date")) {
			numValues = splitString(pairs[1], '-', values, 3);
			if (numValues != 3) {
				return RST_ERROR_PARSE_DATE;
			}
			if (!setDate(values[0].toInt(), values[1].toInt(),
				values[2].toInt())) {
				return RST_ERROR_SET_DATE;
			}
		}
		//time=12:23:34
		else if (pairs[0].equalsIgnoreCase("time")) {
			numValues = splitString(pairs[1], ':', values, 3);
			if (numValues != 3) {
				return RST_ERROR_PARSE_TIME;
			}
			if (!setTime(values[0].toInt(), values[1].toInt(),
				values[2].toInt())) {
				return RST_ERROR_SET_TIME;
			}
		}
		//week=3
		else if (pairs[0].equalsIgnoreCase("week")) {
			if (!setDay(pairs[1].toInt())) {
				return RST_ERROR_SET_WEEK;
			}
		}

		//Timer parameters
		//timer=1
		else if (pairs[0].equalsIgnoreCase("tid")) {
			if (!setTimerId(pairs[1].toInt())) {
				return RST_ERROR_SET_TIMERID;
			}
		}
		//relayid=1
		else if (pairs[0].equalsIgnoreCase("rid")) {
			if (!setRelayId(pairs[1].toInt())) {
				return RST_ERROR_SET_RELAYID;
			}
		}
		//sdays=5 
		else if (pairs[0].equalsIgnoreCase("sdays")) {
			if (!setStartDays(pairs[1].toInt())) {
				return RST_ERROR_SET_SDAYS;
			}
		}
		//stime=12:23:34
		else if (pairs[0].equalsIgnoreCase("stime")) {
			numValues = splitString(pairs[1], ':', values, 3);
			if (numValues != 3) {
				return RST_ERROR_PARSE_SDATE;
			}
			if (!setStartTime(values[0].toInt(), values[1].toInt(),
				values[2].toInt())) {
				return RST_ERROR_SET_SDATE;
			}
		}
		//etime=23:34:45
		else if (pairs[0].equalsIgnoreCase("etime")) {
			numValues = splitString(pairs[1], ':', values, 3);
			if (numValues != 3) {
				return RST_ERROR_PARSE_ETIME;
			}
			if (!setEndTime(values[0].toInt(), values[1].toInt(),
				values[2].toInt())) {
				return RST_ERROR_SET_ETIME;
			}
		}
		//interm=12-45
		else if (pairs[0].equalsIgnoreCase("interm")) {
			numValues = splitString(pairs[1], '-', values, 3);
			if (numValues != 2) {
				return RST_ERROR_PARSE_INTERM;
			}
			if (!setIntermSec(values[0].toInt(), values[1].toInt())) {
				return RST_ERROR_SET_INTERM;
			}
		}

		// Signal parameters
		else if (pairs[0].equalsIgnoreCase("sId")) {
			if (!setSignalId(pairs[1].toInt()))
				return RST_ERROR_SET_SIGNALID;
		}
		else if (pairs[0].equalsIgnoreCase("insts0")) {
			if (!setInstS0(pairs[1].toInt()))
				return RST_ERROR_SET_INSTS0;
		}
		else if (pairs[0].equalsIgnoreCase("insts1")) {
			if (!setInstS1(pairs[1].toInt()))
				return RST_ERROR_SET_INSTS1;
		}
		else {
			EPRINT("Unknow key:[");
			EPRINT(params[0]);
			EPRINTLN("]");
			return RST_ERROR_UNKNOWNPARAM;
		}
	}

	if (!hasCmdId() || !hasCmdType()) {
		return RST_ERROR_CMD_ID_TYPE;
	}

	return RST_SUCCESS;
}

boolean Req::isTimeReq() {
	if (cmdType >= 1 && cmdType <= 9)
		return true;
	else
		return false;
}

boolean Req::isTimerReq() {
	if (cmdType >= 11 && cmdType <= 19)
		return true;
	else
		return false;
}

boolean Req::isSignalReq() {
	if (cmdType >= 21 && cmdType <= 29)
		return true;
	else
		return false;
}

void Req::setParamBit(uint8_t n) {
	bitSet(paramBits, n);
}

uint8_t Req::getParamBit(uint8_t n) {
	return bitRead(paramBits, n);
}

// set value for header
boolean Req::setCmdId(uint8_t cmdId) {
	this->cmdId = cmdId;
	setParamBit(PARAMBIT_CMDID);
	return true;
}
boolean Req::setCmdType(uint8_t cmdType) {
	this->cmdType = cmdType;
	setParamBit(PARAMBIT_CMDTYPE);
	return true;
}

//set value for Time
boolean Req::setTime(uint8_t hour, uint8_t min, uint8_t sec) {
	if ((hour >= 24) || (min >= 60) || (sec >= 60))
		return false;

	time.hour = hour;
	time.min = min;
	time.sec = sec;
	setParamBit(PARAMBIT_TIME);
	return true;
}

boolean Req::setDate(uint16_t year, uint8_t mon, uint8_t date) {
	if (year > 3000 || mon == 0 || mon > 12 || date == 0 || date > 31)
		return false;

	time.year = year;
	time.mon = mon;
	time.date = date;
	setParamBit(PARAMBIT_DATE);
	return true;
}

boolean Req::setDay(uint8_t day) {
	if (day <= 0 || day > 7)
		return false;

	time.dow = day;
	setParamBit(PARAMBIT_DAY);
	return true;
}

//set value for Timer
boolean Req::setTimerId(uint8_t id) {
	if (!timer.setTimerId(id))
		return false;

	setParamBit(PARAMBIT_TIMERID);
	return true;
}

boolean Req::setRelayId(uint8_t id) {
	if (!timer.setRelayId(id))
		return false;

	setParamBit(PARAMBIT_RELAYID);
	return true;
}

boolean Req::setStartDays(uint8_t days) {
	if (!timer.setStartDays(days))
		return false;

	setParamBit(PARAMBIT_STARTDAYS);
	return true;
}

boolean Req::setStartTime(uint8_t hour, uint8_t min, uint8_t sec) {
	if (!timer.setStartTime(hour, min, sec))
		return false;

	setParamBit(PARAMBIT_STARTTIME);
	return true;
}

boolean Req::setEndTime(uint8_t hour, uint8_t min, uint8_t sec) {
	if (!timer.setEndTime(hour, min, sec))
		return false;

	setParamBit(PARAMBIT_ENDTIME);
	return true;
}

boolean Req::setIntermSec(uint8_t on, uint8_t off) {
	if (!timer.setIntermSec(on, off))
		return false;

	setParamBit(PARAMBIT_INTERMSEC);
	return true;
}

boolean Req::setSignalId(uint8_t id) {
	signal.signalId = id;

	setParamBit(PARAMBIT_SIGNALID);
	return true;
}

boolean Req::setInstS0(uint8_t sec) {
	signal.instants[0].InstantSec = sec;
	signal.instants[0].available = true;

	setParamBit(PARAMBIT_INSTANTS0);
	return true;
}

boolean Req::setInstS1(uint8_t sec) {
	signal.instants[1].InstantSec = sec;
	signal.instants[1].available = true;

	setParamBit(PARAMBIT_INSTANTS1);
	return true;
}

//check existence for header
boolean Req::hasCmdId() {
	return getParamBit(PARAMBIT_CMDID);
}

boolean Req::hasCmdType() {
	return getParamBit(PARAMBIT_CMDTYPE);
}

//check existence for time
boolean Req::hasTime() {
	return getParamBit(PARAMBIT_TIME);
}

boolean Req::hasDate() {
	return getParamBit(PARAMBIT_DATE);
}

boolean Req::hasDay() {
	return getParamBit(PARAMBIT_DAY);
}

//check existence for Timer
boolean Req::hasTimerId() {
	return getParamBit(PARAMBIT_TIMERID);
}

boolean Req::hasRelayId() {
	return getParamBit(PARAMBIT_RELAYID);
}

boolean Req::hasStartDays() {
	return getParamBit(PARAMBIT_STARTDAYS);
}

boolean Req::hasStartTime() {
	return getParamBit(PARAMBIT_STARTTIME);
}

boolean Req::hasEndTime() {
	return getParamBit(PARAMBIT_ENDTIME);
}

boolean Req::hasIntermSec() {
	return getParamBit(PARAMBIT_INTERMSEC);
}

boolean Req::hasSignalId() {
	return getParamBit(PARAMBIT_SIGNALID);
}

boolean Req::hasInstS0() {
	return getParamBit(PARAMBIT_INSTANTS0);
}

boolean Req::hasInstS1() {
	return getParamBit(PARAMBIT_INSTANTS1);
}


String Req::toPrintString() {
	String s = "Request: CmdId:" + String(cmdId);
	s += " CmdType:" + String(cmdType);
	s += " paramBits:" + String(paramBits);
	if (isTimeReq()) {
		s += String(" Date:") + String(time.year) + String("/")
			+ String(time.mon) + String("/") + String(time.date);
		s += String(" Time:") + String(time.hour) + String(":")
			+ String(time.min) + String(":") + String(time.sec);
		s += String(" Day:") + String(time.dow);
	}
	if (isTimerReq()) {
		s += " Timer request.";
	}
	if (isSignalReq()) {
		s += " Signal request.";
	}

	return s;
}


/*
 * Response methosd
 */
Resp::Resp() {
	cmdId = 0;
	cmdType = CMD_UNKNOW;
	cmdRst = RST_ERROR_UNKNOW;
}


boolean Resp::isTimeResp() {
	if (cmdType >= 1 && cmdType <= 9)
		return true;
	else
		return false;
}

boolean Resp::isTimerResp() {
	if (cmdType >= 11 && cmdType <= 19)
		return true;
	else
		return false;
}

boolean Resp::isSignalResp() {
	if (cmdType >= 21 && cmdType <= 29)
		return true;
	else
		return false;
}


String Resp::toPrintString() {
	String s = "Response: CmdId:" + String(cmdId);
	s += " Result:" + String(cmdRst);

	return s;
}

String Resp::getRespString() {
	String s;

	switch (cmdType) {
	case REQ_TIME_GET:
		return getTimeRespString();
	case REQ_TIMER_GET:
		return getTimerRespString();
	case REQ_SIGNAL_GET:
		return getSignalRespString();
	case REQ_TIME_SET:
	case REQ_TIME_SHOW:
	case REQ_TIMER_CREATE:
	case REQ_TIMER_UPDATE:
	case REQ_TIMER_DISABLE:
	case REQ_TIMER_SHOWALL:
	case REQ_SIGNAL_SET:
	default:
		s = String("Id=") + String(cmdId);
		s += String("&cmd=") + String(cmdType);
		s += String("&rst=") + String(cmdRst);
		s += "#";
		return s;
	}
}

String Resp::getTimeRespString() {
	String s;

	s = String("Id=") + String(cmdId);
	s += String("&cmd=") + String(cmdType);
	s += String("&rst=") + String(cmdRst);
	if (cmdRst == RST_SUCCESS) {
		s += String("&date=") + String(time.year) + "-" + String(time.mon) + "-" + String(time.date);
		s += String("&day=") + String(time.dow);
		s += String("&time=") + String(time.hour) + ":" + String(time.min) + ":" + String(time.sec);
	}
	s += "#";
	return s;
}

String Resp::getTimerRespString() {
	String s;

	s = String("Id=") + String(cmdId);
	s += String("&cmd=") + String(cmdType);
	s += String("&rst=") + String(cmdRst);
	if (cmdRst == RST_SUCCESS) {
		s += String("&") + timer.toRespString();
	}
	s += "#";
	return s;
}

String Resp::getSignalRespString() {
	String s;

	s = String("Id=") + String(cmdId);
	s += String("&cmd=") + String(cmdType);
	s += String("&rst=") + String(cmdRst);
	if (cmdRst == RST_SUCCESS) {
		s += "&" + signal.toRespString();
	}
	s += "#";
	return s;
}
