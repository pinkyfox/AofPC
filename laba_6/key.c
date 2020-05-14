#include <stdio.h>
#include <dos.h>


unsigned char message[80];
int count = 0;

//  rg.h.cl -- ascii | write
//  rg.h.ch -- scan code | write
//  rg.h.al -- ascii | read
//  rg.h.ah -- int | white
// rg.h.ah = 5;
// int86(0x16, &rg, &rg);

int qwerty[] = {11,11,19,20,21};
char rty[] = {'w', 'e', 'r','t','y'};
int hello[] = {12,38,38,24};
char lo[] = {'e','l','l','o'};
int student[] = {14,16,32,18,49,20};
char dent[] = {'t','u','d','e','n','t'};
int Anthony[] = {31,14,35,24,49,21};
char hony[] = {'n','t','h','o','n','y'};
int BSUIR[] = {16,23,19};
char ir[] = {'S','U','I','R'};

union REGS  rg;


void interrupt(*old09hHandler) (...);

void interrupt new09hHandler(...)
{
	// rg.h.ah = 0;
	// int86(0x16, &rg, &rg);
	// rg.h.ah = 5;
	// rg.h.cl = rg.h.al;
	// int86(0x16, &rg, &rg);
	old09hHandler();
	unsigned char value = 0;
	value = inp(0x60);
	message[count] = value;
	if (value == 0x10)
	{
		rg.h.ah = 5;
		for (size_t i = 0; i < 5; ++i)
		{
			rg.h.cl = rty[i];
			rg.h.ch = qwerty[i];

			int86(0x16, &rg, &rg);
		}
	}
	if (value == 0x23)
	{
		rg.h.ah = 5;
		for (size_t i = 0; i < 4; ++i)
		{
			rg.h.cl = lo[i];
			rg.h.ch = hello[i];

			int86(0x16, &rg, &rg);
		}
	}
	if (value == 0x1F)
	{
		rg.h.ah = 5;
		for (size_t i = 0; i < 6; ++i)
		{
			rg.h.cl = dent[i];
			rg.h.ch = student[i];

			int86(0x16, &rg, &rg);
		}
	}
	if (value == 0x1E)
	{
		rg.h.ah = 5;
		for (size_t i = 0; i < 6; ++i)
		{
			rg.h.cl = hony[i];
			rg.h.ch = Anthony[i];

			int86(0x16, &rg, &rg);
		}
	}
	if (value == 0x30)
	{
		rg.h.ah = 5;
		for (size_t i = 0; i < 4; ++i)
		{
			rg.h.cl = ir[i];
			rg.h.ch = BSUIR[i];

			int86(0x16, &rg, &rg);
		}
	}
	if (value == 0x01)
	{
		count = 0;
	} else {
		count++;
	}
}


void init()
{
	disable();
	old09hHandler = getvect(0x09);
	setvect(0x09, new09hHandler);
	enable();
}


int main()
{
	unsigned far *fp;

	init();

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 10);

	return 0;
}