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
    /*MySemaphore* sem = new MySemaphore(value);
    sem->priority_sem = false;   // eksplicitno
    return sem;*/
}

int MySemaphore::sem_close() {
    if (!this->open) {
        return -1;
    }
    this->open = false;   // <<< ZATVARA se semafor

    while (this->blocked_threads.getFirst()) {
        TCB* thread = this->blocked_threads.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }

    return 0;
}



int MySemaphore::sem_wait() {
    //if this sem is not for further usage
    if (!this->open) {
        return -1;
    }
    /*printString("[DEBUG] sem_signal pozvan | val = ");
    printInt(this->val);
    printString(" | priority_sem = ");
    printInt(this->priority_sem);
    printString("\n");*/

    if (--this->val < 0) {
        //suspend the running process by putting it into the queue,
        //take another process from the ready queue and switch context to it
        if(!TCB::running) return -2;
        TCB::running->setBlocked(true);
        this->blocked_threads.addLast(TCB::running);


        /*
        //deadlock
        if (this->blocked_threads.duzina() >= Scheduler::maks - 2)  {
            // svi su blokirani — razbij deadlock
            printString("[Deadlock detected] Unblocking one thread!\n");
            TCB* to_unblock = this->blocked_threads.removeFirst();
            if (to_unblock) {
                to_unblock->setBlocked(false);
                Scheduler::put(to_unblock);
            }
        }*/


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

    if (this->val++ < 0) {
        if (!priority_sem) {
            // Standardni FIFO semafor
            TCB* unblocked_thread = blocked_threads.removeFirst();
            if (!unblocked_thread) return 0;

            unblocked_thread->setBlocked(false);
            Scheduler::put(unblocked_thread);
            return 0;
        }

        //  PRIORITETNI SEMAFOR
        if (blocked_threads.duzina() == 0) return 0;

        // Pronađi nit sa NAJMANJIM ID (najveći prioritet)
        int minID = blocked_threads.getFirst()->getId();
        for (TCB* t = blocked_threads.getFirst(); t; t = blocked_threads.sledeci(t)) {
            if (t->getId() < minID) {
                minID = t->getId();
            }
        }

        printString("[DEBUG] Broj blokiranih: ");
        printInt(blocked_threads.duzina());
        printString("  |  Najmanji ID: ");
        printInt(minID);
        printString("\n");

        // Nađi nit s tim ID-jem i odblokiraj je
        for (TCB* t = blocked_threads.getFirst(); t; t = blocked_threads.sledeci(t)) {
            if (t->getId() == minID) {
                TCB* pokreni = blocked_threads.obrisi(t);
                if (!pokreni) break;

                pokreni->setBlocked(false);
                Scheduler::put(pokreni);

                printString("[DEBUG] Odblokirana nit sa ID-jem ");
                printInt(pokreni->getId());
                printString("\n");
                break;
            }
        }
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