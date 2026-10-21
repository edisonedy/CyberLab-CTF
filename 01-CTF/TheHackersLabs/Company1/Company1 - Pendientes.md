---
tags: [ctf, thehackerslabs, linux, pendientes, en-progreso]
tipo: pendientes
estado: en-progreso
ip: 192.168.150.135
---

# Company1 — Pendientes

> [!important] Solo hechos confirmados
> Este documento describe el **último punto confirmado**. Las hipótesis van separadas y priorizadas, y **no** se presentan como hechos.

## Último punto confirmado

- Acceso **SSH como `marco.silva`** documentado.
- Cuentas enumeradas: `carlos.lopez`, `rodgar`.
- `su` a esas cuentas **falló** (sin credenciales).
- Se enumeraron homes, permisos, UID, servicios, timers, procesos y archivos, **sin resultado de escalada confirmado**.

## Hipótesis priorizadas

| # | Hipótesis | Fundamento | Comando de comprobación (no destructivo) |
|---|---|---|---|
| 1 | Tarea programada (cron/timer) ejecutada por root con script escribible | Enumeración incluía cron/timers sin cerrarla | `systemctl list-timers --all; ls -la /etc/cron*; cat /etc/crontab` |
| 2 | Binario **SUID/SGID** poco común | Clásico vector Linux | `find / -perm -4000 -type f 2>/dev/null` |
| 3 | **Capabilities** en binarios | No documentado aún | `getcap -r / 2>/dev/null` |
| 4 | Permisos laxos en el home de otro usuario | Se revisaron homes | `ls -laR /home 2>/dev/null` |
| 5 | Credenciales reutilizadas (config, historial, `.bash_history`) | Patrón habitual | `grep -riE 'pass|pwd' ~ 2>/dev/null` |
| 6 | Servicio systemd ejecutándose como otro usuario con ruta escribible | Enumeración de servicios | `systemctl cat <svc>; ls -l <ruta>` |

## Próximo punto de reanudación

1. Confirmar identidad/grupos: `id; groups; sudo -l`.
2. Revisar **timers** de systemd y sus ejecutables/permisos.
3. Buscar SUID/SGID + capabilities.
4. Revisar `~/.bash_history` y configs.

## Hipótesis descartadas (documentadas)

| Hipótesis | Por qué se descartó |
|---|---|
| `su` a `carlos.lopez` / `rodgar` | No se disponía de sus contraseñas; los intentos fallaron |
| Root ya conseguido | **No hay evidencia** que lo respalde |
