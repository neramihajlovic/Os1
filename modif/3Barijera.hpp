//
// Created by os on 10/9/25.
//

#ifndef PROJECT_BASE_3BARIJERA_HPP
#define PROJECT_BASE_3BARIJERA_HPP



#include "../h/syscall_cpp.hpp"


class SyncBarrier {
public:
    SyncBarrier(int n);

    void pass();

private:
    int total;          // koliko niti mora da stigne
    int arrived;        // koliko je do sada stiglo
    Semaphore* sem;     // blokira niti dok sve ne stignu
    Semaphore* lock;    // zaštita pristupa (mora da postoji i da se alocira)
};


#
#endif //PROJECT_BASE_3BARIJERA_HPP