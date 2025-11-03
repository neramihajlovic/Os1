#include "test3Barijera.hpp"

static SyncBarrier* bar3 = nullptr;
static volatile int finished = 0;

class Nit1 : public Thread {
public:
    void run() override {
        printString("Nit1 pocela\n");
        for (int i = 0; i < 3; i++) {
            printString("Nit1 stigla do barijere\n");
            bar3->pass();
            printString("Nit1 prosla barijeru\n");
        }
        finished++;
    }
};

class Nit2 : public Thread {
public:
    void run() override {
        printString("Nit2 pocela\n");
        for (int i = 0; i < 3; i++) {
            for (volatile int j = 0; j < 50000; j++); // malo kasni
            printString("Nit2 stigla do barijere\n");
            bar3->pass();
            printString("Nit2 prosla barijeru\n");
        }
        finished++;
    }
};

class Nit3 : public Thread {
public:
    void run() override {
        printString("Nit3 pocela\n");
        for (int i = 0; i < 3; i++) {
            for (volatile int j = 0; j < 100000; j++); // još više kasni
            printString("Nit3 stigla do barijere\n");
            bar3->pass();
            printString("Nit3 prosla barijeru\n");
        }
        finished++;
    }
};

void testBar3() {
    printString("Pokrecem test sa 3 barijere...\n");
    bar3 = new SyncBarrier(3);

    Thread* t1 = new Nit1();
    Thread* t2 = new Nit2();
    Thread* t3 = new Nit3();

    t1->start();
    t2->start();
    t3->start();


    while (finished < 3) {
        Thread::dispatch();
    }

    printString("Sve niti zavrsile test!\n");

    delete t1;
    delete t2;
    delete t3;
    delete bar3;
}
