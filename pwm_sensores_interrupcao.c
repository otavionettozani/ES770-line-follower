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

#define MOTOR1_KP 8.55
#define MOTOR1_KI 0.16
#define MOTOR1_KD 0

#define MOTOR2_KP 1
#define MOTOR2_KI 0
#define MOTOR2_KD 0

#define SYSTEM_KP 1
#define SYSTEM_KI 0
#define SYSTEM_KD 0
#define PERCENTAGE_MAX_VELOCITY 0.5

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

void configInterruptionTimer(){
	OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_64);
	//aprox 0.8s
}

void resetInterruptionTimer(){
	WriteTimer0(0);
	INTCONbits.TMR0IF = 0;
}



//----------------CORE FUNCTIONS -----------------------//


void startMotor1(char direction, int percentageVelocity){
	int dutycycle;

	percentageVelocity = percentageVelocity*PERCENTAGE_MAX_VELOCITY;
	dutycycle = ((MAX_MOTOR_ON-MIN_MOTOR_ON)*percentageVelocity)/100 + MIN_MOTOR_ON;
	

	MOTOR1_A = direction;
	MOTOR1_B = !direction;

	SetDCPWM2(dutycycle);

}

void startMotor2(char direction , int percentageVelocity){
	int dutycycle;

	percentageVelocity = percentageVelocity*PERCENTAGE_MAX_VELOCITY;
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


/* retorno = [-2.5 , 2.5] a depender da posicao da linha */
float readSensors(){
	float leituras[6];
	float eixo[6] = {-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};	
	int i;
	unsigned int adc_leitura;
	float a,b,c, derv, det;
	float pontoMinimo = 0, minimo = 10000, testeMinimo1, testeMinimo2;


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

	//splines
	i=0;
	det = eixo[i]*eixo[i]*eixo[i+1];
	det = det - eixo[i]*eixo[i]*eixo[i+2];
	det = det - eixo[i]*eixo[i+1]*eixo[i+1];
	det = det + eixo[i]*eixo[i+2]*eixo[i+2];
	det = det + eixo[i+1]*eixo[i+1]*eixo[i+2];
	det = det - eixo[i+1]*eixo[i+2]*eixo[i+2];


	a = leituras[i]*(eixo[i+1]-eixo[i+2]);
	a = a - leituras[i+1]*(eixo[i]-eixo[i+2]);
	a = a + leituras[i+2]*(eixo[i]-eixo[i+1]);
	a = a/det;

	b = leituras[i+1]*(eixo[i]*eixo[i]-eixo[i+2]*eixo[i+2]);
	b = b - leituras[i]*(eixo[i+1]*eixo[i+1]-eixo[i+2]*eixo[i+2]);
	b = b - leituras[i+2]*(eixo[i]*eixo[i]-eixo[i+1]*eixo[i+1]);
	b = b/det;

	c = eixo[i]*eixo[i+1]*leituras[i+2]*(eixo[i]-eixo[i+1]);
	c = c - eixo[i]*eixo[i+2]*leituras[i+1]*(eixo[i]-eixo[i+2]);
	c = c + eixo[i+1]*eixo[i+2]*leituras[i]*(eixo[i+1]-eixo[i+2]);
	c = c/det;

	

	if(-b/(2*a)>=eixo[0] && -b/(2*a)<=eixo[2]){
		pontoMinimo = -b/(2*a);
		minimo = a*pontoMinimo*pontoMinimo+b*pontoMinimo+c;
	}else{
		testeMinimo1 = a*eixo[0]*eixo[0]+b*eixo[0]+c;
		testeMinimo2 = a*eixo[2]*eixo[2]+b*eixo[2]+c;
		minimo = testeMinimo1<testeMinimo2?testeMinimo1:testeMinimo2;
		pontoMinimo = testeMinimo1<testeMinimo2?eixo[0]:eixo[2];
	}

	for(i=2; i<5; i++){
		derv = 2*a*eixo[i]+b;

		det = eixo[i]*eixo[i]-2*eixo[i]*eixo[i+1]+eixo[i+1]*eixo[i+1];

		a = leituras[i+1]-leituras[i]+derv*(eixo[i]-eixo[i+1]);
		a = a/det;

		b = 2*eixo[i]*leituras[i];
		b = b - derv*(eixo[i]*eixo[i]-eixo[i+1]*eixo[i+1]);
		b = b - 2*eixo[i]*leituras[i+1];
		b = b/det;

		c = eixo[i]*eixo[i]*leituras[i+1];
		c = c - leituras[i]*(2*eixo[i]*eixo[i+1] - eixo[i+1]*eixo[i+1]);
		c = c + derv*eixo[i]*eixo[i+1]*(eixo[i]-eixo[i+1]);
		c = c/det;

		testeMinimo1 = a*(-b/(2*a))*(-b/(2*a))+b*(-b/(2*a))+c;
		if(-b/(2*a)>=eixo[i] && -b/(2*a)<=eixo[i+1] && minimo>testeMinimo1 ){
			pontoMinimo = -b/(2*a);
			minimo = a*pontoMinimo*pontoMinimo+b*pontoMinimo+c;
		}else{
			testeMinimo1 = a*eixo[i]*eixo[i]+b*eixo[i]+c;
			testeMinimo2 = a*eixo[i+1]*eixo[i+1]+b*eixo[i+1]+c;
			if(minimo>testeMinimo1 || minimo>testeMinimo2){
				minimo = testeMinimo1<testeMinimo2?testeMinimo1:testeMinimo2;
				pontoMinimo = testeMinimo1<testeMinimo2?eixo[i]:eixo[i+1];
			}
		}

	}


	return pontoMinimo;
	
}


unsigned int pulse1Count=0, pulse2Count=0;
unsigned char lastSensor1=0, lastSensor2=0;
void readEncoders(){
	if(ENCODER1 == 1 && ENCODER1 != lastSensor1){
		pulse1Count ++;
	}
	if(ENCODER2 == 1 && ENCODER2 != lastSensor2){
		pulse2Count ++;
	}

	lastSensor1 = ENCODER1;
	lastSensor2 = ENCODER2;

}


//------------------  INTERRUPTION ---------------------//

/*
float currentVelocity1=0, currentVelocity2=0;

void timer_interrupt(void);
void velocityCalculation(void);

#pragma code low_vector = 0x08
void low_interrupt(void){
	_asm GOTO timer_interrupt _endasm
}

#pragma code
#pragma interruptlow timer_interrupt

void timer_interrupt(void){

	velocityCalculation();
	resetInterruptionTimer();
}


void velocityCalculation(){
		currentVelocity1 = pulse1Count/0.8;
		pulse1Count = 0;
		currentVelocity2 = pulse2Count/0.8;
		pulse2Count = 0;
}
*/
//------------------   MAIN    -------------------------//

void main(void){
	float line;
	int percentageVelocity1, percentageVelocity2;
	unsigned char lastRead1, lastRead2;
//	float velocityReference1 = 10, velocity1Integrate =0;
//	float errorMotorVelocity;
	float PID, integrative, lastValue, derivative;

	configADC();
	delay_10ms();
	configLEDS_DIR();
	delay_10ms();
	configureMotors();
	delay_10ms();

	configInterruptionTimer();
	delay_10ms();
	resetInterruptionTimer();
	delay_10ms();


	INTCONbits.GIE = 1;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
	delay_10ms();

	lastValue = 0;

	while(1){

		line = readSensors();

		integrative += line;
		integrative = integrative>10.?10.:integrative;
		integrative = integrative<-10.?-10.:integrative;
		
		derivative = line - lastValue;
		lastValue = line;

		PID = SYSTEM_KP*line + SYSTEM_KI*integrative + SYSTEM_KD*derivative;

		//acionamento dos motores: se nossa saida do PID == -2.5, girar um dos motores apenas para frente na velocidade maxima limite, caso 2.5 girar o outro motor;

		
		//startMotor1(FORWARD, 50.-20.*PID);
		//startMotor2(FORWARD, 50.+20.*PID);

		startMotor1(FORWARD, 100);
		startMotor2(FORWARD, 100);

	}


	return 0;
}