---
tags: [tecnica, escalada-linux, apt, hooks]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [THLCPPTV16]
---

# APT Pre-Invoke (hooks como root)

## 1. Qué es

Abuso de **hooks** de APT (`Pre-Invoke`/`Post-Invoke`) cuando un fichero de configuración de APT es escribible por el usuario y `apt` se ejecuta como root.

## 2. Cómo funciona

- APT ejecuta los hooks como **root**.
- Si `/etc/apt/apt.conf.d/*` es escribible, se inyecta un comando que se ejecuta en el siguiente `apt` privilegiado.

## 3. Cuándo sospecharla

- Permisos de escritura en `/etc/apt/apt.conf.d/`.
- `apt update`/`upgrade` ejecutado por root (cron, tareas).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
ls -la /etc/apt/apt.conf.d/
echo 'APT::Update::Pre-Invoke {"cp /bin/bash /tmp/bash; chmod +s /tmp/bash";};' > /etc/apt/apt.conf.d/99x
sudo apt update
/tmp/bash -p; id
```

## 5. Herramientas

- Permisos `ls -la`, `systemctl`, `cron`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | Config de APT escribible en el host de Jerry | root de Jerry |

## 7. Falsos positivos / pruebas fallidas

- Tener escritura en `apt.conf.d` **no** basta si `apt` nunca se ejecuta como root.

## 8. Prevención y mitigación

- Propiedad root y sin escritura en configs de APT.
- Auditar tareas que ejecutan `apt` como root.

## 9. Técnicas relacionadas

- [[Cron-Privilege-Escalation]], [[SUID]]
