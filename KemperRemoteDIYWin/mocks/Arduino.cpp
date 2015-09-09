

#include "Arduino.h"

#include <sys\timeb.h> 

unsigned long millis() {
	static struct timeb start;
	if (start.time == 0)
		ftime(&start);
	struct timeb end;
	ftime(&end);
	return (int)((int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm)));
}

void HardwareSerial::write(byte a) {
	fwrite(&a,sizeof(byte),1,stdout);
}

void HardwareSerial::print(const char* s) {
	fputs(s,stdout);
}

void HardwareSerial::print(int s) {
	printf("%d", s);
}

void HardwareSerial::println(const char* s) {
	print(s);
	printf("\n");
}

void HardwareSerial::println(int s) {
	print(s);
	printf("\n");
}

void HardwareSerial::flush() {
	fflush(stdout);
}

HardwareSerial Serial;

int analogRead(int pin) {
	return 0;
}
