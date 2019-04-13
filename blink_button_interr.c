/* ************************************************************************** */
/** Descriptive File Name
 @Date
    09-April-2018
  
  @Programmer
    Josué Pagán  
   
  @Company
    Universidad Politécnica de Madrid

  @File Name
    blink_2.c

  @Summary
    Pulse a button and blink a LED 3 times for 0.3 seconds
/* ************************************************************************** */

/* INLCUDES */
#include <xc.h>
#include <sys/attribs.h>
#include "myMacros_pic32mx440f256h.h"
#include "pinguinoConf.h"

/* DEFINES */
#define _SUPRESS_PLIB_WARNING 1 //evitar warning de plib

/* VARIABLES */
unsigned char state = 0;

/* FUNCTIONS */
/* @Description: Interrupt Routine Service
 */
void  __ISR(_EXTERNAL_0_VECTOR, IPL1AUTO) button_ISR(void) {
    if (BUTTON_PUSHED) {
        state = 1;
        BUTTON_IF_CLEAR(); // Reset interruption
    } 
}

/*
 * delay_ms_setup()
 *  * @info: For simulation, divide and truncate PR1 by 1000
 */
void delay_ms_setup() {
    //Configure Timer1 for 1us count with 8 MHz clock for simulator
    //Configure Timer1 for 1ms count with 8 MHz clock for loading

    // T1CON to control multiplexers of the TIMER1 BLOCK DIAGRAM
    TIMER1_RESET(); // Reset timer
    TIMER1_DISABLE();     // TIMER1 OFF    
    TIMER1_SOURCE_INTERNAL(); // Configure clock as PBCLK (Peripheral Bus Clock) 
    TIMER1_PRESCALE_1_1(); // Prescaler 1:1
    
    PR1 = 79; // 32 bits register (16 available) to compare with TMR1      
}

void delay_ms(int ms) {
    //Use Timer1 interrupt flag
    TMR1 = 0;
    TIMER1_ENABLE();

    while (ms--) {
        TIMER1_RESET();
        while (!TIMER1_IF) {
            _nop();
        }
    }
    TIMER1_DISABLE();
}

void led_setup() {    
    LED2_INIT(); // Configure PORT registers for LED2
}

void button_setup() {
    // Configure PORT registers for BUT (user button in the board)
    BUTTON_INIT();
    
     // Interruption setup
    BUTTON_INTERR_DIS(); // disable this interrupt
    BUTTON_EDGE_RISING(); // edge polarity for this interrupt     
    BUTTON_INTERR_PRIOR(1); // interrupt priority
    BUTTON_INTERR_SUBPRIOR(1); // interrupt sub priority
    BUTTON_INTERR_EN(); // enable this interrupt
}

int main() {
    int i, r = 3;
    int msOn = 300, msOff = 700;
    
    delay_ms_setup();
    led_setup();
    button_setup();

    INTERR_MVEC_EN(); //Interrupt Controller configured for multi vectored mode
    GLOBAL_INTERR_EN(); // Enable global interrupt
    
    while (1) {
        if (state){
            for (i=0; i<r; i++){
                LED2_ON(); 
                delay_ms(msOn);
                LED2_OFF();
                delay_ms(msOff);                
            }
            state = 0;
        }
    }
    return 0;
}