/*
 * File:   funciones.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 25 de junio de 2019, 19:10
 */


#include <xc.h>
#define MPU_READ 0b11010001
#define MPU_WRITE 0b11010000


void I2C_Init(void)
{
    
    INTCON2bits.nRBPU = 1;
    ADCON1|=0X0F;           //Importante: Configurar el pin como IO DIGITAL.
    TRISB0=1;               //SDA and SDC pins configured as inputs
    TRISB1=1;
    
    //SSPSTAT &= 0x3F; 	//Default
    SSPCON1=0x00;       //Def
    SSPCON2=0x00;
    SSPCON1 |= 0x08;           // Enable SSP port for I2C Master mode,  clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPSTAT |= 0x80;                // slew rate off 
    
    SSPADD=119;     /* Clock 100 kHz: SSDADD=119 (Fosc=48MHz) */  
    SSPCON1 |= 0x20;        //Enable I2C AFTER configurin pins
    
    
}

void I2C_Wait(void)
{
    while((SSPSTAT & 0X04)||(SSPCON2 & 0X1F));
}
void I2C_Start(void)
{
    
    SSPCON2bits.SEN=1;		/* Send start pulse */
    while(SSPCON2bits.SEN);	/* Wait for completion of start pulse */
    //    SSPIF=0;
    //    while(!SSPSTATbits.S);
    
}

void I2C_Stop(void)
{
    SSPCON2bits.PEN = 1;            // initiate bus stop condition
    while(SSPCON2bits.PEN);                              //wait untill SSPIF flag is set
    //    SSPIF=0;
    //    SSPEN=0;                                    //Disable I2C operation
}
unsigned char I2C_Read(void)
{
    
    SSPCON2bits.RCEN = 1;           // enable master for 1 byte reception
    while( !SSPSTATbits.BF);          //Wait while trasmit in progress
    I2C_Wait();
    return ( SSPBUF );              // return with read byte 
    
}

void I2C_Send(unsigned char data)
{
    SSPBUF=data;	/* Write data to SSPBUF*/
    //if ( SSPCON1bits.WCOL )      // test if write collision occurred
    //    return ( -1 );              // if WCOL bit is set return negative #
    //else
    //{
    while( SSPSTATbits.BF || SSPSTATbits.R_nW );   // wait until write cycle is complete   
    I2C_Wait();
    while( SSPCON2bits.ACKSTAT ); // test for ACK condition received
    //    return ( -2 );			// return NACK
    //else return ( 0 );              //return ACK
    //}
}

void I2C_Ack(void)
{
    ACKDT=0;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
    while(ACKEN);
}

void I2C_Nack(void)
{
    ACKDT=1;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
    while(ACKEN);
}

void I2C_Restart( void )
{
    SSPCON2bits.RSEN = 1;           // initiate bus restart condition
    while(SSPCON2bits.RSEN);          //Wait end of restart signal
}

void USART_Init(void)
{
//    TRISD1=0;
//    RD1=0;
    
    TRISC6=0;		/* Make Tx pin as output*/
    TRISC7=1;  		/* Make Rx pin as input*/
    
    
    SPBRG = 77;	/* Baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/   
    
    TXSTA = 0x20;  	/* Enable Transmit(TX) */ 
    RCSTA = 0x90;  	/* Enable Receive(RX) & Serial */
}

void USART_Send (char out)
{
    while (TXIF == 0);	/* Wait for transmit interrupt flag*/
    TXREG = out;  	/* Write char data to transmit register */    
}

char USART_Receive()
{
    while(RCIF==0);      /*wait for receive interrupt flag*/
    if(RCSTAbits.OERR)
    {           
        CREN = 0;
        NOP();
        CREN=1;
    }
    return(RCREG);       /*received in RCREG register and return to main program */
}

char Lectura(char registro)
{
    char data;
    I2C_Start();
    I2C_Send(MPU_WRITE); //Address+Write bit
    I2C_Send(registro); //Register
    I2C_Restart();
    I2C_Send(MPU_READ); //Address+Read bit
    data = I2C_Read();  //Read
    I2C_Nack();
    I2C_Stop();
    return data;
}

void Escritura (char registro, char valor)
{
    I2C_Start();
    I2C_Send(MPU_WRITE); //Address+Write bit
    I2C_Send(registro); //Register
    I2C_Send(valor);
    I2C_Stop();
}

int Concatenar (char MSB, char LSB)
{
    int aceleracion;
    
//Optimized concatenation in Little-Endian system:
 ((unsigned char*) &aceleracion)[0] = LSB; //Start from the "little end"
 ((unsigned char*) &aceleracion)[1] = MSB;    

 return aceleracion;
}

void Rafaga (char *data) //Data[6]
{

    char i;
    I2C_Start();
    I2C_Send(MPU_WRITE); //Address+Write bit
    I2C_Send(0x3B); //First regirster: ACCELX_MSB
    I2C_Restart();
    I2C_Send(MPU_READ); //Address+Read bit
    
    for (i=0; i<5; i++) //5 first datas
    {
        data[i] = I2C_Read();
        I2C_Ack();
                
    } //i=5   
    data[i] = I2C_Read();  //Read lasst data followed by NACK
    I2C_Nack();
    I2C_Stop();
    
}