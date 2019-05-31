#include "lpc21xx.h"
#include "minilib.h"
#include <string.h>
#include <stdlib.h>
/****************LCD DIRECTIVES***************/
#define LCD_CLEAR        0x01
#define CURSOR_OFF     0x0C
#define FIRST_ROW        0x80
#define SECOND_ROW    0xC0
#define Enable_Pulse()    IOSET0|=1<<EN;Delay_ms(1);IOCLR0=1<<EN;Delay_ms(1);
/*Pin Configuration for LCD*/
#define        RS                2
#define        RW                3
#define        EN                4
#define BAUD    9600
#define DIVISOR ((PCLK/16)/BAUD)

#define TX_READY (1<<5)
#define BAUD    9600
/*********************************************/

/*********************************************/
/**************Function Prototypes************/

void Lcd_Init(void);
void Lcd_Cmd(unsigned char value);
void Lcd_Write(unsigned char value);
void Lcd_Write_Text(unsigned char msg[]);
void Lcd_Data_Shift(unsigned char value);
char c;
char bytePrueba;
void Delay_ms(unsigned long times);
char key(void);

char dir[8];

int contadorPulsaciones;
char pulsado[4];
char cmd[8];

int step;
void UART0_Init(void);
void UART0_Write(unsigned char value);
void UART0_Write_Text(unsigned char msg[], int modo);
void decToBin(unsigned char msg[]);
unsigned char UART0_Read(void);
/*********************************************/

int main() {
		
	char mostrarLista[] = "Mostrar lista";
	char conectadoPic[] = "Conenctado con el PIC";
	
	contadorPulsaciones=0;

	dir[0]='1';
	dir[1]='0';
	dir[2]='0';
	dir[3]='0';
	dir[4]='0';
	dir[5]='0';
	dir[6]='0';
	dir[7]='0';

	bytePrueba = '\x02';

	Lcd_Init();
	UART0_Init();
	Delay_ms(10);
	//Lcd_Cmd(SECOND_ROW);
	//Lcd_Write_Text(echo_test);
	//Delay_ms(100);
	//Lcd_Cmd(LCD_CLEAR);                    //Clear the LCD
	//Lcd_Cmd(SECOND_ROW);
	Lcd_Cmd(LCD_CLEAR);
	IODIR0|=0x0001E000;

			
	UART0_Write(bytePrueba);
 	while(1) {
		//UART0_Write_Text((unsigned char *)pulsado, 0);
		/*c=key();
		if(c=='#'){
			Lcd_Cmd(LCD_CLEAR);
			if (contadorPulsaciones>0){
				pulsado[contadorPulsaciones-1] = NULL;
				contadorPulsaciones--;
			}
			Lcd_Write_Text((unsigned char *)pulsado);
		}else if(c=='*') {
				UART0_Write_Text((unsigned char *)pulsado, 0);
			Lcd_Cmd(LCD_CLEAR);
			Lcd_Write_Text((unsigned char *)mostrarLista);
			//Delay_ms(500);
			Lcd_Cmd(LCD_CLEAR);
			// ahora tendriamos que enviar el mensaje a todos los pic, para saber cuales
			// estan conectados y mostrar sus direcciones por pantalla
			contadorPulsaciones=0;
			while(contadorPulsaciones<3) {
				c=key();
				Lcd_Write(c);
				pulsado[contadorPulsaciones]=c;
				contadorPulsaciones++;
			}
			// enviamos el mensaje al PIC que se ha especificado, y esperamos a enviar los comandos
			Delay_ms(500);
			Lcd_Cmd(LCD_CLEAR);
			Lcd_Write_Text((unsigned char *)conectadoPic);
			//Delay_ms(500);
			Lcd_Cmd(LCD_CLEAR);
			// enviamos el mensaje al PIC10F200 por el UART
			UART0_Write_Text((unsigned char *)pulsado, 0);

			// si despues de un determiando periodo de tiempo el PIC sigue sin responder, entonces podemos
			// decir que ha saltado un timeout y que el PIC no ha contestado. 

			// comprobamos que el PIC responde, si el pic responde pasamos a la siguiente fase, sino tenemos
			// que reiniciar y volver a la parte anterior

			
			// limpiamos la memoria de las pulsaciones
			pulsado[0]='0';
			pulsado[1]='0';
			pulsado[2]='0';
			pulsado[3]='0';
			// entramos en el bucle de interaccion con el PIC
			contadorPulsaciones=0;
			do{
				c=key();
				pulsado[contadorPulsaciones]=c;
				contadorPulsaciones++;
				Lcd_Write(c);
				// enviamos el mensaje el PIC se ejecuta, y recibimos al respuesta
			} while (c!='0')

			// vaciamos la cadena que almacena las pulsaciones para los proximos comandos
			pulsado[0]='0';
			pulsado[1]='0';
			pulsado[2]='0';
			pulsado[3]='0'; 
			*/
		/*}else{
			pulsado[contadorPulsaciones]=c;
			contadorPulsaciones++;
			Lcd_Write(c);
		} */
		Delay_ms(100);
    }
}

/****************Function Definition**********/
void decToBin(unsigned char msg[]) {
	int value = ((int) msg[0])*10 + ((int) msg[1]);
	int counter = 7;
	// byte de direccion [0 1 X X X X X]
	// los 2 primeros bits no cambian
	dir[0]='0';
	dir[1]='1';

	dir[2]='0';
	dir[3]='0';
	dir[4]='0';
	dir[5]='0';
	dir[6]='0';
	dir[7]='0';

	do{
		if((value%2) ==0)
			dir[counter]='0';
		else
			dir[counter]='1';
		value=(value/2);
		counter--;
	} while(value>0);
}

void decComando(unsigned char msg[]) {
	int value = (int) msg[0];
	int counter = 7;
	// byte de direccion [0 1 X X X X X]
	// los 2 primeros bits no cambian
	cmd[0]='0';
	cmd[1]='0';
	cmd[2]='0';
	cmd[3]='0';
	switch (value) {
		case (0):
			cmd[4]='0';
			cmd[5]='0';
			cmd[6]='0';
			cmd[7]='0';
			break;
		case (1):
			cmd[4]='0';
			cmd[5]='0';
			cmd[6]='0';
			cmd[7]='1';
			break;
		case (2):
			cmd[4]='0';
			cmd[5]='0';
			cmd[6]='1';
			cmd[7]='0';
			break;
		case (3):
			cmd[4]='0';
			cmd[5]='0';
			cmd[6]='1';
			cmd[7]='1';
			break;
		case (4):
			cmd[4]='0';
			cmd[5]='1';
			cmd[6]='0';
			cmd[7]='1';
			break;
		case (5):
			cmd[4]='0';
			cmd[5]='1';
			cmd[6]='1';
			cmd[7]='0';
			break;
		case (6):
			cmd[4]='0';
			cmd[5]='1';
			cmd[6]='1';
			cmd[7]='1';
			break;
		case (7):
			cmd[4]='1';
			cmd[5]='0';
			cmd[6]='0';
			cmd[7]='0';
			// devuelve el estado pero no es ningun comadno anterior, por lo que
			// tenemos que formar uno diferente, solo podeos envair 8 de manera normal
			break;
		case (8):
			cmd[4]='0';
			cmd[5]='1';
			cmd[6]='0';
			cmd[7]='0';
			break;
	}
}

/***************UART-0 Functions**************/
void UART0_Init(void) {
    PINSEL0 = 0x00000005;     // P0.0 as TX0 and P0.1 as RX0

    U0LCR = 3 | (1<<7);       // 8 bits, no parity, 1 bitstop
    U0DLL = 6;                // our baud rate is 38400 --> DLL = PCLK/(16*BaudRate)
    U0LCR = 3;       // 8 bits, no parity, 1 bitstop
    U0DLM = 0;
	U0FCR = 7;
}

void UART0_Write(unsigned char value) {
    while(!(U0LSR & 0x20));                        //THRE = 0 stay here
    U0THR = value;
}

// especificamos si lo que queremos enviar es el byte de direccionamiento
// o el byte de comandos
//
// 0 --> direccion
// 1 --> comando
void UART0_Write_Text(unsigned char msg[], int modo) {
	/*
	int i =0;
	// enviamos el byte con la direccion para que el PIC se prepare
	if (modo==0) {
		// decodificamos el mensaje y enviamos los dos bytes por el UART
		decToBin((unsigned char *)msg);	
		for(i=0; i<8 ;i++)
			UART0_Write(dir[i]);
	}
	// enviamos los comandos al PIC
	if (modo==1) {
		decComando((unsigned char *)msg);	
		for(i=0; i<8 ;i++)
			UART0_Write(cmd[i]);
	}
	*/

}

unsigned char UART0_Read(void) {
    while(!(U0LSR & 0x01));                        //RDR = 0 stay here
    return (U0RBR);
}
/*********************************************/

/****************Delay Function***************/
void Delay_ms(unsigned long times) {
    unsigned long i,j;
    for(j=0;j<times;j++)
        for(i=0;i<7500;i++);
}
/*********************************************/

/*****************LCD Functions***************/
void Lcd_Init(void)
{
    IODIR0 |= (1<<RS);                    //RS Pin as Output Pin
    IODIR0 |= (1<<RW);                    //RW Pin as Output Pin
    IODIR0 |= (1<<EN);                    //EN Pin as Output Pin
                    
    IODIR0 |= 0xFF000000;                //P0.8 to P0.15 as Data Line of LCD
   
    Lcd_Cmd(0x38);                            //Send 8-bit initialization command to lcd
    Delay_ms(10);
    Lcd_Cmd(CURSOR_OFF);                //Cursor OFF
    Delay_ms(10);
    Lcd_Cmd(LCD_CLEAR);
    Delay_ms(1);
    Lcd_Cmd(FIRST_ROW);
}

void Lcd_Data_Shift(unsigned char value) {
    /*
    This Function will shift the eight bit data stored in variable value,
    to the Port Pin P0.8 to P0.15 Successfully.
    */
    unsigned char i;
   
    for(i=0;i<8;i++) {
        if(value & 0x01) {
            IOSET0 |= (1<<(i+24));
        } else {
            IOCLR0 |= (1<<(i+24));
        }
        value = value >> 1;
    }
}

void Lcd_Cmd(unsigned char value) {
    /*Configure LCD for receiving Command Data*/
    IOCLR0 |= (1<<RS);
    IOCLR0 |= (1<<RW);
    IOSET0 |= (1<<EN);
    Lcd_Data_Shift(value);
    Enable_Pulse();
}

void Lcd_Write(unsigned char value) {
    /*Configure LCD for receiving Display Data*/
    IOSET0 |= (1<<RS);
    IOCLR0 |= (1<<RW);
    IOSET0 |= (1<<EN);
    Lcd_Data_Shift(value);
    Enable_Pulse();
}

void Lcd_Write_Text(unsigned char msg[]) {
    while(*msg)
    {
        Lcd_Write(*msg);
        msg++;
    }
}
/*********************************************/

char key(void){
	while(1){
		IOCLR0 |=  ((1<<14)|(1<<15)|(1<<16));   //Making row1 LOW     
		IOSET0 |=  (1<<13);  //Making rest of the rows '1'
		if(( (IO0PIN & (1<<10))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<10))));
			return '1';
		}
		if(( (IO0PIN & (1<<11))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<11))));
			return '2';
		}
		if(( (IO0PIN & (1<<12))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<12))));
			return '3';
		}
			

		IOCLR0|=(1<<13)|(1<<15)|(1<<16);          
		IOSET0|=(1<<14);   
		if(( (IO0PIN & (1<<10))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<10))));
			return '4';
		}
		if(( (IO0PIN & (1<<11))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<11))));
			return '5';
		}
		if(( (IO0PIN & (1<<12))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<12))));
			return '6';
		}
			 
		IOCLR0|=(1<<14)|(1<<13)|(1<<16);    
		IOSET0|=(1<<15);
		if(( (IO0PIN & (1<<10))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<10))));
			return '7';
		}
		if(( (IO0PIN & (1<<11))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<11))));
			return '8';
		}
		if(( (IO0PIN & (1<<12))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<12))));
			return '9';
		}

		IOCLR0|=(1<<14)|(1<<15)|(1<<13);  //Making row1 LOW
		IOSET0|=(1<<16);
		if(( (IO0PIN & (1<<10))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<10))));
			return '*';
		}
		if(( (IO0PIN & (1<<11))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<11))));
			return '0';
		}
		if(( (IO0PIN & (1<<12))))//CHECK FIRST ROW - R0
		{
			while(( (IO0PIN & (1<<12))));
			return '#';
		}
	}
}

/*

Project notes

0x10000000 : 0x08



*/


