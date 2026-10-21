---
tags: [ctf, thehackerslabs, linux, sqli, file-upload, path-traversal, nodejs, suid, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: media
estado: resuelto
ip: 192.168.56.167
fecha: 2026-10-20
user: true
root: true
tecnicas: [nmap, vhost, sqli, file-upload, nodejs, path-traversal, sudo-ar, path-hijacking, suid]
fuente: roadmap_securitron_ctf.md
ultima_revision: 2026-10-20
---

# Securitron — Ficha

> [!info] Resumen
> Fuga de vhost y API key desde una app con IA; SQLi en el login; subida de avatar con PHP; API Node/Express interna por IPv6; path traversal; `sudo ar` para leer crontab; y PATH hijacking en un cron de root.

## Identificación

| Campo | Valor |
|---|---|
| Nombre | Securitron |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.56.167` |
| Virtual hosts | `admin19-32.securitron.thl` |
| Fuente | `roadmap_securitron_ctf.md` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 22 | SSH | acceso como `securitybot` |
| 80 | HTTP | app + vhost |
| ::1:3000 | Node/Express (IPv6 localhost) | API interna |

## Resultado

- **User:** ✅ `www-data` → `securitybot`
- **Root:** ✅ PATH hijacking en cron de root

## Técnicas utilizadas

- [[Fuga-Info-IA]] — vhost/API key filtrados.
- [[SQL-Injection]] (boolean/time-based).
- [[File-Upload]] con PHP incrustado y validaciones MIME.
- Diferencia **localhost IPv4 vs IPv6** (`::1:3000`).
- [[Path-Traversal]] en Node/Express y `curl --path-as-is`.
- [[Sudo-Ar]] para leer crontab.
- [[Path-Hijacking]] en cron y [[SUID]].

## Notas relacionadas

- [[Securitron - Roadmap]]
- [[Securitron - Walkthrough]]
- [[Securitron - Evidencias]]
- [[Securitron - Lecciones]]
- [[Securitron - Defensas]]
