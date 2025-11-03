//
// Created by os on 9/22/25.
//
#include "../test/IDpromKonteksta_test.hpp"
//#include "../h/tcb.hpp"

const int N= 10;
static Semaphore* semForAll;

struct Matrica{
    int matrica[N][N];
    int rowSum[N]; //svaki red cuva sumu
};

class Nit: public Thread{
    Matrica *m;
    int id;//prosledimo red za sum
public:
    Nit(Matrica *m, int id):Thread(), m(m), id(id) {}

    void run() override{
        int sum = 0;
        for (int j = 0; j < N; j++){
            sum += m->matrica[id][j];
        }
        m->rowSum[id] = sum;
        printString("Nit ");
        printInt(id);
        printString(" finished, sum = ");
        printInt(sum);
        printString("\n");
    }
};



void TestMatrica(){
    Matrica m;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
                m.matrica[i][j] = 1;
        }
          m.rowSum[i] = 0;
    }
    int i = 0;
    for (i = 0; i < 10; i++) {
        Thread* t = new Nit(&m,i);
        t->start();    //poziva run pokrece nit
    }
    bool done = false;
    while (!done){
        done = true;
        for (int i = 0; i < 10; i++) {
            if(m.rowSum[i] == 0){//nije zavrsila jos ta nit
                done = false;
                break;
            }
        }
        thread_dispatch();
    }
    int konacno = 0;
    for (int i = 0; i < 10; i++) {
        konacno += m.rowSum[i];
    }
    printString("Konacan rez: ");
    printInt(konacno);
    printString("\n");

}


/*====================================================================================================================                SET MAX threads */
void workerBody(void *arg){

    int id = Thread::getThreadId();
    printString("nit : ");
    printInt(id-2);
    printString("\n");
    for (int i = 0; i < 5; i++) {
        printString("Hello! Thread id : "); printInt(id-2); printString("\n");
    }
    for(uint64 j = 0; j < 4000; j++) {
        for (uint64 k = 0; k < 200000; k++) {  }
        thread_dispatch();
    }
    printString("iz workerBody finished!");printInt(id-2); printString("\n");
}
class workerBodyNN: public Thread{
    workerBodyNN():Thread(){}
void run() override{
        int id = Thread::getThreadId();
        printString("nit : ");
        printInt(id-2);
        printString("\n");
        for (int i = 0; i < 5; i++) {
            printString("Hello! Thread id : "); printInt(id-2); printString("\n");
        }
        for(uint64 j = 0; j < 4000; j++) {
            for (uint64 k = 0; k < 200000; k++) {  }

        }
        Thread::dispatch();
        printString("iz workerBody finished!");printInt(id-2); printString("\n");
}


};

void MaxThreadTest() {
    printString("default : ");
    printInt(TCB::GetMaximumThreads());
    printString("\n");
    TCB::setMaximumThreads(3);
    printString("Max Threads : ");
    printInt(TCB::GetMaximumThreads());
    printString("\n");

    for (int i = 0; i < 20; i++) {
        TCB::createThread(workerBody,nullptr);
    }
    while (true) {
        thread_dispatch();
    }

}
/*====================================================================================================================                  thread ID */

void workerBodyN(void* arg) {

    int id = Thread::getThreadId();
    for (uint64 i = 0; i < 5; i++) {
        printString("N: id = "); printInt(id-2);  printString("\n");
        for (uint64 j = 0; j < 4000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /*busy wait*/ }
            thread_dispatch();
        }
    }
    /*int id = Thread::getThreadId();
        printString("Thread ID je: ");
        printInt(id); printString("\n");*/
    printString("iz workerBody N finished!\n");
}
void workerBodyS(void* arg) {

    int id = Thread::getThreadId();
    for (int i = 0; i < 5; i++) {
        printString("S: id = ");printInt(id-2);; printString("\n");
        for (uint64 j = 0; j < 4000; j++) {
            for (uint64 k = 0; k < 30000; k++) {  /*busy wait*/  }
            thread_dispatch();
        }
    }
    /*int id = Thread::getThreadId();
    printString("Thread ID je: ");
    printInt(id); printString("\n");*/
    printString("iz workerBody S finished!\n");
}

void workerBodyD(void* arg) {

    int id = Thread::getThreadId();

    for (int i = 0; i < 5; i++) {
        printString("D: id = ");printInt(id-2);; printString("\n");
        for (uint64 j = 0; j < 4000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */}
            thread_dispatch();
        }
    }
    /*int id = Thread::getThreadId();
    printString("Thread ID je: ");
    printInt(id); printString("\n");*/
    printString("iz workerBody D finished!\n");
}
class WorkerN:public Thread{
public:
    WorkerN():Thread(){}
    void run() override{
        int id = Thread::getThreadId();

        for (int i = 0; i < 5; i++) {
            printString("N: id = ");printInt(id-2);; printString("\n");
            for (uint64 j = 0; j < 4000; j++) {
                for (uint64 k = 0; k < 30000; k++) { /* busy wait */}
                Thread::dispatch();
            }
        }
        printString("iz workerBody N finished!\n");
        Thread::dispatch();
        semForAll->signal();

    }

};
class WorkerS:public Thread{
public:
    WorkerS():Thread(){}
    void run() override{
        int id = Thread::getThreadId();

        for (int i = 0; i < 5; i++) {
            printString("S: id = ");printInt(id-2);; printString("\n");
            for (uint64 j = 0; j < 4000; j++) {
                for (uint64 k = 0; k < 30000; k++) { /* busy wait */}
                Thread::dispatch();
            }
        }
        printString("iz workerBody S finished!\n");
        Thread::dispatch();
        semForAll->signal();

    }

};
class WorkerDD:public Thread{
public:
    WorkerDD():Thread(){}
    void run() override{
        int id = Thread::getThreadId();

        for (int i = 0; i < 5; i++) {
            printString("D: id = ");printInt(id-2);; printString("\n");
            for (uint64 j = 0; j < 4000; j++) {
                for (uint64 k = 0; k < 30000; k++) { /* busy wait */}
                Thread::dispatch();
            }
        }
        printString("iz workerBody D finished!\n");
        Thread::dispatch();
        semForAll->signal();

    }

};


void IDpromKonteksta(){
    printString("Created thread ID: ");
    printInt(Thread::getThreadId());
    printString("\n");
    semForAll= new Semaphore(0);
    Thread *threads[3];
    threads[0] = new WorkerN();

    threads[1] = new WorkerS();
    threads[2] = new WorkerDD();

    for (int i = 0; i < 3; i++) {
        threads[i]->start();
    }
    for(int i = 0; i < 3; i++) {
        printString("\nWaiting on semaphore\n");
        semForAll->wait();
        thread_dispatch();
    }
    for (auto thread : threads) {
        delete thread;
    }
    delete semForAll;
    printString("Sve niti su zavrsile!\n");

/*
    printString("Created thread ID: ");
    printInt(Thread::getThreadId());
    printString("\n");

    TCB *threads[3];

    threads[0] =TCB::createThread(workerBodyN, nullptr);
    printString("fja Thread N created\n");

    threads[1] = TCB::createThread(workerBodyS, nullptr) ;
    printString("fja Thread S created\n");

    threads[2] = TCB::createThread(workerBodyD, nullptr);
    printString("fja Thread D created\n");


    while (!(threads[0]->isFinished() &&
            threads[1]->isFinished()&& threads[2]->isFinished()))
    {
        TCB::yield();
    }
    printString("Sve niti su zavrsile!\n");
*/

}