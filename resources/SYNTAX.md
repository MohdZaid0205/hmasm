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

## 5. Type-Aware Constants

Constants are defined using the `%const` directive followed by a type identifier. These types directly map to the AST `DECLARATION_TYPE` system, enforcing compile-time type safety.

* `@B`: Byte
* `@W`: Word
* `@D`: Double
* `@Q`: Quadword
* `@T`: Tenword
* `@Y`: Y-word
* `@Z`: Z-word

```asm
%const @W MAX_BUFFER_SIZE = 0xFFFF
%const @D TARGET_ARCH_ID  = 0x01
```

## 6. Type-Aware Data Definitions and Reservations

To allocate data in `.data` or `.rodata` sections, use the `%data` directive. For uninitialized memory in the `.bss` section, use `%reserve`.

```asm
%section DATA [RW-]
    %data @B msg = "Hello World", 0
    %data @W fibonacci = [1, 1, 2, 3, 5]
    %data @D magic = 0xDEADBEEF

%section BSS [RW-]
    %reserve @B buffer[1024]    ; Reserves 1024 bytes
```

## 7. Alignment

Different ISAs have strict memory alignment rules. The `%align` directive ensures the next instruction or data block is aligned to a specific byte boundary.

```asm
%align 4
%data @D aligned_value = 0x12345678
```

## 8. Optimization Blocks (Multi-Architecture Fallbacks)

To fully leverage the 3-layered assembler, you can provide architecture-specific implementations for a block of code. 

The assembler expects a `@default` block using universal IR. If the target architecture matches an provided `@<arch>` block, the assembler will parse that specific block instead. If native (unknown) instructions are encountered in an architecture block, the ISA backend will attempt to compile them via a fallback framework (e.g., Keystone).

```asm
%optimization @default
    ; Generic IR implementation
    add a0, a1, a2

%optimization @x86-64
    ; x86-64 specific implementation
    lea a0, [a1 + a2]

%optimization @RISC-V
    ; RISC-V specific implementation
    add a0, a1, a2

%optimization .
```

If a fallback is completely impossible for an architecture, the `%error` directive can be used to explicitly halt compilation.

```asm
%optimization @default
    %error "Operation not supported on generic IR!"
%optimization @x86-64
    ; Implementation
%optimization .
```
