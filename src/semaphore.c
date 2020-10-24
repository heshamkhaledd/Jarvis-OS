/******************************************************************************
 * [File Name]:     semaphore.c
 *
 * [Description]:   Semaphore Implementation Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/
#include "semaphore.h"

extern void Thread_Suspend (uint32_t port_DELAY);
extern void sei (void);
extern void cli (void);

/******************************************************************************
 *
 * [Function Name]: CreateBinarySemaphore
 *
 * [Description]:   Creates a binary Semaphore with an initial state of Given '1'
 *
 * [Arguments]:     SemaphoreHandle_t *semaphore
 * [Return]:        void
 *
 *****************************************************************************/
void SemaphoreCreateBinary (SemaphoreHandle_t *semaphore)
{
    *semaphore = 1;
}

/******************************************************************************
 *
 * [Function Name]: CreateSemaphore
 *
 * [Description]:   Creates a binary Semaphore with an initial state of Given '1'
 *
 * [Arguments]:     SemaphoreHandle_t *semaphore, uint32_t num_of_tokens
 * [Return]:        void
 *
 *****************************************************************************/
void SemaphoreCreate (SemaphoreHandle_t *semaphore, uint32_t num_of_tokens)
{
    *semaphore = num_of_tokens;
}

/******************************************************************************
 *
 * [Function Name]: SemaphorePend
 *
 * [Description]:   Creates a binary Semaphore with an initial state of Given '1'
 *
 * [Arguments]:     SemaphoreHandle_t *semaphore, uint32_t ThreadDelay
 * [Return]:        void
 *
 *****************************************************************************/
void SemaphorePend (SemaphoreHandle_t *semaphore, uint32_t ThreadDelay)
{
    sei();
    
    while(*semaphore == 0)
        Thread_Suspend(ThreadDelay);

    *semaphore = *semaphore - 1;

    cli();
}

/******************************************************************************
 *
 * [Function Name]: SemaphorePost
 *
 * [Description]:   Creates a binary Semaphore with an initial state of Given '1'
 *
 * [Arguments]:     SemaphoreHandle_t *semaphore
 * [Return]:        void
 *
 *****************************************************************************/
void SemaphorePost (SemaphoreHandle_t *semaphore)
{
    sei();
    *semaphore = *semaphore + 1;
    cli();
}
