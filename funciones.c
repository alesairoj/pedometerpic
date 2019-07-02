/*
 * File:   funciones.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 25 de junio de 2019, 19:10
 */


#include <xc.h>

void I2C_Init(void)
{
    SSPSTAT &= 0x3F; 	//Default
    SSPCON1=0x00;       //Def
    SSPCON2=0x00;
    SSPCON1 |= 0x08;           // Enable SSP port for I2C Master mode,  clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPSTAT |= 0x80;                // slew rate off 
    
    TRISB0=1;               //SDA and SDC pins configured as inputs
    TRISB1=1;
    
    SSPCON1 |= 0x20;        //Enable I2C AFTER configurin pins
    
    SSPADD=119;     /* Clock 100 kHz: SSDADD=119 (Fosc=48MHz) */  
    
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
    //    while(!SSPIF);                              //wait untill SSPIF flag is set
    //    SSPIF=0;
    //    SSPEN=0;                                    //Disable I2C operation
}
unsigned char I2C_Read(void)
{
            I2C_Wait();
  SSPCON2bits.RCEN = 1;           // enable master for 1 byte reception
  while( SSPSTATbits.BF || SSPSTATbits.R_nW );          //Wait while trasmit in progress
  return ( SSPBUF );              // return with read byte 
   
}

unsigned char I2C_Send(unsigned char data)
{
    SSPBUF=data;	/* Write data to SSPBUF*/
    if ( SSPCON1bits.WCOL )      // test if write collision occurred
        return ( -1 );              // if WCOL bit is set return negative #
    else
    {
        while( SSPSTATbits.BF || SSPSTATbits.R_nW );   // wait until write cycle is complete   
        I2C_Wait();
        while( SSPCON2bits.ACKSTAT ); // test for ACK condition received
        //    return ( -2 );			// return NACK
		//else return ( 0 );              //return ACK
    }
}

void I2C_Ack(void)
{
    ACKDT=0;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
}

void I2C_Nack(void)
{
    ACKDT=1;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
}

void I2C_Restart( void )
{
  SSPCON2bits.RSEN = 1;           // initiate bus restart condition
  while(SSPCON2bits.RSEN);          //Wait end of restart signal
}

