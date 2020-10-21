#include "JarvisOS_kernel.h"
#include "JarvisOS_CONFIG.h"





void Thread_1(void)
{
    uint8_t Counter_1 = 0;

    while (1)
    {
        Counter_1++;
        Thread_Suspend(50);
    }
}

void Thread_2(void)
{
    uint8_t Counter_2 = 0;

    while (1)
    {
        Counter_2++;
        Thread_Suspend(50);
    }
}

void Thread_3(void)
{
    uint8_t Counter_3 = 0;

    while (1)
    {
        Counter_3++;
        Thread_Suspend(50);
    }
}


int main ()
{
    ThreadCreate (Thread_1,1);
    ThreadCreate (Thread_2,2);
    ThreadCreate (Thread_3,3);

    JARVIS_initKernel ();   /* Start running Threads inside Jarvis Kernel */

    /* UNREACHABLE CODE */
    while (1);
}
