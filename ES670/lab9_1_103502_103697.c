#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF

#define VENTILADOR_dir TRISCbits.RC1
#define VENTILADOR PORTCbits.RC1

#define TACOMETRO_L TMR1L
#define TACOMETRO_H TMR1H
#define TIMER_CON T1CON

#define TIMER0_CON T0CON

#define DISPLAY_ENABLE PORTEbits.RE1
#define DISPLAY_ENABLE_dir TRISEbits.RE1
#define RS PORTEbits.RE0
#define RS_dir TRISEbits.RE0
#define DISPLAY7 LATD
#define DISPLAY7_DIR TRISD

#define LED_dir TRISBbits.RB0
#define LED PORTBbits.RB0

unsigned long int cont = 0;


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

void setDisplay(int read1, int read2){
	char values[10], RS_list[17], DB_list[17];
	char i;


	values[0]=(read1%10)+'0';
	values[1]=((read1/10)%10)+'0';
	values[2]=((read1/100)%10)+'0';
	values[3]=((read1/1000)%10)+'0';
	values[4]=((read1/10000)%10)+'0';

	values[9]=(read2%10)+'0';
	values[8]=((read2/10)%10)+'0';
	values[7]=((read2/100)%10)+'0';
	values[6]=((read2/1000)%10)+'0';
	values[5]=',';
	

	RS_list[0] = 0;
	DB_list[0] = 1;

	RS_list[1] = 0;
	DB_list[1] = 2;
 
	RS_list[2] = 1;
	DB_list[2] = values[4];

	RS_list[3] = 1;
	DB_list[3] = values[3];

	RS_list[4] = 1;
	DB_list[4] = values[2];

	RS_list[5] = 1;
	DB_list[5] = values[1];

	RS_list[6] = 1;
	DB_list[6] = values[0];

	RS_list[7] = 1;
	DB_list[7] = values[5];

	RS_list[8] = 1;
	DB_list[8] = values[6];

	RS_list[9] = 1;
	DB_list[9] = values[7];

	RS_list[10] = 1;
	DB_list[10] = values[8];

	RS_list[11] = 1;
	DB_list[11] = values[9];


	for(i = 0;i < 12;i++){
		DISPLAY_ENABLE = 1;
		RS = RS_list[i];
		DISPLAY7 = DB_list[i];
		delay_1ms();
		delay_1ms();
		DISPLAY_ENABLE = 0;
		delay_1ms();
		delay_1ms();
	}

}


void configLCD(void){
	char RS_list[6], DB_list[6], i=0;

	DISPLAY7_DIR = 0;
	DISPLAY_ENABLE_dir = 0;
	RS_dir = 0;

	RS_list[0] = 0;
	DB_list[0] = 0b00111000;

	RS_list[1] = 0;
	DB_list[1] = 1;

	RS_list[2] = 0;
	DB_list[2] = 2;

	RS_list[3] = 0;
	DB_list[3] = 6;

	RS_list[4] = 0;
	DB_list[4] = 15;

	RS_list[5] = 0;
	DB_list[5] = 0b10000000;

	for(i = 0;i < 6;i++){
		DISPLAY_ENABLE = 1;
		RS = RS_list[i];
		DISPLAY7 = DB_list[i];
		delay_10ms();
		DISPLAY_ENABLE = 0;
		delay_10ms();
	}

}

void timer_interrupt(void);

#pragma code low_vector = 0x08
void low_interrupt(void){
	_asm GOTO timer_interrupt _endasm
}

#pragma code
#pragma interruptlow timer_interrupt

void timer_interrupt(void){
	float vel = 0;
	int vel_int = 0, vel_part = 0;
	cont = (unsigned long int)TACOMETRO_L;
	cont |= ((unsigned long int) TACOMETRO_H)<<8;

	vel = ((float) cont)/(1.4);
	vel_int = vel;
	vel_part = 10000.*(vel-vel_int);

	setDisplay(vel_int, vel_part);

	INTCONbits.TMR0IF = 0;

	TMR0H = 0x3C;
	TMR0L = 0xB0;

	TMR1H = 0;
	TMR1L = 0;
}


void main(void){


	ADCON1 = 0x0f;

	LED_dir = 0;
	LED = 0;

	TMR0H = 0x3C;
	TMR0L = 0xB0;

	TIMER_CON = 0b10000011;


	TIMER0_CON = 0b10000001;



	INTCONbits.GIE = 1;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;

	VENTILADOR_dir = 0;
	VENTILADOR = 1;

	configLCD();

	
	while(1){
	}

}