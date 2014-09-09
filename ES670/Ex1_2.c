#include <p18f4550.h>

#pragma config WDT = OFF  // Configura watchdog timer
#pragma config FOSC = HS  // Configura oscilador
#pragma config LVP = OFF  // Configura low-voltage

// Segundo circuito da placa
#define LED1            PORTBbits.RB0
#define LED1_DIR        TRISBbits.RB0
#define LED2            PORTBbits.RB1
#define LED2_DIR        TRISBbits.RB1
#define LED3            PORTBbits.RB2
#define LED3_DIR        TRISBbits.RB2
#define LED4            PORTBbits.RB3
#define LED4_DIR        TRISBbits.RB3
#define SWITCH1         PORTBbits.RB0
#define SWITCH1_DIR     TRISBbits.RB0
#define SWITCH2         PORTBbits.RB1
#define SWITCH2_DIR     TRISBbits.RB1
#define SWITCH3         PORTBbits.RB2
#define SWITCH3_DIR     TRISBbits.RB2
#define SWITCH4         PORTBbits.RB3
#define SWITCH4_DIR     TRISBbits.RB3
#define VENTILADOR      PORTCbits.RC1
#define VENTILADOR_DIR  TRISCbits.RC1

// Definições para IO
#define SWITCH_ON   0
#define SWITCH_OFF  1
#define ON          1
#define OFF         0
#define OUTPUT      0
#define INPUT       1

void main() {
	int buttonMask = 0;

	ADCON1 = 0x0f; // Configura os pinos do conversor AD para IO

  // Configura valor da porta, depois sua direcao
	LED1 = OFF;
	LED2 = OFF;
	LED3 = OFF;
	LED4 = OFF;
	VENTILADOR = OFF;

	VENTILADOR_DIR = OUTPUT;


	while (1) {

		SWITCH1_DIR = INPUT;
		SWITCH2_DIR = INPUT;
		SWITCH3_DIR = INPUT;
		SWITCH4_DIR = INPUT;
		Nop();

		buttonMask = (~SWITCH1&1) | ((~SWITCH2&1)<<1) | ((~SWITCH3&1)<<2) | ((~SWITCH4&1)<<3) | ((SWITCH1^SWITCH2^SWITCH3^SWITCH4)<<4);

		LED1_DIR = OUTPUT;
		LED2_DIR = OUTPUT;
		LED3_DIR = OUTPUT;
		LED4_DIR = OUTPUT;
		Nop();

		LED1 =  buttonMask&1;
		LED2 = (buttonMask>>1)&1;
		LED3 = (buttonMask>>2)&1;
		LED4 = (buttonMask>>3)&1;
		VENTILADOR = (buttonMask>>4)&1;
		
		
	}
}