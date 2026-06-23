%section rodata
    %data @Z msg = [ "Read-only string\n" ]

%section bss
    %reserve @B buffer = 64

%section text
    %global _start

_start:
    la a1, msg
    li a7, 0x40
    li a0, 0x01
    li a2, 0x11
    ecall
    
    li a7, 0x5d
    li a0, 0x00
    ecall
