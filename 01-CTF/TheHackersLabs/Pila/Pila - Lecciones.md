---
tags: [ctf, thehackerslabs, windows, lecciones, buffer-overflow]
tipo: lecciones
---

# Pila — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| (no documentado) | — | — |

> Se conserva la sección para mantener el formato; no hay errores registrados en el material.

## Conceptos aprendidos

- [[Buffer-Overflow]] de pila en **Windows x86** (Server 2008, sin ASLR).
- Banner de **Vulnserver** y enumeración de comandos con `HELP`.
- Cálculo de **offset** (`msf-pattern_create`/`pattern_offset`).
- **`JMP ESP`**, validación de **badchars** y shellcode `windows/shell_reverse_tcp`.
- Lectura **offline** del NTFS con [[Sleuthkit]] (`fls`/`icat`).

## Señales para reconocer en otra máquina

- [ ] Puerto con banner de servicio «de laboratorio» (`vulnserver`).
- [ ] Binario con funciones que usan `strcpy`/`sprintf` sin límites.
- [ ] Sistemas **sin ASLR/DEP** (x86 antiguos) → BOF clásico viable.

## Preguntas de autoevaluación

1. ¿Cómo calculas el offset exacto hasta EIP?
2. ¿Por qué `JMP ESP` es útil cuando `ESP` apunta a tu buffer?
3. ¿Cómo identificas y evitas **badchars** en el shellcode?

## Técnicas que necesitan más práctica

- [[Buffer-Overflow]] (con mona.py / windbg).
- [[Sleuthkit]] para extracción offline.
