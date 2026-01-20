# Grammar for `hmasm` assembly

## `load` or `move` instruction

```
move <register>, <what?>
load <register>, <what?>
```

Instruction is type indidpendent, depending upon type of `<what?>` instruction is to be expanded into respective handler for `intiger` or `register` or `... (other supported types)`.
This specific details shall be provided by `isa-<type>` in case of unsupported types passed as parametere, an (asseble-time) excecption is to be shown.

