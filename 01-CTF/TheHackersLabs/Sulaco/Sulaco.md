---
tags: [ctf, thehackerslabs, linux, web, nonce, csrf, sleuthkit, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Linux (Debian, Apache 2.4.65)
dificultad: media
estado: resuelto
ip: 192.168.56.101
fecha: 2026-10-20
user: true
root: true
tecnicas: [vbox-nic-diagnostico, ffuf, robots-txt, panel-oculto, nonce-doble-submit, ua-gating, csrf-md5, sleuthkit]
fuente: Sulaco.md
ultima_revision: 2026-10-20
---

# Sulaco — Ficha (The Hackers Labs)

## Estado
✅ Resuelto (2026-10-20) — USER y ADMIN/root obtenidos

> [!tip] Writeup completo
> Ver [[Sulaco - Roadmap]]

## Identificación

| Campo | Valor |
|---|---|
| Nombre | Sulaco |
| Plataforma | The Hackers Labs · Creador: K0B4KS |
| Sistema operativo | Linux (Debian) — Apache 2.4.65 |
| Dificultad | media |
| Estado | ✅ Resuelto |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.56.101` (DHCP vboxnet0) |
| Hostname | sulaco |
| MAC | `08:00:27:77:A5:F6` |

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only · Interfaz: vboxnet0
- Kali: 192.168.56.1
- Sulaco: **192.168.56.101** (DHCP de vboxnet0)

## Problema detectado (resuelto)
La VM venía con el adaptador en **bridged sobre wlan0**, no en Host-Only,
por eso no aparecía en 192.168.56.x.
Solución: `VBoxManage controlvm "The Hackers Labs - Sulaco" nic1 hostonly vboxnet0`
+ reinicio → la VM cogió 192.168.56.101 por DHCP.

## Flags
- USER: `df193336a3a7d6ab7c4e25944b797b7f` ✅
- ADMIN/ROOT: `24818cc312fbd0e78e1176ef2d304680` ✅

## Reconocimiento
Solo web: 80 (Apache default) y 8080 (app "SULACO — Extraterrestrial Life Division").
robots.txt (8080) → `Disallow: /api/admin_flag.php`.

## Explotación (user) ✅
Fuzzing → `/admin` (panel), `/api/telemetry_ua.php`, `/api/admin_flag.php`, `/ip.php`.
USER flag vía `POST /api/telemetry_ua.php` con:
- User-Agent de navegador (curl bloqueado)
- Cookie `sulaco_nonce=<N>` == Header `X-Sulaco-Nonce: <N>` (doble-submit, N aleatorio `openssl rand -hex 16`)
Respuesta: `{"flag":"...","type":"user_flag"}` + `Set-Cookie: sulaco_user=1`.
El cookie `sulaco_user=1` desbloquea `/lab/notes/` (app de notas con IDs secuenciales).

## Escalada de privilegios (admin) ✅
El admin flag está embebido en `/var/www/sulaco/api/admin_flag.php`, protegido por:
- la puerta `inc_gate.php` (`sulaco_is_unlocked()`), y
- un CSRF: `POST id=<id>` + cabecera `X-Flag-CSRF: md5('sulaco-secret-'.$id)`.

Vía usada (offline, sin explotar la web):
```bash
fls -o 2048 -r -p sulaco.raw | grep -i flag
icat -o 2048 sulaco.raw 1045959      # api/admin_flag.php
```
Resultado: `{"flag":"24818cc312fbd0e78e1176ef2d304680","type":"admin_flag"}`.

## Lecciones aprendidas
- Diagnóstico de red VirtualBox (bridged vs host-only, DHCP).
- Enumeración web (ffuf), robots.txt, panel oculto.
- Autenticación por doble-submit de nonce (cookie == header) + método/UA gating.

Detalle paso a paso y comandos: [[Sulaco - Roadmap]]

## Notas relacionadas

- [[Sulaco - Roadmap]]
- [[Sulaco - Walkthrough]]
- [[Sulaco - Evidencias]]
- [[Sulaco - Lecciones]]
- [[Sulaco - Defensas]]
