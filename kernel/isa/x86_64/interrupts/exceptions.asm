; Catalyst: A Standalone General Purpose OS Kernel
; Copyright (C) 2023  Mohit D. Patel (mdpatelcsecon)

; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License
; along with this program.  If not, see https://www.gnu.org/licenses/


; ISRs for CPU based interrupts a.k.a. exceptions

bits 64

%macro save_gprs 0
        push rax
        push rbx
        push rcx
        push rdx
        push rsi
        push rdi
        push rsp
        push rbp
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
%endmacro

%macro restore_gprs 0
        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rbp
        pop rsp
        pop rdi
        pop rsi
        pop rdx
        pop rcx
        pop rbx
        pop rax
%endmacro

%macro get_int_error_code 0
        pop r10
        pop rdi
        push r10
%endmacro

section .bss
bsp_int_regs: resq 16

section .text
global isr_divide_by_zero
extern ih_divide_by_zero
isr_divide_by_zero:
        save_gprs
        call ih_divide_by_zero
        restore_gprs
        iretq

global isr_overflow
extern ih_overflow
isr_overflow:
        save_gprs
        call ih_overflow
        restore_gprs
        iretq

global isr_invalid_opcode
extern ih_invalid_opcode
isr_invalid_opcode:
        save_gprs
        call ih_invalid_opcode
        restore_gprs
        iretq

global isr_double_fault
extern ih_double_fault
isr_double_fault:
        call get_int_error_code
        ;this call does not return so there is no reason to put an iretq at the end of this function
        call ih_double_fault

global isr_general_protection_fault
extern ih_general_protection_fault
isr_general_protection_fault:
        save_gprs
        get_int_error_code
        call ih_general_protection_fault
        restore_gprs
        iretq

global isr_page_fault
extern ih_page_fault
isr_page_fault:
        save_gprs
        get_int_error_code
        call ih_page_fault
        restore_gprs
        iretq

global interrupt
interrupt:
        int 0
        ret