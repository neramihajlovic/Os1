//
// Created by os on 9/17/24.
//
#include "../h/MySemaphore.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

/*MySemaphore* MySemaphore::init_sem(int value) {
    return new MySemaphore(value);
}*/
MySemaphore *MySemaphore::sem_open(int value) {
    //return init_sem(value);
    return new MySemaphore(value);
}

int MySemaphore::sem_close() {
    if (!this->open) {
        return -1;
    }

    // Ako je aktiviran owner mod, proveri da li pozivalac jeste vlasnik
    if (this->owner_mode && TCB::running != this->owner) {
        return -1;  // nije vlasnik, ne sme da zatvori
    }

    while (this->blocked_threads.getFirst()) {
        TCB* thread = this->blocked_threads.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }
    this->open = false;   // <<< ZATVARA se semafor
    return 0;
}



int MySemaphore::sem_wait() {
    //if this sem is not for further usage
    if (!this->open) {
        return -1;
    }
    // Ako je aktiviran owner mod, proveri da li je vlasnik
    if (this->owner_mode && TCB::running != this->owner) {
        return -1;  // nije vlasnik — nema pristup
    }

    if (--this->val < 0) {
        //suspend the running process by putting it into the queue,
        //take another process from the ready queue and switch context to it
        if(!TCB::running) return -2;
        TCB::running->setBlocked(true);
        this->blocked_threads.addLast(TCB::running);



        //deadlock
        if (this->blocked_threads.duzina() >= Scheduler::maks - 2)  {
            // svi su blokirani — razbij deadlock
            printString("[Deadlock detected] Unblocking one thread!\n");
            TCB* to_unblock = this->blocked_threads.removeFirst();
            if (to_unblock) {
                to_unblock->setBlocked(false);
                Scheduler::put(to_unblock);
            }
        }


        TCB::yield();

        //if thread waited on deallocated sem, return error code
        if (!this->open) {
            return  -2;
        }
    }
    return 0;
}
int MySemaphore::sem_signal() {
    //if this sem is not for further usage
    if (!this->open) {
        return -1;
    }
    // Ako je aktiviran owner mod, proveri da li je vlasnik
    if (this->owner_mode && TCB::running != this->owner) {
        return -1;  // nije vlasnik — nema pristup
    }

    if (this->val++ < 0) {
        //take one process from the suspended queue
        // and unblock it by putting it into the ready queue
        TCB* unblocked_thread = this->blocked_threads.removeFirst();
        if(!unblocked_thread) {
            return -1;
        }
        unblocked_thread->setBlocked(false);
        Scheduler::put(unblocked_thread);
        printString("odblokrana je i radi nit :");
        printInt(thread_id());
        printString("\n");

    }

    return 0;
}

// int MySemaphore::sem_trywait() {
//     if (!this->open) {
//         return -1;
//     }
//     if (this->val - 1 < 0) {
//         return 0;
//     } else {
//         return 1;
//     }
// }