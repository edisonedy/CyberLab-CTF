---
tags: [ctf, thehackerslabs, linux, defensas]
tipo: defensas
---

# El Baifo — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | MQTT anónimo | Broker sin autenticación ni ACLs; publica datos internos |
| 2 | Redis sin `AUTH` | Servicio expuesto sin contraseña ni `bind` a localhost |
| 3 | Cron de root con script escribible | Permisos laxos sobre un script ejecutado por root |
| 4 | SUID explotable | Binario SUID innecesario o mal configurado |

## Impacto

- Exposición de información interna (usuarios, telemetría).
- **Escritura arbitraria de ficheros** como el usuario de Redis → acceso remoto por SSH.
- Escalada a **root** total.

## Prevención

- MQTT: `allow_anonymous false`, autenticación por usuario y ACLs por tema.
- Redis: `requirepass`, `bind 127.0.0.1`, `rename-command CONFIG ""`, firewall.
- Cron: scripts propiedad de root, sin escritura para otros, rutas absolutas.
- Auditar binarios SUID y eliminar los innecesarios.

## Detección

- Conexiones a Redis sin `AUTH` desde IPs no esperadas.
- Escrituras en `~/.ssh/authorized_keys` fuera de ventanas de gestión.
- Cambios en `CONFIG dir`/`dbfilename` de Redis (comandos en logs/SIEM).

## Hardening recomendado

- [ ] Deshabilitar modo anónimo en MQTT y dividir por ACLs.
- [ ] `requirepass` + `bind` local en Redis.
- [ ] Revisar permisos de todo script invocado por cron/root.
- [ ] Inventario y limpieza de SUID.
