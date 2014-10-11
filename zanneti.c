#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF

#define MOTOR1_en PORTCbits.RC1
#define MOTOR1_dir TRISCbits.RC1
#define MOTOR1s1_dir TRISBbits.RB5
#define MOTOR1s1 PORTBbits.RB5

#define MOTOR1s2_dir TRISBbits.RB4
#define MOTOR1s2 PORTBbits.RB4

#define LED1 PORTDbits.RD0
#define LED1_dir TRISDbits.RD0
#define LED2 PORTDbits.RD1
#define LED2_dir TRISDbits.RD1

#define LED3 PORTCbits.RC6
#define LED3_dir TRISCbits.RC6

#define LEITURA PORTAbits.RA0
#define LEITURA_dir TRISAbits.RA0

void delay_1ms(){
	char i;
	for(i=0;i<69;i++);
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void delay_10ms(){
	char i;
	for(i=0; i<9; i++) delay_1ms();
	for(i=0; i<59; i++);
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void delay_100ms(){
	char i;
	for(i=0; i<9; i++) delay_10ms();
	for(i=0; i<9; i++) delay_1ms();
	for(i=0; i<49; i++);
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}


void delay_1s(){
	char i;
	for(i=0; i<9; i++) delay_100ms();
	for(i=0; i<9; i++) delay_10ms();
	for(i=0; i<9; i++) delay_1ms();
	for(i=0; i<39; i++);
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void main(void){

	char i;	

	ADCON1 = 0x0f;
	

	LED1_dir = 0;
	LED2_dir = 0;
	LED3_dir = 0;

	MOTOR1_dir = 0;
	MOTOR1s1_dir = 0;
	MOTOR1s2_dir = 0;

	LEITURA_dir = 1;

	MOTOR1s1 = 1;
	MOTOR1s2 = 0;
	MOTOR1_en = 1;

	for(i = 0; i < 5; i++)
		delay_1s();

	MOTOR1_en = 0;
	MOTOR1s1 = 0;
	MOTOR1s2 = 1;
	
	for(i = 0; i < 2; i++)
		delay_1s();

	MOTOR1_en = 1;

	for(i = 0; i < 5; i++)
		delay_1s();

	MOTOR1_en = 0;

	while(1){
		LED2 = 1;
		LED3 = 1;
		delay_1s();
		LED2 = 0;
		LED3 = 0;
		delay_1s();

		if(LEITURA == 0)
			LED1 = 1;
		else
			LED1 = 0;

	}	

}