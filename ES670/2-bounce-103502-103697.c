//Matheus Freire Wu	103502
//Otávio Netto Zanni	103697

#include <p18f4550.h>

#pragma config WDT = OFF 
#pragma config FOSC = HS 
#pragma config LVP = OFF

#define SWITCH1	PORTBbits.RB0 
#define SWITCH1_dir	TRISBbits.RB0
#define LED1	PORTBbits.RB0
#define LED1_dir	TRISBbits.RB0
#define SWITCH2	PORTBbits.RB1 
#define SWITCH2_dir	TRISBbits.RB1
#define LED2	PORTBbits.RB1
#define LED2_dir	TRISBbits.RB1 
#define SWITCH3	PORTBbits.RB2
#define SWITCH3_dir	TRISBbits.RB2
#define LED3	PORTBbits.RB2
#define LED3_dir	TRISBbits.RB2 
#define SWITCH4	PORTBbits.RB3 
#define SWITCH4_dir	TRISBbits.RB3
#define LED4	PORTBbits.RB3
#define LED4_dir	TRISBbits.RB3 
#define VENTILADOR	PORTCbits.RC1
#define VENTILADOR_dir	TRISCbits.RC1 

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
	for(i=0;i<9;i++) delay_1ms();
	for(i=0;i<59;i++);
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
	for(i=0;i<9;i++) delay_10ms();
	for(i=0;i<9;i++) delay_1ms();
	for(i=0;i<49;i++);
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
	for(i=0;i<9;i++) delay_100ms();
	for(i=0;i<9;i++) delay_10ms();
	for(i=0;i<9;i++) delay_1ms();
	for(i=0;i<39;i++);
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

void toggle(int *var, int pos){
	int i = (*var>>pos)&1;
	int aux = 1<<pos;
	
	if(i)
		*var = *var&(~aux);
	else
		*var = *var|aux;
	return;
}

void main(){
	char i, buttonStatus1, buttonStatus2, buttonStatus3, buttonStatus4;
	int buttonMask = 0;
	char aux = 0 ,aux2 = 0, aux3 = 0, aux4 = 0;
	ADCON1 = 0x0F;

	buttonStatus1 =1;
	buttonStatus2 =1;
	buttonStatus3 =1;
	buttonStatus4 =1;

	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;

	VENTILADOR_dir = 0;
	
	
	
	while(1){
		SWITCH1_dir = 1;
		Nop();

		if(SWITCH1 == 0){
			delay_10ms();
			delay_10ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			if(SWITCH1 == 0){
				buttonStatus1 = 0;
			}else{
				buttonStatus1 =1;
			}
		}else {
			buttonStatus1 = 1;
		
		}
		SWITCH1_dir =0;

		SWITCH2_dir = 1;
		Nop();
		if(SWITCH2 == 0){
			delay_10ms();
			delay_10ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			if(SWITCH2 == 0){
				buttonStatus2 = 0;
			}else{
				buttonStatus2 =1;
			}
		}else {
			buttonStatus2 = 1;
		
		}
		SWITCH2_dir = 0;
		SWITCH3_dir = 1;
		Nop(); 


		if(SWITCH3 == 0){
			delay_10ms();
			delay_10ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			if(SWITCH3 == 0){
				buttonStatus3 = 0;
			}else{
				buttonStatus3 =1;
			}
		}else {
			buttonStatus3 = 1;
		
		}
		SWITCH3_dir = 0;
		SWITCH4_dir = 1;
		Nop();
		if(SWITCH4 == 0){
			delay_10ms();
			delay_10ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			delay_1ms();
			if(SWITCH4 == 0){
				buttonStatus4 = 0;
			}else{
				buttonStatus4 =1;
			}
		}else {
			buttonStatus4 = 1;
		
		}
		SWITCH4_dir = 0;
		Nop();

		if(aux == 0&&buttonStatus1 == 0){
			toggle(&buttonMask, 0);
			aux = 1;
		}
		else if(aux == 1 && buttonStatus1 == 1)
			aux = 0;

		if(aux2 == 0&&buttonStatus2 == 0){
			toggle(&buttonMask, 1);
			aux2 = 1;
		}
		else if(aux2 == 1 && buttonStatus2 == 1)
			aux2 = 0;

		if(aux3 == 0&&buttonStatus3 == 0){
			toggle(&buttonMask, 2);
			aux3 = 1;
		}
		else if(aux3 == 1 && buttonStatus3 == 1)
			aux3 = 0;

		if(aux4 == 0&&buttonStatus4 == 0){
			toggle(&buttonMask, 3);
			aux4 = 1;
		}
		else if(aux4 == 1 && buttonStatus4 == 1)
			aux4 = 0;
			
		LED1_dir = 0;
		LED2_dir = 0;
		LED3_dir = 0;
		LED4_dir = 0;
		Nop();

		LED1 = buttonMask&1;
		LED2 = (buttonMask>>1)&1;
		LED3 = (buttonMask>>2)&1;
		LED4 = (buttonMask>>3)&1;
		VENTILADOR = (buttonMask&1)^((buttonMask&2)>>1)^((buttonMask&4)>>2)^((buttonMask&8)>>3);



	}
}