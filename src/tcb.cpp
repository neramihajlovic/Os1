//
// Created by os on 8/14/24.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
int TCB::cnt = 0;

int TCB::max_threads = 5;      // default
int TCB::active_threads   = 0;
List<TCB> TCB::waiting_threads;

TCB *TCB::createThread(Body body, uint64* stack, void* arg)
{
    return (TCB*)new TCB(body, TIME_SLICE, stack, arg);

    //return (TCB*)new TCB();
}
TCB *TCB::createThread(Body body, void* arg)
{
   return (TCB*)new TCB(body, TIME_SLICE, arg);


    /*       //create za setmax
    TCB *t = (TCB*)new TCB(body, TIME_SLICE, arg);

    int id = t->getId();
    if(id==0 ) return t;
    else {
        printString("Created thread: ");
        printInt(id-2);
        printString("\n");
    }
    if(active_threads < max_threads){
        active_threads++;
        Scheduler::put(t);// aktivna nit ide u Scheduler
        printString("Added to scheduler\n");
    }else{
        t->setBlocked(true);
        waiting_threads.addLast(t);// blokirana, čeka mesto
        printString("Blocked and waiting\n");

    }
    return t;*/
}

void TCB::yield()
{
    volatile unsigned int trapCode = 0x13;
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB* old = TCB::running;

    if(old->isPinged()){
        printString("Current thread took: ");
        printInt(old->numOfBlocks);
        printString("num od bloks\n");
        old->setPinged(false);
    }

    if(old != nullptr && !old->isFinished() && !old->isBlocked()){
        Scheduler::put(old);
    }

    running = Scheduler::get();
    contextSwitch(&old->context,&running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);

   // threadFinished();   //setmax threads radi

    // odblokiraj sve niti koje čekaju na ovu nit        modif join
    //running->releaseJoin();
    //running->releaseJoinAll();

    TCB::yield();

}


int TCB::killThread() {
    TCB::running->setFinished(true);
    if (!running->isFinished()) {
        return -1;
    }
    return 0;
}


void TCB::setMaximumThreads(int num_of_threads){
    if(num_of_threads > 0) max_threads = num_of_threads;
    else max_threads = 5;
}
int TCB::GetMaximumThreads(){
    return max_threads;
}
void TCB::threadFinished() {
    active_threads--;
    if(waiting_threads.getFirst()){
        TCB* t = waiting_threads.removeFirst();
        t->setBlocked(false);
        Scheduler::put(t);
        printString("Thread dodata\n");
        active_threads++;
    }
}

void TCB::join(thread_t* t){
    if(!(*t)->isFinished()){
        TCB::running->setBlocked(true);
        (*t)->joined.addLast(TCB::running);
        dispatch();
    }
}
void TCB::releaseJoin() {
    while(this->joined.getFirst()) {
        TCB* thread = this->joined.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }
}
void TCB::joinAll(){
    TCB* t = TCB::running;
    for(int i = 0;i<t->numOfChildren;i++){
        if(t->children.getFirst()){
             t->setBlocked(true);
             TCB* child = t->children.removeFirst();
             child->joined.addLast(t);
             t->children.addLast(child);
        }
        else {break;}
    }
    dispatch();
}
void TCB::releaseJoinAll(){
    while(this->joined.getFirst()) {
        TCB* t = this->joined.removeFirst();
        if(t==this->parent){
            t->numOfChildren--;
            if(t->numOfChildren == 0){
                t->setBlocked(false);
                Scheduler::put(t);
            }
        }
        else{
            t->setBlocked(false);
            Scheduler::put(t);
        }
    }
}