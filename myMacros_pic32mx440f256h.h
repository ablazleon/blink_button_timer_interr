/* ************************************************************************** */
/** Descriptive File Name
 @Date
    09-April-2018
  
  @Programmer
    Josué Pagán  
 
  @Company
    Universidad Politécnica de Madrid
 */
/* ************************************************************************** */

#ifndef MY_MACROS_PIC32MX440F256H_H    /* Guard against multiple inclusion */
#define MY_MACROS_PIC32MX440F256H_H 

/* DEFINES-MACROS*/
/* TIMERS */
#define TIMER1_ENABLE() T1CONSET = _T1CON_ON_MASK
#define TIMER1_DISABLE() T1CONCLR = _T1CON_ON_MASK
#define TIMER1_RESET() IFS0CLR = _IFS0_T1IF_MASK
#define TIMER1_IF IFS0bits.T1IF
#define TIMER1_SOURCE_INTERNAL() T1CONbits.TCS = 0 //TCS: Timer Clock Source Select bit
#define TIMER1_PRESCALE_1_1() T1CONbits.TCKPS = 0 // Timer Input Clock Prescale Select bits
#define TIMER1_INTERR_EN() IEC0SET = _IEC0_T1IE_MASK
#define TIMER1_INTERR_PRIOR(x) IPC1bits.T1IP = x
#define TIMER1_INTERR_SUBPRIOR(x) IPC1bits.T1IS = x

/* INTERRUPTIONS */
#define GLOBAL_INTERR_EN()  __builtin_mtc0(12,0,(__builtin_mfc0(12,0) | 0x0001))  // Global interrupt enable
#define GLOBAL_INTERR_DIS()  __builtin_mtc0(12,0,(__builtin_mfc0(12,0) & 0xFFFE))  // Global interrupt disable
#define INTERR_MVEC_EN() INTCONbits.MVEC = 1

#endif /* _PINGUINO_CONF_H */


