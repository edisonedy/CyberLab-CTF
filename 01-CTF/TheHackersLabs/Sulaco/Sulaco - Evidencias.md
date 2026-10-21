---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# Sulaco — Evidencias

> [!warning] Origen
> Basado en [[Sulaco]] y [[Sulaco - Roadmap]].

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Sulaco/Sulaco.md` | sesión original |
| 2 | md | Roadmap | `01-CTF/TheHackersLabs/Sulaco/Sulaco - Roadmap.md` | sesión original |
| 3 | disco | `The Hackers Labs - Sulaco-disk001.vdi` | VirtualBox VMs | laboratorio |
| 4 | copia | `sulaco.raw` | local | copia para análisis offline |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Endpoints (`/admin`, `telemetry_ua`, `admin_flag`, `/ip.php`) | fuzzing + robots.txt |
| 2 | Requisito de doble-submit de nonce | prueba con `curl` |

## Inferencias (no verificadas)

- El valor `<id>` y el nonce concretos no se conservan; se documenta el **mecanismo** (`md5('sulaco-secret-'.$id)`), no valores literales.

## Comandos clave reproducibles

```bash
curl -s http://192.168.56.101:8080/robots.txt
N=$(openssl rand -hex 16)
curl -s -X POST http://192.168.56.101:8080/api/telemetry_ua.php -A "<UA navegador>" -H "X-Sulaco-Nonce: $N" -b "sulaco_nonce=$N"
fls -o 2048 -r -p sulaco.raw | grep -i flag
icat -o 2048 sulaco.raw 1045959
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `sulaco.raw` | local | imagen para extracción offline |
