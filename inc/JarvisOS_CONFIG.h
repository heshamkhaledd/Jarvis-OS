/******************************************************************************
 * File Name: JarvisOS_CONFIG.h
 *
 * Description: Jarvis-OS Configure File. You can Configure the Operating System
 *              basic settings inside here.
 *
 * Engineer: Hesham Khaled
 *
 *******************************************************************************/

#ifndef _JARVISOS_CONFIG_H
#define _JARVISOS_CONFIG_H

#define NULL (void*) 0
#define MS_TO_TICKS( xTimeInMs ) ( ( uint32_t ) ( ( ( uint32_t ) ( xTimeInMs ) * ( uint32_t ) (F_CPU) ) / ( uint32_t ) 1000 ) )

#define F_CPU                   16000000
#define NUM_OF_THREADS          3
#define STACK_SIZE              100
#define QUANTA                  100
#define THREAD_ID_MAX_LENGTH    15
#define port_MAX_DELAY          2


#endif
