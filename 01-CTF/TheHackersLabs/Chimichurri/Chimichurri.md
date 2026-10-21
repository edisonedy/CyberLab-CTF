---
tags: [ctf, thehackerslabs, windows, active-directory, domain-controller, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows Server 2016 — Domain Controller (Active Directory)
dificultad: media
estado: resuelto
ip: 192.168.56.101
fecha: 2026-10-20
user: true
root: true
tecnicas: [dhcp-diagnostico, vbox-sethduuid, credenciales-fichero, sleuthkit, ntds-dit, chntpw]
fuente: Chimichurri.md
ultima_revision: 2026-10-20
---

# Chimichurri.thl — The Hackers Labs

## Estado
✅ Resuelto (2026-10-20)

> [!tip] Writeup completo
> Ver [[Chimichurri - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
Windows Server 2016 — **Controlador de Dominio (Active Directory)**
Dominio: `CHIMICHURRI`

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only · Interfaz `vboxnet0`
- Kali: `192.168.56.1`
- DHCP VirtualBox: `192.168.56.2` (pool `.101–.254`)
- MAC de la VM: `08:00:27:3F:F9:0C`
- Disco: `Chimichurri.thl-disk001.vdi` (30 GB)

## Problemas detectados (resueltos)
- La VM arrancaba **sin lease DHCP / con APIPA** → no respondía en la red.
- Tras restaurar el VDI, VirtualBox fallaba con `NS_ERROR_FAILURE` por **UUID del disco distinto**. Solución:
  `VBoxManage internalcommands sethduuid "<vdi>" c9fd0695-6ab4-4e18-8c7f-443d746f1a65`

## Credenciales
- `C:\drogas\credenciales.txt` → pista: usar el usuario **hacker**
- `C:\Users\hacker\Desktop\perico.txt` → **`hacker:Perico69`**

## Flags
- USER: `acrsgvs6edr8f5vaw9a8eadv6fa9b`
  → `C:\Users\hacker\Desktop\user.txt`
- ROOT: `hjafcdv8a75e3cvsdfg6asd4f9vbsf9sa`
  → `C:\Users\Administrador\Desktop\root.txt`

## Ruta
1. Pista en `C:\drogas\credenciales.txt`.
2. Credenciales `hacker:Perico69` en `perico.txt`.
3. Como `hacker` se lee `user.txt`.
4. Escalar a **Administrador** y leer `root.txt`.

## Lecciones aprendidas
- En un **DC** las cuentas están en `NTDS.dit`, **no** en el SAM local: `chntpw` sobre el SAM no cambia el login de dominio.
- El `THL{9A88909B-…}` era un **falso positivo**: nombre de GPO (`CHIMICHURRI.THL`) + GUID del registro.
- Lectura **offline** del NTFS con sleuthkit (`fls`/`icat`) para obtener el flag sin login.
- Corregir el **UUID del VDI** (`sethduuid`) cuando VirtualBox marca el disco como inaccesible.

Detalle paso a paso y comandos: [[Chimichurri - Roadmap]]

## Notas relacionadas

- [[Chimichurri - Roadmap]]
- [[Chimichurri - Walkthrough]]
- [[Chimichurri - Evidencias]]
- [[Chimichurri - Lecciones]]
- [[Chimichurri - Defensas]]
