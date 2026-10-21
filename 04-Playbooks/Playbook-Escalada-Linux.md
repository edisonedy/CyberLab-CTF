---
tags: [playbook, linux, escalada]
tipo: playbook
estado: activo
ultima_revision: 2026-10-20
---

# Playbook — Escalada en Linux

## 1. Enumeración

```bash
id; groups; uname -a; hostname
sudo -l
find / -perm -4000 -type f 2>/dev/null        # SUID
find / -perm -2000 -type f 2>/dev/null        # SGID
getcap -r / 2>/dev/null                        # capabilities
crontab -l; cat /etc/crontab; ls -la /etc/cron.*; systemctl list-timers --all
ps aux; ls -laR /home 2>/dev/null
grep -riE 'pass|pwd|secret' ~ 2>/dev/null
```

## 2. Vectores más comunes

| Vector | Técnica |
|---|---|
| SUID/SGID | [[SUID]] + [[GTFOBins]] |
| sudo con binarios | [[GTFOBins]], [[Sudo-Vim]], [[Sudo-Telnet]], [[Sudo-Find-Exec]], [[Sudo-Nginx]], [[Sudo-Ar]] |
| Cron/timers | [[Cron-Privilege-Escalation]] |
| PATH | [[Path-Hijacking]] |
| Hooks APT | [[APT-Pre-Invoke]] |
| Grupos | [[Grupos-Linux]] |

## 3. Regla de oro

- Cambiar **una** variable por prueba y verificar `id`/`EUID`.
- Un binario SUID que no ejecuta comandos **no** sirve solo: analizarlo ([[Reversing-Token-Validation]]).
