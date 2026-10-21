---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# El Baifo — Evidencias

> [!warning] Origen de los datos
> Este laboratorio se documentó originalmente en `Roadmap_CTF_El_Baifo.docx`. Las salidas aquí son **reconstrucciones** del roadmap de estudio salvo que se indique lo contrario. No hay capturas literales recuperadas.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | docx | Roadmap de estudio | (documento de ChatGPT, no en disco) | sesión original |

## Reconstrucciones

| # | Qué | Base de la reconstrucción |
|---|---|---|
| 1 | Puertos abiertos `22/80/1883/6379` | lista de puertos del roadmap |
| 2 | Usuario `pastor01` filtrado por MQTT | descripción del roadmap |
| 3 | Escritura de `authorized_keys` vía Redis | descripción del roadmap |

## Inferencias (no verificadas)

- El SUID concreto usado para la escalada no queda especificado en el material disponible; se documenta el **método** (SUID + `bash -p`) pero no el binario exacto.

## Comandos clave reproducibles

```bash
arp-scan -I eth0 -g 192.168.56.0/24
nmap -p- --open -sS --min-rate 5000 -n -Pn 192.168.56.157
mosquitto_sub -h 192.168.56.157 -t '#' -v
redis-cli -h 192.168.56.157
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| clave SSH de laboratorio | `./baifo_key` | desechable, solo CTF |
