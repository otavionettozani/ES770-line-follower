#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF
#pragma config CCP2MX = ON

//#define VENTILADOR PORTCbits.RC1
#define VENTILADOR_dir TRISCbits.RC1

#define PWMP PR2
#define TIMER T2CON
#define PWM_conf CCP2CON
#define DUTYCYCLE CCPR2L


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

void main(void){
	char i=0, j, k=0;
	
	ADCON1 = 0x0f;
	
	PWMP = 79; //contador do timer conta ate 79
	DUTYCYCLE = 0;
	VENTILADOR_dir = 0;
	TIMER = 0b00000110; //seta o timer on e pre-scale 16
	PWM_conf = 0b00001100; //seta a configuracao do PWN como PWM e os 2bits menos sig. como 0


	while(1){
		i = k? i-1 : i+1;
		k = i==0? 0 : (i==80 ? 1:k);
		DUTYCYCLE = i;
		delay_10ms();
	}

}