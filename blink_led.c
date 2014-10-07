#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF


//------------------ CODE DEFINES --------------------------//

#define LEDS LATD
#define LED_DIR TRISD
#define IR_RECEPTOR_7 PORTAbits.RA0
#define IR_RECEPTOR_6 PORTAbits.RA1
#define IR_RECEPTOR_5 PORTAbits.RA2
#define IR_RECEPTOR_4 PORTAbits.RA3
#define IR_RECEPTOR_3 PORTAbits.RA4
#define IR_RECEPTOR_2 PORTAbits.RA5

#define IR_RECEPTOR_7_DIR TRISAbits.RA0
#define IR_RECEPTOR_6_DIR TRISAbits.RA1
#define IR_RECEPTOR_5_DIR TRISAbits.RA2
#define IR_RECEPTOR_4_DIR TRISAbits.RA3
#define IR_RECEPTOR_3_DIR TRISAbits.RA4
#define IR_RECEPTOR_2_DIR TRISAbits.RA5

//------------------ WAIT FUNCTIONS -----------------------//


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



//----------------- MATH FUNCTIONS ------------------------//


//----------------CONFIG FUNCTIONS ---------------------//


//----------------CORE FUNCTIONS -----------------------//


//------------------   MAIN    -------------------------//

void main(void){
	char i = 0;	
	char a = 0xff;
	ADCON0 = 0x0f;
	LED_DIR = 0;


	IR_RECEPTOR_7_DIR =1;
	IR_RECEPTOR_6_DIR =1;
	IR_RECEPTOR_5_DIR =1;
	IR_RECEPTOR_4_DIR =1;
	IR_RECEPTOR_3_DIR =1;
	IR_RECEPTOR_2_DIR =1;
	
	while(1){
	
		if(IR_RECEPTOR_7 || IR_RECEPTOR_6 || IR_RECEPTOR_5 || IR_RECEPTOR_4 || IR_RECEPTOR_3 || IR_RECEPTOR_2){
			a = 0xff;
		}
		else{
			a = ~a;
			delay_1s();
		}
		LEDS = a;
	}


	return 0;
}