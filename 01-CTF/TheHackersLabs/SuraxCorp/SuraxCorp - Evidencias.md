---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# SuraxCorp — Evidencias

> [!warning] Origen
> Documentado en `SuraxCorp_Roadmap_Estudio.md`. Contenido **reconstruido** salvo indicación.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | md | Roadmap de estudio | (documento ChatGPT, no en disco) | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Rsync IPv6 módulo `backup` con config de vhost | roadmap |
| 2 | Login admin con contraseña vacía | roadmap |
| 3 | `.deb` `surapharm.deb` con vhost alternativo | roadmap |
| 4 | SSTI en `/event` con `Template(description)` | roadmap |
| 5 | `sudo telnet` como escalada | roadmap |

## Inferencias (no verificadas)

- El **payload exacto** de command injection y la cadena SSTI concreta no se conservan; se documenta el método y el sink.

## Comandos clave reproducibles

```bash
rsync -av --list-only rsync://[<IPv6>]:873/backup/
dpkg-deb -x surapharm.deb ./data
curl -s -X POST http://127.0.0.1:5000/event --data 'description={{7*7}}'
sudo -l
sudo telnet
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `surapharm.deb` | local | paquete analizado |
| copia del módulo `backup` | local | config de vhost |
