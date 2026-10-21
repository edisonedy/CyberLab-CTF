---
tags: [dashboard, indice, maquinas]
tipo: indice
---

# 🗂️ Máquinas — Índice por categoría

## Por sistema operativo

### Windows / AD
- [[Phantom]] — AD, ADCS ESC3, NTLM theft
- [[Elevator]] — AD, BitLocker
- [[Chimichurri]] — DC, credenciales en disco
- [[Pila]] — Buffer Overflow
- [[Folclore]] — Cadena SMB
- [[Welcome To The Jungle]] — IIS/PHP, webshell + GodPotato a SYSTEM
- [[Patata Magica]] — XAMPP/LFI, SeImpersonate + GodPotato (resuelta por forense offline)

### Linux
- [[El Baifo]] — Redis + SUID
- [[Cold Fear]] — Esteganografía audio + GTFOBins
- [[SuraxCorp]] — SSTI + IPv6
- [[Securitron]] — SQLi + cron
- [[NovaVault]] — SSRF + reversing SUID
- [[Company1]] 🟡 — escalada pendiente

### Web
- [[BabyCorp]] — LFI como root
- [[AllSafe]] 🟡 — LaTeX/LFI sin confirmar
- [[Welcome To The Jungle]] — webshell PHP en IIS + escalada SYSTEM

### Multi-host
- [[THLCPPTV16]] — pivoting Ligolo/WireGuard

## Por técnica principal

| Técnica | Máquinas |
|---|---|
| [[SQL-Injection]] | [[Securitron]], [[THLCPPTV16]] |
| [[SSTI-Jinja2]] | [[SuraxCorp]] |
| [[Path-Traversal]] | [[BabyCorp]], [[Securitron]], [[AllSafe]]🟡 |
| [[SSRF]] | [[NovaVault]] |
| [[Command-Injection]] | [[SuraxCorp]], [[NovaVault]] |
| [[ADCS-ESC3]] | [[Phantom]] |
| [[AD-Abuso-de-ACLs]] | [[Phantom]], [[Elevator]] |
| [[BitLocker-BEK]] | [[Elevator]] |
| [[Buffer-Overflow]] | [[Pila]] |
| [[Responder-NTLM-Theft]] | [[Phantom]] |
| [[Esteganografia-WAV-LSB]] | [[Cold Fear]], [[Welcome To The Jungle]] |
| [[Redis-Sin-Autenticacion]] | [[El Baifo]] |
| [[GodPotato-SeImpersonate]] | [[Welcome To The Jungle]], [[Patata Magica]] |
| [[LFI]] / File Upload | [[Patata Magica]] |
| [[PHP-Webshell]] | [[Welcome To The Jungle]] |

## Referencias

- [[HappyJump - Referencia]]
