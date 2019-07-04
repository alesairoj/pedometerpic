/* 
 * File:   funciones.h
 * Author: sith
 *
 * Created on 25 de junio de 2019, 20:35
 */

#ifndef FUNCIONES_H
#define	FUNCIONES_H

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif

void I2C_Wait(void);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Restart( void );
void I2C_Stop(void);
unsigned char I2C_Read(void);
void I2C_Send(unsigned char data);
void I2C_Ack();
void I2C_Nack(void);

void USART_Init(void);
void USART_Send (char out);
char USART_Receive();

char Lectura(char registro);



#endif	/* FUNCIONES_H */


//
//void i2c_master_start(void);
//
//void i2c_master_repeated_start(void);
//void i2c_master_sent(unsigned b);
//
//void i2c_master_stop(void);
