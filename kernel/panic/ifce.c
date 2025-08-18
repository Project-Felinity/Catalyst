#include "isa/ifce.h"
#include "log/log.h"
#include <stdbool.h>

void panic(void)
{
    log_puts("A kernel panic has occurred!!!");
    isa_halt();
}

void assert([[maybe_unused]] bool expr)
{
#ifdef ASSERT
    if (expr == false)
        panic();
#endif
}

void assert_not_null([[maybe_unused]] const void *const ptr)
{
#ifdef ASSERT
    if (ptr == nullptr)
        panic();
#endif
}
