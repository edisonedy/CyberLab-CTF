---
tags: [cheatsheet, reconocimiento]
tipo: cheatsheet
ultima_revision: 2026-10-20
---

# Cheatsheet — Reconocimiento

```bash
# Descubrimiento
arp-scan -I eth0 -g 192.168.56.0/24
nmap -sn 192.168.56.0/24

# Puertos y servicios
nmap -p- --open -sS --min-rate 5000 -n -Pn <IP> -oG allPorts
nmap -sCV -p<puertos> <IP> -oN services

# IPv6 (no olvidar)
nmap -6 -p- --open -sS <IPv6>

# Web
gobuster dir -u http://vhost.thl/ -w /usr/share/wordlists/dirb/common.txt
ffuf -u http://<IP>/ -H "Host: FUZZ.thl" -w subdomains.txt
curl -s http://<IP>/robots.txt

# SMB
netexec smb <IP> -u guest -p '' --rid-brute --shares

# FTP anónimo
curl -s ftp://anonymous:anonymous@<IP>/

# Rsync
rsync -av --list-only rsync://[<IPv6>]:873/
```

## Técnicas

- [[Nmap]], [[Virtual-Hosts]], [[SMB-Enum]], [[FTP-Anonimo]], [[Rsync]]
