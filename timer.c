/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the timer driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "gpio.h"
#include "timer.h"


#if (SysTick_EN == 1)

void SysTick_init (SysTick_CONFIG const *CONFIG_Ptr)
{

    /* Enable the Counter */
    SET_BIT (SysTick,STCTRL,PIN_0);

    /* Choose the Interrupt state when the count down is over */
    if (CONFIG_Ptr -> state == INT_ON)
        SET_BIT(SysTick,STCTRL,1);
    else if (CONFIG_Ptr -> state == INT_OFF)
        CLEAR_BIT(SysTick,STCTRL,1);

    /* Determine the Clock source for the SysTick count down */
    if (CONFIG_Ptr -> source == Sys_CLOCK)
        SET_BIT(SysTick,STCTRL,2);
    else if (CONFIG_Ptr -> source == OSCILLATOR)
        CLEAR_BIT(SysTick,STCTRL,2);

    /* Set the initial value for the counter */
    if ((CONFIG_Ptr -> initial_value) > 0xFFFFFF)
    {
        while (1) {

        }
    }

    ACCESS_REG(SysTick,STRELOAD) = CONFIG_Ptr -> initial_value;

    sei() ;

    return;
}

void SysTick_fun (void)
{

}

#endif


#if (TIMER_32_16_EN == 1)
void TIMER_32_16_init (TIMER_32_16_CONFIG const *CONFIG_Ptr)
{
    
    /* Initialize clock for the required timer */
    TIMER_32_16_initClock (CONFIG_Ptr -> timer);



    if (CONFIG_Ptr -> bit_mode == MODE_16 )
    {
        /* Disable TAEN / TBEN Before Tuning the timer configuartions */
        ACCESS_REG (CONFIG_Ptr -> timer,GPTMCTL) &= ~((1<<PIN_0) | (1<<PIN_8));

        /* GPTMCFG = 0b100 to configure it at 16 bit mode */
        ACCESS_REG(CONFIG_Ptr -> timer, GPTMCFG) = 0x4;

        /* 16 bit mode, Channel 'A' timer configuration  */
        if (CONFIG_Ptr -> channel == 'A')
        {
            /* One shot timer configuration */
            if (CONFIG_Ptr -> operation_mode == ONE_SHOT)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);

                /* If compare match mode is activated, assign the compare value */
                if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = CONFIG_Ptr -> compare_value;
            }

            /* Periodc timer configuration */
            else if (CONFIG_Ptr -> operation_mode == PERIODIC)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);

                /* If compare match mode is activated, assign the compare value */
                if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMATCHR) = CONFIG_Ptr -> compare_value;
            }

            /* Capture timer configuration */
            else if (CONFIG_Ptr -> operation_mode == CAPTURE)
            {
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMR) |= (1<<PIN_0) | (1<<PIN_1);
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_3);

                /* Edge count capture mode configuration */
                if (CONFIG_Ptr -> capture_mode == EDGE_COUNT)
                    CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);

                /* Edge time capture mode configuration */
                else if (CONFIG_Ptr -> capture_mode == EDGE_TIME)
                    SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
            }

            /* Count direction configuration */
            if (CONFIG_Ptr -> count_mode == COUNT_UP)
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);
            else if (CONFIG_Ptr -> count_mode == COUNT_DOWN)
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);

            /* Timer Interrupt Configuration */
            if (CONFIG_Ptr -> interrupt_state == INT_ON)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_5);
                TIMER_32_16_initInterrupt (CONFIG_Ptr);
            }

            /* Timer Start Configuration */
            if (CONFIG_Ptr -> start_mode == ON_ENABLE)
                CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);
            else if (CONFIG_Ptr -> start_mode == ON_TRIGGER)
                SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);

            /* Set Timer prescaler */
            ACCESS_REG(CONFIG_Ptr -> timer, GPTMTAPR) = CONFIG_Ptr -> prescaler;

            /* Re-Enable the Timer again to start counting */
            SET_BIT(CONFIG_Ptr -> timer,GPTMCTL,PIN_0);
            return;

        }

        /* 16 bit mode, Channel 'B' timer configuration  */
        else if (CONFIG_Ptr -> channel == 'B')
        {
            /* One shot timer configuration */
            if (CONFIG_Ptr -> operation_mode == ONE_SHOT)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_0);
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_1);

                /* If compare match mode is activated, assign the compare value */
                if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = CONFIG_Ptr -> compare_value;
            }

            /* Periodic timer configuration */
            else if (CONFIG_Ptr -> operation_mode == PERIODIC)
            {
                SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_1);
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_0);

                /* If compare match mode is activated, assign the compare value */
                if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
                    ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMATCHR) = CONFIG_Ptr -> compare_value;
            }

            /* Capture timer configuration */
            else if (CONFIG_Ptr -> operation_mode == CAPTURE)
            {
                ACCESS_REG(CONFIG_Ptr -> timer,GPTMTBMR) |= (1<<PIN_0) | (1<<PIN_1);
                SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_3);

                /* Edge count capture mode configuration */
                if (CONFIG_Ptr -> capture_mode == EDGE_COUNT)
                    CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_0);

                /* Edge time capture mode configuration */
                else if (CONFIG_Ptr -> capture_mode == EDGE_TIME)
                    SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_1);
            }

            /* Count direction configuration */
            if (CONFIG_Ptr -> count_mode == COUNT_UP)
                SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_4);
            else if (CONFIG_Ptr -> count_mode == COUNT_DOWN)
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_4);

            /* Timer Interrupt Configuration */
            if (CONFIG_Ptr -> interrupt_state == INT_ON)
            {
                /*SET_BIT(CONFIG_Ptr -> timer,GPTMTBMR,PIN_5);*/
                TIMER_32_16_initInterrupt (CONFIG_Ptr);
            }

            /* Timer Start Configuration */
            if (CONFIG_Ptr -> start_mode == ON_ENABLE)
                CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);
            else if (CONFIG_Ptr -> start_mode == ON_TRIGGER)
                SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);

            /* Set Timer prescaler */
            ACCESS_REG(CONFIG_Ptr -> timer, GPTMTBPR) = CONFIG_Ptr -> prescaler;

            /* Re-Enable the Timer again to start counting */
            SET_BIT(CONFIG_Ptr -> timer,GPTMCTL,PIN_8);
            return;

        }
    }

    else if (CONFIG_Ptr -> bit_mode == MODE_32)
    {
        /* Disable TAEN / TBEN Before Tuning the timer configuartions */
        ACCESS_REG (CONFIG_Ptr -> timer,GPTMCTL) &= ~((1<<PIN_0) | (1<<PIN_8));

        /* Choose 32-bit mode */
        ACCESS_REG(CONFIG_Ptr -> timer,GPTMCFG) = 0x0;
        /* One shot timer configuration */
        if (CONFIG_Ptr -> operation_mode == ONE_SHOT)
        {
            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);

            /* If compare match mode is activated, assign the compare value */
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                ACCESS_REG(CONFIG_Ptr -> timer, GPTMTAILR) = CONFIG_Ptr -> compare_value;
            }
        }

        /* Periodc timer configuration */
        else if (CONFIG_Ptr -> operation_mode == PERIODIC)
        {
            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);

            /* If compare match mode is activated, assign the compare value */
            if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            {
                ACCESS_REG(CONFIG_Ptr -> timer, GPTMTAILR) = CONFIG_Ptr -> compare_value;
            }
        }

        /* Capture timer configuration */
        else if (CONFIG_Ptr -> operation_mode == CAPTURE)
        {
            ACCESS_REG(CONFIG_Ptr -> timer,GPTMTAMR) |= (1<<PIN_0) | (1<<PIN_1);
            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_3);

            /* Edge count capture mode configuration */
            if (CONFIG_Ptr -> capture_mode == EDGE_COUNT)
                CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_0);

            /* Edge time capture mode configuration */
            else if (CONFIG_Ptr -> capture_mode == EDGE_TIME)
                SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_1);
        }

        /* Count direction configuration */
        if (CONFIG_Ptr -> count_mode == COUNT_UP)
            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);
        else if (CONFIG_Ptr -> count_mode == COUNT_DOWN)
            CLEAR_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_4);

        /* Timer Interrupt Configuration */
        if (CONFIG_Ptr -> interrupt_state == INT_ON)
        {
            SET_BIT(CONFIG_Ptr -> timer,GPTMTAMR,PIN_5);
            TIMER_32_16_initInterrupt (CONFIG_Ptr);
        }

        /* Timer Start Configuration */
        if (CONFIG_Ptr -> start_mode == ON_ENABLE)
            CLEAR_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);
        else if (CONFIG_Ptr -> start_mode == ON_TRIGGER)
            SET_BIT(CONFIG_Ptr -> timer, GPTMTAMR,PIN_6);

        /* Set Timer prescaler */
        ACCESS_REG(CONFIG_Ptr -> timer, GPTMTAPR) = CONFIG_Ptr -> prescaler;

        /* Re-Enable the Timer again to start counting */
        SET_BIT(CONFIG_Ptr -> timer,GPTMCTL,PIN_0);
        return;



    }
}


void TIMER_32_16_initClock (uint32_t timer)
{
    switch (timer)
    {
        case TIMER0:
            RCGCTIMER |= (1<<PIN_0);
            break;
        
        case TIMER1:
            RCGCTIMER |= (1<<PIN_1);
            break;

        case TIMER2:
            RCGCTIMER |= (1<<PIN_2);
            break;

        case TIMER3:
            RCGCTIMER |= (1<<PIN_3);
            break;

        case TIMER4:
            RCGCTIMER |= (1<<PIN_4);
            break;
        
        case TIMER5:
            RCGCTIMER |= (1<<PIN_5);
            break;
    }
    return;
}


void TIMER_32_16_initInterrupt (TIMER_32_16_CONFIG const *CONFIG_Ptr)
{
    if (CONFIG_Ptr -> channel == 'A')
    {
        if (CONFIG_Ptr -> operation_mode_2 == OVF)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_0);
        else if (CONFIG_Ptr -> bit_mode == MODE_RTC)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_3);
        else if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_4);

        SET_BIT(NVIC,ISER_0,PIN_19);
    }

    else if(CONFIG_Ptr -> channel == 'B')
    {
        if (CONFIG_Ptr -> operation_mode_2 == OVF)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_8);
        else if (CONFIG_Ptr -> operation_mode_2 == COMPARE)
            SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_11);

        SET_BIT(NVIC,ISER_0,PIN_20);
    }

    if (CONFIG_Ptr -> bit_mode == MODE_32)
        SET_BIT(CONFIG_Ptr -> timer,GPTMIMR,PIN_0);

    sei() ;
}

void TIMER_32_16_fun (void)
{
    static uint8_t flag = 0;
    flag++;
    if (flag == 1)
    {
        CLEAR_BIT(PORTF,GPIODATA,PIN_1);
        SET_BIT(PORTF,GPIODATA,PIN_3);
    }

    else if (flag == 2)
    {
        CLEAR_BIT(PORTF,GPIODATA,PIN_3);
        SET_BIT(PORTF,GPIODATA,PIN_1);
        flag = 0;
    }

    SET_BIT(TIMER0,GPTMICR,PIN_0);
}

#endif
