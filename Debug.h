#ifndef _DEBUG_
#define _DEBUG_
#include <arduino.h>

//#define PRINT_DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef PRINT_DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define PRINT_INFO   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef PRINT_INFO    //Macros are usually in all capital letters.
#define IPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
#define IPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
#define IPRINT(...)     //now defines a blank line
#define IPRINTLN(...)   //now defines a blank line
#endif

#define PRINT_ERROR   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef PRINT_ERROR    //Macros are usually in all capital letters.
#define EPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
#define EPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
#define EPRINT(...)     //now defines a blank line
#define EPRINTLN(...)   //now defines a blank line
#endif

#endif
