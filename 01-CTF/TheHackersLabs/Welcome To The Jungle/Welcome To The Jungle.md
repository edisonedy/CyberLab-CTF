---
tags: [ctf, thehackerslabs, windows, iis, php, esteganografia, webshell, impersonacion, godpotato, resolta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows Server 2022 (IIS 10.0 + PHP 8.4.10)
dificultad: media
estado: resuelto
ip: 192.168.56.106
hostname: THEHEXGUNS
fecha: 2026-10-21
user: true
root: true
tecnicas: [esteganografia-wav-lsb, stegseek, reverse-engineering-strings, php-webshell, seimpersonateprivilege, godpotato, windows-privesc, evil-winrm]
fuente: Welcome To The Jungle.md
ultima_revision: 2026-10-21
---

# Welcome To The Jungle — The Hackers Labs

## Estado
✅ Resuelto (2026-10-21) — USER y ROOT obtenidos

> [!tip] Writeup completo
> Ver [[Welcome To The Jungle - Roadmap]]

## Identificación

| Campo | Valor |
|---|---|
| Nombre | Welcome To The Jungle |
| Plataforma | The Hackers Labs |
| Sistema operativo | Windows Server 2022 — **IIS 10.0 + PHP 8.4.10** |
| Hostname | `THEHEXGUNS` |
| Dificultad | media |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-21 |
| IP | `192.168.56.106` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 80/tcp | HTTP (IIS 10.0) | Sitio público de la banda + aplicación `theh3xgun5` |
| 135, 139, 445/tcp | RPC / NetBIOS / SMB | |
| 5985/tcp | WinRM | Escalada a shell remota como `axl` |
| 47001/tcp | HTTPAPI | |
| 49664-49670/tcp | RPC dinámico | |

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only · Interfaz `vboxnet0` · Atacante `192.168.56.1`
- Disco: `The Hackers Labs - Welcome To The Jungle` (VDI 51 GB)
- **Reloj del sistema desincronizado** (2026) → las contraseñas de laboratorio aparecían como *expiradas*

## Flags

| Nivel | Flag | Ubicación |
|---|---|---|
| USER | `8f1aecc0263f4ccc3d67ac0bb6f218a9` | `C:\Users\Axl\Desktop\user.txt` |
| ROOT | `ef708b85a127545dec1ea99dcd1ebb17` | `C:\Users\Administrador\Desktop\root.txt` |

## Credenciales

| Usuario | Contraseña | Origen |
|---|---|---|
| `slash` | `sweetjungle2025` | Steganografía en `solo_final.wav` |
| `thehexguns\axl` | `SoloMaster2025` (caducada) → `NuevaPass2026!` | Strings de `commlink.exe` + cambio por consola |
| `thehexguns\Administrador` | — | Objetivo de escalada (cuenta local por defecto) |

## Ruta
1. Esteganografía en `solo_final.wav` (stegseek) → credenciales web `slash:sweetjungle2025` y ruta `theh3xgun5`.
2. Login web → `panel.php` entrega `commlink.exe` → strings: passphrase `GuitarHeroes1987` y `axl password: SoloMaster2025`.
3. Contraseña de `axl` **expirada** → cambio por consola interactiva de VirtualBox (VNC + `keyboardputscancode`) → `NuevaPass2026!`.
4. **USER flag** en `C:\Users\Axl\Desktop\user.txt` vía WinRM (`evil-winrm`).
5. Descubrimiento del **webshell** en `panel.php` (`shell_exec($_POST['cmd'])`) → ejecución como `IIS AppPool\DefaultAppPool` con **`SeImpersonatePrivilege`**.
6. **Escalada a SYSTEM** con `GodPotato` → **ROOT flag**.

> [!warning] Falso positivo importante
> `C:\Program Files\HexGuns\setlist_uploader.exe` (cargaba `config.dll`, con la carpeta escribible por `BUILTIN\Usuarios`) **es un señuelo**: su `uploader.log` no se modifica desde el 27/07/2025 **ni tras reiniciar la VM**, y no existe ninguna tarea programada, servicio, suscripción WMI ni clave `Run` que lo invoque. El hijack de DLL nunca se habría disparado.

## Lecciones aprendidas
- Un **webshell PHP** en `panel.php` puede ser el vector principal, no un binario "sospechoso" en disco.
- `IIS AppPool\DefaultAppPool` tiene **`SeImpersonatePrivilege`** + `SeAssignPrimaryTokenPrivilege` → ruta directa a SYSTEM con la familia *Potato* (`GodPotato`).
- El **formato del log** puede delatar un señuelo: `uploader.log` en inglés vs. strings del exe en español → el log **no lo generó ese binario**.
- `schtasks /query` como usuario **no privilegiado** no lista todas las tareas; un `Get-ScheduledTask` con el **app pool de IIS** sí enumeró 159 tareas (todas de Microsoft) → descarta el trigger.
- Con `SeImpersonatePrivilege` se puede enumerar **mucho más** que como usuario normal (`sc query`, `schtasks`, `Get-CimInstance`), porque el token del app pool tiene permisos distintos.
- Un binario "que carga DLLs" solo es exploitable si **algo lo ejecuta** con privilegios: buscar siempre el *trigger* antes de construir el exploit.

Detalle paso a paso y comandos: [[Welcome To The Jungle - Roadmap]]

## Notas relacionadas

- [[Welcome To The Jungle - Roadmap]]
- [[Welcome To The Jungle - Walkthrough]]
- [[Welcome To The Jungle - Evidencias]]
- [[Welcome To The Jungle - Lecciones]]
- [[Welcome To The Jungle - Defensas]]
- [[Esteganografia-WAV-LSB]]
- [[Sleuthkit]]
