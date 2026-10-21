---
tags: [ctf, thehackerslabs, windows, buffer-overflow, vulnserver, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows Server 2008 (32-bit)
dificultad: media
estado: resuelto
ip: 
fecha: 2026-10-20
user: true
root: true
tecnicas: [nmap, buffer-overflow, vulnserver, pattern-create, jmp-esp, shellcode, sleuthkit]
fuente: Pila.md
ultima_revision: 2026-10-20
---

# Pila — The Hackers Labs

## Estado
✅ Resuelto (2026-10-20)

> [!tip] Writeup completo
> Ver [[Pila - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
**Windows Server 2008 (32-bit)**

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only · Interfaz `vboxnet0`
- Kali: `192.168.56.1`
- MAC de la VM: `08:00:27:EB:F7:86`
- Disco: `The Hackers Labs - Pila-disk001.vdi` (25 GB)
- ISO: `...-disk002.iso` (medio de instalación de Windows Server 2008)

## Usuarios
- `Administrador` (admin)
- `Pila` (usuario normal)

## Vulnerabilidad principal
En el escritorio del Administrador hay:
- `vulnserver.exe` / `vulnserver.c`
- `essfunc.dll` / `essfunc.c`

**Vulnserver** escucha en el puerto **9999** y es vulnerable a **desbordamiento de pila (stack Buffer Overflow)**.
Comandos: `STATS`, `RTIME`, `LTIME`, `SRUN`, `TRUN`, `GMON`, `GDOG`, `KSTET`, `GTER`, `HTER`, `LTER`, `KSTAN`.

`Function3()` usa `char Buffer2S[2000]; strcpy(Buffer2S, Input);`
→ comando **`TRUN`** (y `GMON`) desbordan la pila.

## Flags

| Flag | Valor | Ubicación |
|---|---|---|
| **USER** | `31633bb7e23ba299d0454e1268de2193` | `C:\Users\Pila\Desktop\user.txt` |
| **ROOT** | `be3751d9c6d3a81a5167e599fcddd622` | `C:\Users\Administrador\Desktop\root.txt` |

> El fichero contiene el hash seguido de `"  -"`; el flag es el hash de 32 caracteres.

## Ruta
1. Escanear el puerto **9999** → servicio `vulnserver`.
2. Enviar `TRUN /.:/` + buffer para desbordar (tras `TRUN ` se copia en `Buffer2S[2000]`).
3. Calcular offset hasta EIP con `pattern_create`, buscar `JMP ESP`, meter shellcode y obtener shell.
4. Leer `user.txt` (Pila) y `root.txt` (Administrador).

## Lecciones aprendidas
- Stack Buffer Overflow clásico en Windows x86 (Server 2008, sin ASLR).
- Banner de Vulnserver y enumeración de comandos con `HELP`.
- Cálculo de offset (`msf-pattern_create`), `JMP ESP`, badchars y shellcode `windows/shell_reverse_tcp`.
- Lectura **offline** del NTFS con sleuthkit (`fls`/`icat`) para extraer los flags sin explotar.

Detalle paso a paso y comandos: [[Pila - Roadmap]]

## Notas relacionadas

- [[Pila - Roadmap]]
- [[Pila - Walkthrough]]
- [[Pila - Evidencias]]
- [[Pila - Lecciones]]
- [[Pila - Defensas]]
