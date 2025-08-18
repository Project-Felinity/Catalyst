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

#include "serial.h"
#include "isa/x86_64/lp_control/cpu.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PORT 0x3f8 // COM1

int ns16550_init_serial(void)
{
	outb(PORT + 1, 0x00); // Disable all interrupts
	outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00); //                  (hi byte)
	outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
	outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
	outb(PORT + 0,
	     0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	if (inb(PORT + 0) != 0xAE) {
		return -1;
	}

	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	outb(PORT + 4, 0x0F);
	return 0;
}

int is_transmit_empty(void)
{
	return inb(PORT + 5) & 0x20;
}

char ns16550_serial_putc(char c)
{
	while (is_transmit_empty() == 0)
		;

	outb(PORT, c);
	return c;
}
