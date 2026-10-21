---
tags: [herramienta, reversing, objdump]
tipo: herramienta
categoria: reversing
ultima_revision: 2026-10-20
---

# objdump

Desensamblado de binarios ELF/PE; útil para entender validaciones (p.ej. tokens SUID).

## Uso típico

```bash
file binario
strings -a binario | less
objdump -d -M intel binario | sed -n '/funcion/,/ret/p'
```

## Técnicas relacionadas

- [[Reversing-Token-Validation]], [[XOR]]
