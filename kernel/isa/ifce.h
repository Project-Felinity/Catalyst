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

#ifndef ISA_API_H
#define ISA_API_H

#include <stdint.h>

/*CPU*/
/*
Disables all maskable interrupts
Preconditions: None
Postconditions: All maskable interrupts are disabled on the calling logical processor
*/
inline void isa_mask_ints(void);
/*
Enables all maskable interrupts
Preconditions: None
Postconditions: All maskable interrupts are enabled on the calling logical processor
*/
inline void isa_unmask_ints(void);
/*
Perform ISA specific initialization on the bootstrap LP
Preconditions: None
Postconditions: The processor will be initialized and ready to execute both kernel
and userspace code
*/
void isa_init_bsp(void);
/*
Halt the executing logical processor
Preconditions: None
Postconditions: The logical processor is halted until an interrupt occurs
Note: This function never returns
*/
[[noreturn]]
void isa_halt(void);

// Port I/O
uint8_t isa_port_in8(const uint16_t port);
void isa_port_out8(const uint16_t port, const uint8_t data);

extern void interrupt_test(void);

#endif
