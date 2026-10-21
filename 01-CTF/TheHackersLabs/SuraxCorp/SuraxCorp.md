---
tags: [ctf, thehackerslabs, linux, rsync, deb, command-injection, ssti, jinja2, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: media
estado: resuelto
ip: 
fecha: 2026-10-20
user: true
root: true
tecnicas: [rsync, ipv6, deb-analysis, command-injection, ssti, jinja2, sudo-telnet]
fuente: SuraxCorp_Roadmap_Estudio.md
ultima_revision: 2026-10-20
---

# SuraxCorp — Ficha

> [!info] Resumen
> Rsync por IPv6 expone un módulo `backup` con la config de un vhost; un panel con contraseña vacía permite descargar un `.deb`; el servicio alternativo es vulnerable a **command injection**; un Flask interno habilita **SSTI**; escalada con `sudo telnet`.

## Identificación

| Campo | Valor |
|---|---|
| Nombre | SuraxCorp |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-20 |
| Virtual hosts | `secure-suraxcorp.thl`, `adminis-surapharm.suraxcorp.thl` |
| Fuente | `SuraxCorp_Roadmap_Estudio.md` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 873 | Rsync | por **IPv6**, módulo `backup` |
| 80 | HTTP | vhosts y panel |
| 5000 | Flask (interno) | `127.0.0.1:5000` |

## Resultado

- **User:** ✅ `www-data` → `suraxddq`
- **Root:** ✅ `sudo telnet`

## Técnicas utilizadas

- [[Rsync]] por IPv6.
- [[Analisis-Paquetes-Deb]] (inspección de `.deb`).
- [[Command-Injection]] con filtros (`${IFS}`, wildcards).
- [[SSTI-Jinja2]] en Flask (traceback de Werkzeug).
- Enumeración de servicios en localhost.
- [[Sudo-Telnet]].

## Lección central

> **Seguir la pista más fuerte y cambiar una sola variable por prueba.**

## Notas relacionadas

- [[SuraxCorp - Roadmap]]
- [[SuraxCorp - Walkthrough]]
- [[SuraxCorp - Evidencias]]
- [[SuraxCorp - Lecciones]]
- [[SuraxCorp - Defensas]]
