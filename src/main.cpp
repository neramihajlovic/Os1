/*
#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/MemAllocator.hpp"
#include "../h/print.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"*/

#include "../h/syscall_cpp.hpp"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"
#include "../h/print.hpp"
//#include "../h/workers.hpp"

extern void userMain();

void userFunction(void* ) {
    userMain();
}

/*void checkNullptr(void* p) {
    static int x = 0;
    if(p == nullptr) {
        __putc('?');
        __putc('0' + x);
    }
    x++;
}

void checkStatus(int status) {
    static int y = 0;
    if(status) {
        __putc('0' + y);
        __putc('?');
    }
    y++;
}*/

/*
void threadBody(void* arg) {
    int id = (int)(uint64)arg;
    printStr("Nit ");
    printInteger(id);
    printStr(" radi.\n");
    for (volatile int i = 0; i < 1000000; i++); // mali delay
}*/
/*void testSchedulerNum() {
    printStr("Pocetak testa Scheduler::num\n");

    printStr("Broj niti u redu: ");
    printInteger(Scheduler::num);
    printStr("\n");
    // Kreiramo 3 niti
    //TCB* t1 = new TCB(threadBody, (void*)1, nullptr);
    //TCB* t2 = new TCB(threadBody, (void*)2, nullptr);
    //TCB* t3 = new TCB(threadBody, (void*)3, nullptr);
    //TCB* t1 = TCB::createThread(threadBody, (void*)1);
    //TCB* t2 = TCB::createThread(threadBody, (void*)2);
    //TCB* t3 = TCB::createThread(threadBody, (void*)3);
    TCB::createThread(threadBody, (void*)1);
    TCB::createThread(threadBody, stack, (void*)2);
    TCB::createThread(threadBody, (void*)3);
    //TCB::createThread(threadBody, (void*)2);
    //uint64* stack = new uint64[TCB::STACK_SIZE]; stavi u public u tcb size

    //Scheduler::put(t1);
    //Scheduler::put(t2);
    //Scheduler::put(t3);   //ne treba ovo, jer vec u create stavljam  scheduler
    printStr("Posle dodavanja 3 niti, num = ");
    printInteger(Scheduler::num);
    printStr("\n");

    // Vadimo jednu nit
    TCB* first = Scheduler::get();
    if(first) printStr("Prva nit iz scheduler-a uzeta.\n");
    printStr("Izvadili smo jednu nit, num = ");
    printInteger(Scheduler::num);
    printStr("\n");

    // Vadimo drugu nit
    TCB* second = Scheduler::get();
    if(second) printStr("Druga nit iz scheduler-a uzeta.\n");
    printStr("Izvadili smo drugu nit, num = ");
    printInteger(Scheduler::num);
    printStr("\n");

    // Vadimo trecu nit
    TCB* third = Scheduler::get();
    if(third) printStr("Treca nit iz scheduler-a uzeta.\n");
    printStr("Izvadili smo trecu nit, num = ");
    printInteger(Scheduler::num);
    printStr("\n");

    printStr("Kraj testa Scheduler::num\n");
}*/
int main()

{

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap); //uvek
    TCB *threads[2];
    threads[0] = TCB::createThread(nullptr,nullptr);

    TCB::running = threads[0];

    /*printString("Main thread ID: ");
    //printInt(TCB::running->getId());
    printInt(Thread::getThreadId());
    printString("\n");*/

    thread_create(&threads[1], userFunction, nullptr);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    /*printString("Created thread ID: ");
    printInt(threads[1]->getId());
    //printInt(Thread::getThreadId());   ako cu ovako moracu u fju za workers u testu da pzoovem, jer se uvek poziva za trenutnu aktivnu nit
    printString("\n");*/

    while(!threads[1]->isFinished()) {
        //printString("\nNSD\n");
        thread_dispatch();
    }

    printString("open je  main\n");

    return 0;

    printString("\nFinished\n");


   //testSchedulerNum();

    //test za mem free i largest
    /*MemAllocator::initMem();

    printStr("Pocetno stanje:\n");
    printStr("Total free: "); printInteger(MemAllocator::mem_get_free_space()); printStr("\n");
    printStr("Largest block: "); printInteger(MemAllocator::mem_get_largest_free_block()); printStr("\n\n");

    void* a = MemAllocator::mem_alloc(1000);
    printStr("Posle alokacije 1000:\n");
    printStr("Total free: "); printInteger(MemAllocator::mem_get_free_space()); printStr("\n");
    printStr("Largest block: "); printInteger(MemAllocator::mem_get_largest_free_block()); printStr("\n\n");

    void* b = MemAllocator::mem_alloc(5000);
    printStr("Posle alokacije 5000:\n");
    printStr("Total free: "); printInteger(MemAllocator::mem_get_free_space()); printStr("\n");
    printStr("Largest block: "); printInteger(MemAllocator::mem_get_largest_free_block()); printStr("\n\n");

    MemAllocator::mem_free(a);
    printStr("Posle free(1000):\n");
    printStr("Total free: "); printInteger(MemAllocator::mem_get_free_space()); printStr("\n");
    printStr("Largest block: "); printInteger(MemAllocator::mem_get_largest_free_block()); printStr("\n\n");
//ne vraća odmah ceo maksimum dokle god blokovi nisu spojeni.
    MemAllocator::mem_free(b);
    printStr("Posle free(5000):\n");
    printStr("Total free: "); printInteger(MemAllocator::mem_get_free_space()); printStr("\n");
    printStr("Largest block: "); printInteger(MemAllocator::mem_get_largest_free_block()); printStr("\n\n");
*/

//tests for semaphores

   /* sem_t sem;
    sem_open(&sem, 1);

    thread_t *threads[4];
    for (int i = 0; i < 4; i++) {
        //threads[i] = (thread_t*)mem_alloc(sizeof(void*));
        threads[i] = (thread_t*) new thread_t;
    }
    printString("\nshort break");
    int result_t0 = thread_create(threads[0], nullptr, sem);
    printString("\nThis is the number of result_t0: ");
    printInteger(result_t0);
    printString("\n");
    TCB::running = *threads[0];

    int result_t1 = thread_create(threads[1], workerBodyA, sem);
    printString("\nThread A created\n");
    printString("\nThis is the number of result_t1: ");
    printInteger(result_t1);
    printString("\n");

    int result_t2 = thread_create(threads[2], workerBodyB, sem);
    printString("\nThread B created\n");
    printString("\nThis is the number of result_t2: ");
    printInteger(result_t2);
    printString("\n");

    int result_t3 = thread_create(threads[3], workerBodyC, sem);
    printString("\nThread C created\n");
    printString("\nThis is the number of result_t3: ");
    printInteger(result_t3);
    printString("\n");

    //TCB::running = *threads[3];
    //thread_exit();
    uint64 cnt = 0;
    while (!((*threads[1])->isFinished() && (*threads[2])->isFinished() && (*threads[3])->isFinished()))
    {
        cnt++;
        //TCB::yield();
        thread_dispatch();



    }

    for (auto &thread: threads)
    {
        printString("Inside deleter area");
        delete thread;
    }
*/


/* main poziva syscall.cpp -> syscal.c(cuva reg na stek, oonda ecall pa vraca rezultat
ecall ide i riscv(switchcase poziva  fje iz mem Alloc , tcba ili mysem)*/

 	/*printInteger(DEFAULT_STACK_SIZE);

    thread_t *threads[4];
    for (int i = 0; i < 4; i++) {
        //threads[i] = (thread_t*)mem_alloc(sizeof(void*));
        threads[i] = (thread_t*) new thread_t;
    }

    printString("\nshort break");
    int result_t0 = thread_create(threads[0], nullptr, nullptr);
    printString("\nThis is the number of result_t0: ");
    printInteger(result_t0);
    printString("\n");
    TCB::running = *threads[0];


    int result_t1 = thread_create(threads[1], workerBodyA, nullptr);
    printString("\nThread A created\n");
    printString("\nThis is the number of result_t1: ");
    printInteger(result_t1);
    printStr("\n");

    int result_t2 = thread_create(threads[2], workerBodyB, nullptr);
    printString("\nThread B created\n");
    printString("\nThis is the number of result_t2: ");
    printInteger(result_t2);
    printString("\n");

    int result_t3 = thread_create(threads[3], workerBodyC, nullptr);
    printString("\nThread C created\n");
    printString("\nThis is the number of result_t3: ");
    printInteger(result_t3);
    printString("\n");

    //TCB::running = *threads[3];
    //thread_exit();
    uint64 cnt = 0;
    while (!((*threads[1])->isFinished() && (*threads[2])->isFinished() && (*threads[3])->isFinished()))
    {
        cnt++;
		//printString("\nusao\n");
        //TCB::yield();
        thread_dispatch();
        if (cnt == 2) {
            printString("\n\ncao\n\n");
        }

    }*/


    /*for (auto &thread: threads)
    {
        printString("Inside deleter area");
        delete thread;
    }*/




/*
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    TCB* mainThread;
    thread_create(&mainThread, nullptr, nullptr);  // nit bez tela
    TCB::running = mainThread;                     // postavi kao trenutno aktivnu

    // 3. Kreiraj korisničku nit sa userMain
    TCB* userThread;
    thread_create(&userThread, userWrapper, nullptr);

    // 4. Dok korisnička nit ne završi, idi kroz scheduler
    while (!userThread->isFinished()) {
        thread_dispatch();
    }

*/









/*
//Test threads
      TCB *thread1;
      TCB *threadA;
      TCB *threadB;

    thread1 = TCB::createThread(nullptr, nullptr);
    TCB::running = thread1;

	threadA = TCB::createThread(workerBodyA, nullptr);
    printString("Thread A created\n");
	threadB = TCB::createThread(workerBodyB, nullptr);
    printStr("Thread B created\n");

	while (!(threadA->isFinished() &&
             threadB->isFinished()))
    {
        TCB::yield();
    }*/

	/*TCB *threads[3];

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA, nullptr);
    printStr("Thread A created\n");

    threads[2] = TCB::createThread(workerBodyB, nullptr);
    printString("Thread B created\n");


    while (!(threads[1]->isFinished() &&
            threads[2]->isFinished()))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }*/


	/*thread_t *t1 = (thread_t*) new (void*);

    TCB* t2 = TCB::createThread(workerBodyA, nullptr);
    printInteger(t2->isFinished());

    int result = thread_create(t1, workerBodyA, nullptr);
    printInteger(result);

    uint64* stack = new uint64[DEFAULT_STACK_SIZE];
    TCB *t3 = TCB::createThread(workerBodyA, stack, nullptr);
    printInteger(t3->isFinished());
	*/






//worker tread provera----------------------------------------------------------------------------------------------

    /*uint64 stack[1024]; // ili na heap-u sa new
    TCB* workerThread = TCB::createThread(simpleWorker, stack, &iterations);*/

	/*int iterations = 5;
    TCB* workerThread = TCB::createThread(simpleWorker, &iterations);
    Scheduler::put(workerThread);  // obavezno
    printString(" workerThread created\n");*/

    /*printString("\nThis is the number of stack: ");
    printInt((uint64) workerThread->getStack());
    printString("\nThis is the number of sp: ");
    printInt( workerThread->getSP());
    printString("\n");*/




// memorija provere----------------------------------------------------------------------------------------------------------
    /*printI((uint64)HEAP_START_ADDR); //2147534720
    printString("\n");
    //MemAllocator::initMem();
    printI((uint64)MemAllocator::mem_alloc(sizeof(int)));
    printString("\n"); //2147534736
    printI((uint64)MemAllocator::mem_alloc(sizeof(double)));
    printString("\n");
    printString("\n");
    printString("wtf\n");

    void* WTF = MemAllocator::mem_alloc(sizeof(int));
    printI((uint64)WTF);
    printString("\n");
    printString("obrisano\n");
    MemAllocator::mem_free(WTF);

    printI((uint64)MemAllocator::mem_alloc(sizeof(double)));
    printString("\n");
    printS("radiiii\n");
*/






    //OVO MORA PRE SVAKOG TESTA SA MEM
    //MemAllocator::initMem();
/*
    //prvi test memorija  ispis n n n n n   10 puta
     int n = 10;
     char* niz = (char*)MemAllocator::mem_alloc(10*sizeof(char));
     if(niz == nullptr) {
         __putc('?');
     }

     for(int i = 0; i < n; i++) {
         niz[i] = 'n';
     }

     for(int i = 0; i < n; i++) {
         __putc(niz[i]);
         __putc(' ');
     }

     int status = MemAllocator::mem_free(niz);
     if(status != 0) {
         __putc('?');
     }
*/
    //drugi test memorija  ispis samo jedno n
    /* int velicinaZaglavlja = sizeof(size_t); // meni je ovoliko

     const size_t maxMemorija = (((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - velicinaZaglavlja)/MEM_BLOCK_SIZE - 1)*MEM_BLOCK_SIZE ;
     char* niz = (char*)MemAllocator::mem_alloc(maxMemorija); // celokupan prostor
     if(niz == nullptr) {
        __putc('?');
    }

    int n = 10;
    char* niz2 = (char*)MemAllocator::mem_alloc(n*sizeof(char));
    if(niz2 == nullptr) {
        __putc('n');
        __putc('\n');
    }

    int status = MemAllocator::mem_free(niz);
    if(status) {
        __putc('?');
    }
    niz2 = (char*)MemAllocator::mem_alloc(n*sizeof(char));
    if(niz2 == nullptr) {
        __putc('?');
    }*/

    // treci test moze n, a moze i bilo sta jer je nedefinisano ponasajne
    /*char *p = nullptr;
    int status = MemAllocator::mem_free(p);
    if(status) {
        __putc('n');
        __putc('\n');
    }*/

    // cetvrti test iznad maina dodatak  (nista ako nema greske)
    /*int velicinaZaglavlja = sizeof(size_t); // meni je ovoliko

    int *p1 = (int*)MemAllocator::mem_alloc(15*sizeof(int)); // trebalo bi da predje jedan blok od 64
    checkNullptr(p1);
    int *p2 = (int*)MemAllocator::mem_alloc(30*sizeof(int));
    checkNullptr(p2);

    int *p3 = (int*)MemAllocator::mem_alloc(30*sizeof(int));
    checkNullptr(p3);

    checkStatus(MemAllocator::mem_free(p1));
    checkStatus(MemAllocator::mem_free(p3));
    checkStatus(MemAllocator::mem_free(p2)); // p2 treba da se spoji sa p1 i p3

    const size_t maxMemorija = (((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - velicinaZaglavlja)/MEM_BLOCK_SIZE - 1)*MEM_BLOCK_SIZE ;
    int *celaMemorija = (int*)MemAllocator::mem_alloc(maxMemorija);
    checkNullptr(celaMemorija);

    checkStatus(MemAllocator::mem_free(celaMemorija));*/

    //peti test,iznad maina dodatak matrica 16x16 sa n slovima
    /*int n = 16;
    char** matrix = (char**)MemAllocator::mem_alloc(n*sizeof(char*));
    checkNullptr(matrix);
    for(int i = 0; i < n; i++) {
        matrix[i] = (char *) MemAllocator::mem_alloc(n * sizeof(char));
        checkNullptr(matrix[i]);
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrix[i][j] = 'n';
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            __putc(matrix[i][j]);
            __putc(' ');
        }
        __putc('\n');
    }


    for(int i = 0; i < n; i++) {
        int status = MemAllocator::mem_free(matrix[i]);
        checkStatus(status);
    }
    int status = MemAllocator::mem_free(matrix);
    checkStatus(status);*/





    //    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);  //ovo mora






    return 0;
}