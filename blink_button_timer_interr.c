/* ************************************************************************** */
/** Descriptive File Name
 @Date
    16-April-2018
  
  @Programmer
    Andrés Alfaro
    Adrián Blázquez  
   
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
#define SYS_FREQ (8000000L)
#define NUM_REPETITIONS 5

/* VARIABLES */
volatile int timeout = 0;

/* FUNCTIONS */
/* @Description: Interrupt Routine Service
 */
void  __ISR(_EXTERNAL_0_VECTOR, IPL1AUTO) button_ISR(void) {
    if (BUTTON_PUSHED) {
        TIMER1_INTERR_EN(); // INT Step 5   
        timeout = (2*NUM_REPETITIONS) -1 ;
        BUTTON_IF_CLEAR(); // Reset interruption
    } 
}

void __ISR(_TIMER_1_VECTOR, IPL5AUTO) timer1_ISR (void) {
    IFS0bits.T1IF = 0;
    LED2_TOGGLE();
    if (timeout){
        timeout--;
    }
}


void led_setup() {    
    LED2_INIT(); // Configure PORT registers' for LED2
}

void button_setup() {
    // Configure PORT registers' for LED2
    BUTTON_INIT();
    
     // Interruption setup
    BUTTON_INTERR_DIS(); // disable this interrupt
    BUTTON_EDGE_RISING(); // edge polarity for this interrupt     
    BUTTON_INTERR_PRIOR(1); // interrupt priority
    BUTTON_INTERR_SUBPRIOR(1); // interrupt sub priority
    BUTTON_INTERR_EN(); // enable this interrupt
}

void timer1_setup () {    
    // INT step 2
    TIMER1_PRESCALE_1_256(); // Prescaler 256
    PR1 = 21874; // 
    TIMER1_SOURCE_INTERNAL(); // Internal peripheral clock
    TIMER1_ENABLE();  
    TIMER1_INTERR_PRIOR(5); // INT Step 3
    TIMER1_INTERR_SUBPRIOR(0); 
    TIMER1_RESET(); // INT Step 4
    //TIMER1_INTERR_EN(); // INT Step 5   
}

int main() {
    // INT step 1
    GLOBAL_INTERR_DIS(); // Disable global interrupt
 
    //delay_ms_setup();
    timer1_setup();
    led_setup();
    button_setup();

    INTERR_MVEC_EN(); //Interrupt Controller configured for multi vectored mode
    // INT Step 7
    GLOBAL_INTERR_EN(); // Enable global interrupt
    
    
    while (1) {
        if (!timeout){
            TIMER1_INTERR_DIS(); // INT Step 6 
        }
        
    }
    return 0;
}
