---
tags: [ctf, thehackerslabs, linux, multi-host, pivoting, ligolo, wireguard, wordpress, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux (multi-host)
dificultad: alta
estado: resuelto
ip: 192.168.56.163
fecha: 2026-10-20
user: true
root: true
tecnicas: [wordpress, my-calendar, sqli-blind, john, php-filters, wrapwrap, ssh-alias, sudo-vim, ligolo-ng, wireguard, rot13, apt-pre-invoke, sudo-nginx]
fuente: THLCPPTV16_CTF_Roadmap.md
ultima_revision: 2026-10-20
---

# THLCPPTV16 — Ficha

> [!info] Resumen
> Entorno **multi-host** con pivoting: WordPress vulnerable (My Calendar SQLi) → Tom; segundo vhost con PHP filters/WrapWrap; alias SSH hacia Rafael; `sudo vim`; pivoting con **Ligolo-ng** + **WireGuard**; ROT13; SSH Jerry; **APT Pre-Invoke**; y finalmente `sudo nginx`.

## Identificación

| Campo | Valor |
|---|---|
| Nombre | THLCPPTV16 / "Examen" |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux (varios hosts) |
| Dificultad | alta (multi-host / pivoting) |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-20 |
| IP objetivo principal | `192.168.56.163` |
| Virtual hosts | `examen.thlcpptv16.thl`, `examendos.thlcpptv16.thl` |
| Fuente | `THLCPPTV16_CTF_Roadmap.md` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 80 | HTTP | WordPress + segundo vhost |
| 22 | SSH | Tom, Rafael, Jerry |
| varios | Redes internas | pivote con Ligolo-ng / WireGuard |

## Resultado

- **User:** ✅ (Tom / WordPress)
- **Root:** ✅ (root final vía `sudo nginx`)

## Técnicas utilizadas

- [[SQL-Injection]] ciega (time-based) en **My Calendar** (WordPress).
- [[John-The-Ripper]] sobre hash **phpass**.
- [[PHP-Filters]] y [[WrapWrap]].
- Reutilización de credenciales y **alias SSH**.
- [[Sudo-Vim]].
- [[Pivoting-Ligolo]] y [[WireGuard]].
- [[ROT13]].
- [[APT-Pre-Invoke]] como root.
- [[Sudo-Nginx]].

## Redes y hosts (resumen)

Ver el diagrama en [[THLCPPTV16 - Roadmap]]. Distinguir **acceso inicial**, **hosts intermedios** y **objetivo final**.

## Notas relacionadas

- [[THLCPPTV16 - Roadmap]]
- [[THLCPPTV16 - Walkthrough]]
- [[THLCPPTV16 - Evidencias]]
- [[THLCPPTV16 - Lecciones]]
- [[THLCPPTV16 - Defensas]]
