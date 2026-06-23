%section text
    %global _start

_start:
    li a0, 0x05
    li a1, 0x0A
    add a2, a0, a1
    
    li a7, 0x5d
    li a0, 0x00
    ecall
