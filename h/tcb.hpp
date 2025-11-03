#ifndef PROJECT_BASE_TCB_H
#define PROJECT_BASE_TCB_H

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "MemAllocator.hpp"

typedef TCB* thread_t;
// Thread Control Block
class TCB
{
public:
    void* operator new(size_t size) {
        return MemAllocator::mem_alloc(size);
    }
    void* operator new[](size_t size) {
        return MemAllocator::mem_alloc(size);
    }
    void operator delete(void *p) {
        MemAllocator::mem_free(p);
    }
    void operator delete[](void *p) {
        MemAllocator::mem_free(p);
    }



    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }
    void setBlocked(bool value) { blocked = value; }


    uint64 getTimeSlice() const { return timeSlice; }
    int time_sleep(){ return 0;}

    using Body = void (*)(void*);


    static TCB *createThread(Body body, uint64* stack, void* arg);
    static TCB *createThread(Body body, void* arg);

    static int killThread();

    static void dispatch();

    static void yield();

    static TCB *running;


    int getId() const { return id; }      //getid

    static void setMaximumThreads(int num_of_threads);
    static int GetMaximumThreads();
    static void threadFinished();

    static void join(thread_t* t);
    void releaseJoin();

    static void joinAll();
    static void incNumOfChildren() { TCB::running->numOfChildren++; }
    void releaseJoinAll();

    bool isPinged(){return pinged;}
    void setPinged(bool p) { this->pinged = p; }
    void allocatedNumOfBlocks(size_t bytes){
        this->numOfBlocks += (bytes + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }



    // Getter samo za stack pointer
    uint64 getSP() const {return context.sp;}
    uint64* getStack() const { return stack;}
    Body getBody() const {return body;}

private:
    TCB() = default;
    TCB(Body body, uint64 timeSlice, uint64* stack, void* arg) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({body != nullptr ? (uint64) threadWrapper : 0,
                     body != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false),
            blocked(false),
            arg(arg),
            parent(nullptr),
            numOfChildren(0),
            pinged(false),
            numOfBlocks(0)
    {
        id = cnt++;

        if(TCB::running ){
            TCB::running->children.addLast(this);
            incNumOfChildren();
            this->parent = TCB::running;
        }

        if (body != nullptr) {
            Scheduler::put(this);   //ubacujemo u scheduler
            //printString("Put in Scheduler from constructor of TCB");

        }

    }
    TCB(Body body, uint64 timeSlice, void* arg) :
           body(body),
           stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
           context({(uint64) &threadWrapper,
                    stack != nullptr ? ((uint64) &stack[STACK_SIZE]) : 0
                   }),
           timeSlice(timeSlice),
           finished(false),
           blocked(false),
           arg(arg),
           parent(nullptr),
           numOfChildren(0),
           pinged(false),
           numOfBlocks(0)
    {
        id = cnt++;
        if(TCB::running ){
            TCB::running->children.addLast(this);
            incNumOfChildren();
            this->parent = TCB::running;
        }
        if (body != nullptr) {    //ukloni za setmax
            Scheduler::put(this);
        }

    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    void* arg;




    friend class Riscv;
    // friend class MySemaphore;
    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static int cnt;
    int id;

    static int max_threads;
    static int active_threads ;//koliko ih ima trenutno
    static List<TCB> waiting_threads;

    List<TCB> joined;
    //join all
    TCB* parent;
    List<TCB> children;
    int numOfChildren;
    //ping
    bool pinged;
    int numOfBlocks;

    //static void dispatch();  // da li u private ili public delu

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //PROJECT_BASE_TCB_H