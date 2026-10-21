---
tags: [ctf, thehackerslabs, windows, smb, exif, esteganografia, powershell, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows 10 / Server 2019 (Build 19041) — standalone
dificultad: media-alta
estado: resuelto
ip: 192.168.56.105
fecha: 2026-10-20
user: true
root: true
tecnicas: [vbox-nic-diagnostico, smb, rid-cycling, exif, stegseek, password-spraying, zip-crack, hashcat, share-writable, powershell-reverse-shell, psexec]
fuente: Folclore.md
ultima_revision: 2026-10-20
---

# Folclore — The Hackers Labs

## Estado
✅ Resuelto (2026-10-20)

> [!tip] Writeup completo
> Ver [[Folclore - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
Windows 10 / Server 2019 (Build 19041) — máquina standalone

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only
- Interfaz: vboxnet0
- Kali: 192.168.56.1
- Objetivo: 192.168.56.105
- Hostname: FOLCLORE — Workgroup: Folclore
- MAC NIC: 08:00:27:EE:0F:0E

## Problema detectado (resuelto)
La VM venía con una NIC en la red `NatNetwork` (inexistente) → sin conectividad.
Solución: cambiar a Host-Only:
```bash
VBoxManage modifyvm "The Hackers Labs - Folclore" --nic1 hostonly --hostonlyadapter1 vboxnet0 --cableconnected1 on
```

## Flags
- USER: `En el viento yace la verdad` (hex en fichero: `456e20656c207669656e746f2079616365206c6120766572646164`)
- ROOT: `En el viento yace la verdDD` (hex en fichero: `456e20656c207669656e746f2079616365206c6120766572644444`)
- Password Administrador: `WwU@49F*`

## Reconocimiento
SMB: 139/445 (signing enabled but not required) + 7680 (Delivery Optimization) + 137/udp.
Sesión nula/guest activa; 6 shares temáticos: Oro, Libertad, Lluvia, Viento, "Dia de Muertos", Santuario.
RID cycling con `guest` → Quetzalcoatl, El_charro_negro, Ix_Chel, Tlaloc, La_mulata_de_Cordoba, La_Catrina.

## Explotación (user)
Ruta de credenciales encadenadas por shares:
```
rockyou → El_charro_negro:abc123 (EXPIRADA)
  └ Oro/El_Charro_Negro.jpeg (EXIF) → Ix_Chel:4+9Ii1wK
      └ Santuario/La clave de Kukulcán.txt → Tlaloc:677Kn$q
          └ Lluvia/Tlaloc.jpg (stegseek "knight") → 45yD#k7
              └ spraying → La_mulata_de_Cordoba:45yD#k7
                  └ Libertad/Mi_amiga.txt + Pacto.zip (john "celina") → La_Catrina:3nM93{S#
                      └ "Dia de Muertos"/Ultimo_mensaje.txt → U + %/
                          └ wordlist J9c?U?%/ → Quetzalcoatl:J9c7U7%/ ─── USER
                              └ Viento → subir .ps1 → reverse shell PowerShell
```

## Escalada de privilegios (admin)
`C:\Users\Quetzalcoatl\Downloads\Actualizacion\Venganza.txt` contiene la clave del admin en claro:
`WwU@49F*` → `impacket-psexec Folclore/Administrador:'WwU@49F*'@192.168.56.105` → SYSTEM → ROOT.

## Lecciones aprendidas
- **RID cycling** sobre SMB para enumerar usuarios sin credenciales.
- Interpretación de **`STATUS_PASSWORD_EXPIRED`**: la password es correcta pero debe cambiarse.
- Contraseñas **expiradas** en Windows 10 19041: SMB bloquea el cambio (el servidor elimina la sesión → `STATUS_USER_SESSION_DELETED`; el pipe `samr` da `ACCESS_DENIED`). El cambio se hace por **GUI/consola**.
- Secretos en **metadatos EXIF** de imágenes (`exiftool`).
- **Esteganografía LSB** con passphrase (`stegseek` + rockyou).
- **Password spraying** dirigido + crackeo de **ZIP** (`zip2john`+john).
- Construcción de wordlist con **patrón conocido** (hashcat `-a 3 --stdout`).
- Share escribible con **tarea programada** que ejecuta `.ps1` → reverse shell.
- Credenciales en claro en el perfil → **psexec** para SYSTEM.

Detalle paso a paso y comandos: [[Folclore - Roadmap]]

## Notas relacionadas

- [[Folclore - Roadmap]]
- [[Folclore - Walkthrough]]
- [[Folclore - Evidencias]]
- [[Folclore - Lecciones]]
- [[Folclore - Defensas]]
