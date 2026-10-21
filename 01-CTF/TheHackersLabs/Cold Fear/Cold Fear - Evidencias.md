---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# Cold Fear — Evidencias

> [!warning] Origen de los datos
> Documentado en `ColdFear_192.168.56.143_Roadmap.md`. Lo aquí listado es **reconstrucción** del roadmap salvo indicación contraria.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | md | Roadmap de estudio | `~/Escritorio`/docx (no localizado) | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | FTP anónimo → `bitacora-01.txt` | descripción del roadmap |
| 2 | EXIF de `bridge-cam.jpg` → `/var/log/bridge/` | descripción del roadmap |
| 3 | LSB del WAV (2 bits) → `Victor` | descripción del roadmap |
| 4 | `sudo aws help` → `less` → `!sh` → Anna | descripción del roadmap |
| 5 | `sudo find -exec` → root | descripción del roadmap |

## Inferencias (no verificadas)

- El **contenido literal** de las codificaciones Base64/ROT13 y del mensaje LSB no se conserva; solo su estructura y resultado.

## Comandos clave reproducibles

```bash
ftp 192.168.56.143
exiftool bridge-cam.jpg
# Base64 -> ROT13
echo '<b64>' | base64 -d | tr 'A-Za-z' 'N-ZA-Mn-za-m'
sudo -l
sudo aws help
sudo find . -exec /bin/sh \; -quit
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `bridge-cam.jpg` | local | imagen analizada |
| audio WAV | local | análisis LSB |
