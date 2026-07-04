# Grammar for `hmasm` assembly

Note the following symbols and their associated meanings:
* `<...>` represents register only argument.
* `#...#` represents immediate only argument.
* `@...@` represents address only argument.
* `?...`  represents unknown followed by possible choices.

## `load` or `move` instruction

```asm
move where?<>@@, what?<>@@##
;; moves from source provided in `what` clause to destination in `where` clause
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

Instruction is type independent, depending upon type of `what?...` and `where?...` the instruction is to be expanded into a respective handler for `#integer#` or `<register>` or other supported types. These specific details shall be provided by `isa-<type>`. In case of unsupported types passed as parameters, an assemble-time exception is to be shown.

## `push` and `pop` instructions

```asm
push what?<>@@##
;; pushes contents of source provided in `what` clause to top of stack.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      @@ : top of stack (implicit).
```

Instruction `push` is to be translated to multiple instructions that facilitate pushing to the top of stack in the required ISA, otherwise use the provided `push` instruction in that ISA.

---

```asm
pop where?<>@@
;; pops contents of stack and puts it into destination in `where` clause.
;; source:
;;      @@ : top of stack (implicit).
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

Instruction `pop` is to be translated to multiple instructions that facilitate popping from the top of stack in the required ISA, otherwise use the provided `pop` instruction in that ISA.

## Arithmetic operation instructions

> It is important to note that these instructions may set carry flag in CPU. \
> All specified instructions update the value in the destination with the new value.

```asm
inc destination?<>@@
;; increments destination by 1. shorthand to avoid `add dest, dest, #1#`.

dec destination?<>@@
;; decrements destination by 1. shorthand to avoid `sub dest, dest, #1#`.
```

### `add` and `sub` instructions

```asm
add destination?<>@@, source1?<>@@##, source2?<>@@##
;; adds source1 and source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

---

```asm
sub destination?<>@@, source1?<>@@##, source2?<>@@##
;; subtracts source2 from source1, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

### `mul` and `div` instructions

```asm
mul destination?<>@@, source1?<>@@##, source2?<>@@##
;; multiplies source1 by source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

---

```asm
div destination?<>@@, source1?<>@@##, source2?<>@@##
;; divides source1 by source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

> TODO: implement widening operations support (maybe in distant future).

## Bitwise operation instructions

```asm
and destination?<>@@, source1?<>@@##, source2?<>@@##
;; bitwise AND of source1 and source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

---

```asm
or destination?<>@@, source1?<>@@##, source2?<>@@##
;; bitwise OR of source1 and source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

---

```asm
xor destination?<>@@, source1?<>@@##, source2?<>@@##
;; bitwise XOR of source1 and source2, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

---

```asm
not destination?<>@@, source?<>@@
;; bitwise NOT of source, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

> Note: `not` is a unary operation. Immediates are not valid as source since negating a
> compile-time constant should be folded by the assembler before emission.

## Shifting operations

```asm
shl destination?<>@@, source1?<>@@##, source2?<>@@##
;; shift source1 left by source2 bits, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

> `destination = source1 << source2`

---

```asm
shr destination?<>@@, source1?<>@@##, source2?<>@@##
;; shift source1 right by source2 bits, storing result in destination.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

> `destination = source1 >> source2`

## Branching and comparison instructions

### Conditionals

All conditional branch instructions take two operands to compare and a label or address to jump to if the condition is true.

```asm
beq  lhs?<>@@##, rhs?<>@@##, target?<>@@##
bneq lhs?<>@@##, rhs?<>@@##, target?<>@@##
;; compares lhs and rhs, branches to target based on equality.
;; beq  : branch if lhs == rhs.
;; bneq : branch if lhs != rhs.
;; lhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; rhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; target:
;;      <> : may be a register holding the jump address.
;;      @@ : may be a direct address.
;;      ## : may be an immediate label or offset.
```

---

```asm
bg   lhs?<>@@##, rhs?<>@@##, target?<>@@##
bgeq lhs?<>@@##, rhs?<>@@##, target?<>@@##
;; compares lhs and rhs, branches to target based on greater-than relation.
;; bg   : branch if lhs >  rhs.
;; bgeq : branch if lhs >= rhs.
;; lhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; rhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; target:
;;      <> : may be a register holding the jump address.
;;      @@ : may be a direct address.
;;      ## : may be an immediate label or offset.
```

---

```asm
bs   lhs?<>@@##, rhs?<>@@##, target?<>@@##
bseq lhs?<>@@##, rhs?<>@@##, target?<>@@##
;; compares lhs and rhs, branches to target based on smaller-than relation.
;; bs   : branch if lhs <  rhs.
;; bseq : branch if lhs <= rhs.
;; lhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; rhs:
;;      <> : may be a register.
;;      @@ : may be an address.
;;      ## : may be a value itself.
;; target:
;;      <> : may be a register holding the jump address.
;;      @@ : may be a direct address.
;;      ## : may be an immediate label or offset.
```

---

```asm
jmp where?<>@@##
;; unconditional jump to target address.
;; where:
;;      <> : may be a register holding the jump address.
;;      @@ : may be a direct address.
;;      ## : may be an immediate label or offset.
```

---

```asm
call target?<>@@##
;; pushes the return address (next instruction) onto the stack and jumps to target.
;; essential for implementing functions and subroutines.

ret
;; pops the return address from the stack and jumps to it.
;; counterpart to call.
```



## Miscellaneous Instructions

```asm
nop
;; no operation. consumes one CPU cycle and does nothing.
;; used for timing alignment, pipeline padding, or busy-wait loops.
;; takes no operands.
```

---

```asm
hlt
;; halts the CPU. execution stops until an external interrupt resumes it,
;; or permanently depending on ISA behaviour.
;; takes no operands.
```

---

```asm
int
int interrupt##
;; invoke interrupts.

sti
;; enable interrupts.

cli
;; disable interrupts.
```

