//
// Created by os on 10/9/25.
//

#include "3Barijera.hpp"


SyncBarrier::SyncBarrier(int total)
    : total(total), arrived(0)
{
    sem = new Semaphore(0);
    lock = new Semaphore(1);
}

void SyncBarrier::pass() {
    lock->wait();           // zaštita pristupa
    arrived++;

    if (arrived == total) {
        // sve niti su stigle, otvaramo barijeru
        for (int i = 0; i < total; i++) sem->signal();
        arrived = 0;        // reset ako želimo da se koristi više puta
    }

    lock->signal();
    sem->wait();            // blokira dok se barijera ne otvori
}