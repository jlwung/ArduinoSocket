

#include "Timer.h"

Timer t;
char str[300];

void setup()
{

	Serial.begin(115200);
	while (!Serial);
	Serial.println("Serial port is ready.");

}

void loop()
{

	t.toString(str, 200);
	Serial.println(str);

	delay(1000);

}
