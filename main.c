/*
 * File:   main.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 15 de junio de 2019, 20:05
 */

#include <xc.h>
#include <stdio.h>
#include"conf.h"
#include"funciones.h"

#define MPU_WHOAMI 0x75
#define MPU_PWR1 0x6B

void putch (char Data){
    USART_Send(Data);
}


void main(void) {
    
    //Clock configuration
    //OSCCON = 0b00000000;
    char data[6];
    char registro;
    unsigned int i;
    char j;
    int X,Y,Z;
    USART_Init();
    
    //USART_Send('h');
    //    TRISB0 = 0;
    //RB0 = 1;
    //RB0 = 0;
    //for(i=0; i<200; i++)
    I2C_Init();
    ;
    //    ;
    //RB0 = 1;
    //TRISB0 = 1;
    for(i=0; i<200; i++);
    
    //    I2C_Start();
    //    I2C_Send(READMPUADDR);
    //    data = I2C_Read();
    //    Escritura(0x6B,0x00);
    //Escritura (MPU_PWR1, 0x00);
    
    while(1)
    {
        //registro=USART_Receive();
        //data = Lectura(registro);
        Escritura (MPU_PWR1, 0x00);
        Rafaga(data);
        X=Concatenar(data[0],data[1]);
        Y=Concatenar(data[2],data[3]);
        Z=Concatenar(data[4],data[5]);

        //USART_Send('S');
//        for (j=0; j<6; j++)
//        {
//            USART_Send(data[j]);
//            
//        }
//        USART_Send('\n');
        
        
        printf("X:%d,Y:%d,Z:%d\n", X, Y, Z);
        
        
        for(i=0; i<65500; i++){
        }

    }
    
    return;
}
