# Jarvis-OS
Jarvis-OS is a live Real time operating system ready to run on ARM Cortex-M processors.<br />
Jarvis-OS MicroKernel supports the following features:<br />
* Preemptive Weighted Round-Robin Scheduler<br />
* Semaphores (Binary and Spinlock)<br />
* Dynamic Queues for Inter-Thread Communication<br />

Table of contents
=================
<!--ts-->
* [Jarvis-OS User Configurations](#Jarvis-OS-User-Configurations)
* [API Functions](#API-Functions)
    * [ThreadsControl](#**•-Threads-Control**)
        * [ThreadCreate](#ThreadCreate)
        * [Thread_Suspend](#Thread_Suspend)
        * [Thread_Block](#Thread_Block)
        * [Thread_Resume](#Thread_Resume)
        * [JARVIS_initKernel](#JARVIS_initKernel)
    * [Semaphores](#**•-Semaphores**)
        * [CreateBinarySemaphore](#CreateBinarySemaphore)
        * [CreateSemaphore](#CreateSemaphore)
        * [SemaphorePend](#SemaphorePend)
        * [SemaphorePost](#SemaphorePost)
    * [Queues](#**•-Queues**)
        * [QueueCreate](#QueueCreate)
        * [QueueWrite](#QueueWrite)
        * [QueueReceive](#QueueReceive)
        * [QueueIsEmpty](#QueueIsEmpty)
        * [QueueIsFull](#QueueIsFull)
* [Notes](#Notes)
<!--te-->

## Jarvis-OS User Configurations
In order to configure Jarvis-OS to work in your favor, you need to change the parameters found in `JarvisOS-CONFIG.h`
```c
#define F_CPU                   16000000      /* Your ARM Cortex-M Frequency */
#define NUM_OF_THREADS          3             /* Number of Threads Your System Require */
#define STACK_SIZE              100           /* Stack size for each thread */
#define QUANTA                  100           /* Scheduler's Quanta in milliseconds */
#define ThreadID_MAX_LENGTH     15            /* Thread ID string maximum length */
#define port_MAX_DELAY          2             /* An Optional Macro to determine delays in Quanta */
```

## API Functions
### **• Threads Control**
1) ### ThreadCreate
___
* **Description**: Creates a thread in the MicroKernel<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  ThreadID |`uint8_t [ ]`  | String to Identifiy the Thread |
|  ThreadAddress | `void(*Thread)` | Thread Address
| Thread Priority | `uint8_t` | Priority of Thread

* **Return**: `void`<br />
* **Example**:
```c
void Thread_Name (void){
    /* Thread inits */
    while (1)
    {
        /* Thread Subroutine */
    }
}

int main ()
{
    ThreadCreate ("ThreadID_1",Thread_Name,5);
    /* Rest of main */
}
```
___
2) ### Thread_Suspend
* **Description**: Suspends a Thread for specific time in Quanta  <br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  delayTime |`uint32_t`  | Delay Time in Quanta |


* **Return**: `void`<br />
* **Example**:
```c
void Thread_Address (void){
    /* Thread inits */
    while (1)
    {
        /* Thread Subroutine */
        Thread_Suspend (2);     /* Thread will be suspended two quantas */
    }
}
```
___

3) ### Thread_Block
* **Description**: Blocks a task entirely and put it in blocked state   <br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  ThreadID |`uint8_t[ ]`  | String to Identifiy the Thread |


* **Return**: `void`<br />
* **Example**:
```c
void Thread_1 (void){
    /* Thread inits */
    while (1)
    {
        /* Thread Subroutine */

        Thread_Block ("DataThread");

        /* Rest of Subroutine */
            
    }
}
void Thread_2 (void){
    
    while (1)
    {
        /* Thread Subroutine */ 

        /* THIS THREAD WILL BE BLOCKED UNTIL RESUMED */  
    }
}
int main ()
{
    ThreadCreate("DMAThread",Thread_1,2);
    ThreadCreate("DataThread",Thread_2,4);
}
```
___
4) ### Thread_Resume

* **Description**: Releases a Thread from its blocked state   <br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  ThreadID |`uint8_t[ ]`  | String to Identifiy the Thread |


* **Return**: `void`<br />
* **Example**:
```c
void Thread_1 (void){
    /* Thread inits */
    while (1)
    {
        /* Thread Subroutine */

        Thread_Block ("DataThread");

        /* Rest of Subroutine */

        Thread_Resume ("DataThread");
            
    }
}
void Thread_2 (void){
    
    while (1)
    {
        /* Thread Subroutine */ 

        /* THIS THREAD WILL BE BLOCKED UNTIL RESUMED */  
    }
}
int main ()
{
    ThreadCreate("DMAThread",Thread_1,2);
    ThreadCreate("DataThread",Thread_2,4);
}
```
___
5) ### JARVIS_initKernel
* **Description**: Stars the Scheduler and initialize the Kernel  <br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
| |`void`  |  |


* **Return**: `void`<br />
* **Example**:
```c
int main 
{
    /* Threads, Queues and Semaphores initializing */
    
    JARVIS_initKernel ();
}
```
___
___
### **• Semaphores**
1) ### CreateBinarySemaphore
___
* **Description**: Creates a Binary Semaphore of 1 token<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  &SemphHandle |`SemaphoreHandle_t`  | Address to Semaphore |

* **Return**: `void`<br />
* **Example**:
```c
SemaphoreHandle_t semaphore_1;  /* Delacring a semaphore Handle */

int main ()
{
    CreateBinarySemaphore (&semaphore_1);
    /* Rest of main */
}
```
___
2) ### CreateSemaphore
* **Description**: Creates a  Semaphore of given number of tokens<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  &SemphHandle |`SemaphoreHandle_t`  | Address to Semaphore |
|  Tokens| `uint32_t` | Number of Tokens this Semaphore |

* **Return**: `void`<br />
* **Example**:
```c
SemaphoreHandle_t semaphore_1;  /* Delacring a semaphore Handle */

int main ()
{
    uint32_t num_of_tokens = 5;
    CreateSemaphore (&semaphore_1,num_of_tokens);
    /* Rest of main */
}
```
___
3) ### SemaphorePend
* **Description**: Pends (Takes) one token of a given semaphore<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  &SemphHandle |`SemaphoreHandle_t`  | Address to Semaphore |
|  ThreadDelay| `uint32_t` | Delay Time in Quanta if there is no tokens |

* **Return**: `void`<br />
* **Example**:
```c
void Thread_1(void){

    while (1){
        SemaphorePend(&semaphore_1,port_MAX_DELAY); /*port_MAX_DELAY is
                                                      found at config file */
        /* Thread Subroutine */
    }
}
```
___
4) ### SemaphorePost
* **Description**: Posts (Gives) one token of a given semaphore<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  &SemphHandle |`SemaphoreHandle_t`  | Address to Semaphore |
|  ThreadDelay| `uint32_t` | Delay Time in Quanta if there is no tokens |

* **Return**: `void`<br />
* **Example**:
```c
void Thread_1(void){

    while (1){
        SemaphorePend(&semaphore_1,port_MAX_DELAY); /*port_MAX_DELAY is
                                                      found at config file */
        /* Thread Subroutine */
    }
}
```
___
___
### **• Queues**
1) ### QueueCreate
___
* **Description**: Creates a queue in heap segment of a given size<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  length |`int32_t`  | Queue Length |
|  size | `uint8_t` | Size of Each Location|

* **Return**: `QueueHandle_t`, If it successfully allocated the Queue<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`NULL`, If there's no heap space to allocate the Queue.
* **Example**:
```c
QueueHandle_t queue_1;
int main ()
{
    queue_1 = QueueCreate(17,sizeof(uint32_t));
    if (queue_1 == NULL)
    {
        /* Code to handle the case */
    }
    else
            /* Rest of main */
}
```
___
2) ### QueueWrite
___
* **Description**: Writes data to a specific queue.<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  queue |`QueueHandle_t`  | Queue Handle |
|  size | `uint8_t` | Size of Each Location|

* **Return**: `'1'`, If it successfully sent data to queue.<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`'-1'`, If the queue is already full.
* **Example**:
```c
QueueHandle_t queue_1;

void Thread_1(void)
{
    uint8_t data = 87;

    while (1)
    {
        /* Thread Subroutine */

        uint8_t var = QueueWrite(queue_1,data);

        if (var == -1)
        {
            /* Code to handle the case */
        }

        /* Rest of Thread Subroutine */

    }
}
```
___
3) ### QueueReceive
___
* **Description**: Reads data from a specific queue.<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  queue  |`QueueHandle_t`  | Queue Handle |
|  &var | `uint32_t` | Variable to read the data into|

* **Return**: `'1'`, If it successfully received data from queue.<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`'-1'`, If the queue is already empty.
* **Example**:
```c
QueueHandle_t queue_1;

void Thread_8(void)
{
    uint32_t data_receive;

    while (1)
    {
        /* Thread Subroutine */

        uint8_t var = QueueReceive(queue_1,&data_receive);

        if (var == -1)
        {
            /* Code to handle the case */
        }

        /* Rest of Thread Subroutine */

    }
}
```
___

4) ### QueueIsEmpty
___
* **Description**: Checks if the Queue is Empty or not.<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  queue  |`QueueHandle_t`  | Queue Handle |

* **Return**: `'1'`, If the queue is empty.<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`'0'`, If the queue is not empty.
___
5) ### QueueIsFull
___
* **Description**: Checks if the Queue is Full or not.<br />
* **Parameters**:

| Parameters    | Type | Description |
| ------------- | ---- | ----------- |
|  queue  |`QueueHandle_t`  | Queue Handle |

* **Return**: `'1'`, If the queue is full.<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`'0'`, If the queue is not full.
___
___
## Notes
Jarvis-OS uses ARM Cortex-M processors SysTick timer. In order to port Jarvis to<br />
your ARM processor, you need to extern `SysTick_Handler` in your startup (stub) code<br />
and place it in SysTick location in the Interrupt Vector Table (IVT)
```c
/* in startup code */

extern void SysTick_Handler (void);
```