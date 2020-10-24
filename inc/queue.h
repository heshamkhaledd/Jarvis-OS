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

#define NULL (void*) 0

typedef struct{
    uint32_t        *Ptr;
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
int8_t QueueWrite(QueueHandle_t queue,uint32_t data);
int8_t QueueReceive(QueueHandle_t queue,uint32_t *var);
uint8_t QueueIsEmpty (QueueHandle_t queue);
uint8_t QueueIsFull (QueueHandle_t queue);


#endif
