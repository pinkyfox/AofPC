#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define IRQ0	0x08 
#define IRQ1	0x09
#define IRQ2	0x0A
#define IRQ3	0x0B
#define IRQ4	0x0C
#define IRQ5	0x0D
#define IRQ6	0x0E
#define IRQ7	0x0F

#define IRQ8	0x70
#define IRQ9	0x71
#define IRQ10	0x72	
#define IRQ11	0x73
#define IRQ12	0x74
#define IRQ13	0x75
#define IRQ14	0x76
#define IRQ15	0x77

struct VIDEO
{
	unsigned char symbol;
	unsigned char attribute;
};
#define NUMBER_OF_COLORS 3
char colors[NUMBER_OF_COLORS] = {0x89, 0x75, 0x12};

char color()
{
	int color_num = rand() % NUMBER_OF_COLORS;	
	return (char)color[color_num];
}	


char color = 0x89;	

void print()
{
	char temp;  
	int i, val;
	
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);   // получение дальнего указателя на видеобуфер сегмент 0xB800, смещение 0

	color = color();

	val = inp(0x21);                                     // получение регистра маски ведущего контроллера
	for (i = 0; i < 8; i++)                              
	{
		temp = val % 2;                                  // остаток от деления на 2
		val = val >> 1;                                  // сдвиг на один бит вправо
		screen->symbol = temp + '0';                     // переводим цифру в символ 
		screen->attribute = color;                       // устанавливаем ее цвет
		screen++;                                        // сдвигаемся на 2 байта вперед
	}
	
	screen++;                                           
	
	val = inp(0xA1);                                     // получение регистра маски ведомого контроллера
	for (i = 0; i < 8; i++)								 
	{                                                    
		temp = val % 2;									 // остаток от деления на 2
		val = val >> 1;									 // сдвиг на один бит вправо
		screen->symbol = temp + '0';					 // переводим цифру в сивол
		screen->attribute = color;						 // устанавливаем ее цвет
		screen++;										 // сдвигаемся на 2 байта вперед 
	}
	
	screen += 63;									     
	outp(0x20, 0x0A);                                    // заносим в порт 20h значение 0Ah, тем самым разрешаем доступ к регистру запросов ведущего 
                                                         // контроллера
	val = inp(0x20);                                     // получаем регистр запросов ведущего контроллера
	for (i = 0; i < 8; i++)								 
	{                                                    
		temp = val % 2;									 // остаток от деления на 2
		val = val >> 1;									 // сдвиг на один бит вправо
		screen->symbol = temp + '0';					 // переводим цифру в сивол
		screen->attribute = color;						 // устанавливаем ее цвет
		screen++;										 // сдвигаемся на 2 байта вперед
	}                                                    
	
	screen++;                                            
	
	outp(0xA0, 0x0A);                                    // заносим в порт А0h значение 0Ah, тем самым разрешаем доступ к регистру запросов ведомого
	                                                     // контроллера
	val = inp(0xA0);                                     // получаем регистр запросов ведомого контроллера
	for (i = 0; i < 8; i++)								 
	{                                                    
		temp = val % 2;									 // остаток от деления на 2
		val = val >> 1;									 // сдвиг на один бит вправо
		screen->symbol = temp + '0';					 // переводим цифру в сивол
		screen->attribute = color;						 // устанавливаем ее цвет
		screen++;										 // сдвигаемся на 2 байта вперед
	}
	
	screen += 63;                                         

	outp(0x20, 0x0B);                                     // заносим в порт 20h значение 0Вh, тем самым разрешаем доступ к регистру обслуживания 
	                                                      // ведущего контроллера
	val = inp(0x20);                                      // получаем регистр обслуживания ведущего контроллера
	for (i = 0; i < 8; i++)                              
	{                                                    
		temp = val % 2;									  // остаток от деления на 2
		val = val >> 1;									  // сдвиг на один бит вправо
		screen->symbol = temp + '0';					  // переводим цифру в сивол
		screen->attribute = color;						  // устанавливаем ее цвет
		screen++;										  // сдвигаемся на 2 байта вперед
	}                                                    
	
	screen++;                                            

	outp(0xA0, 0x0B);                                     // заносим в порт А0h значение 0Вh, тем самым разрешаем доступ к регистру обслуживания
	                                                      // ведомого контроллера
	val = inp(0xA0);                                      // получаем регистр обслуживания ведомого контроллера
	for (i = 0; i < 8; i++)                              
	{                                                     
		temp = val % 2;								      // остаток от деления на 2
		val = val >> 1;									  // сдвиг на один бит вправо
		screen->symbol = temp + '0';					  // переводим цифру в сивол
		screen->attribute = color;						  // устанавливаем ее цвет
		screen++;										  // сдвигаемся на 2 байта вперед
	}
}

// IRQ 0-7
void interrupt(*old_vect8) (...);
void interrupt(*old_vect9) (...);
void interrupt(*old_vect10) (...);
void interrupt(*old_vect11) (...);
void interrupt(*old_vect12) (...);
void interrupt(*old_vect13) (...);
void interrupt(*old_vect14) (...);
void interrupt(*old_vect15) (...);

// IRQ 8-15
void interrupt(*old_vect70) (...);
void interrupt(*old_vect71) (...);
void interrupt(*old_vect72) (...);
void interrupt(*old_vect73) (...);
void interrupt(*old_vect74) (...);
void interrupt(*old_vect75) (...);
void interrupt(*old_vect76) (...);
void interrupt(*old_vect77) (...);


void interrupt  new_vect08(...) { print(); old_vect8(); }                    
void interrupt  new_vect09(...) { print(); old_vect9(); }	 
void interrupt  new_vect0A(...) { print(); old_vect10(); }	 
void interrupt  new_vect0B(...) { print(); old_vect11(); }	 
void interrupt  new_vect0C(...) { print(); old_vect12(); }	 
void interrupt  new_vect0D(...) { print(); old_vect13(); }	 
void interrupt  new_vect0E(...) { print(); old_vect14(); }	 
void interrupt  new_vect0F(...) { print(); old_vect15(); }	 

void interrupt  new_vectB8(...) { print(); old_vect70(); }	 
void interrupt  new_vectB9(...) { print(); old_vect71(); }	 
void interrupt  new_vectBA(...) { print(); old_vect72(); }	 
void interrupt  new_vectBB(...) { print(); old_vect73(); }	 
void interrupt  new_vectBC(...) { print(); old_vect74(); }	 
void interrupt  new_vectBD(...) { print(); old_vect75(); }	 
void interrupt  new_vectBE(...) { print(); old_vect76(); }	 
void interrupt  new_vectBF(...) { print(); old_vect77(); }	 

void initialize()
{
	//IRQ 0-7
	old_vect8 =  _dos_getvect(IRQ0); 
	old_vect9 =  _dos_getvect(IRQ1); 
	old_vect10 = _dos_getvect(IRQ2); 
	old_vect11 = _dos_getvect(IRQ3); 
	old_vect12 = _dos_getvect(IRQ4); 
	old_vect13 = _dos_getvect(IRQ5); 
	old_vect14 = _dos_getvect(IRQ6); 
	old_vect15 = _dos_getvect(IRQ7); 

    //IRQ 8-15
	old_vect70 = _dos_getvect(IRQ8); 
	old_vect71 = _dos_getvect(IRQ9); 
	old_vect72 = _dos_getvect(IRQ10);
	old_vect73 = _dos_getvect(IRQ11);
	old_vect74 = _dos_getvect(IRQ12);
	old_vect75 = _dos_getvect(IRQ13);
	old_vect76 = _dos_getvect(IRQ14);
	old_vect77 = _dos_getvect(IRQ15);

	//set new handlers for IRQ 0-7
	_dos_setvect(IRQ0, new_vect08);  
	_dos_setvect(IRQ1, new_vect09);  
	_dos_setvect(IRQ2, new_vect0A);  
	_dos_setvect(IRQ3, new_vect0B);  
	_dos_setvect(IRQ4, new_vect0C);  
	_dos_setvect(IRQ5, new_vect0D);  
	_dos_setvect(IRQ6, new_vect0E);  
	_dos_setvect(IRQ7, new_vect0F);  

	//set new handlers for IRQ8-15
	_dos_setvect(0xB8, new_vectB8);  
	_dos_setvect(0xB9, new_vectB9);
	_dos_setvect(0xBA, new_vectBA); 
	_dos_setvect(0xBB, new_vectBB); 
	_dos_setvect(0xBC, new_vectBC); 
	_dos_setvect(0xBD, new_vectBD); 
	_dos_setvect(0xBE, new_vectBE); 
	_dos_setvect(0xBF, new_vectBF); 

	_disable(); // CLI
	// Master init
	outp(0x20, 0x11);	//ICW1 0001 0001b -- срабатывание триггера по уровню,
						// размер вектора прерываний -- 8 байт, не использовать 
						// ведомый контроллер, вызвать команду инициализации 4
	outp(0x21, 0x08);   //ICW2 - установка адреса вектора прерывания
	outp(0x21, 0x04);	//ICW3 - к irq2 подключен ведомый контроллер
	outp(0x21, 0x01);	//ICW4 - поддержка 8086

	// Slave init
	outp(0xA0, 0x11);  	//ICW1 - как и у ведущего
	outp(0xA1, 0xB8);	//ICW2 - начало базового вектора
	outp(0xA1, 0x02);   //ICW3 - из 1-го выхода подключение к ведущему
	outp(0xA1, 0x01);   //ICW4 - поддержка 8086

	_enable(); // STI
}

int main()
{
	unsigned far *fp;                           
	initialize();                               
	system("cls");                              

	printf("                   - mask\n");       // the mask of Master and Slave
	printf("                   - prepare\n");    // the request of Master and Slave
	printf("                   - service\n");    // the service of Master and Slave
	printf("Master   Slave\n");

	FP_SEG(fp) = _psp;                         
	FP_OFF(fp) = 0x2c;                         
	_dos_freemem(*fp);                         

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); 
	return 0;
}