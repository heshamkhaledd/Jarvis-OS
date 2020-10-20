/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the timer driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"

/* Timers define Enable */
#define SysTick_EN      1
#define TIMER_32_16_EN  0


/*******************************************************************************
 *                   Timers Registers and Ports Definitions
 ******************************************************************************/
#define SysTick      0xE000E000
#define TIMER0       0x40030000
#define TIMER1       0x40031000
#define TIMER2       0x40032000
#define TIMER3       0x40033000
#define TIMER4       0x40034000
#define TIMER5       0x40035000


                      /*******************************************************************************
                      *                                                                              *
                      *                           SysTick Count Down Timer                           *
                      *                                                                              *
                      ********************************************************************************/


                                                  /* Registers offset */

/* CTCTRL: SysTick Control Register.
 *
 * Bit[0]  ->        0 := Counter Disabled
 *                   1 := Enable SysTick Count Down
 *
 * Bit[1]  ->        0 := Interrupt Generation is Disabled
 *                   1 := Interrupt Generation is Enabled when Counter reaches 0
 *
 * Bit[2]  ->        0 := Internal Oscillator/4 Clock
 *                   1 := System Clock
 *
 * Bit[16] ->        Count down Flag, Cleared by either reading STCTRL or Writing to CTCURRENT
 */
#define STCTRL      0x010


/* STRELOAD: Stores the start value of SysTick down counter
 * Ranges from 0x000000 ~ 0xFFFFFF (Only the first 24-bits are used)
*/
#define STRELOAD    0x014

/* STCURRENT: The actual register that's counted down */
#define STCURRENT   0x018






/*******************************************************************************
 *                  Configuration Structures and Enumerators
 ******************************************************************************/


                               /* Enumerators */

/******************************************************************************
 * Name: INTERRUPT_STATE
 * Description: Enumerator to hold the interrupt configuration of SysTick
 * 0 := INTERRUPT_ON
 * 1 := INTERRUPT_OFF
 *****************************************************************************/
typedef enum{
    INT_ON, INT_OFF
}INTERRUPT_STATE;


/******************************************************************************
 * Name: CLK_SOURCE
 * Description: Enumerator to hold the clock configuration of SysTick
 * 0 := OSCILLATOR
 * 1 := Sys_CLOCK
 *****************************************************************************/
typedef enum{
    OSCILLATOR, Sys_CLOCK
}CLK_SOURCE;


                                /* Structs */

typedef struct{

    INTERRUPT_STATE state;
    CLK_SOURCE      source;
    uint32_t        initial_value;
}SysTick_CONFIG;

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/
void SysTick_init (SysTick_CONFIG const *CONFIG_Ptr);
void SysTick_fun (void);




                 /*******************************************************************************
                 *                                                                              *
                 *                                16/32 Bit Timers                              *
                 *                                                                              *
                 ********************************************************************************/




/*******************************************************************************
 *                   Timers Registers and Ports Definitions
 ******************************************************************************/
#define TIMER0      0x40030000
#define TIMER1      0x40031000
#define TIMER2      0x40032000
#define TIMER3      0x40033000
#define TIMER4      0x40034000
#define TIMER5      0x40035000


                                                /* Registers Offset */



/* RCGCTIMER: Responsible for Enabling clock signal for each timer
 * 0 := Clock Disabled
 * 1 := Clock Enabled
 */
#define RCGCTIMER    (*((volatile uint32_t*)(0x400FE000+0x604)))

/* GPTMCFG: Configuration Register for Timer0
 * 000 := 32-Bit Mode
 * 001 := RTC Counter
 * 100 := 16-Bit Mode
 */
#define GPTMCFG         0x000


/* GPTMTAMR/GPTMTBMR: Mode select register.
 * Bit[0]~[1]  ->       00 := Reserved              10 := Periodic Mode
 *                      01 := One-Shot Mode         11 := Capture Mode
 *
 * Bit[2]      ->       0 := Edge-Count Mode
 *                      1 := Edge-Time Mode
 *
 * Bit[3]      ->       0 := Capture/Compare Mode
 *                      1 := PWM Mode
 *
 * Bit[4]      ->       0 := Count Down
 *                      1 := Count Up
 *
 * Bit[5]      ->       0 := Interrupt Enabled
 *                      1 := Interrupt Disabled
 *
 * Bit[6]      ->       0 := Start when Enabled
 *                      1 := Start on Trigger
 */
#define GPTMTAMR        0x004
#define GPTMTBMR        0x008


/* GPTMCTL: It's needed to disable the Timer A/B before configuring it
 * by clearing bit[0/8] in this register.
 */
#define GPTMCTL         0x00C


/* GPTMIMR: Interrupt Control Register */
#define GPTMIMR         0x018

/* GPTMICR: Interrupt Clear Register */
#define GPTMICR         0x024

/* GPTMTAILR/GPTMTBILR: Timer A/B Interval Load Register */
#define GPTMTAILR       0x028
#define GPTMTBILR       0x02C


/* GPTMTAMATCHR/GPTMTBMATCHR: Timer A/B Match Compare Value */
#define GPTMTAMATCHR    0x030
#define GPTMTBMATCHR    0x034

/* GPTMTAPR/GPTMTBPR: Timer Prescale Register
 * Bit[0]~Bit[7] -> Timer A/B Prescale
 */ 
#define GPTMTAPR        0x038
#define GPTMTBPR        0x03C

/* GPTMTAPMR/GPTMTBPMR: Timer A/B Prescale Match */
#define GPTMTAPMR       0x040
#define GPTMTBPMR       0x044


/*******************************************************************************
 *                  Configuration Structures and Enumerators
 ******************************************************************************/


                            /* Enumerators */ 

typedef enum{
    MODE_32, MODE_RTC, MODE_16
}TIMER_BIT_MODE;

typedef enum{
    ONE_SHOT,PERIODIC,CAPTURE
}TIMER_MODE_2;

typedef enum{
    OVF,COMPARE
}TIMER_MODE_3;

typedef enum{
    EDGE_COUNT,EDGE_TIME,PWM
}MODE_CONFIG;

typedef enum{
    COUNT_UP,COUNT_DOWN
}COUNT_CONFIG;

typedef enum{
    ON_ENABLE, ON_TRIGGER
}START_STATE;

                            /* Structs */

typedef struct{

    uint32_t        timer;
    uint8_t         channel;
    TIMER_BIT_MODE  bit_mode;
    TIMER_MODE_2    operation_mode;
    TIMER_MODE_3    operation_mode_2;
    MODE_CONFIG     capture_mode;
    COUNT_CONFIG    count_mode;
    INTERRUPT_STATE interrupt_state;
    START_STATE     start_mode;
    uint32_t        compare_value;
    uint32_t        prescaler;
}TIMER_32_16_CONFIG;


void TIMER_32_16_init (TIMER_32_16_CONFIG const *CONFIG_Ptr);
void TIMER_32_16_initClock (uint32_t timer);
void TIMER_32_16_initInterrupt (TIMER_32_16_CONFIG const *CONFIG_Ptr);

#endif /* TIMER_H_ */
