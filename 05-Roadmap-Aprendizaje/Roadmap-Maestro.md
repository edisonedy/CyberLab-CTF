---
tags: [roadmap, aprendizaje, indice]
tipo: roadmap-aprendizaje
estado: activo
ultima_revision: 2026-10-20
---

# Roadmap de Aprendizaje — Índice

> [!info] Cómo usar este roadmap
> Refleja lo **practicado** en mis máquinas, no teoría. Cada nivel enlaza a técnicas reales y a las máquinas donde se aplicaron.

## Nivel 0 — Fundamentos

- [x] [[Nmap]] y descubrimiento de hosts.
- [x] [[Linux-Enum]] básica (id, sudo -l, find, getcap).
- [x] [[Virtual-Hosts]] y fuzzing web.
- [ ] Redes: modelo OSI aplicado, enrutado, IPv6.

## Nivel 1 — Web básico

- [x] [[Path-Traversal]] / [[LFI]].
- [x] [[SQL-Injection]] (boolean/time-based).
- [x] [[Command-Injection]].
- [x] [[SSTI-Jinja2]].
- [x] [[File-Upload]].
- [x] [[PHP-Filters]].
- [x] [[SSRF]].
- [ ] XSS / CSRF (ofensivo y defensivo).
- [ ] Deserialización insegura.

## Nivel 2 — Servicios e infraestructura

- [x] [[FTP-Anonimo]].
- [x] [[Redis-Sin-Autenticacion]].
- [x] [[MQTT]].
- [x] [[Rsync]] (IPv6).
- [x] [[Gitea]] y fuga de código.
- [ ] SMTP/IMAP, NFS, Docker/K8s.

## Nivel 3 — Escalada Linux

- [x] [[SUID]] + [[GTFOBins]].
- [x] [[Cron-Privilege-Escalation]].
- [x] [[Path-Hijacking]].
- [x] [[APT-Pre-Invoke]].
- [x] [[Grupos-Linux]].
- [ ] Capabilities (en profundidad).
- [ ] Contenedores / escape.

## Nivel 4 — Windows / Active Directory

- [x] [[SMB-Enum]] / RID cycling.
- [x] [[Password-Spraying]].
- [x] [[Cracking]] (NetNTLMv2, ZIP).
- [x] [[Responder-NTLM-Theft]].
- [x] [[AD-Abuso-de-ACLs]].
- [x] [[AD-Tombstones]].
- [x] [[ADCS-ESC3]].
- [x] [[PKINIT-Pass-The-Hash]].
- [ ] Kerberoasting / AS-REP Roasting.
- [ ] Delegaciones (unconstrained/constrained/RBCD).
- [ ] Otros ESC (ESC1, ESC4, ESC8...).

## Nivel 5 — Reversing y binarios

- [x] [[Reversing-Token-Validation]] + [[XOR]].
- [x] [[Buffer-Overflow]] (stack x86, Windows).
- [ ] Heap overflow.
- [ ] Format strings.
- [ ] ROP / mitigaciones modernas (ASLR/DEP/CFG).

## Nivel 6 — Forense y offline

- [x] [[Sleuthkit]] (NTFS/ext4).
- [x] [[BitLocker-BEK]] + dislocker.
- [x] [[Exif-Metadata]], [[Esteganografia-LSB]], [[Esteganografia-WAV-LSB]].
- [ ] Memoria (Volatility).
- [ ] Timeline y artefactos Windows.

## Nivel 7 — Pivoting y redes

- [x] [[Pivoting-Ligolo]].
- [x] [[WireGuard]] (enrutado).
- [ ] Doble pivote, port forwarding, SOCKS.

## Habilidades transversales

- [x] Documentación estructurada (6 notas/máquina).
- [x] Distinguir **evidencia** de **inferencia**.
- [x] Diagnóstico de entorno ([[Playbook-Laboratorio-VirtualBox]]).
- [ ] Redacción de reportes ejecutivos.
- [ ] Automatización de reconocimiento.

## Próximos objetivos (priorizados)

1. Profundizar **AD CS** (ESC1/ESC4/ESC8) y delegaciones.
2. **Reversing** moderno (ROP, mitigaciones).
3. **Forense de memoria** (Volatility).
4. Pulir **CSRF/XSS** y deserialización.

## Enlaces

- [[Metodo-de-Resolucion]]
- [[Resueltas]] · [[En-Progreso]]
- [[Lecciones-Globales]]
