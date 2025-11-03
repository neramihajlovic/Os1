#include "testBarijera.hpp"

Semaphore* sem = nullptr;
Barrier* bar = nullptr;

static int flag = 0;
static int i = 0;
class NitA : public Thread{
    void run() override{
        printString("A started\n");
        while(flag == 0) Thread::dispatch();
        for (int i = 0; i < 10; ++i) {
            bar->open();
            printString("A open bar\n");
            bar->pass();
            printString("A pass bar\n");
            sem->wait();
            printString("A wait\n");
        }
        i++;

    }
};
class NitB : public Thread{
    void run() override{
        printString("B started\n");
        sem = new Semaphore(0);
        bar = new Barrier(1);
        flag = 1;
        for (int i = 0; i < 10; ++i) {
            bar->pass();
            printString("B pass bar\n");
            bar->close();
            printString("B close bar\n");
            sem->signal();
            printString("B signal\n");
        }
        i++;
    }
};
void testBar(){
    printString("test bar\n");
    Thread* t1 = new NitA();
    Thread* t2 = new NitB();
    t1->start();
    t2->start();
    while(i!=2)Thread::dispatch();
}