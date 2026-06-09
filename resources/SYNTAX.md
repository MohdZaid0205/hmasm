# Syntax for `hmasm` assembly

This document specifies intended style for this assembler and how different components are defined to be used with examples.

> Currently under development (SUBJECTED TO CHANGES)

## 1. Comments
Comments are supported and are ignored by the assembler. A comment begins with a semicolon `;` (or `;;`) and extends to the end of the line.

```asm
; This is a single-line comment
;; This is also a comment, typically used for important notes
add a0, a1, a2  ; This is an inline comment
```

## 2. Registers and Aliasing

Every architecture defines its own names for registers and its own types of registers with different purposes. Each register also has an associated size with it. To deal with this, the following syntax for aliasing registers is defined:

```asm
%register <name> := <reg> @<arch>
```

Read as: register `name` defined as `reg` for `architecture`.

---

```asm example.asm
%register temp := t0 @RISC-V
%register temp := ax @x86-64

%register value := a0 @RISC-V
%register value := sx @x86-64

move value, 0x10
add  temp, value, 0x10
```

> NOTE: This definition of register names is only present up to the intermediate representation of the assembly. After that, depending upon the target architecture, all such definitions are resolved by the assembler and removed.

