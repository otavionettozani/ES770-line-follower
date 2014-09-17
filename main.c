#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF


//------------------ CODE DEFINES --------------------------//

#define SENSOR_INPUT TRISA
#define WHITE_OFFSET_H 0
#define WHITE_OFFSET_L 0

#define LED_IR_ENABLE LATD
#define LED_IR_DIR TRISD


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

/* retorno>0 <=> i1>i2 --- retorno<0 <=> i1<i2 --- retorno=0 <=> i1=i2 */
int twoByteComp(int i1H, int i1l, int i2h, int i2l){
	if(i1H!=i2H){
		return i1H-i2H;
	}
	return i1L-i2L;
}


//----------------CONFIG FUNCTIONS ---------------------//

void configADC(){

	SENSOR_INPUT = 0x3F;

	ADCON1 = 0x09;
	ADCON0 = 0x01;
	ADCON2 = 0b10001000;

}

void configLEDS_DIR{
	LED_IR_DIR = 0x0;
}


//----------------CORE FUNCTIONS -----------------------//

/* retorno = [-3 , 3] a depender da posicao da linha */
float readSensors(){
	char highLedRead, lowLedRead;
	char activeLeds;
	char i;
	
	
	LED_IR_ENABLE = 0xFC;
	for(i=0;i<6;i++){
		ADCON0 = i<<2;
		delay_10ms();
		ADCON0 = ADCON0 | 1;
		delay_10ms();
		ADCON0 = ADCON0 | 2;
		while(ADCON0 & 2);
		if(twoByteComp(ADRESH, ADRESL, WHITE_OFFSET_H, WHITE_OFFSET_L)<0){
			activeLeds = activeLeds | 1<<i;
		}
	}
	LED_IR_ENABLE = 0x00;
	
	//posicao da linha a partir dos valors lidos	
	
}


//------------------   MAIN    -------------------------//

void main(void){
	
	configADC();
	configLEDS_DIR();

	while(1){


	}


	return 0;
}