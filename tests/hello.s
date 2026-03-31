.section .data
    msg: .asciz "Hello World!\n"

.section .text
    .globl _start

_start:
    # this apparently is supposed
    # to print msg to console using
    # linux provided syscall funcs
    li a7, 0x40
    li a0, 0x01
    la a1, msg
    li a2, 0x0d
    ecall

    # then we proceed to exit this
    # with 0 as return code as this
    # rnd sucessfully as expected
    li a7, 0x5d
    li a0, 0x00
    ecall
