#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler{
private:
    static List<TCB> readyThreadQueue;

public:
    static int num;    //vraca broj koliko ima niti blokiranih

    static void put(TCB *ccb);

    static TCB *get();

    static TCB *head();


    //deadlock
    static int maks;
    static int brojNiti(){
        return readyThreadQueue.duzina();
    }
};

#endif //PROJECT_BASE_SCHEDULER_HPP
