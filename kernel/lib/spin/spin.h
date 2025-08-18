#ifndef LIB_SPIN_RWLK
#define LIB_SPIN_RWLK

/*A spin read-write lock implemented using ISA specific CAS operations or equivalent*/
#include <stdint.h>
struct rwlk;
struct rwlk_guard;

enum rwlk_status {
    RWLK_LOCKED_RO,
    RWLK_LOCKED_RW,
    RWLK_LOCK_RELEASED,
    RWLK_LOCK_AVAILABLE,
    RWLK_WOULD_SPIN,
};

typedef union {
    enum {
        WRITE = 1 << 0,
    };
    uint8_t num;
} rwlk_flags;

enum rwlk_status rwlk_try_lock(struct rwlk *const lock, const struct rwlk_guard **guard, rwlk_flags options);

#endif
