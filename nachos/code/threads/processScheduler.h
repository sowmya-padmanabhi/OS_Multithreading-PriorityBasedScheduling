//
// Created by sowmy on 11/2/2018.
//

#ifndef NACHOSPA1_PROCESSSCHEDULER_H
#define NACHOSPA1_PROCESSSCHEDULER_H
#pragma once
#include "../lib/list.h"
#include "../lib/copyright.h"
#include "process.h"

class Process;
class ProcessScheduler{
public:
    ProcessScheduler(); //Constructor
    ~ProcessScheduler(); //Destructor

    SortedList<Process *> *process_ReadyList;
    void ProcessReadyToRun(Process *process);
    Process* FindNextProcessToRun();
    void ProcessRun (Process *nextProcess, bool finishing);
    void CheckToBeDestroyed();
    void Print();


private:
    Process *processToBeDestroyed;
};

#endif //NACHOSPA1_PROCESSSCHEDULER_H
