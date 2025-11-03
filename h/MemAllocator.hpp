#ifndef PROJECT_BASE_MEMALLOCATOR_H
#define PROJECT_BASE_MEMALLOCATOR_H


#include "../lib/console.h"
#include "../lib/hw.h"

struct FreeMem{
    size_t size;
    FreeMem* next;
};
const int FREE_MEM_CHUNK = 16;

class MemAllocator{
private:
    static FreeMem* mem_free_head;
    static int isInitialised ;

public:

//    MemAllocator();
//    ~MemAllocator();

    static void initMem(){
        mem_free_head = (FreeMem*) HEAP_START_ADDR;
        mem_free_head->next = nullptr;
        mem_free_head->size = (char*) HEAP_END_ADDR  - (char*) HEAP_START_ADDR ;   //sizeof(Freemem)

    }


    static void* mem_alloc(size_t sz);
    static int mem_free(void* memAloc);

    static void tryToJoin(FreeMem* cur);

	static size_t mem_get_free_space();
	static size_t mem_get_largest_free_block();



//
//    void probaj(){
//        __putc('N');
//        __putc('\n');
//        __putc('E');
//        __putc('\n');
//        __putc('R');
//        __putc('\n');
//        __putc('A');
//        __putc('\n');
//    }
};

#endif //PROJECT_BASE_MEMALLOCATOR_H
