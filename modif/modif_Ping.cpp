//
// Created by os on 9/25/25.
//
#include "modif_Ping.hpp"
static Semaphore* semForAll;
class MyWorkerA:public Thread{
public:
    MyWorkerA():Thread(){}
    void run() override{
    for(int i=0;i<10;i++){
        printString("A: ");
        printInt(i);
        printString("\n");
        for(uint64 j=0;j<100000;j++){ /*busy wait*/}
        Thread::dispatch();
    }
    semForAll->signal();
    }
};
class MyWorkerB:public Thread{
public:
    MyWorkerB():Thread(){}
    void run() override{
        for(int i=0;i<10;i++){
            printString("B: ");
            printInt(i);
            printString("\n");
            for(uint64 j=0;j<100000;j++){ /*busy wait*/}
            Thread::dispatch();
        }
        semForAll->signal();
    }
};
class MyWorkerC:public Thread{
private:
    Thread* myHandle;
public:
    MyWorkerC(Thread* t):Thread(){
    this->myHandle = t;
    }
    void run() override{
        for(int i=0;i<10;i++){
            printString("C: ");
            printInt(i);
            printString("\n");
            for(uint64 j=0;j<100000;j++){ /*busy wait*/}
            if(i==5){
                myHandle->pingThread();
            }
            Thread::dispatch();
        }
        semForAll->signal();
    }
};


void modif_Ping(){
    printString("\n----Start modification Ping---------\n");
    semForAll=new Semaphore(0);
    Thread* threads[3];

     threads[0]= new MyWorkerA();
     threads[1]= new MyWorkerB();
     threads[2]= new MyWorkerC(threads[0]);

    for(int i=0;i<3;i++){
        printString("\ninside staring\n");
        threads[i]->start();
    }
    for(int i=0;i<3;i++){
        semForAll->wait();
        thread_dispatch();
    }
    for(auto thread:threads){
        delete thread;
    }
    delete semForAll;
    printString("\n----Finishing modification Ping---------\n");

}