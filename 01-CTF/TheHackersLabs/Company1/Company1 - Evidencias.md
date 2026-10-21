---
tags: [ctf, thehackerslabs, linux, evidencias, en-progreso]
tipo: evidencias
estado: en-progreso
---

# Company1 — Evidencias

> [!warning] Estado
> Se documenta lo observado en los registros locales. **No hay capturas literales recuperadas** en esta importación.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | — | (no recuperadas) | — | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Acceso SSH como `marco.silva` | registros |
| 2 | Existencia de cuentas `carlos.lopez` y `rodgar` | enumeración |
| 3 | Fallo de `su` a esas cuentas | registros |

## Inferencias (no verificadas)

- Que alguna de esas cuentas sea vía de escalada: **no confirmado**.

## Comandos clave reproducibles

```bash
ssh marco.silva@192.168.150.135
cat /etc/passwd
systemctl list-timers --all
find / -perm -4000 -type f 2>/dev/null
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| — | — | — |
