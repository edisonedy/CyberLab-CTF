---
tags: [ctf, thehackerslabs, linux, ftp, exif, esteganografia, gtfobins, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: media
estado: resuelto
ip: 192.168.56.143
fecha: 2026-10-20
user: true
root: true
tecnicas: [ftp-anonimo, exif, esteganografia-wav-lsb, base64, rot13, gtfobins, sudo]
fuente: ColdFear_192.168.56.143_Roadmap.md
ultima_revision: 2026-10-20
---

# Cold Fear — Ficha

> [!info] Resumen
> FTP anónimo con una bitácora; análisis EXIF de una foto; esteganografía LSB en un WAV; una cadena de "tres sesiones"/usuarios y escalada final con `sudo find -exec`.

## Identificación

| Campo | Valor |
|---|---|
| Nombre | Cold Fear |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Dificultad | media |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.56.143` |
| Fuente | `ColdFear_192.168.56.143_Roadmap.md` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 21 | FTP | **anónimo** → `bitacora-01.txt` |
| 22 | SSH | transiciones de usuario |
| 80 | HTTP | `bridge-cam.jpg` |

## Resultado

- **User:** ✅ conseguido
- **Root:** ✅ conseguido (`sudo find -exec`)

## Técnicas utilizadas

- [[FTP-Anonimo]]
- [[Exif-Metadata]] — análisis y comparación de imágenes.
- [[Esteganografia-WAV-LSB]] — reconstrucción de 2 bits LSB en audio PCM.
- [[Base64]] y [[ROT13]].
- [[GTFOBins]] — escape del pager `less` con `sudo`.
- [[Sudo-Find-Exec]] — escalada a root.

## Notas relacionadas

- [[Cold Fear - Roadmap]]
- [[Cold Fear - Walkthrough]]
- [[Cold Fear - Evidencias]]
- [[Cold Fear - Lecciones]]
- [[Cold Fear - Defensas]]
