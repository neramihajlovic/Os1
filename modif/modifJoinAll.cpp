//
// Created by os on 9/25/25.
//
#include "modifJoinAll.hpp"

static Semaphore* semForAll;
class WorkerJoinAllA : public Thread {
public:
    WorkerJoinAllA() : Thread() {}
    void run() override {
        for(int i = 0; i < 10; i++) {
            printString("A: ");
            printInt(i);
            printString("\n");
            for(uint64 j = 0; j < 100000; j++) {
                //busy-wait
            }
            Thread::dispatch();
        }
        semForAll->signal();
    }
};
class WorkerJoinAllB : public Thread {
public:
    WorkerJoinAllB() : Thread() {}
    void run() override {
        for(int i = 0; i < 10; i++) {
            printString("B: ");
            printInt(i);
            printString("\n");
            for(uint64 j = 0; j < 100000; j++) {
                //busy-wait
            }
            Thread::dispatch();
        }
        semForAll->signal();
    }
};
class WorkerJoinAllC : public Thread {
public:
    WorkerJoinAllC() : Thread() {}
    void run() override {
        for(int i = 0; i < 10; i++) {
            printString("C: ");
            printInt(i);
            printString("\n");
            for(uint64 j = 0; j < 100000; j++) {
                //busy-wait
            }
            Thread::dispatch();
        }
        semForAll->signal();
    }
};

void modifJoinAll() {
    printString("\n--------Entering JOINALL modification------\n");

    semForAll = new Semaphore(0);
    Thread* threads[3];
    threads[0] = new WorkerJoinAllA();
    threads[1] = new WorkerJoinAllB();
    threads[2] = new WorkerJoinAllC();

    for(int i = 0; i < 3; i++) {
        printString("\nInside Starting\n");
        threads[i]->start();
    }
    //thread_dispatch();//da ima prva iteracija
    printString("\n------------------------------Before Join All---------------------\n");
    Thread::joinAll();
    printString("\n------------------------------After Join All---------------------\n");


    for(auto thread : threads) {
        delete thread;
    }

    delete semForAll;
}