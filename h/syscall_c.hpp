//
// Created by os on 9/17/24.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "MySemaphore.hpp"

void* mem_alloc(size_t size);

int mem_free(void* ptr);

size_t mem_get_free_space();

size_t mem_get_largest_free_block();

//Threads NEEEEKIIII
class TCB;
typedef TCB* thread_t;

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit ();
void thread_dispatch ();

int thread_id();

void thread_join(thread_t* handle);
void thread_join_all();

void ping(thread_t handle);


class MySemaphore;
typedef MySemaphore* sem_t;

int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;

char getc ();
void putc (char);

#endif //PROJECT_BASE_SYSCALL_C_HPP
