/*
 * File:   main.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 15 de junio de 2019, 20:05
 */

#include <xc.h>
#include"conf.h"
#include"funciones.h"

#define MPU_WHOAMI 0x75

void main(void) {
    
    //Clock configuration
    //OSCCON = 0b00000000;
    char data = 0;
    unsigned int i;
    USART_Init();
    I2C_Init();
//    TRISB0 = 0;
//RB0 = 1;
//RB0 = 0;
for(i=0; i<200; i++)
//    ;
//RB0 = 1;
//TRISB0 = 1;
    //for(i=0; i<1000; i++);
    
//    I2C_Start();
//    I2C_Send(READMPUADDR);
//    data = I2C_Read();
    
        while(1)
    {

        data = Lectura(MPU_WHOAMI);
        
        USART_Send(data);
        
            for(i=0; i<1000; i++)
                ;
            
}

    return;
}
