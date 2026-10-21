---
tags: [tecnica, escalada-linux, cron, systemd]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [El Baifo, Securitron, THLCPPTV16, Company1]
---

# Cron / Tareas programadas — Escalada

## 1. Qué es

Abuso de tareas programadas ejecutadas por **root** cuyos scripts, rutas o binarios pueden ser controlados por un usuario sin privilegios.

## 2. Cómo funciona

- Un `cron`/`systemd timer` ejecuta un **script escribible** o un binario resuelto por un **`PATH`** manipulable.
- Al modificarlo/sustituirlo, la siguiente ejecución corre código como root.

## 3. Cuándo sospecharla

- Scripts de cron/timer escribibles por el usuario actual.
- Tareas que llaman binarios **sin ruta absoluta** (`date`, `tar`, `find`).
- Rutas con wildcards en carpetas escribibles.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
crontab -l; cat /etc/crontab; ls -la /etc/cron.*
systemctl list-timers --all; systemctl cat <servicio>.timer
ls -la <script>            # ¿escribible?
# PATH hijacking: crear binario falso en un dir anterior en PATH
```

## 5. Herramientas

- `pspy`, `find`, `systemctl`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | Cron root con script modificable | root |
| [[Securitron]] | Cron root + **PATH hijacking** (`date`) | root |
| [[THLCPPTV16]] | Hooks de **APT** (Pre-Invoke) como root | root de Jerry |
| [[Company1]] | Timers/cron en investigación | pendiente |

## 7. Falsos positivos / pruebas fallidas

- Un script escribible que se ejecuta como **nuestro** usuario no escala nada: comprobar el propietario/contexto.
- Cambiar varias rutas a la vez impide saber cuál funcionó.

## 8. Prevención y mitigación

- Scripts propiedad de root, **sin escritura** para otros; rutas absolutas.
- `PATH` fijo en cron; evitar wildcards en carpetas escribibles.

## 9. Técnicas relacionadas

- [[Path-Hijacking]], [[SUID]], [[APT-Pre-Invoke]]
