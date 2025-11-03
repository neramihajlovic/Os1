//
// Created by os on 10/19/25.
//

#include "resource.hpp"
#include "printing.hpp"

Resource::Resource(int n)  {
    this->num_of_instances = n;
    this->free_resource = n;
    this->busy_resource = 0;
    this->sem = new Semaphore(0);
    this->mutex = new Semaphore(1);
    this->waiting = 0;
}

void Resource::take(int requested) {
    mutex->wait();
    if(requested > num_of_instances) {
        printString("Not enough resources available\n");
        mutex->signal();
        return;
    }
    while(requested > free_resource) {
        waiting++;
        mutex->signal();
        printString("Waiting for enough resources to be available\n");
        sem->wait();
        mutex->wait();
    }

    printString("Enough resources is now available");
    //critical section
    printString("Inside critical section of take()\n");
    free_resource -= requested;
    busy_resource += requested;
    mutex->signal();

}

int Resource::give_back(int requested) {
    mutex->wait();
    if(busy_resource < requested) {
        printString("Error\n");
        mutex->signal();
        return -1;
    }
    //deo sa onim da li je vece od onoga sto je pozajmila nit
    //...
    //...

    busy_resource -= requested;
    free_resource += requested;
    while(waiting) {
        printString("Waiting\n");
        waiting--;
        sem->signal();
    }
    mutex->signal();
    return requested;
}
