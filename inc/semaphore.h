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

/* Definition of Semaphore Handles */
typedef uint32_t     SemaphoreHandle_t;


/*******************************************************************************
 *                          Public Functions Prototypes.
 ******************************************************************************/
void SemaphoreCreateBinary (SemaphoreHandle_t *semaphore);
void SemaphoreCreate (SemaphoreHandle_t *semaphore, uint32_t num_of_tokens);
void SemaphorePend (SemaphoreHandle_t *semaphore, uint32_t ThreadDelay);
void SemaphorePost (SemaphoreHandle_t *semaphore);

#endif
