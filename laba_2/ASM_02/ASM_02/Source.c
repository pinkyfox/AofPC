//C = A + kB

#define COLUMNS 4
#define ROWS 4
#define TIMES 1000000

#include <stdint.h>

#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int16_t* matrix);
void fillMatrix(int16_t* matrix);

int main(void) {
	double timer_x = 0;
	int16_t* matrixA = malloc(ROWS * COLUMNS * sizeof *matrixA);
	int16_t* matrixB = malloc(ROWS * COLUMNS * sizeof *matrixB);
	int16_t* resultC = malloc(ROWS * COLUMNS * sizeof *resultC);
	int16_t* resultASM = malloc(ROWS * COLUMNS * sizeof *resultASM);
	int16_t* resultMMX = malloc(ROWS * COLUMNS * sizeof *resultMMX);
	int16_t multiplier = -2;
	//-----------------input variables-----------------
	fillMatrix(matrixA);
	printf("Matrix A[%d, %d]:\n", ROWS, COLUMNS);
	printMatrix(matrixA);
	fillMatrix(matrixB);
	printf("Matrix B[%d, %d]:\n", ROWS, COLUMNS);
	printMatrix(matrixB);
	//---------------implementation in C---------------
	timer(START_TIMER);
	printf("---------------implementation in C---------------\n");
	for (size_t counter = 0; counter < TIMES; counter++) {
		for (size_t i = 0; i < ROWS; i++) {
			for (size_t j = 0; j < COLUMNS; j++) {
				resultC[i * ROWS + j] = matrixA[i * ROWS + j] + multiplier * matrixB[i * ROWS + j];
			}
		}
	}
	timer_x = timer(STOP_TIMER);
	printf("Matrix C[%d, %d]:\n", ROWS, COLUMNS);
	printMatrix(resultC);
	printf("C implementation time = %lf macroseconds\n", timer_x);
	//--------------implementation in ASM--------------
	timer(START_TIMER);
	printf("--------------implementation in ASM--------------\n");
	for (int counter = 0; counter < TIMES; counter++) {
		_asm {
			PUSHA
			MOV ECX, COLUMNS * ROWS
			XOR ESI, ESI
			ADD_LOOP :
				MOV EDI, matrixA
				MOV BX, WORD PTR [ESI + EDI]

				MOV AX, multiplier
			
				MOV EDI, matrixB
				MOV DX, WORD PTR [ESI + EDI]

				IMUL DX
				ADD AX, BX

				MOV EDI, resultASM
				MOV WORD PTR [ESI + EDI], AX
			
				
				ADD ESI, 2
			LOOP ADD_LOOP
			POPA
		}
	}
	timer_x = timer(STOP_TIMER);
	printf("Matrix C[%d, %d]:\n", ROWS, COLUMNS);
	printMatrix(resultASM);
	printf("ASM implementation time = %lf macroseconds\n", timer_x);
	//--------------implementation in MMX--------------
	timer(START_TIMER);
	printf("--------------implementation in MMX--------------\n");
	for (int counter = 0; counter < TIMES; counter++) {
		int16_t sizeOfMultiplier = 8 * sizeof multiplier;
		_asm {
			PUSHA

			XOR ESI, ESI
			XOR EAX, EAX
			MOV AX, multiplier

			PXOR MM2, MM2
			MOVD MM3, EAX

			MOV ECX, COLUMNS
			MULTIPLIERLOOP:
				PSLLQ MM2, sizeOfMultiplier
				PADDQ MM2, MM3
			LOOP MULTIPLIERLOOP

			MOV ECX, ROWS
			LOOPROWS:
				MOV EDI, matrixA
				MOVQ MM0, [ESI + EDI]
				MOV EDI, matrixB
				MOVQ MM1, [ESI + EDI]

				PMULLW MM1, MM2
				PADDSW MM1, MM0

				MOV EDI, resultMMX
				MOVQ [ESI + EDI], MM1

				ADD ESI, 8
			LOOP LOOPROWS
			EMMS
			POPA
		}
	}
	timer_x = timer(STOP_TIMER);
	printf("Matrix C[%d, %d]:\n", ROWS, COLUMNS);
	printMatrix(resultMMX);
	printf("MMX implementation time = %lf macroseconds\n", timer_x);
	system("pause");
	return 1;
}

void printMatrix(int16_t* matrix) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			printf("%d ", matrix[i * ROWS + j]);
		}
		printf("\n");
	}
}

void fillMatrix(int16_t* matrix) {
	
	srand(timer(STOP_TIMER));
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			matrix[i * ROWS + j] = rand() % (UINT8_MAX - INT8_MAX);
		}
	}
}