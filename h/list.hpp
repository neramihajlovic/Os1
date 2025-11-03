#ifndef PROJECT_BASE_LIST_HPP
#define PROJECT_BASE_LIST_HPP

#include "MemAllocator.hpp"

template<typename T>
class List
{
private:
    void *operator new(size_t n)
    {
        return MemAllocator::mem_alloc(n);
    }
    void *operator new[](size_t n)
    {
        return MemAllocator::mem_alloc(n);

    }
    void operator delete(void *p)
    {
        MemAllocator::mem_free(p);
    }
    void operator delete[](void *p)
    {
        MemAllocator::mem_free(p);
    }


    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;
    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem* elem = new Elem(data, head);
		/*Elem *elem = (Elem*) MemoryAllocator::mem_alloc(sizeof(Elem));
		elem->data = data;
        elem->next = nullptr;*/
        head = elem;
        if (!tail) {
            tail = head;
        }
    }

    void addLast(T *data)
    {
        Elem* elem = new Elem(data, 0);
        /*Elem *elem = (Elem*) MemoryAllocator::mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = nullptr;*/

        if (tail){
            tail->next = elem;
            tail = elem;
        } else{
            head = tail = elem;
        }
    }

    T* removeFirst()
    {
        if (!head) { return 0;}

        Elem* elem = head;
        head = head->next;
        if (!head) { tail = 0;}

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T* getFirst()
    {
        if (!head) { return 0;}
        return head->data;
    }

    T* removeLast()
    {
        if (!head) {
            return 0;
        }

        Elem* prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem* elem = tail;
        if (prev) {
            prev->next = 0;
        }
        else {
            head = 0;
        }
        tail = prev;

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* getLast()
    {
        if (!tail) {
            return 0;
        }
        return tail->data;
    }

    int duzina(){
        if(!head){return 0;}
        int broj = 0;
        for(Elem* curr= head; curr != nullptr; curr = curr->next){
            ++broj;
        }
        return broj;
    }
    T* sledeci(T* trenutni) {
        if (!head || !trenutni) return nullptr;

        for (Elem* curr = head; curr != nullptr; curr = curr->next) {
            if (curr->data == trenutni) {
                return curr->next ? curr->next->data : nullptr;
            }
        }
        return nullptr;
    }
    T* obrisi(T* element) {
        if (!head || !element) return nullptr;
        Elem* prev = nullptr;
        Elem* curr = head;

        while (curr) {
            if (curr->data == element) {
                // Ako je prvi element
                if (curr == head) {
                    head = head->next;
                    if (head == nullptr) tail = nullptr;
                }
                // Ako je poslednji element
                else if (curr == tail) {
                    tail = prev;
                    if (prev) prev->next = nullptr;
                }
                // Ako je element u sredini
                else {
                    if (prev) prev->next = curr->next;
                }

                T* ret = curr->data;
                delete curr;
                return ret;  // vraćamo uklonjeni element
            }
            prev = curr;
            curr = curr->next;
        }
        return nullptr;  // element nije pronađen
    }

};

#endif //PROJECT_BASE_LIST_HPP