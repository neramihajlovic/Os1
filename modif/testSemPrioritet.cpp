
#include "testSemPrioritet.hpp"
#include "../h/MySemaphore.hpp"

MySemaphore* sem = nullptr;
class NitNera: public Thread {
public:
    NitNera() : Thread(){}
    void run() override {
        int id=getThreadId();
        for (uint64 i = 0; i < 5; i++) {
            sem->sem_wait();
            printString("ID=  "); printInt(id);
            printString("  i= "); printInt(i);
            printString("\n");
            for (uint64 j = 0; j < 10000; j++) {
                for (uint64 k = 0; k < 30000; k++) { }
                thread_dispatch();
            }
            sem->sem_signal();
            thread_dispatch();
        }
        printString("ID=  "); printInt(id);
        printString(" finished!\n");
    }
};
void testSemPrioritet() {
    printString("\n=== TEST PRIORITETNOG SEMAFORA  ===\n");
    /*TCB::setMaximumThreads(10);printString("Max Threads : ");
    printInt(TCB::GetMaximumThreads());printString("\n");*/
    sem = MySemaphore::sem_open(5);
    sem->setPriorityMode(true);
    Thread* thread[50];

    for (int i = 0; i < 50; ++i) {
        thread[i]=new NitNera();
    }
    for (int i = 0; i < 50; ++i) {
        thread[i]->start();
    }

    for(int i=0; i<50; ++i){
        while(thread[i]->getHandle() &&
            !thread[i]->getHandle()->isFinished()){
            thread_dispatch();
        }
    }
    for (int i = 0; i < 50; ++i) {
        delete thread[i];
    }
    sem->sem_close();
    delete sem;
    printString("=== TEST ZAVRŠEN ===\n");
}