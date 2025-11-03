//
// Created by os on 10/19/25.
//
#include "testResource.hpp"
#include "printing.hpp"
#include "resource.hpp"


bool finishedA = false;
bool finishedB = false;
bool finishedC = false;


class A: public Thread{
public:
    Resource* resource;
    A(Resource* r):Thread() {this->resource = r;}

    void run() override {
        workA(resource);
    }

    void workA(Resource* r) {
        r->take(10);
        printString("A: uzeo sam 10\n");
        r->take(10);
        printString("A: uzeo sam 10\n");
        thread_dispatch();
        r->give_back(20);
        printString("A: Vratio sam 20\n");
        thread_dispatch();
        r->give_back(20);
        printString("A: Vratio sam 20\n");
        thread_dispatch();
        printString("A: Finished!\n");
        finishedA = true;
    }
};

class B: public Thread{
public:
    Resource* resource;
    B(Resource* r):Thread() {this->resource = r;}

    void run() override {
        workB(resource);
    }

    void workB(Resource* r) {
        r->take(20);
        printString("B: uzeo sam 20\n");
        //thread_dispatch();
        r->take(20);
        printString("B: uzeo sam 20\n");
        thread_dispatch();
        r->give_back(40);
        printString("B: Vratio sam 40\n");
        printString("B: Finished!\n");
        finishedB = true;
    }
};

class C: public Thread{
public:
    Resource* resource;
    C(Resource* r):Thread() {this->resource = r;}

    void run() override {
        workC(resource);
    }

    void workC(Resource* r) {
        r->take(30);
        printString("C: uzeo sam 30\n");
        thread_dispatch();
        r->give_back(30);
        printString("C: Vratio sam 30\n");
        printString("C: Finished!\n");
        finishedC = true;
    }
};


void testResource() {
    Thread* threads[3];
    auto * resource = new Resource(50);
    threads[0] = new A(resource);
    threads[1] = new B(resource);
    threads[2] = new C(resource);

    threads[0]->start();
    threads[1]->start();
    threads[2]->start();

    while(!finishedA && !finishedB && !finishedC) {
        printString("\nWaiting for all to finish\n");
        thread_dispatch();
    }
    printString("Main Resourced finished\n");
}

