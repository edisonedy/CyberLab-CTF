---
tags: [tecnica, reconocimiento, nmap, arp-scan]
tipo: tecnica
categoria: reconocimiento
estado: practicado
maquinas: [todas]
---

# Nmap y descubrimiento de hosts

## 1. Qué es

Escaneo de red y puertos para descubrir hosts, servicios y versiones.

## 2. Cómo funciona

- Descubrimiento (`-sn`), escaneo de puertos (`-sS`), detección de servicio (`-sV`), scripts (`-sC`).

## 3. Cuándo sospecharla

- Siempre, como primer paso en un laboratorio autorizado.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# descubrimiento
arp-scan -I eth0 -g 192.168.56.0/24
nmap -sn 192.168.56.0/24
# escaneo completo
nmap -p- --open -sS --min-rate 5000 -n -Pn <IP> -oG allPorts
nmap -sCV -p<puertos> <IP> -oN services
# IPv6
nmap -6 -p- --open -sS <IPv6>
```

## 5. Herramientas

- [[Nmap]], `arp-scan`, [[NetExec]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | Descubrimiento + puertos 22/80/1883/6379 | servicios de infraestructura |
| [[SuraxCorp]] | Escaneo **IPv6** | Rsync por IPv6 descubierto |
| [[Folclore]] | SMB/RID cycling | 6 usuarios |

## 7. Falsos positivos / pruebas fallidas

- Escanear solo IPv4 deja oculto Rsync por IPv6 (SuraxCorp).
- Confiar en el top-1000 omite servicios en puertos altos.

## 8. Prevención y mitigación

- (Desde defensa) inventario de servicios, firewall, monitorización de escaneos.

## 9. Técnicas relacionadas

- [[Gobuster]], [[NetExec]], [[RID-Cycling]]
