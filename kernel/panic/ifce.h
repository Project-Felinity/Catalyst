#ifndef PANIC_IFCE_H
#define PANIC_IFCE_H

void panic(void);
void assert([[maybe_unused]] const bool expr);
void assert_not_null([[maybe_unused]] const void *const ptr);

#endif
