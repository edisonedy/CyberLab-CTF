---
tags: [ctf, thehackerslabs, windows, xampp, lfi, godpotato, seimpersonate, forense, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows 10 Enterprise LTSC (standalone / workgroup)
dificultad: media (web + escalada por Potato)
estado: resuelto
ip: 192.168.56.107
fecha: 2026-10-21
user: true
root: true
tecnicas: [nmap, smb-guest, rid-brute, lfi, file-upload, reverse-shell, seimpersonate, godpotato, juicypotato, password-expired, forense-offline, sleuthkit]
fuente: Phantom.md
ultima_revision: 2026-10-21
---

# Patata Mágica — The Hackers Labs

## Estado
✅ Resuelto (2026-10-21) — flags obtenidas por **lectura forense offline** (la instancia estaba en estado no jugable, ver abajo).

> [!tip] Writeup completo
> Ver [[Patata Magica - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
Windows 10 Enterprise LTSC Evaluation (Build 19041) — standalone, workgroup `Patata-Magica`.

## Entorno
- Hipervisor: VirtualBox (Host-Only `vboxnet0`)
- Kali: 192.168.56.1
- Objetivo: **192.168.56.107** (hostname `PATATA-MAGICA`)
- MAC: 08:00:27:C9:CE:CF

## Flags
Los ficheros `User.txt` / `Root.txt` contienen base64 con mensajes del autor:

- **USER** (`C:\Users\Hacker\Desktop\User.txt`)
  - base64: `YmllbiBoZWNobyBhcXVpIHRpZW5lcyBsYSBiYW5kZXJhMSEh`
  - texto: **"bien hecho aqui tienes la bandera1!!"**
- **ROOT** (`C:\Users\Administrador\Desktop\Root.txt`)
  - base64: `QSBxdWUgZXJhIG1hcyBzZW5jaWxsbyBkZSBsbyBxdWUgcGFyZWNpYT8=`
  - texto: **"A que era mas sencillo de lo que parecia?"**

> [!warning] Flags trampa
> El share `Secreto` (accesible por `guest`) contiene `Banderas.txt` con dos base64 **falsos** que decodifican a *"verdad que te engañe?"* y *"esta tampoco es..."*. Las flags reales están en los escritorios de `Hacker` y `Administrador`.

## Reconocimiento
Standalone Windows (sin AD). Puertos: 135, 139, 445 (SMB), 5985/47001 (WinRM). **Sin puerto 80** en esta instancia (servicio web caído). Usuarios: `patata`, `Hacker`, `www-data`, `Administrador`.

## Camino oficial (intencionado)
Web (XAMPP, Apache como **www-data**) → LFI en `juegos.php?file=` revela creds SMB (`patata:00000`) → RID brute → brute de `Hacker` (rockyou → `clifford`) → `revershell.php` en `/uploads/` da shell como **www-data** con **SeImpersonate** → **GodPotato/JuicyPotato** → SYSTEM.

## Qué pasó en esta instancia (desviación)
- El servicio **Apache2.4** (que corre como `.\www-data`) estaba en **DISABLED** y la VM **no hace auto-login** → el puerto 80 nunca levantaba.
- Las creds del writeup (`patata:00000`, `Hacker:clifford`) eran válidas pero **`STATUS_PASSWORD_EXPIRED`**; al no ser dominio, el cambio por red (SAMR) estaba denegado.
- Solución aplicada: login por **consola** de `patata:00000` (forzó cambio → `Seguridad2025`), reconocimiento local, y al no haber escalada para `patata` → **lectura offline del disco** con sleuthkit.

## Lecciones aprendidas
- `STATUS_PASSWORD_EXPIRED` = contraseña correcta pero caducada; en standalone solo se cambia por **login interactivo** (consola/RDP), no por SMB/SAMR si está restringido.
- Permisos débiles de servicio: `Authenticated Users` tenía **MODIFY** sobre `C:\xampp` y `httpd.exe`, pero inútil con el servicio **Disabled** y sin derecho de START.
- `www-data` solo obtiene **SeImpersonate** cuando Apache corre **como servicio** (SID `SERVICE` en el token), no por login normal.
- Lectura forense offline (`qemu-img` → `mmls`/`fls`/`icat`) salta cualquier ACL de Windows cuando tienes el disco.

Detalle paso a paso y comandos: [[Patata Magica - Roadmap]]

## Notas relacionadas
- [[Patata Magica - Roadmap]]
- [[GodPotato-SeImpersonate]]
- [[Phantom]]
