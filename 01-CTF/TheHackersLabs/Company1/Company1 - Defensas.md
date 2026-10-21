---
tags: [ctf, thehackerslabs, linux, defensas, en-progreso]
tipo: defensas
estado: en-progreso
---

# Company1 — Defensas (preliminar)

> [!note] Preliminar
> Sin causa raíz confirmada (máquina sin resolver). Se listan buenas prácticas aplicables.

## Causa raíz

- Pendiente de confirmar (no hay explotación documentada).

## Impacto

- Pendiente de determinar.

## Prevención (recomendaciones generales)

- Mínimo privilegio en cuentas y grupos.
- Revisar timers/cron y permisos de scripts.
- Auditar binarios SUID/SGID.
- Autenticación fuerte (claves/2FA) y rotación.

## Detección

- Ejecución inesperada de tareas programadas.
- Intentos de `su` fallidos repetidos.

## Hardening recomendado

- [ ] Inventario de tareas programadas y sus permisos.
- [ ] Limpieza de SUID innecesarios.
