
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
/*********************************************/

/*********************************************/
/**************Function Prototypes************/

void Lcd_Init(void);
void Lcd_Cmd(unsigned char value);
void Lcd_Write(unsigned char value);
void Lcd_Write_Text(unsigned char msg[]);
void Lcd_Data_Shift(unsigned char value);
char c;
void Delay_ms(unsigned long times);
char key(void);
char dir[8];
char cmd[8];
char test[8];
int step;
void UART0_Init(void);
void UART0_Write(unsigned char value);
void UART0_Write_Text(int number);
void decToBin(int value);
unsigned char UART0_Read(void);
/*********************************************/

int main()
{
		
		char mensaje[] = "Mostrar lista";
		int a=0;
		
		dir[0]='1';
		dir[1]='0';
		dir[2]='0';
		dir[3]='0';
		dir[4]='0';
		dir[5]='0';
		dir[6]='0';
		dir[7]='0';
	
		cmd[0]='0';
		cmd[1]='0';
		cmd[2]='0';
		cmd[3]='0';
		cmd[4]='0';
	
		
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

			
 	while(1)
    {
      c=key();
			
			if(c=='#'){
				Lcd_Cmd(LCD_CLEAR);
				if (a>0){
				 test[a-1] = NULL;
					a--;
				}
				Lcd_Write_Text((unsigned char *)test);
			}else if(c=='*') {
				if(a==0){//comprobar si es vacia la cadena
					Lcd_Cmd(LCD_CLEAR);
					Lcd_Write_Text((unsigned char *)mensaje);
					//funcion -enviaPetionAtodos
					memset(test,NULL,a);
					a=0;
					Delay_ms(500);
					Lcd_Cmd(LCD_CLEAR);
				}
				else{
						//paso la dirección a binario y lo meto en la cadena
						int valor;
						if(a==1){
							valor = ((int)test[0])-48;
						}else if(a==2){
							valor = ((((int)test[0])-48)*10)+(((int)test[1])-48) ;
						}else{
						}	
						decToBin(valor);
						Lcd_Write(c);
						//espero a la orden
						c=key();
						//paso la orden a binario y lo meto en la cadena
						if(c=='0'){
						Lcd_Cmd(LCD_CLEAR);
						memset(test,NULL,a);
						a=0;
						}else if(c=='1'){
						
						cmd[5]='0';
						cmd[6]='0';
						cmd[7]='1';								
							
						test[a]=c;
						a++;
						Lcd_Write(c);	
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
						
						memset(test,NULL,a);
						a=0;
							
						}else if(c=='2'){
							
						cmd[5]='0';
						cmd[6]='1';
						cmd[7]='0';		
							
						test[a]=c;
						a++;
						Lcd_Write(c);		
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR); 
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
							
						memset(test,NULL,a);
						a=0;
							
						}else if(c=='3'){
						
						cmd[5]='0';
						cmd[6]='1';
						cmd[7]='1';								
							
						test[a]=c;
						a++;
						Lcd_Write(c);	
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
							
						memset(test,NULL,a);
						a=0;
						}else if(c=='4'){
							
						cmd[5]='1';
						cmd[6]='0';
						cmd[7]='1';		
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);	
							
						test[a]=c;
						a++;
						Lcd_Write(c);		
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);

						memset(test,NULL,a);
						a=0;
						}else if(c=='5'){
							
						cmd[5]='1';
						cmd[6]='1';
						cmd[7]='0';		
							
						test[a]=c;
						a++;
						Lcd_Write(c);		
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
							
						memset(test,NULL,a);
						a=0;
						}else if(c=='6'){
						
						cmd[5]='1';
						cmd[6]='1';
						cmd[7]='1';							
							
						test[a]=c;
						a++;
						Lcd_Write(c);			
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
							
						memset(test,NULL,a);
						a=0;
							
						}else if(c=='7'){
							
						cmd[5]='0';
						cmd[6]='0';
						cmd[7]='0';	
							
						test[a]=c;
						a++;
						Lcd_Write(c);		
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);
							
						memset(test,NULL,a);
						a=0;
							
						}else if(c=='8'){
							
						cmd[5]='1';
						cmd[6]='0';
						cmd[7]='0';
							
						test[a]=c;
						a++;
						Lcd_Write(c);	
						Delay_ms(100);
						Lcd_Cmd(LCD_CLEAR);
							
						UART0_Write_Text(2);
						UART0_Write_Text(1);	
							
						memset(test,NULL,a);
						a=0;
							
						}else{
							Lcd_Cmd(LCD_CLEAR);
							memset(test,NULL,a);
							a=0;
						}		
						
				}
			}else{
					test[a]=c;
					a++;
					Lcd_Write(c);
			}
			Delay_ms(100);
    }
}

/****************Function Definition**********/
void decToBin(int n)
{
	dir[2]='0';
	dir[3]='0';
	dir[4]='0';
	dir[5]='0';
	dir[6]='0';
	dir[7]='0';
	step=7;
		 do{
			 if((n%2) ==0){
				 dir[step]='0';
			 }else{
				 dir[step]='1';
			 }
			 n=(n/2);
			 step--;
		 } while(n>0);
	
	

}
/***************UART-0 Functions**************/
void UART0_Init(void)
{
    PINSEL0 = 0x00000005;                //P0.0 as TX0 and P0.1 as RX0
    U0LCR = 0x83;                                //Enable access to Divisor Latches
    U0DLM = 0;
    U0DLL = 25;    
		U0LCR = 3;
		U0FCR = 7;

}
void UART0_Write(unsigned char value)
{
    while(!(U0LSR&0x20));                        //THRE = 0 stay here
    U0THR = value;
}

void UART0_Write_Text(int a)
{
		int i;
    for(i=0; i<8 ;i++){	
				if(a==1){
					UART0_Write(cmd[i]);
				}else{
					UART0_Write(dir[i]);
				}
			}
}

unsigned char UART0_Read(void)
{
    while(!(U0LSR & 0x01));                        //RDR = 0 stay here
    return (U0RBR);
}
/****************Delay Function***************/
void Delay_ms(unsigned long times)
{
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

void Lcd_Data_Shift(unsigned char value)
{
    /*
    This Function will shift the eight bit data stored in variable value,
    to the Port Pin P0.8 to P0.15 Successfully.
    */
    unsigned char i;
   
    for(i=0;i<8;i++)
    {
        if(value & 0x01)
        {
            IOSET0 |= (1<<(i+24));
        }
        else
        {
            IOCLR0 |= (1<<(i+24));
        }
        value = value >> 1;
    }
}

void Lcd_Cmd(unsigned char value)
{
    /*Configure LCD for receiving Command Data*/
    IOCLR0 |= (1<<RS);
    IOCLR0 |= (1<<RW);
    IOSET0 |= (1<<EN);
    Lcd_Data_Shift(value);
    Enable_Pulse();
}

void Lcd_Write(unsigned char value)
{
    /*Configure LCD for receiving Display Data*/
    IOSET0 |= (1<<RS);
    IOCLR0 |= (1<<RW);
    IOSET0 |= (1<<EN);
    Lcd_Data_Shift(value);
    Enable_Pulse();
}

void Lcd_Write_Text(unsigned char msg[])
{
    while(*msg)
    {
        Lcd_Write(*msg);
        msg++;
    }
}

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


