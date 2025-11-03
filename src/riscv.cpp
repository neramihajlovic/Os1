//
// Created by os on 8/14/24.
//


#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

void Riscv::popSppSpie()
{
//    //sedmi test Nuekii
//    /*if(TCB::running->body) {
//        __asm__ volatile("csrc sstatus, %0" :: "r"(SSTATUS_SPP));
//    }*/
//    __asm__ volatile("csrw sepc, ra");
//    __asm__ volatile("sret");

    Riscv::mc_sstatus(Riscv::SSTATUS_SPP); //podmetnem niti da je vec bila ranije u drugom rezyimu
    __asm__ volatile("csrw sepc, ra");//ulazak u rezim rada
    __asm__ volatile("sret");
}
inline uint64 Riscv::r_a2() {
    uint64 volatile a2;
    __asm__ volatile ("mv %0, a2" : "=r"(a2));
    return a2;
}

inline void Riscv::w_a2(uint64 val) {
    __asm__ volatile ("mv a2, %0" : : "r"(val));
}

inline uint64 Riscv::r_a3() {
    uint64 volatile a3;
    __asm__ volatile ("mv %0, a3" : "=r"(a3));
    return a3;
}

inline void Riscv::w_a3(uint64 val) {
    __asm__ volatile ("mv a3, %0" : : "r"(val));
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    uint64 whichCase  = Riscv::r_a0();
    uint64 volatile sepc = r_sepc() + 4;
    uint64 volatile sstatus = r_sstatus();
    //uint64 stval = r_stval();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        //printStr("pozivamo Ecall\n");
        //MySemaphore* sem_id;     //dodano u svakom sem delu
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        // uint64 volatile sepc = r_sepc() + 4;
        //uint64 sstatus = r_sstatus();

        sstatus = r_sstatus();

        switch (whichCase ) {
            //mem alloc
            case 0x01: {
                size_t size;
                __asm__ volatile("ld %[size], 11 * 8(x8)" : [size] "=r"(size));
                void* ptr;
                ptr = MemAllocator::mem_alloc(size);
                __asm__ volatile("mv t0, %0" : : "r"(ptr));
                __asm__ volatile ("sw t0, 80(x8)"); //load it into the location at which a0 is saved
                break;
            }
            //mem free
            case 0x02: {
                void* ptr;
                __asm__ volatile("ld %[ptr], 11 * 8(x8)" : [ptr] "=r"(ptr));
                int result = MemAllocator::mem_free(ptr);
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result));
                break;
            }
            // mem_get_free_space
            case 0x03:{
                size_t total_free= MemAllocator::mem_get_free_space();
                __asm__ volatile("mv t0, %0" : : "r"(total_free));
                __asm__ volatile("sw t0, 80(x8)"); // upisujemo na adresu gde je a0 sačuvan
                break;
            }
            //mem_get_largest_free_block
            case 0x04:{
                size_t largest= MemAllocator::mem_get_largest_free_block();
                __asm__ volatile("sd %[largest], 10 * 8(x8)" : : [largest] "r" (largest));
                break;
            }
            //thread_create
            case 0x11: {
                using Body = void (*)(void*);
                Body body;
                TCB** handle;
                void *arg;
                uint64 *stack;

                __asm__ volatile("ld %[body], 11 * 8(x8)" : [body] "=r"(body));
                __asm__ volatile("ld %[arg], 12 * 8(x8)" : [arg] "=r"(arg));
                __asm__ volatile("ld %[stack], 13 * 8(x8)" : [stack] "=r"(stack));
                __asm__ volatile("ld %[handle], 14 * 8(x8)" : [handle] "=r"(handle));
                // __asm__ volatile("mv %0, a1" : "=r" (handle));
                // __asm__ volatile("mv %0, a2" : "=r" (body));
                // __asm__ volatile("mv %0, a7" : "=r" (arg));

                *handle = TCB::createThread(body, stack, arg);
                uint64 result = (*handle != nullptr) ? 0 : -1;

                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
            //thread_exit
            case 0x12: {
                uint64 result = TCB::killThread();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
            //thread_dispatch
            case 0x13: {
                TCB::dispatch();
                break;
            }
            case 0x21: {// sem_open
                int init;
                MySemaphore** sem_handle;

                __asm__ volatile("ld %[init], 11 * 8(x8)" : [init] "=r"(init));
                __asm__ volatile("ld %[sem_handle], 12 * 8(x8)" : [sem_handle] "=r"(sem_handle));

                //*sem_handle = MySemaphore::init_sem(init);
                *sem_handle = MySemaphore::sem_open(init);
                uint64 result = (*sem_handle != nullptr) ? 0 : -1;
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }

            case 0x22: { // sem_close
                MySemaphore* sem_id;
                __asm__ volatile("ld %[sem_id], 11 * 8(x8)" : [sem_id] "=r"(sem_id));
                uint64 result = sem_id->sem_close();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
            case 0x23: { // sem_wait
                MySemaphore* sem_id;
                __asm__ volatile("ld %[sem_id], 11 * 8(x8)" : [sem_id] "=r"(sem_id));
                uint64 result = sem_id->sem_wait();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
            case 0x24: { // sem_signal
                MySemaphore* sem_id;

                __asm__ volatile("ld %[sem_id], 11 * 8(x8)" : [sem_id] "=r"(sem_id));
                uint64 result = sem_id->sem_signal();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
            case 0x31: { //time_sleep
                time_t time;
                __asm__ volatile("ld %[time], 11 * 8(x8)" : [time] "=r"(time));
                uint64 result = TCB::running->time_sleep();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0

                break;
            }
            case 0x41: { //getc
                /*char ret_char = __getc();
                __asm__ volatile("sd %[ret_char], 10 * 8(x8)" : : [ret_char] "r" (ret_char)); //write on loc of saved a0
                break;*/

                char ret;
                ret = __getc();
                __asm__ volatile("mv a0, %0":: "r"(ret));
                __asm__ volatile("sd a0, 10*8(fp)");

                break;
            }
            case 0x42: { //putc
                char put_char;
                __asm__ volatile("ld %[put_char], 11 * 8(x8)" : [put_char] "=r"(put_char));
                __putc(put_char);
                break;
            }
                //thread_id
            case 0x50: {
                uint64 result = TCB::running->getId();
                __asm__ volatile("sd %[result], 10 * 8(x8)" : : [result] "r" (result)); //write on loc of saved a0
                break;
            }
                //thread_setmax
                /*case 0x51: {
                    int num;
                    __asm__ volatile("ld %[num], 11 * 8(x8)" : [num] "=r"(num));
                    TCB::setMaximumThreads(num);
                    break;
                }*/
            case 0x52: { //join
                thread_t* handle;
                __asm__ volatile("ld %[handle], 11 * 8(x8)" : [handle] "=r"(handle));
                TCB::join(handle);
                break;
            }
            case 0x53: {//join all
                TCB::joinAll();
                break;
            }
            case 0x54: { //ping
                thread_t handle;
                __asm__ volatile("ld %[handle], 11 * 8(x8)" : [handle] "=r"(handle));
                handle->setPinged(true);
                //handle->numOfBlocks++;
                break;
            }

            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        //     za proj od 30, ne ceka na unos sa tastature, vec krece
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        //konzola

        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        console_handler();

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else {//greska
            //unexpected trap cause
            printString("\nScause: ");
            printInt(scause);
           /*  printString("\nSepc: ");
            printInt(sepc);
            printString("\nStval: ");
            printInt(stval);
            printString("\nSstatus: ");
            printInt(sstatus);*/
    }
}