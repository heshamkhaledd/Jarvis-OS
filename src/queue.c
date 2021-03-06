/******************************************************************************
 * [File Name]:     queue.c
 *
 * [Description]:   Queues Implementation Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/
#include "queue.h"
#include <stdlib.h>


/******************************************************************************
 *
 * [Function Name]: QueueCreate
 *
 * [Description]:   Responsible Dynamic Allocating a queue in heap segment
 *                  according to a given length and size.
 *
 * [Arguments]:     uint32_t length, uint8_t size
 * [Return]:        QueueHandle_t
 *
 *****************************************************************************/
QueueHandle_t QueueCreate(uint32_t length, uint8_t size)
{

    QueueHandle_t queue = (QueueHandle_t) malloc(sizeof(xQUEUE));

    if (queue == NULL)
        return NULL;

    else
    {
        queue->tail = 0;
        queue->head = 0;
        queue->length = length;
        queue->Data_Ptr = (uint32_t *) calloc(length,size);
        queue->size = 0;

        if(queue->Data_Ptr == NULL)
            return NULL;
        else
            return queue;
    }
}


/******************************************************************************
 *
 * [Function Name]: QueueWrite
 *
 * [Description]:   Function responsible for writing the required data to the
 *                  next free space in the FIFO queue.
 *
 * [Arguments]:     QueueHandle_t queue, uint32_t data
 * [Return]:        int8_t
 *
 *****************************************************************************/
uint8_t QueueWrite(QueueHandle_t queue, uint32_t data)
{
    if (queue == NULL)
        return ERROR_QUEUE_NULL;
        
    else if (QueueIsFull(queue))
        return ERROR_QUEUE_FULL;

    else
    {
        queue->Data_Ptr[queue->tail] = (size_t)data;
        queue->tail = (queue->tail + 1) % (queue->length);
        queue->size  = queue->size + 1;
    }
    return SUCCESS;
}

/******************************************************************************
 *
 * [Function Name]: QueueReceive
 *
 * [Description]:   Function responsible for reading the required data
 *                  in the FIFO queue.
 *
 * [Arguments]:     QueueHandle_t queue, uint32_t *var
 * [Return]:        int8_t
 *
 *****************************************************************************/

uint8_t QueueReceive(QueueHandle_t queue,uint32_t *var)
{
    if (queue == NULL)
        return ERROR_QUEUE_NULL;

    else if (QueueIsEmpty(queue))
        return ERROR_QUEUE_EMPTY;

    else
    {
        *var = (queue->Data_Ptr[queue->head]);

        queue->head = (queue->head + 1) % (queue->length);

        queue->size  = queue->size - 1;
    }
    return SUCCESS;
}

/******************************************************************************
 *
 * [Function Name]: QueueIsFull
 *
 * [Description]:   Returns 1 if the Queue is Full.
 *
 * [Arguments]:     QueueHandle_t queue
 * [Return]:        int8_t
 *
 *****************************************************************************/

uint8_t QueueIsFull (QueueHandle_t queue)
{
    return ((queue->size) == queue->length);
}

/******************************************************************************
 *
 * [Function Name]: QueueIsEmpty
 *
 * [Description]:   Returns 1 if the Queue is Empty.
 *
 * [Arguments]:     QueueHandle_t queue
 * [Return]:        int8_t
 *
 *****************************************************************************/

uint8_t QueueIsEmpty (QueueHandle_t queue)
{
    return ((queue->size) == 0);
}
