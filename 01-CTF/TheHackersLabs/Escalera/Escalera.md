---
tags: [ctf, thehackerslabs, linux, escalada, suid, path-hijack, command-injection, flask, pyyaml, sudo, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Ubuntu 26.04.1 LTS
dificultad: facil-media (cadena de escalada por niveles)
estado: resuelto
ip: 192.168.56.111
fecha: 2026-11-03
user: true
root: true
tecnicas: [nmap, gobuster, credenciales-en-web, ssh, suid, path-hijack, command-injection, waf-bypass, flask, pyyaml, yaml-loader, sudo-nopasswd]
ultima_revision: 2026-11-03
---

# Escalera — The Hackers Labs

## Estado
✅ Resuelto (2026-11-03) — **user** y **root** obtenidas por la cadena intencionada.

> [!tip] Writeup completo
> Ver [[Escalera - Roadmap]] · Evidencias: [[Escalera - Evidencias]]

## Escenario
Máquina Linux planteada como una **escalera** de usuarios: `facil` → `medio` →
`dificil` → `root`. Cada nivel introduce una técnica distinta (credenciales
expuestas, SUID con `PATH` secuestrable, inyección de comandos en un servicio
interno y deserialización insegura de YAML vía `sudo`).

## Sistema operativo
Ubuntu 26.04.1 LTS — hostname `TheHackersLabs-Escalera`.
OpenSSH 10.2p1, nginx 1.28.3, Flask en `127.0.0.1:8080`.

## Entorno
- Hipervisor: VirtualBox (Host-Only `vboxnet0`)
- Atacante: `192.168.56.1`
- Objetivo: **192.168.56.111** (MAC `08:00:27:8C:06:26`)
- VM: `The Hackers Labs - Escalera`

## Flags
- **USER** (`/home/medio/user.txt`): `THL{M3di0St3p892kLq1wZ4vX}`
- **ROOT** (`/root/root.txt`): `THL{R00tC1m4Esc4l3r477kNmP}`

## Reconocimiento
Puertos TCP: **22** (OpenSSH 10.2p1) y **80** (nginx 1.28.3, raíz `403`).
`gobuster` con `directory-list-2.3-medium` descubre `/pass.txt`.

## Credenciales y secretos
| Servicio | Usuario | Clave / token | Origen |
|---|---|---|---|
| SSH | `facil` | `Escal0nFacil2026` | `http://192.168.56.111/pass.txt` |
| SSH | `dificil` | `D1f1c1lStairs89` | `/home/dificil/.credenciales` (vía RCE) |
| Flask interno | — | `M3di0T0k3nS3ss10n9841X` | `/home/medio/.token` |

## Camino (intencionado)
1. **Reconocimiento** → 22/80. La raíz web da 403; `gobuster` encuentra `/pass.txt` con `facil:Escal0nFacil2026`.
2. **Acceso inicial** → SSH como `facil`.
3. **`facil` → `medio`** → `/usr/local/bin/escalon_medio` es SUID de `medio` y ejecuta `system("uptime")` sin sanear el `PATH`. Se secuestra `uptime` → comandos como `medio` → **user flag** en `/home/medio/user.txt` y token del servicio interno.
4. **`medio` → `dificil`** → Flask en `127.0.0.1:8080` (solo accesible por UID `medio`/`dificil` según iptables) con token en cabecera. Inyección en `host` (`127.0.0.1 | sh ...`); el WAF solo bloquea `;`, `&` y `nc` → RCE como `dificil` → `.credenciales`.
5. **`dificil` → `root`** → `sudo NOPASSWD: /usr/bin/python3 /opt/admin/gestion_sistema.py *`, que hace `yaml.load(..., Loader=yaml.Loader)` sobre un fichero controlado → RCE root → **root flag**.

## Lecciones aprendidas
- Credenciales en texto plano dentro del webroot (`/pass.txt`).
- Un SUID que llama a `system()` con nombre relativo de comando es un `PATH` hijack inmediato.
- WAF por lista negra (`;`, `&`, `nc`) insuficiente: el pipe `|` sigue permitiendo inyección.
- `yaml.load` con `yaml.Loader` permite instanciar objetos Python → RCE; usar `yaml.safe_load`.
- El firewall por UID no sustituye el mínimo privilegio: ejecutar como `medio` da acceso al puerto interno.

Detalle paso a paso y comandos: [[Escalera - Roadmap]]

## Notas relacionadas
- [[Escalera - Roadmap]]
- [[Escalera - Evidencias]]
- [[Achinech]]
