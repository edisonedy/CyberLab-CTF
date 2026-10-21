---
tags: [ctf, thehackerslabs, linux, defensas, lfi]
tipo: defensas
---

# BabyCorp — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Lectura arbitraria de archivos | Endpoint sin validación de ruta ni control de acceso |
| 2 | Servicio web como **root** | El proceso Flask corre con privilegios de root |

Corregir **cualquiera** de los dos evita el compromiso total.

## Impacto

- Lectura de cualquier fichero como **root**, incluidas `user.txt`, `root.txt`, `/etc/shadow` y secretos de Gitea.

## Prevención

- Validar/normalizar rutas (allow-list de directorios) y **requerir autenticación**.
- Nunca ejecutar servicios web como root: usuario dedicado sin privilegios.
- No exponer repositorios con código sensible en Gitea público.

## Detección

- Peticiones a `/api/load-file` con rutas absolutas o `../`.
- Lectura de `/etc/shadow`, `/root/*` por el usuario del servicio.

## Hardening recomendado

- [ ] Sanitizar `file` y restringir a un directorio base.
- [ ] Autenticación en endpoints de lectura.
- [ ] Ejecutar Flask con usuario de servicio sin privilegios.
