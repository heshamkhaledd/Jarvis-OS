/******************************************************************************
 * [File Name]:     queue.h
 *
 * [Description]:   Queues Implementation Header File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h>
#include "JarvisOS_CONFIG.h"

typedef enum {
    SUCCESS,
    ERROR_QUEUE_FULL,
    ERROR_QUEUE_EMPTY,
    ERROR_QUEUE_NULL
}Queue_ErrorCode;

typedef struct{
    uint32_t        *Data_Ptr;
    uint32_t        tail;
    uint32_t        head;
    uint32_t        length;
    uint8_t         size;
}xQUEUE;

/* Typedef to any created Queue Handle  */
typedef xQUEUE*   QueueHandle_t;


/*******************************************************************************
 *                          Public Functions Prototypes.
 ******************************************************************************/
QueueHandle_t QueueCreate(uint32_t length, uint8_t size);
uint8_t QueueWrite(QueueHandle_t queue,uint32_t data);
uint8_t QueueReceive(QueueHandle_t queue,uint32_t *var);
uint8_t QueueIsEmpty (QueueHandle_t queue);
uint8_t QueueIsFull (QueueHandle_t queue);


#endif
