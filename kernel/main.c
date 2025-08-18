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

/*freestanding headers*/
#include <stddef.h>
#include <stdint.h>

#include "boot/requests.h"
#include "init/init.h"
#include "isa/ifce.h"
#include "isa/x86_64/cpuid/cpuid.h"
#include "log/printf.h"

static const char license_string[] =
    "Catalyst: A General Purpose OS Kernel\r\n\r\n"
    "Copyright (C) 2023  Mohit D. Patel (mdpatelcsecon)\r\n\r\n"

    "This program is free software: you can redistribute it and/or modify\r\n"
    "it under the terms of the GNU General Public License as published by\r\n"
    "the Free Software Foundation, either version 3 of the License, or\r\n"
    "(at your option) any later version.\r\n"

    "This program is distributed in the hope that it will be useful,\r\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n"
    "GNU General Public License for more details.\r\n"

    "You should have received a copy of the GNU General Public License\r\n"
    "along with this program. If not, see https://www.gnu.org/licenses/\r\n\r\n";
// kernel entry point
extern void main(void)
{

    ct_init();

    printf(license_string);
    printf("Initializing Catalyst\r\n\r\n");

    isa_init_bsp();
    printf("Initialized BSP\r\n");

    // log_puts("Performing interrupt test\r\n");
    // interrupt();
    // log_puts("Returned from interrupt service routine\r\n");

    printf("Memory Map Response Address: %x\r\n", memory_map_request.response);

    printf("Obtaining CPU information...");
    enum cpuid_status id_err = get_cpuid_info(&cpuinfo);
    printf("Obtaining CPU information complete with status: ");
    switch (id_err) {
    case CPUID_NOT_SUPPORTED:
        printf("CPUID_NOT_SUPPORTED\r\n");
        break;
    case INVALID_DEST:
        printf("INVALID_DEST\r\n");
        break;
    case SUCCESS:
        printf("SUCCESS\r\n");
        printf("CPU vendor ID: %s\r\n", cpuinfo.vendor_id);
        printf("Max supported EAX value for CPUID instruction: %d\r\n", cpuinfo.max_cpuid_eax);
        printf("Number of significant physical address bits: %d\r\n", cpuinfo.paddr_bits);
        printf("Number of significant virtual address bits: %d\r\n", cpuinfo.vaddr_bits);
        break;
    }

    printf("Checking Limine HHDM request for response...");
    if (hhdm_request.response == nullptr) {
        printf("Higher Half Direct Map not available");
    } else {
        printf("Higher Half Direct Map available at virtual address %x\r\n",
               hhdm_request.response->offset);
    }

    printf("Testing printf\nString: %s\nDecimal: %d\nHex: %x\nBinary: %b\n",
           "The meaning of life is...", 42ull, 42ull, 42ull);
    printf("Testing printf without arguments.\n");

    // We're done, just hang...
    printf("Initialization complete, waiting for interrupts...");
    isa_halt();
}
