---
tags: [ctf, thehackerslabs, linux, mqtt, redis, cron, suid, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: media
estado: resuelto
ip: 192.168.56.157
fecha: 2026-08-15
user: true
root: true
tecnicas: [nmap, vhost-gobuster, mqtt, redis, ssh-keys, cron, suid]
fuente: Roadmap_CTF_El_Baifo.docx
ultima_revision: 2026-10-20
---

# El Baifo — Ficha

> [!info] Resumen
> Linux con MQTT anónimo que filtra un usuario, Redis sin autenticación usado para escribir un `authorized_keys` y entrar por SSH, y escalada por un script de cron modificable + SUID.

## Identificación

| Campo | Valor |
|---|---|
| Nombre | El Baifo |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Dificultad | media |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-08-15 |
| IP | `192.168.56.157` |
| Virtual hosts | `elbaifo.thl` |
| Fuente | `Roadmap_CTF_El_Baifo.docx` (docx de estudio) |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 22 | SSH | acceso final con `pastor01` |
| 80 | HTTP | web + virtual host `elbaifo.thl` |
| 1883 | MQTT | **anónimo** → filtra telemetría |
| 6379 | Redis | **sin auth** → persistencia RDB → `authorized_keys` |

## Resultado

- **User:** ✅ conseguido (`pastor01` vía SSH)
- **Root:** ✅ conseguido (cron con script modificable → SUID)

## Técnicas utilizadas

- [[Nmap]] y [[Arp-Scan]] para descubrimiento.
- [[Gobuster]] + resolución de **virtual hosts**.
- [[MQTT]] — `mosquitto_sub` sobre temas de telemetría.
- [[Redis-Sin-Autenticacion]] — keyspace y persistencia RDB → escritura de `authorized_keys`.
- [[SSH-Keys]] — gestión de pares de claves.
- [[Cron-Privilege-Escalation]] — script de cron modificable ejecutado por root.
- [[SUID]] — escalada a root.

## Notas relacionadas

- [[El Baifo - Roadmap]]
- [[El Baifo - Walkthrough]]
- [[El Baifo - Evidencias]]
- [[El Baifo - Lecciones]]
- [[El Baifo - Defensas]]
