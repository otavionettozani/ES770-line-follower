#include <p18f4550.h>
#include <adc.h>

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



//----------------- MATH FUNCTIONS ------------------------//


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
	MOTOR1_ENABLE_DIR = 0;
	MOTOR2_ENABLE_DIR = 0;
	MOTOR1_A_DIR = 0;
	MOTOR1_B_DIR = 0;
	MOTOR2_A_DIR = 0;
	MOTOR2_B_DIR = 0;
}


//----------------CORE FUNCTIONS -----------------------//


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


/* retorno = [-2.5 , 2.5] a depender da posicao da linha */
float readSensors(){
	float retorno=0;
	float leituras[6];
	float eixo[6] = {-1./2.5, -1./1.5, -1./0.5, 1./0.5, 1./1.5, 1./2.5};	
	int i;
	unsigned int adc_leitura;
	float a,b,c, mc, mb, ma;
	float pontoMinimo = 0, minimo = 10000;


	SetChanADC(ADC_CH5);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[0] = ((float)adc_leitura - 18944.)*10./44224.;

	SetChanADC(ADC_CH4);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[1] = ((float)adc_leitura - 14272.)*10./48896.;
	
	SetChanADC(ADC_CH3);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[2] = ((float)adc_leitura - 15424.)*10./47808.;

	SetChanADC(ADC_CH2);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[3] = ((float)adc_leitura - 13376.)*10./49856.;

	SetChanADC(ADC_CH1);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[4] = ((float)adc_leitura - 8192.)*10./54656.;

	SetChanADC(ADC_CH0);
	ConvertADC();
	while(BusyADC());
	adc_leitura = ReadADC();
	leituras[5] = ((float)adc_leitura - 5632.)*10./47936.;


	for(i=0; i<4;i++){
		c = leituras[i+2]*eixo[i+2]*eixo[i+2]/(eixo[i+2] - eixo[i]);
		c -=leituras[i]*eixo[i]*eixo[i]/(eixo[i+2] - eixo[i]);
		c -=leituras[i+1]*eixo[i+1]*eixo[i+1]/(eixo[i+1] - eixo[i]);
		c +=leituras[i]*eixo[i]*eixo[i]/(eixo[i+1] - eixo[i]);
		mc = eixo[i+2]*eixo[i+2]/(eixo[i+2] - eixo[i]);
		mc -= eixo[i]*eixo[i]/(eixo[i+2] - eixo[i]);
		mc -= eixo[i+1]*eixo[i+1]/(eixo[i+1] - eixo[i]);
		mc += eixo[i]*eixo[i]/(eixo[i+1] - eixo[i]);
		c = c/mc;

		b = leituras[i+1]*eixo[i+1]*eixo[i+1]/(eixo[i+1] - eixo[i]);
		b -=leituras[i]*eixo[i]*eixo[i]/(eixo[i+1] - eixo[i]);
		mb = eixo[i+1]*eixo[i+1]/(eixo[i+1] - eixo[i]);
		mb -= eixo[i]*eixo[i]/(eixo[i+1] - eixo[i]);
		b -=c*mb;

		a = leituras[i]*eixo[i]*eixo[i] - b*eixo[i] - c*eixo[i]*eixo[i];

		if(a<0)
			continue;

		pontoMinimo = -b/(2.*a);

		if(pontoMinimo > 1./eixo[i] && pontoMinimo < 1./eixo[i+2]){
			if(minimo > a*pontoMinimo*pontoMinimo + b*pontoMinimo + c){
				minimo = a*pontoMinimo*pontoMinimo + b*pontoMinimo + c;
				retorno = pontoMinimo;
			}
		}

	}

	return retorno;
	
}



//------------------   MAIN    -------------------------//

void main(void){
	float line;
	int lineOffset;
	configADC();
	delay_10ms();
	configLEDS_DIR();
	delay_10ms();

	while(1){
		line = readSensors();

		if(line>1.0){
			LED1 = 1;
			LED2 = 0;
		}else if(line<=1.0 && line >=-1.0){
			LED1 = 0;
			LED2 = 0;
		}else if(line < -1.0){
			LED1 = 0;
			LED2 = 1;
		}
	}


	return 0;
}