//
// Created by os on 10/19/25.
//

#ifndef PROJECT_BASE_RESOURCE_HPP
#define PROJECT_BASE_RESOURCE_HPP



#include "../h/syscall_cpp.hpp"


struct Holders {
    int num;
    Thread* thread;
    Holders() : num(0), thread(nullptr) {}
};

class Resource {

private:

    int num_of_instances;
    Semaphore* sem;
    Semaphore* mutex;
    //List<Holders> holders;
    int free_resource;
    int busy_resource;
    int waiting;

public:
    Resource(int n);
    void take(int requested);
    int give_back(int requested);


};


#endif //PROJECT_BASE_RESOURCE_HPP