//
// Created by os on 9/17/24.
//

#include "../h/syscall_c.hpp"
#include"../test/printing.hpp"
void* mem_alloc(size_t size) {
    volatile unsigned int trapCode = 0x01;
    size_t realSize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %[realSize]" : : [realSize] "r"(realSize));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");

    void* result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;

}
int mem_free(void* ptr) {
    volatile unsigned int trapCode = 0x02;
    //prvo veci reg, poslednji je a0, da ne pregazimo nesto
    __asm__ volatile("mv a1, %[ptr]" : : [ptr] "r"(ptr));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");

    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}
size_t mem_get_free_space() {
    volatile unsigned int trapCode = 0x03;

    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}
size_t mem_get_largest_free_block() {
    volatile unsigned int trapCode = 0x04;

    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
    uint64 result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

//threads
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg) {
    volatile unsigned int trapCode = 0x11;

    //uint64* stack = new uint64[DEFAULT_STACK_SIZE];
    uint64* stack;
    if (start_routine != nullptr) {
        //blocks = (DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
        //stack = (uint64*) MemAllocator::mem_alloc(blocks);
        //stack = (uint64*) MemAllocator::mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64) /  MEM_BLOCK_SIZE + 1);
        stack = new uint64[DEFAULT_STACK_SIZE / sizeof(uint64)];

        //stack = (uint64*) mem_alloc(DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE + 1);
        //stack = (uint64*) mem_alloc(DEFAULT_STACK_SIZE);
        //(uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64) /  MEM_BLOCK_SIZE + 1);
    } else {
        stack = nullptr;
    }

    __asm__ volatile("mv a4, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile("mv a3, %[stack]" : : [stack] "r"(stack));
    __asm__ volatile("mv a2, %[arg]" : : [arg] "r"(arg));
    __asm__ volatile("mv a1, %[start_routine]" : : [start_routine] "r"(start_routine));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));

    __asm__ volatile ("ecall");
    //printString("\nreturned from ecall");
    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));
    return result;
}

int thread_exit () {
    volatile unsigned int trapCode = 0x12;
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));

    __asm__ volatile("ecall");
    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));
    return result;

}
void thread_dispatch () {
    volatile unsigned int trapCode = 0x13;
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}



int sem_open (sem_t* handle, unsigned init) {
    volatile unsigned int trapCode = 0x21;

    __asm__ volatile("mv a2, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile("mv a1, %[init]" : : [init] "r"(init));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));

    __asm__ volatile("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}
int sem_close (sem_t id) {
    volatile unsigned int trapCode = 0x22;

    __asm__ volatile("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}
int sem_wait (sem_t id) {
    volatile unsigned int trapCode = 0x23;

    __asm__ volatile("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}
int sem_signal (sem_t id) {
    volatile unsigned int trapCode = 0x24;

    __asm__ volatile("mv a1, %[id]" : : [id] "r"(id));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

int time_sleep(time_t time) {
    volatile unsigned int trapCode = 0x31;

    __asm__ volatile("mv a1, %[time]" : : [time] "r"(time));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

char getc () {

	/*asm volatile("li a0, 0x41");
    asm volatile("ecall");

    uint64 ret_value;
    asm volatile("mv %0, a0" : "=r"(ret_value));

    return (char)ret_value;*/
    volatile unsigned int trapCode = 0x41;

    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");

    char ret_char;
    __asm__ volatile("mv %[ret_char], a0" : [ret_char] "=r"(ret_char));

    return ret_char;
    // return 'N';
}

void putc (char c) {
    volatile unsigned int trapCode = 0x42;

    __asm__ volatile("mv a1, %[c]" : : [c] "r"(c));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
    //::putc(c);
}

int thread_id () {
    volatile unsigned int trapCode = 0x50;
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");

    uint64 volatile result;
    __asm__ volatile("mv %[result], a0" : [result] "=r"(result));

    return result;
}

/* //za sis poziv
void thread_setmax(int num) {
    volatile unsigned int trapCode = 0x51;
    __asm__ volatile("mv a1, %[num]" : : [num] "r"(num));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}*/
void thread_join (thread_t* handle) {
    volatile unsigned int trapCode = 0x52;
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}
void thread_join_all(){
    printString("inside join all\n");
    volatile unsigned int trapCode = 0x53;
     __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}
void ping(thread_t handle){
    printString("inside ping all\n");
    volatile unsigned int trapCode = 0x54;
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r"(handle));
    __asm__ volatile("mv a0, %[trapCode]" : : [trapCode] "r"(trapCode));
    __asm__ volatile ("ecall");
}
