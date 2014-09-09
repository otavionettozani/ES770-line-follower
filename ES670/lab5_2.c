#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF
#pragma config CCP2MX = ON


#define DISPLAY_ENABLE PORTEbits.RE1
#define DISPLAY_ENABLE_dir TRISEbits.RE1
#define RS PORTEbits.RE0
#define RS_dir TRISEbits.RE0
#define DISPLAY7 LATD
#define DISPLAY7_DIR TRISD


#define VENTILADOR_dir TRISCbits.RC1

#define PWMP PR2
#define TIMER T2CON
#define PWM_conf CCP2CON
#define DUTYCYCLE CCPR2L

#define TACOMETRO TMR1L
#define TIMER_T T1CON

#define LED PORTBbits.RB0
#define LED_DIR TRISBbits.RB0

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

void configLCD(void){
	char RS_list[6], DB_list[6], i=0;

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

void setDisplay(int valueStart, int valueStop){
	char values[4], RS_list[15], RS_list2[15], DB_list[15], DB_list2[15];
	char i;


	values[0]=(valueStart%10)+'0';
	values[1]=((valueStart/10)%10)+'0';
	values[2]=((valueStart/100)%10)+'0';
	

	RS_list[0] = 0;
	DB_list[0] = 1;

	RS_list[1] = 0;
	DB_list[1] = 2;

	values[0]=(valueStop%10)+'0';
	values[1]=((valueStop/10)%10)+'0';
	values[2]=((valueStop/100)%10)+'0';

	RS_list[2] = 0;
	DB_list[2] = 0b10101000;

	RS_list[3] = 1;
	DB_list[3] = values[2];

	RS_list[4] = 1;
	DB_list[4] = values[1];

	RS_list[5] = 1;
	DB_list[5] = values[0];

	for(i = 0;i < 6;i++){
		DISPLAY_ENABLE = 1;
		RS = RS_list[i];
		DISPLAY7 = DB_list[i];
		delay_10ms();
		DISPLAY_ENABLE = 0;
		delay_10ms();
	}

}



void main(void){
	char ultimo_tacometro=0, count=0, pwmLiga=0, pwmDesliga=0, j=0;
	int i=0,k;
	ADCON1 = 0x0f;
	
	PWMP = 79; //contador do timer conta ate 79
	DUTYCYCLE = 0;
	VENTILADOR_dir = 0;
	TIMER = 0b00000110; //seta o timer on e pre-scale 16
	PWM_conf = 0b00001100; //seta a configuracao do PWN como PWM e os 2bits menos sig. como 0

	TIMER_T = 0b00000011; //

	DISPLAY7_DIR = 0;
	DISPLAY_ENABLE_dir = 0;
	RS_dir = 0;

	configLCD();

	setDisplay(pwmLiga,pwmDesliga);

	TACOMETRO = 0;
	while(1){
	
		j = TACOMETRO;
		while(1){
			i = i==80 ? 80 : i+1;
			DUTYCYCLE = i;
			delay_10ms();
			if(TACOMETRO > j+4)
				break;
		}
		LED_DIR = 0;
		LED = 1;
		pwmLiga = (i*100)/80;
		setDisplay(0,pwmLiga);
		delay_1s();
		count =0;

		while(1){
			j = TACOMETRO;
			i = i==0 ? 0 : i-1;
			DUTYCYCLE = i;
			delay_1s();
			if(TACOMETRO == j)
				break;

		}
		pwmDesliga = (i*100)/80;
		setDisplay(0,pwmDesliga);
		delay_1s();
		count = 0;

		while(1);
	}

}