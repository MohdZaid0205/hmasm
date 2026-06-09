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

## 3. Sections and Attributes

`hmasm` uses format-agnostic sections that allow optional permission flags. This allows the backend Format plugin (e.g., ELF, PE) to assign the correct memory protections without hardcoding assumptions about section names.

```asm
%section <NAME> [FLAGS]
```

Where flags can be combinations of `R` (Read), `W` (Write), and `X` (Execute), or `-` to denote absence of the permission.

```asm
%section TEXT [R-X]    ; Executable code
%section DATA [RW-]    ; Initialized read/write data
%section RODATA [R--]  ; Read-only data
%section BSS [RW-]     ; Uninitialized memory
```

## 4. Labels, Visibility, and Entry Points

Labels define specific memory addresses. Global and external visibility directives are used to link multiple files and specify execution entry points.

```asm
%extern printf       ; Symbol is imported from an external object
%global main         ; Symbol is exported to the linker

%entry _start        ; Explicitly defines the program entry point

_start:
    call main
    hlt

.local_loop:         ; Prefix with '.' for local scope labels
    inc a0
```
