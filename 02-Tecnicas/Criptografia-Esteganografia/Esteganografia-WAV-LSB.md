---
tags: [tecnica, criptografia, esteganografia, audio, wav, lsb]
tipo: tecnica
categoria: criptografia-esteganografia
estado: por-reforzar
maquinas: [Cold Fear, Welcome To The Jungle]
---

# Esteganografía en WAV (LSB de audio PCM)

## 1. Qué es

Ocultación de datos en los bits menos significativos de las muestras **PCM** de un WAV.

## 2. Cómo funciona

- Cada muestra (8/16 bits) puede aportar 1–2 bits LSB.
- Concatenando esos bits y reconstruyendo bytes se obtiene el mensaje.

## 3. Cuándo sospecharla

- Ficheros de **audio** en web/FTP sin otra función aparente.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
python3 - <<'PY'
import wave
w = wave.open('archivo.wav','rb')
data = w.readframes(w.getnframes())
bits = ''.join(str(b & 0b11) for b in data)   # 2 bits LSB por muestra
# reconstruir bytes y buscar el mensaje
PY
```

## 5. Herramientas

- Python (`wave`), `sox`, `binwalk`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | Reconstrucción de **2 bits LSB** del PCM | mensaje con `Victor` |
| [[Welcome To The Jungle]] | `stegseek` sobre `solo_final.wav` (extraído de `songs.zip`) | `Password:sweetjungle2025` + `URL:theh3xgun5` |

## 7. Falsos positivos / pruebas fallidas

- El vector correcto **no** siempre es la imagen: en Cold Fear fue el **audio** ([[Esteganografia-LSB]] falló en la imagen).
- **No** todo WAV tiene esteganografía: en [[Welcome To The Jungle]] el fichero era una pista musical legítima y el mensaje estaba en la capa de datos que `stegseek` extrae (no en los LSB del PCM). El `stegseek` sin passphrase puede dar falsos positivos; conviene deducir la clave del contexto (en esa máquina, el nombre de la banda `thehexguns`).

## 8. Prevención y mitigación

- Recomprimir/reencodear audio sensible antes de publicarlo.

## 9. Técnicas relacionadas

- [[Esteganografia-LSB]], [[Exif-Metadata]]
