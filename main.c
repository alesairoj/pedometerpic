/*
 * File:   main.c
 * Author: Alejandro Sainz Rojas
 *
 * Created on 15 de junio de 2019, 20:05
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>
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
    
    char j;
    
    int acel[3],acelold[3],acelfilt[3],acelreal[3];
    int acelold2[3], acelold3[3];
    signed long acelg;
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
    for(j=0; j<200; j++);
    
    //Inicio
    Escritura (MPU_PWR1, 0x00);
    Rafaga(acel);

    for (j=0; j<3; j++) //Filter initialitation for faster convergence
    {
        acelold[j]=acel[j];
        acelold2[j]=acelold[j];
        acelold3[j]=acelold[j];
    }
    
    
    
    while(1)
    {
        

        acelg=0;
        for (j=0; j<3;j++)
        {
        //Filtering: 3rd Order FIR without discarding samples works fine
        //Cascade implementation for avoiding custom coeficients therefore avoiding division operations
            acelfilt[j]=acelold[j] + ((acel[j] - acelold[j]) >> 4);
            acelold[j]=acelfilt[j];
            acelfilt[j]=acelold2[j] + ((acelfilt[j] - acelold2[j]) >> 4);
            acelold2[j]=acelfilt[j];
            acelfilt[j]=acelold3[j] + ((acelfilt[j] - acelold3[j]) >> 3);
            acelold3[j]=acelfilt[j];
            
        acelreal[j]=acelreal[j]+ ((acelold[j]-acelfilt[j]-acelreal[j])>>4);
        acelg=acelg + ((signed long)acelreal[j]*(signed long)acelfilt[j]);
        } 
        
        //SAMPLING
        Escritura (MPU_PWR1, 0x00);
        Rafaga(acel);

                 printf("%d,%d,%d,%ld\n",acelreal[0], acelreal[1], acelreal[2], acelg);
//        printf("%d\n",);

    }
    
    return;
}