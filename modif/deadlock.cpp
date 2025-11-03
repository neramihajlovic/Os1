//
// Created by os on 9/25/25.
//
#include "deadlock.hpp"

static const int N = 5;
static MySemaphore *sem = nullptr;
static volatile int finishedCount = 0;


class Nit : public Thread {
public:
    Nit(): Thread() {}

    void run() override {
        printString("Nit ");  printString(" pokusava da udje u semafor...\n");

        sem->sem_wait();

        printString("\n\nNit "); printInt((uint64)TCB::running); printString(" je usla u semafor!\n");

        // Simuliraj neki posao
        for (uint64 i = 0; i < 5; i++) {
            printString("Nit "); printInt((uint64)TCB::running); printString(" radi...\n");
            printString("Nit: i="); printInt(i); printString("\n");
            for (uint64 j = 0; j < 10000; j++) {
                for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
                thread_dispatch();

            }
        }

        sem->sem_signal();
        printString("Nit "); printInt((uint64)TCB::running); printString(" zavrsila!\n");

        finishedCount++;
    }
};


void deadlockTest(){
    printString("Pokrecem deadlock test...\n\n");
    sem =  MySemaphore::create(0); // svi ce odmah da se blokiraju

    Thread* threads[N];

    for (int i = 0; i < N; i++) {
        threads[i] = new Nit();
        threads[i]->start();
    }

    while(finishedCount < N){
        thread_dispatch();
    }

    printString("\nSve niti zavrsile!\n");
    for (int i = 0; i < N; i++) {
        delete threads[i];
    }
    MySemaphore::destroy(sem);

}