/*
 * File:   adc_pwm.c
 * Author: at
 *
 * Created on April 8, 2018, 4:03 PM
 */

#include "config.h"
#include <xc.h>


void SetupClock(void);
void PWM1_Init(unsigned char period);
void PWM1_setDC(unsigned int dutycycle);

void Setup_ADC(void); 

void interrupt CheckSwitchPress();

void main(void) {
    
    
    
    SetupClock();
    
    /*Port configurations */
    
    /* INPUTS */
     //Make port B pin 0 as INput 
    TRISBbits.RB0 = 1 ;
     
     
     /*OUTPUTS */
    
     //Make port B pin 1 as OUTput 
     TRISBbits.RB1 = 0 ; 
     
     //Make port B pin 2 as OUTput   
     TRISBbits.RB2 = 0 ; 
    
     /*END of Port configurations */
    
    
     
     /*Direction Set-up */
     
     /* Forward Direction Motion  */
      LATBbits.LATB1 = 1;
       LATBbits.LATB2 = 0;  
      //END  
       
  
    /* PWM Initialization */
     
    //create a period 
    PWM1_Init(0x7C);     //1KHZ  period 
    
    //set up initial Duty cycle 
    PWM1_setDC(512);

    /*END PWM Initialization */
    

    
    /* ADC Initialization*/
            
    // ADC  set-up 
    unsigned int  pot_val ; 
    Setup_ADC() ; 
    
    /* END of ADC initialization */
    
    
    
    
    /* External-INTERRUPT Initialization */   
       
    INTCONbits.INT0IE = 1 ;  // ENABLE interrupt 0 ( RB0)
     
     INTCON2bits.INTEDG0 = 0 ; // Falling EDGE interrupt 
     INTCONbits.INT0IF = 0 ; //clear flag 
     INTCONbits.GIE=1;		/* Enable Global Interrupt*/
     ei();   // Enable Global interrupts 
     
       
    /* End OF interrupt Initialization*/
    
     
    
    while (1){
       
    //Delay for acquisition time
     __delay_us(5);
    
    //start conversion 
    ADCON0bits.GO = 1;  //set GO/DONE bit 
    
    //wait until conversion done
    while(ADCON0bits.GO==1){};  // wait until GO/DONE bit is cleared
    
    
    //Read from  the register the value 
    pot_val = ADRES;
    
    // change Duty Cycle 
    PWM1_setDC(pot_val);
    
    NOP();
   }
    
    
    
    return;
}



void Setup_ADC(){
    
    TRISA0 = 1  ;  // set RA0 as input  
    
    //msb and msb-1 don't care
    //Vss as Ground , VDD as voltage reference 5V , AN0 only analog channel 
    ADCON1 = 0b00001110; 
    
    //msb-1 don't care
    //right justified, 4 Tad , 1/8 FOSC 
    ADCON2 = 0b10010001;
    
    //Active only CHAN0  , Turn on A/D converter 
   ADCON0 = ADCON0 | 1 ; 
    
    
}

void SetupClock()
{
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
}


void PWM1_Init(unsigned char period){
    //TRISC &=0xFD;           //  Set RC2 as output
    
    TRISCbits.RC2=0;  //make Port2 as an Output
    /* CCP PWM mode */
    CCP1CON &= 0xCF;        //  5,4 bits zeroed (DC1B1:DC1B0 = 00)
    CCP1CON |= 0x0C;        //  PWM mode ( CCP1M3:CCP1M0 = 1100)
    
    /* Timer2 configuration */
    //TMR2 = 0x0F              //Timer 2 inital value , if needed 
            
    PR2 = period;           //  configure timer2 period
    T2CON = 0x02;           //  Set prescalar 16   
    TMR2ON = 1;             //  timer2 on
    
}


void PWM1_setDC(unsigned int dutycycle){
    
    CCPR1L = dutycycle>>2;  //  PWM duty cycle - first 8-bits (MSb)
    CCP1CON &= 0xCF;        //  5,4 bits zeroed (DC1B1:DC1B0 = 00)
    CCP1CON |= ((dutycycle<<4)&0x30);  //  PWM duty cycle - last 2-bits (LSb) in CCP1CON 5,4 bits    
}


void interrupt CheckSwitchPress(){
    if (  INTCONbits.INT0IF == 1)  {
      //Service the interrupt
       
        /* Change direction of Wheel */
       LATBbits.LATB1 = ~LATBbits.LATB1  ;
       LATBbits.LATB2 = ~LATBbits.LATB2  ;
       /* Finished */  
         
      /* clear Flag */
      INTCONbits.INT0IF = 0 ;// clear interrupt flag
      
  }
  
} 