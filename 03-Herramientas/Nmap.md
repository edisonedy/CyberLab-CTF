---
tags: [herramienta, reconocimiento, redes]
tipo: herramienta
categoria: reconocimiento
ultima_revision: 2026-10-20
---

# Nmap

Escáner de red y puertos. Usado en **todas** las máquinas.

## Uso típico

```bash
nmap -sn 192.168.56.0/24
nmap -p- --open -sS --min-rate 5000 -n -Pn <IP> -oG allPorts
nmap -sCV -p<puertos> <IP> -oN services
nmap -6 -p- --open -sS <IPv6>
```

## Notas / lecciones

- Escanear **IPv4 e IPv6** (SuraxCorp: Rsync solo IPv6).
- No fiarse del top-1000 ni de resultados "por defecto".

## Técnicas relacionadas

- [[Nmap]], [[Virtual-Hosts]], [[SMB-Enum]]
