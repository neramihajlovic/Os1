//
// Created by os on 8/14/24.
//


#include "../h/scheduler.hpp"


List<TCB> Scheduler::readyThreadQueue;
int Scheduler::num = 0;

int Scheduler::maks=0;

TCB *Scheduler::get()
{
    if (!readyThreadQueue.getFirst()) return nullptr;
    num--;
    if(maks<brojNiti()) maks=brojNiti();//deadlock
    //printString("\nGot into scheduler\n");
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    num++;
    //printString("\nPut from scheduler\n");
    readyThreadQueue.addLast(ccb);
    if(maks<brojNiti()) maks=brojNiti();
}
/*int Scheduler::brojNiti() {
    return num;
}*/

TCB * Scheduler::head(){
    return readyThreadQueue.getFirst();
}