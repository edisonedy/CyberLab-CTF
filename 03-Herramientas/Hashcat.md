---
tags: [herramienta, criptografia, cracking]
tipo: herramienta
categoria: criptografia
ultima_revision: 2026-10-20
---

# Hashcat

Cracking por GPU/CPU con modos y máscaras; también generación de wordlists.

## Uso típico

```bash
hashcat -m 5600 hash.txt /usr/share/wordlists/rockyou.txt      # NetNTLMv2
hashcat -a 3 --stdout 'J9c?u?l?d?l?d%/' > wordlist.txt          # máscara -> wordlist
```

## Técnicas relacionadas

- [[Cracking]]
