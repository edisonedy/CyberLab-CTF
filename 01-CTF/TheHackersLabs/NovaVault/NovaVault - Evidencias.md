---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# NovaVault — Evidencias

> [!warning] Origen
> Fuente declarada: `~/ctf/novavault/SOLVED.md` (**no localizado en disco**). Contenido **reconstruido** del material de la misión.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | md | `SOLVED.md` | `~/ctf/novavault/SOLVED.md` | ❌ no encontrado |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | SSRF `/api/v1/vault/inspect` + bypass `127.1` | material de la misión |
| 2 | `/api/v1/docs` y SSRF POST `/vault/sync` | material de la misión |
| 3 | Grupo `crypto-operators` | material de la misión |
| 4 | SUID `vault-guard`, `verify_recovery_token`, XOR `0x5A` | material de la misión |

## Inferencias (no verificadas)

- El **token de recuperación** exacto y el formato del endpoint de tareas no se conservan.

## Comandos clave reproducibles

```bash
curl -s -X POST http://api.novavault.thl/api/v1/vault/inspect \
  -H 'Content-Type: application/json' -d '{"url":"http://127.1:5000/"}'
curl -s http://api.novavault.thl/api/v1/docs
objdump -d -M intel /usr/local/bin/vault-guard
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| — | — | limpiar artefactos del mecanismo de recuperación |
