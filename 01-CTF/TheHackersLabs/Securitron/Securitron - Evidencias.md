---
tags: [ctf, thehackerslabs, linux, evidencias]
tipo: evidencias
---

# Securitron — Evidencias

> [!warning] Origen
> Documentado en `roadmap_securitron_ctf.md`. Contenido **reconstruido** salvo indicación.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | md | Roadmap | (documento ChatGPT, no en disco) | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Fuga de vhost + API key desde la IA | roadmap |
| 2 | SQLi en `Admin/login.php` | roadmap |
| 3 | Upload de avatar con PHP | roadmap |
| 4 | API Node por `::1:3000` + path traversal | roadmap |
| 5 | `sudo ar` → crontab | roadmap |
| 6 | PATH hijacking (`date`) → root | roadmap |

## Inferencias (no verificadas)

- El nombre exacto del binario sustituido (`date`) se toma del roadmap; el método (PATH hijacking) es el confirmado.

## Comandos clave reproducibles

```bash
curl -s --path-as-is "http://[::1]:3000/api/models/..%2f..%2findex.js"
sudo -l
sudo ar r /tmp/x.a /etc/crontab
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `shell.php`/avatar | local | subida de prueba |
| `/tmp/bash` | víctima | bash SUID temporal (limpiar) |
