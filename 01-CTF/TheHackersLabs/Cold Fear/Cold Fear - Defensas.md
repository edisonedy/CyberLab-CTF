---
tags: [ctf, thehackerslabs, linux, defensas]
tipo: defensas
---

# Cold Fear — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | FTP anónimo | Configuración que permite `anonymous` sin necesidad |
| 2 | Información en metadatos | Secretos/pistas en EXIF de imágenes publicadas |
| 3 | Secretos en audio/logs | Datos ocultos en WAV y rutas internas expuestas |
| 4 | `sudo` con pager/`-exec` | Concesiones de `sudo` excesivamente amplias |

## Impacto

- Descubrimiento de usuarios y rutas internas.
- Ejecución de comandos como otros usuarios y, finalmente, **root**.

## Prevención

- Deshabilitar FTP anónimo o restringirlo a un directorio de solo lectura sin datos sensibles.
- Sanear metadatos EXIF antes de publicar imágenes (`exiftool -all=`).
- No publicar audio/logs con información interna.
- Revisar `sudoers`: evitar conceder herramientas que permitan shell (`less`, `find`, `aws`, `vim`…).

## Detección

- Sesiones FTP anónimas.
- Lectura inusual de ficheros de audio/imágenes por procesos de análisis.
- Uso de `sudo` sobre binarios no habituales.

## Hardening recomendado

- [ ] `anonymous_enable=NO` en el servidor FTP.
- [ ] Pipeline de saneado de metadatos.
- [ ] `visudo`: mínimo privilegio, sin pagers ni `-exec`.
- [ ] Alertas ante `sudo` de herramientas con capacidades de ejecución.
