---
tags: [ctf, thehackerslabs, linux, walkthrough, en-progreso]
tipo: walkthrough
estado: en-progreso
---

# Company1 — Walkthrough (parcial)

> [!warning] Incompleto
> No hay explotación confirmada. Se documenta el trabajo real sin inventar pasos.

## Paso 1 — Acceso SSH como `marco.silva`

**Comando**
```bash
ssh marco.silva@192.168.150.135
id; whoami; hostname
```

**Interpretación / por qué**
- Acceso inicial documentado. Confirmar identidad y entorno.

## Paso 2 — Enumeración de usuarios y homes

**Comando**
```bash
cat /etc/passwd
ls -la /home
getent passwd | awk -F: '$3>=1000{print $1,$3,$6,$7}'
```

**Interpretación / por qué**
- Se identificaron las cuentas `carlos.lopez` y `rodgar` como candidatas.

## Paso 3 — Intento de `su` a otras cuentas

**Comando**
```bash
su carlos.lopez
su rodgar
```

**Interpretación / por qué**
- Ambos intentos **fallaron** (no se dispone de sus credenciales). Documentar como prueba descartada.

## Paso 4 — Enumeración de servicios, timers y procesos

**Comando**
```bash
systemctl list-units --type=service --state=running
systemctl list-timers --all
ps aux --sort=-%cpu | head
ls -la /etc/cron* ; cat /etc/crontab
find / -perm -4000 -type f 2>/dev/null
```

**Interpretación / por qué**
- Se buscaron vectores de escalada (cron/timers/SUID/servicios). Sin resultado confirmado hasta la fecha.

## Qué funcionó

- Acceso SSH como `marco.silva`.

## Qué falló (y por qué)

- `su` a `carlos.lopez` y `rodgar`: sin credenciales.

## Qué queda por aprender

- Ver [[Company1 - Pendientes]].
