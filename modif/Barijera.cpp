#include "Barijera.hpp"
Barrier::Barrier (int open){
    this->opened = open;
    this->owner = TCB::running;
    sem = new Semaphore(0);
    this->has_blk = false;
}
void Barrier::open(){
    opened = 1;
    if(has_blk) sem->signal();
    has_blk = 0;
}
void Barrier::close(){
    opened = 0;
}
void Barrier::pass(){
    if(TCB::running != owner) return;
    if(opened == 0){
        has_blk = true;
        sem->wait();
    }
}

