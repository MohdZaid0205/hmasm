# Grammar for `hmasm` assembly

Note the following symbols and their associated meanings:
* `<...>` represents register only argument.
* `#...#` repersents immidiate only argument.
* `@...@` represents address only argument.
* `?...`  represents unknown followed by possible choices.

## `load` or `move` instruction

```asm
move where?@@<>, what?<>@@##
;; moves from source provided in `what` caluse to destiniation in `where` clause
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value iteself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

Instruction is type indidpendent, depending upon type of `what?...` and `where?...` instruction is to be expanded into respective handler for `#intiger#` or `<register>` or `... (other supported types)`. This specific details shall be provided by `isa-<type>` in case of unsupported types passed as parametere, an (asseble-time) excecption is to be shown.

## `push` and `pop` instruction

```asm
push what?<>@@##
;; pushes contents of source provided in `what` clause to top of stack.
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value iteself.
;; destination:
;;      @@ : top of stack (implicit).
```

Instruction `push` is to be translated to multiple instructions that facilitate pushing to the top of stack in required isa, otherwise use provided push instuction in that isa.

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

Instruction `pop` is to be translated to multiple instructions that facilitate pushing to the top of stack in required isa, otherwise use provided pop instuction in that isa.

## Arithmatic operation instructions

> It is important to note that these instruction may ser carry flag in cpu. \
> All of specified functions update value in destination register with new value.

### `add` and `sub` instructions

```asm
add where?<>@@, what?<>@@##
;; adds contents of source `what` to contents of distination `where`
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
sub where?<>@@, what?<>@@##
;; subtracts contents of source `what` from contents of distination `where`
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
mul where?<>@@, what?<>@@##
;; multiplies contents of source `what` with contents of distination `where`
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
div where?<>@@, what?<>@@##
;; divides contents of source `what` by contents of distination `where`
;; source:
;;      <> : source may be a register.
;;      @@ : source may be an address.
;;      ## : source may be a value itself.
;; destination:
;;      <> : destination may be a register.
;;      @@ : destination may be an address.
```

> TODO: implement widening operations support (maybe in distant future).

