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
#define THRESHOLD 300

int acel[3],acelold[3],acelfilt[3],acelreal[3];
int acelold2[3], acelold3[3];
signed long acelg;
int acelgint, acelgintold;
char j;

int steps = 0, samples =0;
char status=0;


void putch (char Data){
    USART_Send(Data);
}


void main(void) {
    
    //Clock configuration
    //OSCCON = 0b00000000;
    
    //    char j;
    //    
    //    int acel[3],acelold[3],acelfilt[3],acelreal[3];
    //    int acelold2[3], acelold3[3];
    //    signed long acelg;
    USART_Init();
    
    
    I2C_Init();
    Timer_Init();
    
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
    
    while(1);
    
    
    
    return;
}

void __interrupt () Timer0_ISR(void){
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) 
        
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
            acelg=acelg + ((signed long)acelreal[j]*(signed long)acelfilt[j]); //32Bit cuadratic acceleration
        } 
        acelgint=acelgintold + (((acelg >> 9) - acelgintold) >> 2); //Vertical acceleration softener
        //SAMPLING
        Escritura (MPU_PWR1, 0x00);
        Rafaga(acel);
        
        samples ++;
        //Step count
        
        //Wait stabilization
        if (samples>300) status = 1; //Reposo
        
        switch (status)
        {
            case 1: 
                if (acelgint >THRESHOLD)
                {
                    status = 2; //UP  
                    samples=0;
                }
                else if (acelgint < (-(THRESHOLD)))
                {
                    status = 3; //DOWN
                    samples = 0;
                }
                else samples=0;
                break;
            case 2:
                if (samples >100)
                {
                    if (acelgint > THRESHOLD){
                        samples=0;
                        status=0; //Unstabilizied
                    } else status = 1;
                } else if (acelgint < (-(THRESHOLD)))
                {
                    steps++;
                    samples=0;
                    status=3;
                }
                break;
            case 3:
                if (samples >100)
                {
                    if (acelgint < THRESHOLD){
                        samples=0;
                        status=0; //Unstabilizied
                    } else status = 1;
                } else if (acelgint > THRESHOLD)
                {
                    steps++;
                    samples=0;
                    status=2;
                }
                break;
            default: //status = 0: Unstabilized
                break;
                
        }
        
        USART_Send((char) steps);
        
        TMR0L = 63661;
        TMR0H = (63661) >> 8;
        INTCONbits.TMR0IF = 0;
    }
}
