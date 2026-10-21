---
tags: [ctf, thehackerslabs, linux, lecciones, en-progreso]
tipo: lecciones
estado: en-progreso
---

# Company1 — Lecciones aprendidas (parcial)

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Intentar `su` sin credenciales | Tiempo perdido | Buscar primero vectores de escalada sin credenciales |
| Enumeración dispersa (homes/UID/servicios/timers/procesos) sin priorizar | Ruido | Priorizar cron/timers/SUID/servicios con permisos |

## Tiempo perdido y por qué

- Intentos de `su` sin base para las contraseñas.

## Conceptos a reforzar

- Enumeración disciplinada de Linux.
- Lectura de **timers** de systemd (no solo cron).

## Señales para reconocer en otra máquina

- [ ] `/home` con varias cuentas humanas.
- [ ] Timers de systemd ejecutando scripts.
- [ ] Permisos raros en homes.

## Preguntas de autoevaluación

1. ¿Qué diferencias hay entre `cron` y `systemd timers` al buscar escalada?
2. ¿Cómo priorizas la enumeración tras conseguir un acceso SSH?

## Técnicas que necesitan más práctica

- [[Linux-Enum]]
- [[Cron-Privilege-Escalation]]
