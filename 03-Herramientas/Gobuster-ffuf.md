---
tags: [herramienta, reconocimiento, web]
tipo: herramienta
categoria: web
ultima_revision: 2026-10-20
---

# Gobuster / ffuf / Feroxbuster

Fuzzing de directorios, ficheros y virtual hosts.

## Uso típico

```bash
gobuster dir -u http://vhost.thl/ -w /usr/share/wordlists/dirb/common.txt
ffuf -u http://<IP>/ -H "Host: FUZZ.thl" -w subdomains.txt
ffuf -u http://vhost.thl/FUZZ -w /usr/share/seclists/Discovery/Web-Content/common.txt
```

## Notas / lecciones

- Fijar el **vhost** antes de fuzzear (si no, resultados del sitio por defecto).
- Partir de `robots.txt` cuando exista (Sulaco).

## Técnicas relacionadas

- [[Virtual-Hosts]], [[Path-Traversal]]
