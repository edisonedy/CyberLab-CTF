---
tags: [ctf, thehackerslabs, defensas]
tipo: defensas
---

# Securitron — Defensas

## Causa raíz

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Fuga desde la IA | Contexto/sistema expuesto al modelo y devuelto al usuario |
| 2 | SQLi | Concatenación de entrada en consultas |
| 3 | Upload PHP | Validación por MIME/cliente, no por contenido real |
| 4 | Path traversal | `fileName` sin normalizar |
| 5 | `sudo ar` | Concesión que permite leer ficheros arbitrarios |
| 6 | PATH hijacking | Cron que llama binarios sin ruta absoluta |

## Impacto

- RCE como `www-data` y escalada a **root**.

## Prevención

- IA: no exponer secretos al modelo; sanear salidas.
- SQLi: **consultas parametrizadas**.
- Upload: validar contenido (magic bytes), renombrar, servir desde otro dominio/directorio, sin ejecución.
- Traversal: `path.resolve` + verificar prefijo permitido.
- `sudoers`: no conceder `ar`/`tar`/pagers.
- Cron: rutas absolutas y `PATH` fijo.

## Detección

- Patrones `SLEEP(`, `OR '1'='1`, `../` en logs.
- Subidas de archivos con `Content-Type` sospechoso.
- Uso de `sudo ar` y modificaciones de `PATH` en cron.

## Hardening recomendado

- [ ] Parametrizar todas las consultas.
- [ ] Endurecer pipeline de subidas.
- [ ] `visudo` mínimo privilegio.
- [ ] Revisar crons: rutas absolutas.
