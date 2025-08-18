/*
Catalyst: A Standalone General Purpose OS Kernel
Copyright (C) 2023  Mohit D. Patel (mdpatelcsecon)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/
*/

#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>

#include "panic/ifce.h"

#define IDT_N_ELEMENTS 256
// gate descriptor size in bytes
#define GATE_DESC_SZ 16
#define GATE_DESC_PRESENT_BIT 47

typedef uint8_t idt_desc_t[80];
typedef uint8_t idt_t[IDT_N_ELEMENTS * GATE_DESC_SZ];
enum gate_type { INTERRUPT_GATE = 0xE, TRAP_GATE = 0xF };

inline bool is_gate_descriptor_present(const idt_t idt, const uint8_t index)
{
	assert_not_null(idt);

	return *((uint64_t *)(&idt[index * GATE_DESC_SZ])) &
		       (((uint64_t)1) << GATE_DESC_PRESENT_BIT)
		   ? true
		   : false;
}
inline void set_gate_descriptor_present_bit(idt_t idt, const uint8_t index, const bool present_val)
{
	assert(idt != nullptr);

	if (is_gate_descriptor_present(idt, index) != present_val) {
		*((uint64_t *)(&idt[index * GATE_DESC_SZ])) ^=
		    (((uint64_t)(present_val ? 1 : 0)) << GATE_DESC_PRESENT_BIT);
	}
}
void set_gate_descriptor(idt_t idt, const uint8_t index, void (*offset)(void),
			 const uint16_t segment_selector, const enum gate_type type,
			 const bool is_present);
inline void make_idt_desc(idt_desc_t dest, const idt_t idt)
{
	assert_not_null(dest);
	assert_not_null(idt);

	uint16_t *const sz_ptr = (uint16_t *const)dest;
	*sz_ptr = IDT_N_ELEMENTS * GATE_DESC_SZ - 1;

	uint64_t *const offset_ptr = (uint64_t *const)&dest[2];
	*offset_ptr = (uint64_t)idt;
}
extern void load_idt(idt_desc_t desc);

void setup_idt(idt_t idt);

#endif
