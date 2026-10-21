---
tags: [ctf, thehackerslabs, linux, lecciones]
tipo: lecciones
---

# Cold Fear — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Intentar fuerza bruta antes de probar FTP anónimo | Tiempo perdido | Probar accesos anónimos primero |
| Mirar la imagen sin leer sus **metadatos** | Se pierde la pista de `/var/log/bridge/` | `exiftool` + comparación de imágenes |
| Asumir una sola codificación | Mensaje ilegible | Encadenar **Base64 → ROT13** |

## Tiempo perdido y por qué

- Pruebas de decodificación/esteganografía descartadas hasta dar con el **orden correcto** y con el vector correcto (audio PCM).

## Conceptos aprendidos

- [[FTP-Anonimo]].
- [[Exif-Metadata]] y comparación de imágenes.
- [[Esteganografia-WAV-LSB]] (2 bits LSB en PCM).
- [[Base64]] + [[ROT13]] encadenados.
- Reconstrucción de contraseñas por **patrón** (tres sesiones).
- [[GTFOBins]]: escape de pager `less` y `sudo find -exec`.

## Señales para reconocer en otra máquina

- [ ] Puerto 21 con login anónimo permitido.
- [ ] Imágenes con metadatos "de más" (copyright, comentarios).
- [ ] Ficheros de audio presentes en la web/FTP.
- [ ] `sudo -l` con pagers o herramientas que permiten `-exec`/`!sh`.

## Preguntas de autoevaluación

1. ¿En qué orden encadenas Base64 y ROT13 y por qué?
2. ¿Cómo reconstruyes un mensaje oculto en 2 bits LSB de un WAV PCM?
3. ¿Qué tres binarios de [[GTFOBins]] permiten escapar a shell vía `sudo`?

## Técnicas que necesitan más práctica

- [[Esteganografia-WAV-LSB]] (automatizar la extracción).
- [[GTFOBins]] — memorizar escapes de pagers y `-exec`.

## Pruebas descartadas

- Esteganografía en la imagen (LSB de píxeles) que no dio resultado: el vector correcto era el **audio**.
