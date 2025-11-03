#include "../h/MemAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/mem.h"
#include "../test/printing.hpp"

using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
//    return __mem_alloc(n);
    return MemAllocator::mem_alloc(n);
}

void *operator new[](size_t n)
{
//    return __mem_alloc(n);
    return MemAllocator::mem_alloc(n);
}

void operator delete(void *p) noexcept
{
//    __mem_free(p);
    MemAllocator::mem_free(p);
}

void operator delete[](void *p) noexcept
{
//    __mem_free(p);
    MemAllocator::mem_free(p);
}