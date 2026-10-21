---
tags: [ctf, thehackerslabs, linux, en-progreso]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
estado: en-progreso
ip: 192.168.150.135
fecha: 2026-10-20
user: null
root: null
tecnicas: [ssh, enumeracion-usuarios, permisos, systemd, timers, procesos]
fuente: registros locales / sesión
ultima_revision: 2026-10-20
---

# Company1 — Ficha

> [!warning] Estado NO confirmado
> Solo hay **acceso SSH como `marco.silva`** documentado. **No existe evidencia suficiente para afirmar root conseguido.** Ver [[Company1 - Pendientes]].

## Identificación

| Campo | Valor |
|---|---|
| Nombre | Company1 (`TheHackersLabs-Company1`) |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Estado | 🟡 En investigación |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.150.135` (según registros) |
| Acceso SSH documentado | `marco.silva` |

## Resultado

- **User:** ⬜ no confirmado (hay acceso SSH como `marco.silva`, pero sin flag documentada)
- **Root:** ⬜ no confirmado

## Trabajo realizado (enumeración)

- Se investigaron las cuentas `carlos.lopez` y `rodgar`.
- Intentos de `su` **fallaron**.
- Se enumeraron: homes, permisos, UID, servicios, timers, procesos y archivos.

## Notas relacionadas

- [[Company1 - Roadmap]]
- [[Company1 - Walkthrough]]
- [[Company1 - Evidencias]]
- [[Company1 - Lecciones]]
- [[Company1 - Defensas]]
- [[Company1 - Pendientes]]
