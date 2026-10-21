---
tags: [ctf, thehackerslabs, linux, ssrf, suid, reversing, xor, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: media
estado: resuelto
ip: 192.168.56.13
fecha: 2026-10-20
user: true
root: true
tecnicas: [flask-api-gateway, ssrf, loopback-bypass, command-execution, grupos-linux, suid, objdump, xor, reversing]
fuente: ~/ctf/novavault/SOLVED.md (no localizado)
ultima_revision: 2026-10-20
---

# NovaVault — Ficha

> [!info] Resumen
> API Gateway Flask con **SSRF** (bypass de loopback con `127.1`), descubrimiento de endpoints internos, **command execution** como `www-data`, pertenencia al grupo `crypto-operators`, y escalada **revirtiendo el binario SUID `vault-guard`** (token de recuperación con XOR `0x5A`).

## Identificación

| Campo | Valor |
|---|---|
| Nombre | NovaVault |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Estado | ✅ Resuelto (user + root) |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.56.13` |
| Virtual hosts | `api.novavault.thl` |
| Fuente | `~/ctf/novavault/SOLVED.md` (declarado; **no localizado en disco**) |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 80/443 | HTTP | API Gateway Flask (`api.novavault.thl`) |
| — | Servicios internos | descubiertos por SSRF |

## Resultado

- **User:** ✅ (vía `crypto-operators` + user flag)
- **Root:** ✅ (SUID `vault-guard` + XOR `0x5A`)

## Técnicas utilizadas

- [[SSRF]] en `/api/v1/vault/inspect`.
- Bypass de validación loopback con `127.1`.
- Descubrimiento de endpoints (`/api/v1/docs`).
- [[SSRF-POST]] en `/api/v1/vault/sync` → servicio interno de tareas.
- [[Command-Execution]] como `www-data`.
- [[Grupos-Linux]] (`crypto-operators`) para leer la user flag.
- Análisis de binario **SUID** `vault-guard`: `strings`, `file`, `objdump`.
- [[Reversing-Token-Validation]] y [[XOR]] (`0x5A`).
- Diferencia entre shell root y comandos fuera de ella; limpieza de artefactos.

## Notas relacionadas

- [[NovaVault - Roadmap]]
- [[NovaVault - Walkthrough]]
- [[NovaVault - Evidencias]]
- [[NovaVault - Lecciones]]
- [[NovaVault - Defensas]]
