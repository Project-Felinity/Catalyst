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

#include "idt.h"

#include "isa/x86_64/interrupts/exceptions.h"
#include "isa/x86_64/init/gdt.h"
#include "isa/x86_64/init/tss.h"
#include "lib/string/string.h"
#include "lib/string/type_conv.h"
#include "log/log.h"
#include "panic/ifce.h"

void set_gate_descriptor(idt_t idt, const uint8_t index, void (*offset)(void),
			 const uint16_t segment_selector, const enum gate_type type,
			 const bool is_present)
{
	// access the gate descriptor via a pointer that is treated as an array of two uint64_t
	uint64_t *const target = (uint64_t *)(idt + (index * GATE_DESC_SZ));
	uint64_t isr_addr = (uint64_t)offset;

	// clear the entire gate descriptor
	utility_memset(target, 0, GATE_DESC_SZ);

	// offset[15:0] = target[15:0]
	target[0] |= (isr_addr & 0xFFFF);
	// segment selector[15:0] = target[31:16]
	target[0] |= ((uint64_t)segment_selector << 16);
	// gate type[3:0] = target[43:40]
	target[0] |= (((uint64_t)type & 0xF) << 40);
	// present = target[47]
	target[0] |= (((uint64_t)(is_present ? 1 : 0)) << 47);
	// offset[31:16] = target[63:48]
	target[0] |= (((isr_addr & 0xFFFF0000) >> 16) << 48);
	// offset[63:32] = target[95:64]
	target[1] |= ((isr_addr & 0xFFFFFFFF00000000) >> 32);
}

void setup_idt(idt_t idt)
{
	char temp_str[1024];

	// Clear the IDT
	utility_memset(idt, 0, IDT_N_ELEMENTS * GATE_DESC_SZ);

	// install exception handlers at the appropriate interrupt vectors
	set_gate_descriptor(idt, 0, isr_divide_by_zero, 1 << 3, TRAP_GATE, true);
	set_gate_descriptor(idt, 4, isr_overflow, 1 << 3, TRAP_GATE, true);
	set_gate_descriptor(idt, 6, isr_invalid_opcode, 1 << 3, TRAP_GATE, true);
	set_gate_descriptor(idt, 8, isr_double_fault, 1 << 3, TRAP_GATE, true);
	set_gate_descriptor(idt, 13, isr_general_protection_fault, 1 << 3, TRAP_GATE, true);
	set_gate_descriptor(idt, 14, isr_page_fault, 1 << 3, TRAP_GATE, true);

	log_puts("Interrupt Descriptor Table\r\n");
	for (size_t i = 0; i < 512; i += 8) {
		utility_memset(temp_str, '\0', 1024);
		utility_u64_to_hex_str(*((uint64_t *)&idt[i]), temp_str);
		log_puts(temp_str);
		log_puts("\r\n");
	}

	idt_desc_t idt_desc;
	make_idt_desc(idt_desc, idt);
	__asm__ volatile("lidt %0"::"m"(idt_desc));
}
