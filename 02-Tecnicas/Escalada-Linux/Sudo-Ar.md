---
tags: [tecnica, escalada-linux, sudo, ar]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [Securitron]
---

# Sudo Ar

## 1. Qué es

`ar` con `sudo` permite **empaquetar y leer** ficheros que el usuario no podría, útil para exfiltrar (p.ej. el crontab de root). Combinado con otros fallos, escala.

## 2. Cómo funciona

- `sudo ar r /tmp/x.a /etc/crontab` mete el fichero en un archivo; luego se extrae/lee.

## 3. Cuándo sospecharla

- `sudo -l` incluye `ar`.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo -l
sudo ar r /tmp/x.a /etc/crontab 2>/dev/null
ar p /tmp/x.a /etc/crontab        # leer contenido
```

## 5. Herramientas

- [[GTFOBins]], lectura manual.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Securitron]] | `sudo ar` para leer el crontab de root | hallazgo → PATH hijacking → root |

## 7. Falsos positivos / pruebas fallidas

- `ar` en sí **no** da shell: es un vector de **lectura** de ficheros.

## 8. Prevención y mitigación

- No conceder `ar` en `sudoers`.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[Cron-Privilege-Escalation]]
