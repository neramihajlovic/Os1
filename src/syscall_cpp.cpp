//
// Created by os on 9/19/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

Thread::Thread(void (*body)(void*), void* arg) : body(body), arg(arg)  {
    thread_create(&this->myHandle, body, arg);
}

Thread::Thread() {
    this->body = Thread::thread_wrapper;    //implicit conversion, the same as: this->body = &Thread::thread_wrapper;
    this->arg = this;
}

int Thread::start () {
    thread_create(&this->myHandle, body, arg);
    return 0;
}

void Thread::dispatch () {
    thread_dispatch();
}
int Thread::sleep(time_t) {
    return 0;
}

Thread::~Thread() {
    operator delete (this->myHandle);
}
int Thread::getThreadId() {
    return thread_id();
}
void Thread::join(){
    thread_join(&this->myHandle);
}
void Thread::joinAll(){
    thread_join_all();
}
void Thread::pingThread(){
     ping(this->myHandle);
}

//semaphores

Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    sem_wait(this->myHandle);
    return 0;
}

int Semaphore::signal() {
    sem_signal(this->myHandle);
    return 0;
}


char Console::getc() {
    //::getc(); 0
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}