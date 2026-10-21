---
tags: [tecnica, criptografia, esteganografia, exif]
tipo: tecnica
categoria: criptografia-esteganografia
estado: practicado
maquinas: [Cold Fear, Folclore]
---

# Metadatos EXIF

## 1. Qué es

Información incrustada en imágenes (cámara, GPS, comentarios, copyright) que puede contener pistas o secretos.

## 2. Cómo funciona

- Los ficheros JPEG/TIFF guardan campos EXIF; algunos editores dejan texto libre.

## 3. Cuándo sospecharla

- Imágenes publicadas en web/FTP/shares (fotos «de cámara»).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
exiftool imagen.jpg
exiftool -a -u -g1 imagen.jpg
# comparar con versión limpia
```

## 5. Herramientas

- `exiftool`, `strings`, `binwalk`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | EXIF de `bridge-cam.jpg` (+ comparación) | pista `/var/log/bridge/`, Boris |
| [[Folclore]] | EXIF de `El_Charro_Negro.jpeg` | credencial `Ix_Chel` |

## 7. Falsos positivos / pruebas fallidas

- Ver la **imagen** no basta: hay que leer sus **metadatos**.

## 8. Prevención y mitigación

- Sanear metadatos antes de publicar (`exiftool -all=`).

## 9. Técnicas relacionadas

- [[Esteganografia-LSB]], [[Esteganografia-WAV-LSB]]
