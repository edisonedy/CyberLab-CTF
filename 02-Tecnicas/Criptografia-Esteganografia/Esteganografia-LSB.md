---
tags: [tecnica, criptografia, esteganografia, lsb, stegseek]
tipo: tecnica
categoria: criptografia-esteganografia
estado: practicado
maquinas: [Folclore, Cold Fear]
---

# Esteganografía LSB (imagen) y stegseek

## 1. Qué es

Ocultación de información en los bits menos significativos (LSB) de píxeles de una imagen, a veces con **passphrase**.

## 2. Cómo funciona

- Cada byte de píxel aporta bits LSB que, concatenados, forman el mensaje.
- `stegseek` prueba passphrases de una wordlist contra `steghide`.

## 3. Cuándo sospecharla

- Imágenes «normales» en shares/web que no dan más pistas.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
stegseek imagen.jpg /usr/share/wordlists/rockyou.txt
steghide extract -sf imagen.jpg        # si conoces la passphrase
zsteg imagen.png
```

## 5. Herramientas

- `stegseek`, `steghide`, `zsteg`, `binwalk`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Folclore]] | `stegseek Tlaloc.jpg rockyou.txt` (passphrase `knight`) | `45yD#k7` |

## 7. Falsos positivos / pruebas fallidas

- En Cold Fear la esteganografía de **imagen** no dio resultado: el vector era el **audio** (ver [[Esteganografia-WAV-LSB]]).

## 8. Prevención y mitigación

- Recomprimir imágenes y sanear metadatos antes de publicarlas.

## 9. Técnicas relacionadas

- [[Esteganografia-WAV-LSB]], [[Exif-Metadata]]
