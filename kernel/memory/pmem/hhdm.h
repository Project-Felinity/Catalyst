#ifndef MEMORY_HHDM_H
#define MEMORY_HHDM_H

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

#include "boot/requests.h"
#include "isa/types.h"
#include "isa/x86_64/types.h"

void hhdm_read_bytes(uint8_t *const dest, paddr_t src, size_t len);
void hhdm_write_bytes(paddr_t dest, const uint8_t *const src, size_t len);
void hhdm_mutate_at(paddr_t, void (*callback)(void*));

#endif
