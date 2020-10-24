/******************************************************************************
 * [File Name]:     JarvisOS-kernel.c
 *
 * [Description]:   Private Kernel Source File
 *
 * [Engineer]:      Hesham Khaled
 *
 *******************************************************************************/

#include "JarvisOS_kernel.h"


/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/
/* Declare an array of g_Threads. +1 for stateIdle subroutine */
static TCB g_Threads[NUM_OF_THREADS+1];

/* Pointer to the current running Thread */
static TCB *g_curr_running_thread = NULL;

/* Declaring TCB (g_Threads) Stack */
static int32_t TCB_Stack[NUM_OF_THREADS+1][STACK_SIZE];

/* Global Variable to count SysTick countdown times */
static volatile uint32_t Jarvis_Ticks = 0;


/*******************************************************************************
 *                              Atomic Functions
 ******************************************************************************/
void sei (void)
{
    __asm("CPSID:");
    return;
}

void cli (void)
{
    __asm("CPSIE:");
    return;
}


/******************************************************************************
 *
 * [Function Name]: nextThreadIndex
 *
 * [Description]:   Responsible for finding the appropriate next Thread to run
 *                  if it's the highest priority and ready thread.
 *
 * [Arguments]:     TCB *ThreadsPtr
 * [Return]:        void
 *
 *****************************************************************************/
uint8_t nextThreadIndex (TCB *ThreadsPtr)
{
    /* Next Thread to run index, by default, is assigned to the IdleThread */
    uint8_t Idx,nextIdx=NUM_OF_THREADS;

    /* Next Thread to run priority, by defauly, is assigned to the lowest priority */
    uint8_t max = 0;

    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)
    {
        if(ThreadsPtr[Idx].status == READY && ThreadsPtr[Idx].priority > max)
        {
            max = ThreadsPtr[Idx].priority;
            nextIdx = Idx;
        }
    }
    return nextIdx;
}


/******************************************************************************
 *
 * [Function Name]: JARVIS_initKernel
 *
 * [Description]:   Responsible for Initializing SysTick Timer Hardware and
 *                  Initializing the Round-Robin Scheduler.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void JARVIS_initKernel(void)
{
    uint8_t Idx;

    /* Call nextThreadIndex to know which Thread will initially run */
    Idx = nextThreadIndex(g_Threads);

    g_curr_running_thread = &g_Threads[Idx];

    /* Assign the running state to the initial thread */
    g_Threads[Idx].status = RUNNING;

    /* Configure SysTick Timer to Round-Robin Quanta Value (in milliseconds) */
    SysTick_init();

    /* Assembly Function @ JarvisOS_port.asm */
    Scheduler_init();
}


/******************************************************************************
 *
 * [Function Name]: checkSuspendedState
 *
 * [Description]:   Releases suspended threads if their finished their suspension
 *                  time. This function is triggered every QUANTA.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void checkSuspendedState (void)
{
    uint8_t Idx;

    for (Idx = 0 ; Idx <NUM_OF_THREADS ; Idx++)
    {
        if (g_Threads[Idx].status == SUSPENDED && g_Threads[Idx].delayTime == Jarvis_Ticks)
            {
                g_Threads[Idx].status = READY;
                g_Threads[Idx].delayTime = 0;
            }
    }
    Jarvis_Ticks++;
    return;
}

/******************************************************************************
 *
 * [Function Name]: LoadNextThread
 *
 * [Description]:   Assembly subroutine called from JarvisOS_port.asm responsible
 *                  for loading the next appropriate thread into ARM processor.
 *
 * [Arguments]:     void
 * [Return]:        void
 *
 *****************************************************************************/
void LoadNextThread(void)
{
    uint8_t Idx;
    checkSuspendedState ();                                 /* Release suspended tasks first before choosing the next thread to run */

    Idx = nextThreadIndex(g_Threads);

    g_curr_running_thread = &g_Threads[Idx];

    if(g_Threads[NUM_OF_THREADS].status == RUNNING)           /* If the idleThread was running before this thread, return it to ready state */
        g_Threads[NUM_OF_THREADS].status = READY;

    g_Threads[Idx].status = RUNNING;                          /* Assign the next thread to run to the running state */

    return;
}

/******************************************************************************
 *
 * [Function Name]:     JARVIS_initStack
 *
 * [Description]:       Responsible for creating the stack of each thread.
 *                      & assigning stack pointer to the top of the exception stack area
 *
 * [Arguments]:         uint32_t Idx
 * [Return]:            void
 *
 *****************************************************************************/
void JARVIS_initStack (uint8_t Idx) {

    g_Threads[Idx].stackPtr = &TCB_Stack[Idx][STACK_SIZE-16]; /* Make the Thread stack pointer points to the Stack Frame section in the thread's stack */
    TCB_Stack[Idx][STACK_SIZE-1] = 0x1000000;               /* Assign Thread to execute in Thumb mode */
}


/******************************************************************************
 *
 * [Function Name]:     ThreadCreate
 *
 * [Description]:       API Function that's responsible for creating a new thread
 *
 * [Arguments]:         void(*Thread)(void), uint8_t a_priority
 * [Return]:            uint8_t
 *
 *****************************************************************************/
void ThreadCreate(uint8_t *idPtr, void(*Thread)(void), uint8_t a_priority)
{
    cli();                                                  /* Disable Global Interrupt bit */

    static uint8_t Idx = 0;

    JARVIS_initStack(Idx);                                  /* Initialize Thread Stack */

    TCB_Stack[Idx][STACK_SIZE-2] = (int32_t)(Thread);       /* Thread PC <- Thread Address */

    g_Threads[Idx].priority = a_priority;                     /* Assign Thread Priority */

    g_Threads[Idx].status = READY;                            /* Thread is initialized in Ready state */

    strcpy(g_Threads[Idx].ThreadID,idPtr);                    /* Assign Thread ID */

    Idx++;

    if (Idx == NUM_OF_THREADS)                              /* Create IdleThread if we reached maximum number of allowed threads */
        Generate_stateIdle (Idx);

    sei();                                                  /* Enable Global Interrupt bit */
}


/******************************************************************************
 *
 * [Function Name]:     Generate_stateIdle
 *
 * [Description]:       Subroutine responsible for creating a wait state task (Wait Task)
 *                      to run when all other threads are suspended.
 *
 * [Arguments]:         uint8_t Idx
 * [Return]:            void
 *
 *****************************************************************************/
void Generate_stateIdle (uint8_t Idx)
{
    JARVIS_initStack(Idx);                                  /* Create stack for IdleThread */
    TCB_Stack[Idx][STACK_SIZE-2] = (int32_t)(stateIdle);    /* Make the PC initially point to IdleThread address */
    g_Threads[Idx].priority = 0;                              /* Assign in to Kernel's lowest priority */
    g_Threads[Idx].status = READY;                            /* Initialize it as ready */

    return;
}


/******************************************************************************
 *
 * [Function Name]:     stateIdle
 *
 * [Description]:       Waiting state when all tasks are suspended / blocked.
 *
 * [Arguments]:         void
 * [Return]:            void
 *
 *****************************************************************************/
void stateIdle (void)
{
    while (1) {}                                            /* Keep waiting until a physical thread is ready to run */
}


/******************************************************************************
 *
 * [Function Name]:     Thread_Suspend
 *
 * [Description]:       API Function responsible for terminating a specific thread
 *                      by triggering SysTick Handler to switch to a new thread.
 *
 * [Arguments]:         uint32_t port_DELAY
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Suspend (uint32_t port_DELAY)
{
    uint8_t Idx;

    if (port_DELAY == 0)                                    /* if port_DELAY time is zero, do nothing and return */
        return;

    for (Idx = 0; Idx<NUM_OF_THREADS ; Idx++)               /* Keep looping on g_Threads until it finds the Thread who called it */
    {
        if (g_Threads[Idx].status == RUNNING)
        {
            g_Threads[Idx].status = SUSPENDED;

            g_Threads[Idx].delayTime = Jarvis_Ticks + port_DELAY;
            break;
        }
    }
    ACCESS_REG(SysTick,STCURRENT) = 0;
    ACCESS_REG(SysTick,INTCTRL) = 0x04000000;               /* Trigger SysTick_Handler found @ JarvisOS_port.asm */
}


/******************************************************************************
 *
 * [Function Name]:     Thread_Block
 *
 * [Description]:       API Function responsible for Blocking a thread from executing
 *                      till it's resumed
 *
 * [Arguments]:         uint8_t *idPtr
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Block (uint8_t *idPtr)
{
    uint8_t Idx;
    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)            /* Keep looping on g_Threads until it finds the thread required to be blocked */
    {
        if(strcmp(idPtr,g_Threads[Idx].ThreadID) == 0)
        {
            g_Threads[Idx].status = BLOCKED;
            break;
        }
    }
}

/******************************************************************************
 *
 * [Function Name]:     Thread_Resume
 *
 * [Description]:       API Function responsible for continue a blocked thread and get
 *                      it back to ready state again.
 *
 * [Arguments]:         uint8_t *idPtr
 * [Return]:            void
 *
 *****************************************************************************/
void Thread_Resume (uint8_t *idPtr)
{
    uint8_t Idx;
    for (Idx = 0 ; Idx < NUM_OF_THREADS ; Idx++)
    {
        if(strcmp(idPtr,g_Threads[Idx].ThreadID) == 0)        /* Keep looping on g_Threads until it finds the thread required to be resumed */
        {
            g_Threads[Idx].status = READY;
            break;
        }
    }
}
