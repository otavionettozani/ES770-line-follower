#include <p18f4550.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF
//------------------ MOTOR CONSTANTS -----------------------//

#define FOWARD 1
#define BACKWARD 0
//------------------ PID CONSTANTS --------------------------//

#define MOTOR1_KP 1
#define MOTOR1_KI 0
#define MOTOR1_KD 0

#define MOTOR2_KP 1
#define MOTOR2_KI 0
#define MOTOR2_KD 0

#define SYSTEM_KP 1
#define SYSTEM_KI 0
#define SYSTEM_KD 0

//------------------ CODE DEFINES --------------------------//

#define SENSOR_INPUT TRISA
#define WHITE_OFFSET_H 0x00
#define WHITE_OFFSET_L 0xff

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1
#define LED_IR_ENABLE LATD
#define LED_IR_DIR TRISD

#define MOTOR1_ENABLE PORTCbits.RC1
#define MOTOR1_ENABLE_DIR TRISCbits.RC1
#define MOTOR2_ENABLE PORTCbits.RC2
#define MOTOR2_ENABLE_DIR TRISCbits.RC2

#define MOTOR1_A PORTBbits.RB4
#define MOTOR1_A_DIR TRISBbits.RB4
#define MOTOR1_B PORTBbits.RB5
#define MOTOR1_B_DIR TRISBbits.RB5

#define MOTOR2_A PORTBbits.RB6
#define MOTOR2_A_DIR TRISBbits.RB6
#define MOTOR2_B PORTBbits.RB7
#define MOTOR2_B_DIR TRISBbits.RB7



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
int twoByteComp(int i1H, int i1L, int i2H, int i2L){
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

void configLEDS_DIR(){
	LED_IR_DIR = 0x00;
}

void configureMotors(){
	MOTOR1_ENABLE_DIR = 0;
	MOTOR2_ENABLE_DIR = 0;
	MOTOR1_A_DIR = 0;
	MOTOR1_B_DIR = 0;
	MOTOR2_A_DIR = 0;
	MOTOR2_B_DIR = 0;
}


//----------------CORE FUNCTIONS -----------------------//

/* retorno = [-3 , 3] a depender da posicao da linha */
float readSensors(){
	char activeLeds, activeLedsCounter;
	char i;
	char retorno;
	
	
	for(i=0;i<6;i++){
		ADCON0 = i<<2;
		delay_10ms();
		ADCON0 = ADCON0 | 1;
		delay_10ms();
		ADCON0 = ADCON0 | 2;
		while(ADCON0 & 2);
		if(twoByteComp(ADRESH, ADRESL, WHITE_OFFSET_H, WHITE_OFFSET_L)>0){
			activeLeds = activeLeds | (1<<i);
		}
	}
	
	//posicao da linha a partir dos valors lidos
	activeLedsCounter =0;
	for(i=0; i<6; i++){
		if(activeLeds&(1<<i)){
			retorno += (2.5-i);
			activeLedsCounter++;
			
		}
	}

	if(activeLedsCounter >=3){
		LED1 = 1;
	}else {
		LED1 = 0;
	}
	
	return retorno/activeLedsCounter;
}


void startMotor1(char direction){
	MOTOR1_ENABLE = 1;

	MOTOR1_A = direction;
	MOTOR1_B = !direction;


}

void startMotor2(char direction){
	MOTOR2_ENABLE = 1;

	MOTOR2_A = direction;
	MOTOR2_B = !direction;

} 


void stopMotor1(){
	MOTOR1_ENABLE = 0;
}
void stopMotor2(){
	MOTOR2_ENABLE = 0;
}



//------------------   MAIN    -------------------------//

void main(void){
	char a = 0x00;

	configADC();
	configLEDS_DIR();
	LED_IR_ENABLE = 0;
	configureMotors();
	startMotor1(FOWARD);
	startMotor2(BACKWARD);

	while(1){
		readSensors();
	}


	return 0;
}