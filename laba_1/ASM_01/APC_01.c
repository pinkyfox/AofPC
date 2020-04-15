//			    tan(x + 1)
//task: f(x) = ------------, x ∈ [a, b)\0, Δx = .
//                sin(x)

#define _CRT_SECURE_NO_WARNINGS

#define START_TIMER 1
#define STOP_TIMER 0

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h>


double timer(int operation);
bool readSymbol(double* dest);
void careInput(double* intervalStart, double* intervalEnd, double* step);
void calculate(double intervalStart, double intervalEnd, double step);
bool wannaCalculateMore();


void main(void) {
	double intervalStart;  
	double intervalEnd;
	double step;

	do {
		careInput(&intervalStart, &intervalEnd, &step);
		calculate(intervalStart, intervalEnd, step);
	} while (wannaCalculateMore());

	system("pause");
	return;
}

bool wannaCalculateMore() {
	char userChoice;
	
	rewind(stdin);
	
	printf("\nDO YOU WANNA CONTINUE CALCULATING (Y/N)? ");
	while (!(userChoice = _getch()) || userChoice != 'Y' && userChoice != 'N' && userChoice != 'y' && userChoice != 'n') {
		rewind(stdin);
		printf("\n\t\tNO SUCH OPTION. TRY AGAIN: ");
	}

	if (toupper(userChoice) == 'Y') {
		system("cls");
		return true;
	}

	return false;
}

bool readSymbol(double* dest) {
	return scanf("%lf", dest);
}

void careInput(double* intervalStart, double* intervalEnd, double* step) {
	printf("\nEnter the interval [a; b) and step dx.\n");

	printf("\t\ta = ");
	while (!readSymbol(intervalStart)) {
		rewind(stdin);
		printf("\t\tInvalid input. Please, try again: a = ");
	}

	printf("\t\tb = ");
	while (!readSymbol(intervalEnd) || (*intervalEnd) <= (*intervalStart)) {
		rewind(stdin);
		printf("\t\tInvalid input. Please, try again: b = ");
	}

	printf("\t\tdx = ");
	while (!readSymbol(step) || ((*intervalEnd) - (*intervalStart)) <= (*step)) {
		rewind(stdin);
		printf("\t\tInvalid input. Please, try again: dx = ");
	}

	return;
}

void calculate(double intervalStart, double intervalEnd, double step) {

	double result = 0.0;
	double time;
	double currentX;

	timer(START_TIMER);
	for (currentX = intervalStart; currentX < intervalEnd; currentX += step) {
		if (currentX == 0.0) {
			continue;
		}
		result += tan(currentX + 1) / sin(currentX);
	}
	time = timer(STOP_TIMER);
	printf("\n------------------------------------C-------------------------------------");
	printf("\n	tan(x + 1)\nf(x) = ------------ = %fl\n          sin(x)\n", result);
	printf("Time - %lf seconds\n--------------------------------------------------------------------------\n", time);

	result = 0.0;

	currentX = intervalStart;
	timer(START_TIMER);
	_asm finit;
	for (currentX = intervalStart; currentX < intervalEnd; currentX += step) {
		if (currentX == 0.0) {
			continue;
		}
		_asm {  
				FLD1;
				FADD currentX;
				FPTAN;
				FSTP ST
				FLD currentX;
				FSIN;
				FDIV;
				FADD result;
				FSTP result;
		}
		_asm fwait;
	}
	time = timer(STOP_TIMER);
	printf("-----------------------------------ASM------------------------------------");
	printf("\n	tan(x + 1)\nf(x) = ------------ = %fl\n          sin(x)\n", result);
	printf("Time - %lf seconds\n--------------------------------------------------------------------------\n", time);

	return;
}

double timer(int operation) {
	static LARGE_INTEGER startTime, stopTime, timeFrequency;
	switch (operation) {
		case START_TIMER: {
			QueryPerformanceCounter(&startTime);
			return;
		} case STOP_TIMER: {
			QueryPerformanceCounter(&stopTime);
			QueryPerformanceFrequency(&timeFrequency);
			return (stopTime.QuadPart - startTime.QuadPart) / timeFrequency.QuadPart;
		} default: {
			return;
		}
	}
}