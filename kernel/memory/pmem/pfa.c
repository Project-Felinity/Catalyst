#include <stddef.h>

#include "../limine/limine.h"

#include "isa/types.h"
#include "isa/x86_64/types.h"
#include "lib/math/cmp.h"
#include "log/printf.h"
#include "panic/ifce.h"

typedef const struct limine_memmap_entry *const *const limine_memmap_t;

const size_t FRAME_SIZE = 4096ull;
const size_t BITS_PER_BYTE = 8ull;

enum pfa_status {
    PFA_SUCCESS = 0,
    PFA_NO_SUITABLE_REGION,
};

struct physical_frame_allocator {
    size_t total_frames;
    size_t free_frames;
    size_t committed_frames;
    size_t frame_bitmap_size;
    uint8_t *frame_bitmap;
};

size_t compute_bitmap_size(limine_memmap_t memory_map)
{
    paddr_t highest_address = 0;
    for (size_t i = 0; memory_map[i] != nullptr; ++i) {
        const struct limine_memmap_entry *entry = memory_map[i];
        highest_address = MAX(highest_address, entry->base + entry->length);
    }

    size_t frame_count = highest_address / FRAME_SIZE;
    if (highest_address % FRAME_SIZE != 0) {
        ++frame_count;
    }
    size_t bitmap_sz = frame_count / BITS_PER_BYTE;
    if (frame_count % BITS_PER_BYTE != 0) {
        ++bitmap_sz;
    }

    return bitmap_sz;
}

paddr_t find_best_fit_for_bitmap(limine_memmap_t memory_map, size_t bitmap_size)
{
    const struct limine_memmap_entry *best_fit = nullptr;

    for (size_t i = 0; memory_map[i] != nullptr; ++i) {
        const struct limine_memmap_entry *entry = memory_map[i];
        if (entry->length >= bitmap_size && (best_fit == nullptr || entry->length < best_fit->length)) {
            best_fit = entry;
        }
    }

    if (best_fit == nullptr) {
        printf("Critical System Failure: No suitable memory region available for the physical frame allocator bitmap.\n");
        panic();
    }

    return best_fit->base;
}

enum pfa_status pfa_init(limine_memmap_t memory_map)
{
    if (memory_map == nullptr) {
        printf("Critical System Failure: Limine failed to provide a memory map.");
        panic();
    }

    size_t bitmap_size = compute_bitmap_size(memory_map);
    paddr_t best_fit = find_best_fit_for_bitmap(memory_map, bitmap_size);

    //Use the HHDM interface to initialize the bitmap


    return PFA_SUCCESS;
}