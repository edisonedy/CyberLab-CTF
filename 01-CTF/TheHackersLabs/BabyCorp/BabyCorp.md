---
tags: [ctf, thehackerslabs, linux, gitea, lfi, flask, root-service, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
dificultad: baja-media
estado: resuelto
ip: 192.168.56.102
fecha: 2026-10-19
user: true
root: true
tecnicas: [nmap, gitea, fuga-codigo, lfi, path-traversal, flask, root-service, base64]
fuente: BabyCorp - Writeup.md
ultima_revision: 2026-10-20
---

# BabyCorp — Ficha

> [!info] Resumen
> Repo **público** en Gitea con el código de la web; endpoint `POST /api/load-file` vulnerable a **LFI/Path Traversal**; el servicio Flask corre como **root** → lectura arbitraria con privilegios de root (flags directas).

## Identificación

| Campo | Valor |
|---|---|
| Nombre | BabyCorp |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux (Debian 12) |
| Dificultad | baja-media |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-19 |
| IP | `192.168.56.102` |
| MAC | `08:00:27:BA:3C:7E` |
| Hipervisor | VirtualBox (Host-Only) |
| Fuente | [[BabyCorp - Writeup]] |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 22 | OpenSSH 9.2p1 Debian | — |
| 3000 | Gitea "BabyCorp" | cookie `i_like_gitea` |
| 3306 | MariaDB | — |
| 5000 | Flask / Werkzeug 3.1.8 | web BabyCorp (**corre como root**) |
| 9000 | Servicio custom | `This is almost exactly the same as jump...` (no necesario) |

## Resultado

- **User:** ✅ `THL{wvv3r3z_th3_g04t}`
- **Root:** ✅ `THL{djabndkasbnFfafhafaghdh18bdajkdbasdbasahsdbghajsbdahsdjasd}`

## Técnicas utilizadas

- [[Nmap]] para reconocimiento.
- [[Gitea]] — enumeración de repos públicos y **fuga de código fuente**.
- [[LFI]] / [[Path-Traversal]] en `POST /api/load-file`.
- Servicio **ejecutándose como root** → escalada "gratis".
- Decodificación [[Base64]] de la respuesta.

## Notas relacionadas

- [[BabyCorp - Writeup]] (nota original preservada)
- [[BabyCorp - Roadmap]]
- [[BabyCorp - Walkthrough]]
- [[BabyCorp - Evidencias]]
- [[BabyCorp - Lecciones]]
- [[BabyCorp - Defensas]]
- [[BabyCorp - Pendientes]] (ruta 9000 / pwn alternativa)
