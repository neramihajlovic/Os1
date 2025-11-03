#include "TestMatricaHistogram.hpp"
#include "../h/MemAllocator.hpp"

Semaphore *mutex;
int *globalHistogram;

int N,M;

int seed = 1234;
int myRandom() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed % 100; // ili bilo koji opseg, ovde primer 0-99
}

int readInt(){
    int num = 0;
    char c = getc();
    while(c != '\n' && c != ' '){
    num = num * 10 + (c - '0');
        c = getc();
    }
    return num;
}
struct Matrica{
    int** matrica;
    int* redZavrsen;   // da označi da je nit završila
    int** matRacunanja; // po redovima brojanje cifara

    Matrica(int n,int m){
        matrica = (int**)MemAllocator::mem_alloc(n * sizeof(int*));
        for(int i=0;i<n;i++){
            matrica[i] = (int*)MemAllocator::mem_alloc(m * sizeof(int));
        }
        redZavrsen = (int*)MemAllocator::mem_alloc(n * sizeof(int));
        matRacunanja = (int**)MemAllocator::mem_alloc(n * sizeof(int*));
        for(int i=0;i<n;i++){
            matRacunanja[i] = (int*)MemAllocator::mem_alloc(10 * sizeof(int));
        }
         for(int i=0;i<n;i++){
             redZavrsen[i] = 0;
             for(int j=0;j<10;j++){
                matRacunanja[i][j] = 0;
             }
         }
    }

    ~Matrica(){
         for(int i=0;i<N;i++){
                MemAllocator::mem_free(matrica[i]);
                MemAllocator::mem_free(matRacunanja[i]);
         }
        MemAllocator::mem_free(matrica);
        MemAllocator::mem_free(matRacunanja);
        MemAllocator::mem_free(redZavrsen);
    }
};

class Nit:public Thread{
    Matrica *m;
    int red;
public:
    Nit(Matrica *m,int red):Thread(),m(m),red(red){}
    void run() override{
        for(int j=0;j<M;j++){
            int broj = m->matrica[red][j];
            int cifra = broj % 10;
            m->matRacunanja[red][cifra]++;

            //na svakih 10 brojeva u redu dispatch
            if((j+1)%10 == 0){
                printString("Nit ");
                printInt(red);
                printString("  10 brojeva sada druga nit\n");
                Thread::dispatch();
            }
        }
        //upis u glob hist
        mutex->wait();
        for(int k=0;k<10;k++){
            globalHistogram[k] += m->matRacunanja[red][k];
        }
        mutex->signal();

        m->redZavrsen[red]=1;

        printString("Nit ");
        printInt(red);
        printString(" finished.\n");
    }
};

void testMatricaHistogram(){
    printString("unesite broj redova:\n");
    N = readInt();
    printString("\n");

    printString("unesite broj kolona:\n");
    M = readInt();
    printString("\n");

    printInt(N);
    printString("-----");
    printInt(M);
    printString("\n");

    /*int seed = 1234; // bilo koji početni broj

    for (int i = 0; i < 5; i++) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        int broj = seed % 90000 + 10000;
        printInt(broj); // ili cout << broj
        printString("\n");
    }*/
    mutex = new Semaphore(1);

    Matrica m(N,M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            m.matrica[i][j] = myRandom();
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
        printInt(m.matrica[i][j]);
        printString("    ");
        }
        printString("\n");
    }


    globalHistogram = (int*)MemAllocator::mem_alloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {globalHistogram[i] = 0;}


    for (int i = 0; i < N; i++) {
        Thread *t = new Nit(&m,i);
        t->start();
    }

    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < N; i++) {
            if(m.redZavrsen[i]==0){
                done = false;
                break;
            }
        }
        thread_dispatch();
    }
    // Ispiši globalni histogram
    printString("\nUkupan broj brojeva po poslednjoj cifri:\n");
    int ukupno=0;
    for (int k = 0; k < 10; k++) {
        ukupno += globalHistogram[k];
        printString("Cifra ");
        printInt(k);
        printString(": ");
        printInt(globalHistogram[k]);
        printString("     ");

    }
    printString("\nUkupno ");
    printInt(ukupno);


    MemAllocator::mem_free(globalHistogram);
    printString("\nSMRDOOOOOOOOOOOO TRAZILA SAM MALO VREMENA \n");

}