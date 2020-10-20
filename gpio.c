/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the GPIO driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#include "gpio.h"


/******************************************************************************
 *
 * Function Name: GPIO_configurePin
 *
 * Description: A function that's responsible for initializing the required Pin
 *              with the given configuration
 *
 * Input: PIN_CONFIG const *CONFIG_Ptr
 * Output: void
 *
 *****************************************************************************/
void GPIO_configurePIN (PIN_CONFIG const *CONFIG_Ptr) {

                        /* Initialize Clock for the required Port */
    GPIO_initClock (CONFIG_Ptr -> PORT);

    ACCESS_REG((CONFIG_Ptr -> PORT), (GPIOLOCK)) = 0x4C4F434B;
    ACCESS_REG((CONFIG_Ptr -> PORT), (GPIOCR)) = (1<< (CONFIG_Ptr -> PIN) );



                        /* Set The Direction of the required PIN */
    if (CONFIG_Ptr -> DIRECTION == INPUT)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIODIR) &= ~(1 << (CONFIG_Ptr -> PIN));
    else if (CONFIG_Ptr -> DIRECTION == OUTPUT)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIODIR) |= (1 << (CONFIG_Ptr -> PIN));

                        /* Set The Behaviour of the required PIN */
    if (CONFIG_Ptr -> BEHAVIOUR == DIGITAL)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIODEN) |= (1 << (CONFIG_Ptr -> PIN));

    else if (CONFIG_Ptr -> BEHAVIOUR == ANALOG)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOAMSEL) |= (1 << (CONFIG_Ptr -> PIN));

                        /* Set the Logic of the required PIN */
    if (CONFIG_Ptr -> LOGIC == PULL_UP)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOUR) |= ( 1 << (CONFIG_Ptr -> PIN));

    else if (CONFIG_Ptr -> LOGIC == PULL_DOWN)
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOPDR) |= ( 1 << (CONFIG_Ptr -> PIN));

    else
        ACCESS_REG((CONFIG_Ptr -> PORT), GPIOODR) |= ( 1 << (CONFIG_Ptr -> PIN));

    if (CONFIG_Ptr -> INTERRUPT == INTERRUPT_ON)
        GPIO_initInterrupt (CONFIG_Ptr -> PORT, CONFIG_Ptr -> PIN, CONFIG_Ptr -> TRIGGER);


    return;
}


/******************************************************************************
 *
 * Function Name: GPIO_initClock
 *
 * Description: A function that's responsible for starting the clock of
 *              a given Port.
 *
 * Input: uint32_t PORT
 * Output: void
 *
 *****************************************************************************/
void GPIO_initClock (uint32_t PORT)
{
    switch (PORT)
    {
        case PORTA:
            RCGCGPIO |= (1<<PIN_0);
            break;
        case PORTB:
            RCGCGPIO |= (1<<PIN_1);
            break;
        case PORTC:
            RCGCGPIO |= (1<<PIN_2);
            break;
        case PORTD:
            RCGCGPIO |= (1<<PIN_3);
            break;
        case PORTE:
            RCGCGPIO |= (1<<PIN_4);
            break;
        case PORTF:
            RCGCGPIO |= (1<<PIN_5);
            break;
    }

    return;
}


void GPIO_initInterrupt (uint32_t PORT, uint32_t PIN, uint8_t TRIGGER)
{
    ACCESS_REG((PORT), (GPIOIM)) |= (1<<PIN);

    switch (PORT)
    {
        case PORTA:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<0);
            break;
        case PORTB:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<1);
            break;
        case PORTC:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<2);
            break;
        case PORTD:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<3);
            break;
        case PORTE:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<4);
            break;
        case PORTF:
            ACCESS_REG((NVIC), (ISER_0)) |= (1<<30);
            break;
    }

    switch (TRIGGER)
    {
        case ACTIVE_LOW:
            ACCESS_REG((PORT), (GPIOIS)) |= ((1<<PIN));
            ACCESS_REG((PORT), (GPIOIEV)) &= ~((1<<PIN));
            break;
        case ACTIVE_HIGH:
            ACCESS_REG((PORT), (GPIOIS)) |= ((1<<PIN));
            ACCESS_REG((PORT), (GPIOIEV)) |= ((1<<PIN));
            break;
        case FALLING_EDGE:
            ACCESS_REG((PORT), (GPIOIS)) &= ~((1<<PIN));
            ACCESS_REG((PORT), (GPIOIEV)) &= ~((1<<PIN));
            break;
        case RISING_EDGE:
            ACCESS_REG((PORT), (GPIOIS)) &= ~((1<<PIN));
            ACCESS_REG((PORT), (GPIOIEV)) |= ((1<<PIN));
            break;
        case BOTH:
            ACCESS_REG((PORT), (GPIOIBE)) |= ((1<<PIN));
            break;
    }

    sei();

    return;
}
