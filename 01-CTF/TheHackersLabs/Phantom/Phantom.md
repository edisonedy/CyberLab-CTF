---
tags: [ctf, thehackerslabs, windows, active-directory, adcs, esc3, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows Server 2022 (DC — Active Directory)
dificultad: media-alta (AD)
estado: resuelto
ip: 192.168.56.100
fecha: 2026-10-16
user: true
root: true
tecnicas: [nmap, netexec, bloodhound, acl-abuse, responder, ntlm-theft, john, ad-tombstones, password-reuse, adcs-esc3, pkinit, pass-the-hash]
fuente: Phantom.md
ultima_revision: 2026-10-20
---

# Phantom — The Hackers Labs

## Estado
✅ Resuelto (2026-10-16)

> [!tip] Writeup completo
> Ver [[Phantom - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
Windows Server 2022 (DC — Active Directory)

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only
- Interfaz: vboxnet0
- Kali: 192.168.56.1
- DC Phantom: 192.168.56.100
- Dominio: PHANTOM.THL (DC01)
- MAC Phantom: 08:00:27:E2:1F:BF

## Problema detectado (resuelto)
La máquina recibía la IP APIPA 169.254.124.106 por un conflicto de IP:
el DHCP de VirtualBox usaba la misma .100 que la IP fija de la VM.
Solución: mover el DHCP de VirtualBox a 192.168.56.2.

## Flags
- USER: `40c943b496ce2ad56cc7f6f2e86abe00`
- ADMIN: `568e7df15c65b751e1a096a131713925`

## Reconocimiento
DC de AD. Puertos: 53,88,135,139,389,445,464,593,636,3268/3269,5985,9389.
Null Auth permitido; enumeración real con credenciales de `mark`.

## Explotación (user)
mark →(GenericAll)→ bob →(HELPDESK ForceChangePassword)→ frank (WinRM)
→ LNK en share "Dev Tools" + Responder → hash de robert → crack (bLink182) → USER.

## Escalada de privilegios (admin)
Tombstone `dev_test` (password en description) → reutilización → tomas
→ ADCS **ESC3** (Phantom-DevAuth) → cert de Administrador → PKINIT → NT hash → PtH → ADMIN.

## Lecciones aprendidas
- Abuso de ACLs en AD (GenericAll/WriteDACL/ForceChangePassword).
- NTLM theft con LNK en shares escribibles + Responder.
- Objetos eliminados (tombstones) y credenciales en `description`.
- Reutilización de contraseñas / password spraying.
- AD CS ESC3 (Enrollment Agent on-behalf-of) + PKINIT + Pass-the-Hash.

Detalle paso a paso y comandos: [[Phantom - Roadmap]]

## Notas relacionadas

- [[Phantom - Roadmap]]
- [[Phantom - Walkthrough]]
- [[Phantom - Evidencias]]
- [[Phantom - Lecciones]]
- [[Phantom - Defensas]]
