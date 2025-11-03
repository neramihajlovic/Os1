#include "../test/modificationJoin.hpp"
#include "../h/syscall_cpp.hpp"


//Thread* threads[2];
Thread* threads[3];
class MyWorkerA:public Thread{
public:
        MyWorkerA():Thread(){}
        void run() override{
            for (uint64 i = 0; i < 3; i++)
            {
                printString("A: i=");
                printInt(i);
                printString("\n");
                threads[1]->join();
                for (uint64 j = 0; j < 10000; j++)
                {
                    for (uint64 k = 0; k < 30000; k++)
                    {
                        /* busy wait */
                    }
                    Thread::dispatch();
                }
            }
            printString("A finished!\n");
            Thread::dispatch();
        }

};


class MyWorkerB:public Thread{
public:
    MyWorkerB():Thread(){}
    void run() override{
        for (uint64 i = 0; i < 5; i++)
        {
            printString("B: i=");
            printInt(i);
            printString("\n");
            Thread::dispatch();//da bi islo naizmenicno ababab, ako stavimo join u a onda ide samo b pa kad zavrsi onda ide a, ako nema ovoga onda ide abbbbbbaa
            for (uint64 j = 0; j < 10000; j++)
            {
                for (uint64 k = 0; k < 30000; k++)
                {
                    /* busy wait */
                }

                Thread::dispatch();
            }
        }

        printString("B finished!\n");
        Thread::dispatch();
    }
};

class MyWorkerC:public Thread{
public:
    MyWorkerC():Thread(){}
    void run() override{
        //threads[0]->join();
        int i = 7;
        printString("C: i=");
        printInt(i);
        printString("\n");
        printString("C finished!\n");
        Thread::dispatch();
    }
};


void modificationJoin(){
    printString("\n-------Start JOIN modification--------\n");

    threads[0] = new MyWorkerA();
    printString("ThreadA created\n");
    threads[1] = new MyWorkerB();
    printString("ThreadB created\n");

    for(int i = 0; i < 2; i++){
        threads[i]->start();
    }
    //thread_dispatch();
    //printString("\nreturned to main\n");

    threads[0]->join();
    threads[1]->join();

    /*for(auto thread : threads) {
        printString("Inside join\n");
        thread->join();
    }*/
    printString("\n--------After joiiin--------------\n");
    threads[2] = new MyWorkerC();
    printString("ThreadC created\n");
    threads[2]->start();
    threads[2]->join();
    for (auto thread: threads)
        delete thread;

    printString("\n----------Finished JOIN modification----------\n");
}