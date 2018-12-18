#include "kernel.h"
#include "main.h"
#include "process.h"

int count = 1;
int threadCount = 1;
void
TestCase(){
    printf("Thread %d  is forked with priority: %d \n", threadCount++, kernel->currentProcess->getProcessPriority() );

}
void
SimpleThread()
{
	cout << ".................................................................................................................................\n";

    printf("Process with pid %d and priority %d is forked. This process has the below threads. \n", kernel->currentProcess->getProcessID(), kernel->currentProcess->getProcessPriority(), kernel->currentProcess->getProcessPriority() );


    kernel->currentProcess->Thread_Fork(count++, (VoidFunctionPtr)TestCase, (void * ) 1);
    kernel->currentProcess->Yield();
    kernel->currentProcess->Terminate();

}


void
ThreadTest()
{
    Process *proc1 = new Process("'Process1'", 2);
    Process *proc2 = new Process("'Process2'", 4); 
    Process *proc3 = new Process("'Process3'", 1);
    Process *proc4 = new Process("'Process4'", 5);
    Process *proc5 = new Process("'Process5'", 3);

    proc1->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    proc2->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    proc3->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    proc4->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    proc5->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    
    proc2->createChProcess();
    proc3->createChProcess();
//    proc4->createChProcess();
//    proc5->createChProcess();

    kernel->currentProcess->Yield();
    IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);
    while (kernel->scheduler->process_ReadyList->NumInList() > 0){
        kernel->currentProcess->Yield();
    }
    cout << "Control going back to Main Thread \n";
    kernel->interrupt->SetLevel(IntOn);

}
