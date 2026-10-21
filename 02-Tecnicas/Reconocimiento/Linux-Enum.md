---
tags: [tecnica, reconocimiento, linux, enumeracion]
tipo: tecnica
categoria: reconocimiento
estado: practicado
maquinas: [Company1, El Baifo, Securitron]
---

# Enumeración Linux post-acceso

## 1. Qué es

Comprobaciones sistemáticas tras obtener acceso a un Linux, para localizar vectores de escalada.

## 2. Cómo funciona

- Revisar identidad/grupos, `sudo`, SUID/SGID, capabilities, cron/timers, servicios, permisos y ficheros con secretos.

## 3. Cuándo sospecharla

- Inmediatamente después de conseguir un acceso (shell/SSH).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
id; groups; umask; hostname; uname -a
sudo -l
find / -perm -4000 -type f 2>/dev/null            # SUID
find / -perm -2000 -type f 2>/dev/null            # SGID
getcap -r / 2>/dev/null                           # capabilities
crontab -l; cat /etc/crontab; ls -la /etc/cron.*; systemctl list-timers --all
ps aux --sort=-%cpu | head
ls -laR /home 2>/dev/null
grep -riE 'pass|pwd|secret' ~ 2>/dev/null
```

## 5. Herramientas

- [[LinPEAS]] (referencia), `find`, `getcap`, `pspy`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Company1]] | Enumeración con `marco.silva` | `su` falló; pendiente |
| [[El Baifo]] | SUID/cron | root |
| [[Securitron]] | `sudo -l`, `ar` | root |

## 7. Falsos positivos / pruebas fallidas

- Intentar `su` sin credenciales (Company1) pierde tiempo: priorizar vectores sin credenciales.

## 8. Prevención y mitigación

- (Defensa) mínimo privilegio, auditoría de SUID/cron/capabilities.

## 9. Técnicas relacionadas

- [[SUID]], [[Cron-Privilege-Escalation]], [[GTFOBins]]
