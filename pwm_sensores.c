#include <p18f4550.h>
#include <adc.h>
#include <pwm.h>
#include <timers.h>

#pragma config WDT = OFF
#pragma config FOSC = HS
#pragma config LVP = OFF
//------------------ MOTOR CONSTANTS -----------------------//

#define FORWARD 1
#define BACKWARD 0
#define MIN_MOTOR_ON 0b1011110000
#define MAX_MOTOR_ON 0b1111111111
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

#define SENSOR_IR0 PORTAbits.RA0
#define SENSOR_IR1 PORTAbits.RA1
#define SENSOR_IR2 PORTAbits.RA2
#define SENSOR_IR3 PORTAbits.RA3
#define SENSOR_IR4 PORTAbits.RA4
#define SENSOR_IR5 PORTAbits.RA5
#define SENSOR_INPUT TRISA
#define WHITE_OFFSET_H 0x00
#define WHITE_OFFSET_L 0xff

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1
#define LED_IR_ENABLE LATD
#define LED_IR_DIR TRISD


#define MOTOR1_A PORTBbits.RB4
#define MOTOR1_A_DIR TRISBbits.RB4
#define MOTOR1_B PORTBbits.RB5
#define MOTOR1_B_DIR TRISBbits.RB5

#define MOTOR2_A PORTBbits.RB6
#define MOTOR2_A_DIR TRISBbits.RB6
#define MOTOR2_B PORTBbits.RB7
#define MOTOR2_B_DIR TRISBbits.RB7

#define MOTOR1_ENABLE_DIR TRISCbits.RC1
#define MOTOR2_ENABLE_DIR TRISCbits.RC2

#define ENCODER1 PORTBbits.RB1
#define ENCODER1_dir TRISBbits.RB1

#define ENCODER2 PORTBbits.RB2
#define ENCODER2_dir TRISBbits.RB2


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

void configEncoders(){
	ENCODER1_dir = 1;
	ENCODER2_dir = 1;
}

void configADC(){

	ADCON1 = 0x0F;
	

	OpenADC(ADC_FOSC_2 & ADC_LEFT_JUST & ADC_20_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 8);

}

void configLEDS_DIR(){
	LED_IR_DIR = 0x00;
}

void configureMotors(){
	MOTOR1_A_DIR = 0;
	MOTOR1_B_DIR = 0;
	MOTOR2_A_DIR = 0;
	MOTOR2_B_DIR = 0;

	MOTOR1_ENABLE_DIR = 0;
	MOTOR2_ENABLE_DIR = 0;

	OpenPWM2(0xF9);
	OpenPWM1(0xF9);
}

void configPWMTimer(){
	OpenTimer2(TIMER_INT_OFF & T2_PS_1_16);
}


//----------------CORE FUNCTIONS -----------------------//


void startMotor1(char direction, int percentageVelocity){
	int dutycycle;

	dutycycle = ((MAX_MOTOR_ON-MIN_MOTOR_ON)*percentageVelocity)/100 + MIN_MOTOR_ON;

	MOTOR1_A = direction;
	MOTOR1_B = !direction;

	SetDCPWM2(dutycycle);

}

void startMotor2(char direction , int percentageVelocity){
	int dutycycle;

	dutycycle = ((MAX_MOTOR_ON-MIN_MOTOR_ON)*percentageVelocity)/100 + MIN_MOTOR_ON;

	MOTOR2_A = !direction;
	MOTOR2_B = direction;

	SetDCPWM1(dutycycle);

} 


void stopMotor1(){
	SetDCPWM2(0);
}
void stopMotor2(){
	SetDCPWM1(0);
}


int calibrateSensors(){
	unsigned int max;
	unsigned int min;	

	SetChanADC(ADC_CH5);
	ConvertADC();
	while(BusyADC());
	max = min = ReadADC();

	SetChanADC(ADC_CH4);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>max)
		max = ReadADC();
	if(ReadADC()<min)
		min = ReadADC();

	SetChanADC(ADC_CH3);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>max)
		max = ReadADC();
	if(ReadADC()<min)
		min = ReadADC();

	SetChanADC(ADC_CH2);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>max)
		max = ReadADC();
	if(ReadADC()<min)
		min = ReadADC();

	SetChanADC(ADC_CH1);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>max)
		max = ReadADC();
	if(ReadADC()<min)
		min = ReadADC();

	SetChanADC(ADC_CH0);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>max)
		max = ReadADC();
	if(ReadADC()<min)
		min = ReadADC();

	return (max+min)/2;

}

/* retorno = [-2.5 , 2.5] a depender da posicao da linha */
float readSensors(int comparador){
	float retorno=0;
	float activeSensors=0;	
	
	SetChanADC(ADC_CH5);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += -2.5;
		activeSensors++;
	}

	SetChanADC(ADC_CH4);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += -1.5;
		activeSensors++;
	}

	
	SetChanADC(ADC_CH3);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += -0.5;
		activeSensors++;
	}


	SetChanADC(ADC_CH2);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += 0.5;
		activeSensors++;
	}


	SetChanADC(ADC_CH1);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += 1.5;
		activeSensors++;
	}


	SetChanADC(ADC_CH0);
	ConvertADC();
	while(BusyADC());
	if(ReadADC()>comparador){
		retorno += 2.5;
		activeSensors++;
	}
	
	return retorno/activeSensors;
	
}



//------------------   MAIN    -------------------------//

void main(void){
	float line;
	int lineOffset;
	unsigned int percentageVelocity1, percentageVelocity2;


	configADC();
	configLEDS_DIR();
	configureMotors();

	lineOffset = calibrateSensors();
	while(1){
		line = readSensors(0x7F);
		
		if(line<0.5){
			percentageVelocity1 = 25;
			percentageVelocity2 = 0;
		}else if(line>0.5){
			percentageVelocity1 = 0;
			percentageVelocity2 = 25;
		}



		startMotor1(FORWARD, percentageVelocity1);
		startMotor2(FORWARD, percentageVelocity2);
	}


	return 0;
}