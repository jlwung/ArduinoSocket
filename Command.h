#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <arduino.h>
#include "DS1307.h"
#include "Timer.h"
#include "Signal.h"
#include "Debug.h"

#define CMD_UNKNOW		0

//time cmd 1-9
#define REQ_TIME_GET	1
#define REQ_TIME_SET	2
#define REQ_TIME_SHOW	3

//timer cmd 11-19
#define REQ_TIMER_CREATE	11
#define REQ_TIMER_GET		12
#define REQ_TIMER_UPDATE	13
#define REQ_TIMER_DISABLE	14
#define REQ_TIMER_RESETALL	15
#define REQ_TIMER_SHOWALL	19

//signal cmd 21-29
#define REQ_SIGNAL_GET		21
#define REQ_SIGNAL_SET		22
#define REQ_SIGNAL_RESET	23
#define REQ_SIGNAL_SHOWALL	29


#define PARAMBIT_CMDID		0
#define PARAMBIT_CMDTYPE	1

#define PARAMBIT_DATE		2
#define PARAMBIT_TIME		3
#define PARAMBIT_DAY		4

#define PARAMBIT_TIMERID	5
#define PARAMBIT_RELAYID	6
#define PARAMBIT_STARTDAYS	7
#define PARAMBIT_STARTTIME	8
#define PARAMBIT_ENDTIME	9
#define PARAMBIT_INTERMSEC	10

#define PARAMBIT_SIGNALID	11
#define PARAMBIT_INSTANTS0	12
#define PARAMBIT_INSTANTS1	13


#define RST_SUCCESS				0

//Error when parse request string and build request
#define RST_ERROR_NO_HEADER   1 
#define RST_ERROR_DATA       2
#define RST_ERROR_EXECUTE    3
#define RST_ERROR_UNKNOWNPARAM    4
#define RST_ERROR_CMD_ID_TYPE    5
#define RST_ERROR_PARSE_ID  10
#define RST_ERROR_PARSE_PARAMS	11
#define RST_ERROR_PARSE_PAIRS	12
#define RST_ERROR_PARSE_DATE  13
#define RST_ERROR_PARSE_SDATE  14
#define RST_ERROR_PARSE_EDATE  15
#define RST_ERROR_PARSE_TIME  16
#define RST_ERROR_PARSE_STIME  17
#define RST_ERROR_PARSE_ETIME  18
#define RST_ERROR_PARSE_INTERM  19
#define RST_ERROR_SET_ID	21
#define RST_ERROR_SET_CMD	22
#define RST_ERROR_SET_DATE  23
#define RST_ERROR_SET_SDATE  24
#define RST_ERROR_SET_EDATE  25
#define RST_ERROR_SET_TIME  26
#define RST_ERROR_SET_STIME  27
#define RST_ERROR_SET_ETIME  28
#define RST_ERROR_SET_WEEK  29
#define RST_ERROR_SET_TIMERID  30
#define RST_ERROR_SET_RELAYID  31
#define RST_ERROR_SET_SDAYS  33
#define RST_ERROR_SET_INTERM  34
#define RST_ERROR_SET_SIGNALID  35
#define RST_ERROR_SET_INSTS0  36
#define RST_ERROR_SET_INSTS1  37

//Error when execute request

#define RST_ERROR_INVALID_TIME    51

#define RST_ERROR_UNKNOWCMD_TIMER 60
#define RST_ERROR_INVALID_TIMER   61
#define RST_ERROR_INVALID_TIMERPARAM  62
#define RST_ERROR_INVALID_TIMERID  63
#define RST_ERROR_TIMER_ALREADYEXIST    64

#define RST_ERROR_UNKNOWCMD_SIGNAL 70
#define RST_ERROR_INVALID_SIGNALPARAM  71
#define RST_ERROR_INVALID_SIGNALID  72


#define RST_ERROR_UNKNOW    98
#define RST_ERROR_FATAL		99

class Req
{
	public:
	uint8_t	cmdId;
	uint8_t cmdType;
	uint16_t paramBits;
	Timer	timer;
	Time	time;
	Signal	signal;
	
	public:
	Req();

	int buildFromString(String * reqStr);

	String toPrintString();
	
	boolean isTimeReq();
	boolean isTimerReq();
	boolean isSignalReq();
	
	void setParamBit(uint8_t n);
	uint8_t getParamBit(uint8_t n);
	
	// set value for header
	boolean setCmdId(uint8_t cmdId);
	boolean setCmdType(uint8_t cmdType);
	
	//set value for Time
	boolean setTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setDate(uint16_t year, uint8_t mon, uint8_t date);
	boolean setDay(uint8_t days);
	
	//set value for Timer
	boolean setTimerId(uint8_t id);
	boolean setRelayId(uint8_t  id);
	boolean setStartDays(uint8_t days);
	boolean setStartTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setEndTime(uint8_t hour, uint8_t min, uint8_t sec);
	boolean setIntermSec(uint8_t on, uint8_t off);
	boolean setSignalId(uint8_t id);
	boolean setInstS0(uint8_t sec);
	boolean setInstS1(uint8_t sec);

	
	//check existence for header
	boolean hasCmdId();
	boolean hasCmdType();
	
	//check existence for time
	boolean hasTime();
	boolean hasDate();
	boolean hasDay();

	//check existence for Timer
	boolean hasTimerId();
	boolean hasRelayId();
	boolean hasStartDays();
	boolean hasStartTime();
	boolean hasEndTime();
	boolean hasIntermSec();
	boolean hasSignalId();
	boolean hasInstS0();
	boolean hasInstS1();

private:
	int splitString(String& str, char delimiter, String strAry[], int sizeAry);
};

class Resp
{
	public:
	uint8_t cmdId;
	uint8_t cmdType;
	uint8_t cmdRst;
	Timer	timer;
	Time	time;
	Signal	signal;
	
	public:
	Resp();
	boolean isTimeResp();
	boolean isTimerResp();
	boolean isSignalResp();
	String toPrintString();
	String getRespString();
	String getTimeRespString();
	String getTimerRespString();
	String getSignalRespString();

};

#endif
