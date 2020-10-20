/******************************************************************************
 *
 *
 * File Name: JarvisOS-kernel.h
 *
 * Description: Private Kernel Header File
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#ifndef _JARVISOS_KERNEL_H
#define _JARVISOS_KERNEL_H

#include <stdint.h>
#include "SysTick.h"
#include "JarvisOS_CONFIG.h"


/* Time MACRO */
#define MS_TO_TICKS( xTimeInMs ) ( ( uint32_t ) ( ( ( uint32_t ) ( xTimeInMs ) * ( uint32_t ) F_CPU ) / ( uint32_t ) 1000 ) )

#define NULL (void*) 0

/*******************************************************************************
 *                          Task Control Block (TCB)
 ******************************************************************************/
typedef enum
{
    READY,BLOCKED,SUSPENDED,RUNNING
}Thread_Status;


typedef struct TCB TCB;
struct TCB{

    int32_t         *stackPtr;
    TCB             *nextPtr;
    uint8_t         priority;
    Thread_Status   status;
    uint32_t        delayTime;
};

/*******************************************************************************
 *                          Private Functions Prototypes.
 ******************************************************************************/
void JARVIS_initStack (uint8_t);
void Scheduler_init (void);
void Thread_Suspend (uint32_t);
void stateIdle (void);
void Generate_stateIdle (uint8_t Idx);
void LoadNextThread(void);
uint8_t nextThreadIndex (TCB *ThreadsPtr);


/*******************************************************************************
 *                          Public Functions Prototypes.
 ******************************************************************************/
uint8_t ThreadCreate(void(*Thread)(void), uint8_t a_priority);
void JARVIS_initKernel (void);
void Thread_Block (void);
void Thread_Resume (void);


#endif
