#include "process.h"
#include "../machine/interrupt.h"
#include "../lib/copyright.h"
#include "../lib/sysdep.h"
#include "switch.h"
#include "synch.h"
//#include "scheduler.h"
#include "../lib/debug.h"
#include "thread.h"

/*int Process :: setState(int st) {
    state = st;
}*/
void ProcessPrint(Process *process) { process->Print(); }

int Process::processID_Counter = 0;

void Process::Fork(VoidFunctionPtr func, void *arg){
//    cout<< "\n----------------------IN PROCESS FORK------------------------------------ \n";
	cout <"\n\n";
    Interrupt *interrupt = kernel->interrupt;
    IntStatus oldLevel;
    this->currentThread->Fork(this,(VoidFunctionPtr)func,(void *)arg);
    childTh->Append(currentThread);
    oldLevel = interrupt->SetLevel(IntOff);
    scheduler->FindNextToRun();
    kernel->scheduler->ProcessReadyToRun(this);
    //this->scheduler->ReadyToRun(this->currentThread);
    (void) interrupt->SetLevel(oldLevel);
    functionPointer = func;
    args = arg;

    cout << "The process: " << this->getProcessName() <<" with priority:  " << this->getProcessPriority() << " is forked.\n";


}
void Process::Thread_Fork(int count, VoidFunctionPtr thread_func, void *arg){
    Thread *fthread[count+5];
    for (int m = 0 ; m < count ; m++){
        fthread[m] = new Thread("Process threads");
        fthread[m]->Fork(this, (VoidFunctionPtr) thread_func,(void *) arg);
    }
    kernel->interrupt->SetLevel(IntOff);
    kernel->currentProcess->currentThread->Yield();
    kernel->interrupt->SetLevel(IntOff);
    

}
void Process::Yield(){
   // cout << "\n----------------------IN PROCESS YIELD-----------------------------------\n";
cout << "\n\n";
     Process *next_Process;
     IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);
     ASSERT(this == kernel->currentProcess);

     DEBUG(dbgThread, "Yielding Process: " << process_name);
     cout << "\nThe Yielding Process: " << process_name <<" Its priority is: "<< this->getProcessPriority() << "\n";

     next_Process = kernel->scheduler->FindNextProcessToRun();
     if (next_Process != NULL) {
        kernel->scheduler->ProcessReadyToRun(this);
        kernel->scheduler->ProcessRun(next_Process, FALSE);
     }
     (void) kernel->interrupt->SetLevel(oldLevel);
}

void
Process::Sleep(bool finishing){
    Process *next_Process;

    ASSERT(this == kernel->currentProcess);
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    //DEBUG(dbgThread, "Sleeping thread: " << name);

    processStatus = PROCESS_BLOCKED;
    while (( next_Process = kernel->scheduler->FindNextProcessToRun()) == NULL)
        kernel->interrupt->Idle();	// no one to run, wait for an interrupt
                                    // returns when it's time for us to run
    kernel->scheduler->ProcessRun(next_Process, finishing);
}

void
Process::Terminate(){
//    cout << "\n----------------------IN PROCESS TERMINATE-----------------------------------\n";
cout << "\n\n";
    (void) kernel->interrupt->SetLevel(IntOff);
    DEBUG(dbgThread, "Process Terminating: " << process_name);
    cout << "\nThe Terminating process is: " << process_name << "and priority: " << priority <<"."  <<"\n";
        Sleep(TRUE);
        isFinished = true;
}

Process :: Process(char* name, int priority) {
    //Constructor
    this->process_name = name;
    this->priority = priority;
    childTh = new List<Thread*>();
    childPr = new List<Process*>();
    processStatus = PROCESS_JUST_CREATED;
    process_ID = processID_Counter++;
    //this->scheduler = new Scheduler();
    Thread *th = new Thread("Main Thread");
    this->currentThread = th;
    this->scheduler = new Scheduler();
}

Process :: ~Process() {
    //Destructor
    delete childPr;
    delete childTh;
    delete scheduler;
    delete currentThread;
}
void Process::createChThread(VoidFunctionPtr func, void *args) {
    Thread *thread = new Thread("Child Thread");
    thread->Fork(this, func, args);
    childTh->Append(thread);
}

void Process::createChProcess() {
   // cout << "\n-----------------IN PROCESS CHILDPROCESS----------------------\n";
cout << "\n\n";
    Process *process = new Process("Child Process" , this->priority);
    process->Fork(this->functionPointer, this->args);
    process->parent_ID = this->parent_ID;
    childPr->Append(process);
    cout << "The child process: " << process->getProcessName() << "of the parent process" << this->getProcessName() <<  " with priority: " << priority <<" is forked. \n";
}






