---
tags: [herramienta, enumeracion, linux]
tipo: herramienta
categoria: linux
ultima_revision: 2026-10-20
---

# LinPEAS / pspy

Herramientas de enumeración de escalada privilegios en Linux.

- **LinPEAS**: script que revisa SUID, capabilities, cron, permisos, servicios, etc.
- **pspy**: monitoriza procesos **sin** root para detectar tareas periódicas.

## Uso típico

```bash
./linpeas.sh | tee linpeas.out
./pspy64
```

## Notas / lecciones

- `pspy` es clave para descubrir **cron/timers de root** (El Baifo, Securitron).

## Técnicas relacionadas

- [[Linux-Enum]], [[Cron-Privilege-Escalation]]
