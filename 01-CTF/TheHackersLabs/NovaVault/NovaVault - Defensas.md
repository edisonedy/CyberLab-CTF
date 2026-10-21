---
tags: [ctf, thehackerslabs, defensas]
tipo: defensas
---

# NovaVault — Defensas

## Causa raíz

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | SSRF | Endpoint que confía en URL del usuario y valida por lista negra |
| 2 | Command execution | Servicio interno que ejecuta comandos con entrada del usuario |
| 3 | Acceso por grupo | Flag en ruta de grupo `crypto-operators` |
| 4 | SUID + token débil | `vault-guard` valida token con XOR trivial |

## Impacto

- RCE como `www-data`, lectura de la flag y **root**.

## Prevención

- SSRF: lista **blanca** de destinos, resolver y validar la IP final (todas las representaciones), sin seguir redirecciones.
- Command execution: nunca `shell=True` con entrada; usar APIs seguras.
- Principio de mínimo privilegio en grupos y ficheros.
- SUID: eliminar si no es imprescindible; si lo es, token con HMAC, no XOR.

## Detección

- Peticiones a `127.1`, `[::1]`, decimal de loopback.
- Comandos spawnados por el servicio de tareas.
- Ejecución de `vault-guard` fuera de ventanas de mantenimiento.

## Hardening recomendado

- [ ] Validación de URL por allow-list e IP resuelta.
- [ ] Eliminar ejecución de shell en servicios internos.
- [ ] Auditar grupos y permisos de flags.
- [ ] Reemplazar XOR por HMAC en `vault-guard`.
