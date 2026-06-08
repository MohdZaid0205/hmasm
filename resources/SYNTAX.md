# Syntax for `hmasm` assembly

This document specifies intended style for this assembler and how different components are defined to be used with examples.

> Currently under development (SUBJECTED TO CHANGES)

## Registers and Naming registers

Every architecuture defines its own names for registers and its own types of registers with different purpose. Each register also has an associated size with it. To deal with this following syntax for naming register has been defined.

```asm
%register <name> := <reg> @<arch>
```

Read as: register `name` defined as `reg` at `architecure`

---

```asm example.asm
%register temp := t0 @RISC-V
%register temp := ax @x86-64

%register value := a0 @RISC-V
%register value := sx @x86-64

load value, 0x10
add  temp, value, 0x10
```

> NOTE: this definition of register names are only present upto intermideate representation of assembly. After that, depending upon target architecture, all such definitions are omitted and therefore useless.

