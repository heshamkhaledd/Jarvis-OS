/******************************************************************************
 * [File Name]:     semaphore.h
 *
 * [Description]:   Semaphore Implementation Header File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <stdint.h>

extern void Thread_Suspend (uint32_t port_DELAY);
extern void sei (void);
extern void cli (void);

/* Definition of Semaphore Handles */
typedef uint32_t     SemaphoreHandle_t;


/*******************************************************************************
 *                          Public Functions Prototypes.
 ******************************************************************************/
void CreateBinarySemaphore (SemaphoreHandle_t *semaphore);
void CreateSemaphore (SemaphoreHandle_t *semaphore, uint32_t num_of_tokens);
void SemaphorePend (SemaphoreHandle_t *semaphore, uint32_t ThreadDelay);
void SemaphorePost (SemaphoreHandle_t *semaphore);

#endif
