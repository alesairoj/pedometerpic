/*
 * File:   main.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 15 de junio de 2019, 20:05
 */

#include <xc.h>
#include"conf.h"
#include"funciones.h"

#define MPU_READ 0b11010001
#define MPU_WRITE 0b11010000
#define MPU_WHOAMI 0x75
char debug;

void main(void) {
    
    //Clock configuration
    OSCCON = 0b00000000;
    char data = 0;
    unsigned int i;
       
    I2C_Init();
    for(i=0; i<1000; i++);
    
//    I2C_Start();
//    I2C_Send(READMPUADDR);
//    data = I2C_Read();
    
        while(1)
    {
        I2C_Start();
        I2C_Send(MPU_WRITE); //Addrex+Write bit
        I2C_Send(0x3B); //Register
        I2C_Restart();
        I2C_Send(MPU_READ); //Addrex+Read bit
        data = I2C_Read();  //Read
        I2C_Nack();
        I2C_Stop();
        debug=data;
            for(i=0; i<100; i++);
            
}

    return;
}
