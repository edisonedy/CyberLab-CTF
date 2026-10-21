---
tags: [ctf, thehackerslabs, linux, pendientes]
tipo: pendientes
estado: resuelto-con-cabos-sueltos
ip: 192.168.56.102
---

# BabyCorp — Pendientes / Cabos sueltos

> [!note] Máquina resuelta
> BabyCorp está **resuelta** por la vía del LFI + servicio como root. Este documento recoge lo **no explorado**, sin presentarlo como necesario.

## Último punto confirmado

- LFI en `POST /api/load-file` (probado con `/etc/passwd`).
- Servicio Flask con `Uid: 0` ⇒ lectura arbitraria como root.
- User y root flags obtenidas.

## Cabos sueltos (no necesarios para resolver)

| # | Elemento | Estado | Comentario |
|---|---|---|---|
| 1 | Puerto `9000` ("jump") | No explorado | Probable reto de **pwn** alternativo para escalar sin el fallo de root |
| 2 | MariaDB (`3306`) | No explotado | Podría contener credenciales (se leyeron secretos de Gitea por LFI) |
| 3 | SSH (`22`) | No usado | No fue necesario |

## Próximo punto de reanudación (opcional, para práctica)

- Analizar el binario/servicio del puerto `9000` como ejercicio de **pwn** independiente.
- Ver si hay una ruta de escalada **sin** el fallo de root (más realista).

## Hipótesis descartadas (documentadas)

| Hipótesis | Por qué se descartó |
|---|---|
| Necesidad de explotar `9000` | No hizo falta: el LFI + root resolvió la máquina |
