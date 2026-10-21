---
tags: [herramienta, criptografia, cracking]
tipo: herramienta
categoria: criptografia
ultima_revision: 2026-10-20
---

# John the Ripper

Cracking de hashes por diccionario/reglas/máscara.

## Uso típico

```bash
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt hash.txt
zip2john archivo.zip > zip.hash && john --wordlist=rockyou.txt zip.hash
john --show hash.txt
```

## Notas / lecciones

- Identificar bien el formato (`netntlmv2`, `phpass`, ZIP…).

## Técnicas relacionadas

- [[Cracking]], [[Responder-NTLM-Theft]]
