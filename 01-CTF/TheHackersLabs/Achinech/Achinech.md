---
tags: [ctf, thehackerslabs, linux, smb, limesurvey, rce, plugin-upload, credential-reuse, sudo, dlopen, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Debian 12 (Bookworm)
dificultad: media (web + reuso de creds + escalada por sudo/dlopen)
estado: resuelto
ip: 192.168.56.108
fecha: 2026-10-24
user: true
root: true
tecnicas: [nmap, smb-anonimo, limesurvey, plugin-upload-rce, mariadb, credential-reuse, ssh, sudo, env_keep, dlopen, so-malicioso]
ultima_revision: 2026-10-24
---

# Achinech — The Hackers Labs

## Estado
✅ Resuelto (2026-10-24) — **user** y **root** obtenidas por la cadena intencionada.

> [!tip] Writeup completo
> Ver [[Achinech - Roadmap]]

## Escenario
Servidor de la asesoría canaria ficticia **Achinech Gestión S.L.**, que ha desplegado una plataforma de encuestas **LimeSurvey** implantada por una consultora externa (*Guanche Sistemas S.L.*). Una compartición SMB anónima filtra documentación interna que orienta hacia las credenciales del panel; desde ahí, una vulnerabilidad conocida de LimeSurvey permite ejecución de código, y una cadena de **reutilización de credenciales** + **abuso de configuración de sudo** conduce a root.

## Sistema operativo
Debian 12 (Bookworm) — hostname `ACHINECH`. OpenSSH 9.2p1, Apache 2.4.68, Samba 4, MariaDB 10.3.

## Entorno
- Hipervisor: VirtualBox (Host-Only `vboxnet0`)
- Kali: 192.168.56.1
- Objetivo: **192.168.56.108** (hostname `ACHINECH`)
- MAC: 08:00:27:81:9F:03

## Flags
- **USER** (`/home/acaimo/user.txt`): `THL{4c41m0_3l_m3nc3y_gu4nch3}`
- **ROOT** (`/root/root.txt`): `THL{m3nc3y_supr3m0_d3l_t31d3}`

## Reconocimiento
Linux. Puertos: **22** (SSH), **80** (LimeSurvey / Apache), **139/445** (Samba 4), **3306** (MariaDB). Null-auth SMB permitido.

## Credenciales recogidas
| Servicio | Usuario | Clave | Origen |
|---|---|---|---|
| Panel LimeSurvey | `Bencomo` | `G3st10n_Ach1n3ch#25` | `despliegue_limesurvey.php.bak` |
| MariaDB | `lsuser` | `Ls_4ch1n3ch_2025` | `despliegue_limesurvey.php.bak` |
| SSH | `acaimo` | `Tinerfe_2025_ERP` | tabla `credenciales_backup` (BD) |

## Camino (intencionado)
1. **SMB anónimo** → share `documentos` → `acta_implantacion.pdf`: nombra al admin `Bencomo` y un fichero de despliegue olvidado en el webroot.
2. **Fuga de credenciales** → `http://IP/despliegue_limesurvey.php.bak` contiene la contraseña del panel y las de la BD.
3. **RCE en LimeSurvey 6.1** → login autenticado + **subida de plugin** malicioso (ZIP con `config.xml` + webshell PHP) → shell como **www-data** en `/upload/plugins/AchiShell/sh.php`.
4. **Reuso de credenciales** → MariaDB `achinech` tiene la tabla `credenciales_backup` con la clave SSH de `acaimo` → **user flag**.
5. **Escalada a root** → `sudo -l`: `(root) NOPASSWD: /opt/achinech/bin/registro` con `env_keep+=ACHINECH_PLUGIN`. El binario hace `dlopen(getenv("ACHINECH_PLUGIN"))` + `dlsym("registrar_evento")` → `.so` malicioso que exporta `registrar_evento` ejecutado como root → **root flag**.

## Lecciones aprendidas
- SMB con **null session** puede filtrar documentación corporativa que encadena todo el ataque.
- Ficheros `.bak`/`.old` de despliegue en el webroot = credenciales en texto plano a un `curl` de distancia.
- LimeSurvey: un admin autenticado tiene **RCE** por la subida de plugins (ZIP con PHP se extrae bajo `upload/plugins/`, servido por Apache).
- **No reutilizar credenciales** entre servicios; y nunca guardar claves en una tabla "backup" legible por la cuenta de la app.
- `sudo` + `env_keep` de una variable que controla un `dlopen` = escalada trivial. Nunca preservar variables que alteren la carga de librerías (`LD_*`, rutas de plugins).

Detalle paso a paso y comandos: [[Achinech - Roadmap]]

## Notas relacionadas
- [[Achinech - Roadmap]]
- [[Patata Magica]]
- [[Welcome To The Jungle]]
