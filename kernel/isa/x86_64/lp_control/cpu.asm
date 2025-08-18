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

bits 64

section .bss
global g_cpu_regs
g_cpu_regs:
        resq 16 ;unitialized array of 16 qwords to store the integer registers

section .text

global disable_interrupts
disable_interrupts:
        cli
        ret

global enable_interrupts
enable_interrupts:
        sti
        ret

global inb
inb:
        mov dx, di
        in al, dx
        ret

global outb
outb:
        mov dx, di
        mov al, sil
        out dx, al
        ret

global get_regs
get_regs:
        mov [rel g_cpu_regs + 8 * 0], rax
        mov [rel g_cpu_regs + 8 * 1], rbx
        mov [rel g_cpu_regs + 8 * 2], rcx
        mov [rel g_cpu_regs + 8 * 3], rdx
        mov [rel g_cpu_regs + 8 * 4], rsi
        mov [rel g_cpu_regs + 8 * 5], rdi
        mov [rel g_cpu_regs + 8 * 6], rbp
        mov [rel g_cpu_regs + 8 * 7], rsp
        mov [rel g_cpu_regs + 8 * 8], r8
        mov [rel g_cpu_regs + 8 * 9], r9
        mov [rel g_cpu_regs + 8 * 10], r10
        mov [rel g_cpu_regs + 8 * 11], r11
        mov [rel g_cpu_regs + 8 * 12], r12
        mov [rel g_cpu_regs + 8 * 13], r13
        mov [rel g_cpu_regs + 8 * 14], r14
        mov [rel g_cpu_regs + 8 * 15], r15
        ret

global hcf
hcf:
        cli ;clear interrupt enable flag
        hlt ;halt in a loop
        jmp hcf
