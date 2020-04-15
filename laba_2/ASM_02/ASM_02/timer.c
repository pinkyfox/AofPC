#include  "timer.h"

double timer(int operation) {
	static LARGE_INTEGER startTime, stopTime, timeFrequency;
	switch (operation) {
	case START_TIMER: {
		QueryPerformanceCounter(&startTime);
		return;
	} case STOP_TIMER: {
		QueryPerformanceCounter(&stopTime);
		QueryPerformanceFrequency(&timeFrequency);
		return (stopTime.QuadPart - startTime.QuadPart) * TO_MACRO_SEC / (timeFrequency.QuadPart);
	} default: {
		return;
	}
	}
}