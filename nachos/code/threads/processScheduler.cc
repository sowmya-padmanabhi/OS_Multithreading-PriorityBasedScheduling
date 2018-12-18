//
// Created by sowmy on 11/2/2018.
//
#include "../lib/debug.h"
#include "processScheduler.h"
#include "main.h"
//-------------------------------------------------------------------------------------------------------------------
//A compare function for the processes to check the priorities between processes and execute them based on priority
//-------------------------------------------------------------------------------------------------------------------
int process_compare(Process *process1, Process *process2){
    if(process1->getProcessPriority() < process2->getProcessPriority()){
        return 1;
    }
    else
    if(process1->getProcessPriority() > process2->getProcessPriority()){
        return -1;
    }
    else{
        return 0;
    }
}
//-------------------------------------------------------------------------------------------------------------------
//ProcessScheduler constructor
//-------------------------------------------------------------------------------------------------------------------
ProcessScheduler::ProcessScheduler()
{
    process_ReadyList = new SortedList<Process*>(process_compare);
    processToBeDestroyed = NULL;
}
//-------------------------------------------------------------------------------------------------------------------
//ProcessScheduler Destrustor
//-------------------------------------------------------------------------------------------------------------------
ProcessScheduler::~ProcessScheduler() {
    delete process_ReadyList;
}
//-------------------------------------------------------------------------------------------------------------------
// Mark as process as ready, but not running.
// Put it in the ready list, for later scheduling.
//-------------------------------------------------------------------------------------------------------------------
void
ProcessScheduler::ProcessReadyToRun(Process *process){
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting process on ready list: " << process->getProcessName());
//    cout<<"\n Putting process on ready list: " <<process->getProcessName() <<"\n";
    process->setStatus(PROCESS_READY);
    process_ReadyList->Insert(process);
}
//-------------------------------------------------------------------------------------------------------------------
// Return the next process to be scheduled.
// If there are no processes ready, mark as NULL
//-------------------------------------------------------------------------------------------------------------------

Process *
ProcessScheduler::FindNextProcessToRun()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (process_ReadyList->IsEmpty()) {
        return NULL;
    } else {
        return process_ReadyList->RemoveFront();
    }

}
//-------------------------------------------------------------------------------------------------------------------
// Save the state of the old Process, and load the state of the new Process, by calling the machine dependent context
// switch routine, SWITCH.
//-------------------------------------------------------------------------------------------------------------------

void 
ProcessScheduler::ProcessRun(Process *nextProcess, bool finishing){
    Thread *oldThread = kernel->currentProcess->currentThread;
    Process *oldProcess = kernel->currentProcess;
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
        ASSERT(processToBeDestroyed == NULL);
        processToBeDestroyed = oldProcess;
    }
    if(oldThread->space!=NULL){
        oldThread->SaveUserState();
        oldThread->space->SaveState();
    }
    oldThread->CheckOverflow();

    Thread *nextTh = nextProcess->currentThread;
    //DEBUG(dbgThread, "Switching from : " << oldProcess->getProcessName());
    kernel->currentProcess = nextProcess;
    nextProcess->setStatus(PROCESS_RUNNING);
    nextTh->setStatus(RUNNING);
    //nextProcess->currentThread->setStatus(RUNNING);
    SWITCH(oldThread,nextTh);
    
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    DEBUG(dbgThread, "Now in thread: " << oldProcess->getProcessName());
    
    CheckToBeDestroyed();
    if (oldThread->space != NULL) {
        oldThread->RestoreUserState();
        oldThread->space->RestoreState();
    }
}
//-------------------------------------------------------------------------------------------------------------------
// To check if a non finishing process still exists and if it does, destroy it.
//-------------------------------------------------------------------------------------------------------------------
void
ProcessScheduler::CheckToBeDestroyed()
{  if ( processToBeDestroyed != NULL){
        delete  processToBeDestroyed;
        processToBeDestroyed = NULL;
    }
}

void
ProcessScheduler::Print()
{
    cout << "Ready list contents of PROCESS:\n";
    process_ReadyList->Apply(ProcessPrint);
}
