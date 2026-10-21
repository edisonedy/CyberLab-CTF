---
tags: [ctf, thehackerslabs, windows, evidencias, smb]
tipo: evidencias
---

# Folclore — Evidencias

> [!warning] Origen
> Basado en [[Folclore]] y [[Folclore - Roadmap]]. Los flags se extrajeron tanto de los ficheros (`User.txt`, `Root.txt`, `Venganza.txt`) como por **lectura offline del VDI**.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Folclore/Folclore.md` | sesión original |
| 2 | ficheros (VM) | `User.txt`, `Venganza.txt`, `Root.txt` | `Users/Quetzalcoatl/Desktop/`, `Users/Quetzalcoatl/Downloads/Actualizacion/`, `Users/Administrador/Desktop/` | laboratorio |
| 3 | VDI | `The Hackers Labs - Folclore-disk001.vdi` | VirtualBox VMs | laboratorio |
| 4 | scripts | intentos de cambio de contraseña por SMB/NTLM (`chpw_*.py`, `chpw_samr.py`, …) | `~/CTF/folclore/scripts/` | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Cadena de credenciales por shares | roadmap |
| 2 | Extracción de flags (hex → texto) | contenido de los ficheros |

## Contenido de los flags (tal cual)

| Flag | Hex en fichero | Valor |
|---|---|---|
| USER | `456e20656c207669656e746f2079616365206c6120766572646164` | `En el viento yace la verdad` |
| ROOT | `456e20656c207669656e746f2079616365206c6120766572644444` | `En el viento yace la verdDD` |

## Comandos clave reproducibles

```bash
netexec smb 192.168.56.105 -u guest -p '' --rid-brute
exiftool El_Charro_Negro.jpeg
stegseek Tlaloc.jpg rockyou.txt
zip2john Pacto.zip > pacto.hash && john --wordlist=rockyou.txt pacto.hash
impacket-psexec Folclore/Administrador:'WwU@49F*'@192.168.56.105
```

## Montaje del VDI (para extracción offline)

```bash
7z x "The Hackers Labs - Folclore-disk001.vdi"          # extraer particiones NTFS
ntfs-3g -o ro,recover,norecover 1.ntfs /mnt/folclore    # 'recover' por NTFS "sucio"
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `*.ntfs` temporales | `/home/edison/CTF/folclore/work/` | **ya limpiados** |
| `scans/` | `/home/edison/CTF/folclore/scans/` | resultados de nmap |
