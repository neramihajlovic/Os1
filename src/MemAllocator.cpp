//
// Created by os on 8/5/24.
//

#include "../h/MemAllocator.hpp"
#include "../h/tcb.hpp"

FreeMem* MemAllocator::mem_free_head = nullptr;
int MemAllocator::isInitialised = 0;

void MemAllocator::tryToJoin(FreeMem *cur) {

   if(cur && cur->next && (char*)(cur) + cur->size == (char*) (cur->next)){
       cur->size += cur->next->size;
       cur->next = cur->next->next;
       // Spojamo trenutni blok sa sledećim blokom
   }
   /*? cur postoji ==> cur!=nullptr
        // Takođe proverite spajanje sa prethodnim blokom
    FreeMem* prev = nullptr;
    FreeMem* tmp = mem_free_head;
    while (tmp && tmp->next != cur) {
        tmp = tmp->next;
    }
    if (tmp) {
        prev = tmp;
    }
    if (prev && (char*)(prev + prev->size) == (char*)cur) {
        prev->size += cur->size;
        prev->next = cur->next;
        cur = prev;
    }*/
}

void* MemAllocator::mem_alloc(size_t sz){
    // if(mem_free_head == nullptr || sz == 0) return nullptr;
     if(!MemAllocator::isInitialised) {
        MemAllocator::isInitialised = 1;
        MemAllocator::initMem();
    }

//    size_t realsz = (sz + MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;              //cuvam broj bajtova
    size_t realsz = sz + sizeof(struct FreeMem);       //da li ovde ide struct freemem ili samo freemem
    realsz = (realsz + MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;//cuvam u bajtovima


    if(realsz > (size_t )((char*) HEAP_END_ADDR  - (char*) HEAP_START_ADDR) - sizeof(FreeMem)){
        realsz = (size_t )((char*) HEAP_END_ADDR  - (char*) HEAP_START_ADDR) - sizeof(FreeMem);
    }

    FreeMem* cur = mem_free_head;
    FreeMem* prev = nullptr;

    for(;cur != nullptr; prev = cur, cur = cur->next){// trazimo dovoljno veliki blok
        if(cur->size >= realsz) break;       //da li je  velicina cur bloka dovoljna za alokaciju
    }

    if(cur == nullptr) return nullptr; //nema dovoljno slobodnog mesta

    FreeMem* new_free_frag;
    if(cur->size > realsz) {  // / Ako je cur veći od potrebnog, podeli ga na fragmenete
        new_free_frag = (FreeMem*) ((char*)cur + realsz);
        new_free_frag->size = cur->size - realsz;
        new_free_frag->next = cur->next;
        if(prev == nullptr) {
            mem_free_head = new_free_frag;
        }
        else {
            prev->next = new_free_frag;
        }
    }
    else {
        if(prev == nullptr){
            mem_free_head = cur->next;
        }
        else{
            prev->next = cur->next;
        }
    }
//ovo je za ping num of bloks
    if(TCB::running) {
        TCB::running->allocatedNumOfBlocks(realsz);
    }

    cur->size = realsz;

    return (void*)((char*)cur + sizeof(FreeMem)) ;

}

int MemAllocator::mem_free(void *mem) {
    if(!mem || (char*)mem - sizeof(FreeMem)  < HEAP_START_ADDR || (char*)mem > (char*)HEAP_END_ADDR) return -1;

    FreeMem* blk = (FreeMem*)((char*)mem - sizeof(FreeMem));
    FreeMem* tmp = mem_free_head;
    if(tmp == nullptr){
        mem_free_head = blk;
        blk->next = nullptr;
    }
    else if(blk < tmp){
        blk->next = tmp;
        mem_free_head = blk;
    }
    else{
        for(;tmp->next != nullptr; tmp = tmp->next){
            if(tmp<blk && blk<tmp->next) {
                blk->next = tmp->next;
                tmp->next = blk;
                break;
            }
        }
        if(tmp->next == nullptr){
            tmp->next = blk;
            blk->next = nullptr;
        }
    }

    FreeMem* prev = nullptr;
    FreeMem* cur = mem_free_head;

    while (cur && cur != blk) {
        prev = cur;
        cur = cur->next;
    }
    tryToJoin(blk);
    if (prev) {
        tryToJoin(prev);
    }

    return 0;

}
size_t MemAllocator::mem_get_free_space() {
    size_t total = 0;
    FreeMem* cur = mem_free_head;
    while (cur) {
        // sabiramo korisnu memoriju, oduzimamo pocetak za svaki segment
        if(cur->size > sizeof(FreeMem))
            total += cur->size - sizeof(FreeMem);
        cur = cur->next;
    }
    return total;
}

size_t MemAllocator::mem_get_largest_free_block() {
    size_t largest = 0;
    FreeMem* cur = mem_free_head;
    while (cur) {
        if (cur->size > largest)
            largest = cur->size;
        cur = cur->next;
    }
    // if(largest > sizeof(FreeMem))
    //     largest -= sizeof(FreeMem);
    // else
    //     largest = 0;
	largest -= sizeof(FreeMem);//da bismo vratili tacno koliko je u bajtovima korisan prostor,
    return largest;
}







//MemAllocator::MemAllocator() {
//    // Inicijalizacija
//}
//
//MemAllocator::~MemAllocator() {
//    // Oslobađanje resursa
//}