//
// Created by os on 10/9/25.
//
#include "../h/syscall_cpp.hpp"
#include    "printing.hpp"
#ifndef PROJECT_BASE_BARIJERA_HPP
#define PROJECT_BASE_BARIJERA_HPP

class Barrier {
public:
    Barrier (int open=1);
    void open();
    void close();
    void pass();
private:
    Semaphore* sem;
    int opened;
    TCB* owner;
    bool has_blk;
};

#endif //PROJECT_BASE_BARIJERA_HPP