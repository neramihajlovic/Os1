//
// Created by os on 9/17/24.
//

#ifndef PROJECT_BASE_MYSEMAPHORE_HPP
#define PROJECT_BASE_MYSEMAPHORE_HPP

#include "tcb.hpp"
class MySemaphore {
public:
    void *operator new(size_t n)
    {
        //return __mem_alloc(n);
        //printInteger(n);
        return MemAllocator::mem_alloc(n);

    }
    void *operator new[](size_t n)
    {
        //return __mem_alloc(n);
        //printStr("\nSize passed to the new operator");
        //printInteger(n);
        return MemAllocator::mem_alloc(n);

    }
    void operator delete(void *p)
    {
        // mem_free(p);
        MemAllocator::mem_free(p);
    }
    void operator delete[](void *p)
    {
        //mem_free(p);
        MemAllocator::mem_free(p);
    }



    MySemaphore(int val = 1) {
        this->val = val;
        this->open = true;
        priority_sem=false;


        this->owner = TCB::running;
        this->owner_mode = false;
    }

    //static MySemaphore* init_sem(int value);
    static MySemaphore* sem_open(int value);
    int sem_close();
    int sem_signal();
    int sem_wait();

  static MySemaphore* create(int initVal) {
        MySemaphore* sem = MySemaphore::sem_open(initVal);  // prosledi argument initVal
        return sem;
    }

    static void destroy(MySemaphore* sem) {
        if (!sem) return;
        sem->sem_close();
    }

    void setPriorityMode(bool on) { priority_sem = on; }

    void setOwnerMode(bool on) { owner_mode = on; }
    bool isOwnerMode() const { return owner_mode; }

private:
    List<TCB> blocked_threads;
    bool open;
    int val;
    bool priority_sem;


    bool owner_mode;
    TCB* owner;

};


#endif //PROJECT_BASE_MYSEMAPHORE_HPP
