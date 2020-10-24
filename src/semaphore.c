/******************************************************************************
 * [File Name]:     semaphore.c
 *
 * [Description]:   Semaphore Implementation Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/
#include "semaphore.h"


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
void CreateBinarySemaphore (SemaphoreHandle_t *semaphore)
{
    *semaphore = 1;
    return;
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
void CreateSemaphore (SemaphoreHandle_t *semaphore, uint32_t num_of_tokens)
{
    *semaphore = num_of_tokens;
    return;
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
    
    return;
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
    return;
}
