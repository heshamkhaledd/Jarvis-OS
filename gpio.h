/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.h
 *
 * Description: Header file for the GPIO driver
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"
#include "common_macros.h"


/*******************************************************************************
 *                   GPIO Registers and Ports Definitions
 ******************************************************************************/

                                /* Ports Base Addresses */
#define PORTA     0x40004000
#define PORTB     0x40005000
#define PORTC     0x40006000
#define PORTD     0x40007000
#define PORTE     0x40024000
#define PORTF     0x40025000
#define NVIC      0xE000E000


                                /* Registers offset */


/* GPIOLOCK: Writing "0x4C4F434B" to unlock GPIOCR register */
#define GPIOLOCK    0x520

/* GPIOCR: Commit Register that allows configuring the GPIO Pins when written with '0x01' */
#define GPIOCR      0x524

/* GPIODATA: Read/Write Register */
#define GPIODATA    0x3FC

/* GPIODIR: Direction Register
 * 0 := Input
 * 1 := Output
 */
#define GPIODIR     0x400

/* GPIODEN: Digital Behaviour
 * 0 := Digital Behaviour OFF
 * 1 := Digital Behaviour ON
 */
#define GPIODEN     0x51C

/* GPIOAMSEL: Analog Behaviour
 * 0 := Analog Behaviour OFF
 * 1 := Analog Behaviour ON
 */
#define GPIOAMSEL   0x528

/* GPIOUR: Pull-up Resistor
 * 0 := OFF
 * 1 := ON
 */
#define GPIOUR      0x510

/* GPIODR: Pull-Down Resistor
 * 0 := OFF
 * 1 := ON
 */
#define GPIOPDR     0x514

/* GPIOODR: Open-Drain Enable
 * 0 := Disabled
 * 1 := Enabled
 */
#define GPIOODR     0x50C

/* GPIOIM: GPIO Interrupt Mask
*  0 := Interrupt Disabled
*  1 := Interrupt Enabled
*/
#define GPIOIM      0x410

/* ISER_0: GPIO IRQs NVIC Enable */
#define ISER_0      0x100

/* GPIOIS: GPIO Interrupt Trigger(1)
*  0 := Edge Sensitive
*  1 := Level Sensitive
*/
#define GPIOIS      0x404

/* GPIOIEV: GPIO Interrupt Trigger(2)
*  0 := Falling/Low ~ Edge/Level
*  1 := Rising/High ~ Edge/Level
*/
#define GPIOIEV     0x40C

/* GPIOIBE: GPIO Interrupt Trigger(2)
*  0 := Both Rising/Falling OFF
*  1 := Both Rising/Falling ON
*/
#define GPIOIBE     0x408

/* GPIOICR: Interrupt Clear Register
 * interrupt is cleared by writing '1' to the corresponding external pin
 */
#define GPIOICR     0x41C


/* GPIO Alternate Function Select
 * PA0: RxD
 * PA1: TxD
 */
#define GPIOAFSEL   0x420

/* For UART Zero -> Write '1' to the corresponding bit of UART0 */
#define GPIOPCTL    0x52C



#define RCGCGPIO     (*((volatile uint32_t*)(0x400FE000+0x608)))


/*******************************************************************************
 *                  Configuration Structures and Enumerators
 ******************************************************************************/


                                /* Enumerators */

/******************************************************************************
 * Name: PIN_DIR
 * Description: Enumerator to hold the type of the required PIN Direction
 * 0 := Input
 * 1 := Output
 *****************************************************************************/
typedef enum{

    INPUT, OUTPUT

}PIN_DIR;


/******************************************************************************
 * Name: PIN_LOGIC
 * Description: Enumerator to hold the type of the required LOGIC
 * 0 := Pull Up Resistor
 * 1 := Pull Down Resistor
 * 2 := Open Drain (Neither Nor)
 *****************************************************************************/
typedef enum{

    PULL_UP, PULL_DOWN, OPEN_DRAIN

}PIN_LOGIC;


/******************************************************************************
 * Name: PIN_LOGIC
 * Description: Enumerator to hold the type of the required LOGIC
 * 0 := Digital
 * 1 := Analog
 *****************************************************************************/
typedef enum{
    DIGITAL, ANALOG
}PIN_BEHAVIOUR;


/******************************************************************************
 * Name: PIN_INT
 * Description: Enumerator to Activate PIN External Interrupt
 * 0 := Interrupt OFF
 * 1 := Interrupt ON
 *****************************************************************************/
typedef enum{
    INTERRUPT_ON, INTERRUPT_OFF
}PIN_INT;


/******************************************************************************
 * Name: PIN_TRIG
 * Description: Enumerator to decide the PIN trigger mode
 * 0 := Active Low      2 := Rising Edge        4 := Both Rising & Falling Edge
 * 1 := Analog          3 := Falling Edge
 *****************************************************************************/
typedef enum{
    ACTIVE_LOW, ACTIVE_HIGH, FALLING_EDGE, RISING_EDGE, BOTH
}PIN_TRIG;

                                    /* Structs */

/******************************************************************************
 * Name: PIN_CONFIG
 * Description: Struct to hold the Configuration of the required Port and Pin
 * PORT / REG / PIN / DIRECTION / LOGIC
 *****************************************************************************/
typedef struct{

    uint32_t        PORT;
    uint32_t        PIN;
    PIN_BEHAVIOUR   BEHAVIOUR;
    PIN_DIR         DIRECTION;
    PIN_LOGIC       LOGIC;
    PIN_INT         INTERRUPT;
    PIN_TRIG        TRIGGER;
}PIN_CONFIG;



/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void GPIO_configurePIN (PIN_CONFIG const *CONFIG_Ptr);
void GPIO_initClock (uint32_t PORT);
void GPIO_initInterrupt (uint32_t PORT, uint32_t PIN, uint8_t TRIGGER);

#endif /* GPIO_H_ */
