//
// Created by Sowmya on 10/30/2018.
//

#ifndef NACHOSPA1_PROCESS_H
#define NACHOSPA1_PROCESS_H
#pragma once
#include "../lib/list.h"
#include "../lib/copyright.h"
#include "../lib/utility.h"
#include "../lib/sysdep.h"
#include "../machine/machine.h"
#include "scheduler.h"
#include "kernel.h"
#include "thread.h"
#include "addrspace.h"
//Process state
enum Status_Process{ PROCESS_JUST_CREATED, PROCESS_RUNNING, PROCESS_READY, PROCESS_BLOCKED };
class Scheduler;
class Thread;
class Kernel;

class Process{
private:
    int process_ID;
    int state;
    int priority;

    List<Process* > *childPr;
    char* process_name;
    VoidFunctionPtr functionPointer;
    void *args;
    Status_Process processStatus;
    static int processID_Counter;

public:
    Process(char* name ,int priority);
    ~Process();

    Process* currentProcess;
    Thread* currentThread;
    int setState(int st);
    void Fork(VoidFunctionPtr func, void *arg);
    void Thread_Fork(int count, VoidFunctionPtr thread_func, void *arg);
    bool isFinished;
    void Yield();
    void Terminate();
    void Sleep(bool finishing);
    List<Thread* > *childTh;
    Scheduler *scheduler ;
    int parent_ID;

    int getState() const{
        return state;
    }

    int getProcessID(){
       return process_ID;
    }
    int getProcessPriority() const {
        return priority;
    }
    List<Thread* > getChThread(){
        return *childTh;
    }
    List<Process* > getPrThread(){
        return *childPr;
    }
    void setStatus(Status_Process st) {
        state = st;
    }
    char* getProcessName() {
        return (process_name);
    }
    void Print() {
        cout << process_name;
    }
    void createChThread(VoidFunctionPtr func, void *args);
    void createChProcess();

    bool operator == (Process process){
        if(process.getProcessID() == process.parent_ID){
            return true;
        } else return false;
    }
};
extern void ProcessPrint(Process *process);

#endif //NACHOSPA1_PROCESS_H

