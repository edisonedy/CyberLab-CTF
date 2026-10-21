---
tags: [tecnica, escalada-linux, sudo, find]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [Cold Fear]
---

# Sudo Find -exec

## 1. Qué es

`find` ejecutado con `sudo` puede lanzar comandos arbitrarios vía `-exec`, otorgando root.

## 2. Cómo funciona

- `sudo find <path> -exec <cmd> \;` ejecuta `<cmd>` como el usuario del `sudo` (root).

## 3. Cuándo sospecharla

- `sudo -l` incluye `find` (con o sin argumentos).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo -l
sudo find . -exec /bin/sh \; -quit
id
```

## 5. Herramientas

- [[GTFOBins]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | `passenger0` con `sudo find -exec` | root |

## 7. Falsos positivos / pruebas fallidas

- Algunas configuraciones restringen `find` a rutas concretas; comprobar el `sudoers`.

## 8. Prevención y mitigación

- No conceder `find` con `sudo`; si es necesario, restringir a rutas y sin `-exec`.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[SUID]]
