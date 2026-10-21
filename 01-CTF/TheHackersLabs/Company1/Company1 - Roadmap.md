---
tags: [ctf, thehackerslabs, linux, roadmap, en-progreso]
tipo: roadmap
estado: en-progreso
ip: 192.168.150.135
fecha: 2026-10-20
---

# Company1 — Roadmap (parcial, en progreso)

> [!warning] Roadmap incompleto
> No hay cadena de explotación confirmada. Este documento recoge lo **verificado** y lo **pendiente**. Ver [[Company1 - Pendientes]].

## Lo confirmado

```mermaid
flowchart TD
    A[Reconocimiento] --> B[SSH disponible]
    B --> C[Acceso SSH como marco.silva]
    C --> D[Enumeracion: homes, permisos, UID, servicios, timers, procesos]
    D --> E{su a carlos.lopez / rodgar}
    E -->|Fallo| F[Bloqueado]
```

## Etapas verificadas

### 1. Reconocimiento
- **Objetivo:** superficie de la máquina.
- **Evidencia que permite avanzar:** servicio **SSH** disponible.

### 2. Acceso SSH como `marco.silva`
- **Objetivo:** acceso inicial.
- **Evidencia:** acceso SSH documentado.

### 3. Enumeración post-acceso
- **Objetivo:** encontrar vectores de escalada.
- **Evidencia:** se revisan **homes, permisos, UID, servicios, timers, procesos y archivos**.
- **Decisión:** probar `su` a otras cuentas → **falló**.

## Bloqueo actual

- No se logró escalada; `su` a `carlos.lopez` y `rodgar` **falló**.

## Próximos pasos

Ver [[Company1 - Pendientes]] (hipótesis priorizadas y comandos no destructivos).

## Enlaces

- [[Company1 - Walkthrough]]
- [[Company1 - Evidencias]]
- [[Company1 - Pendientes]]
