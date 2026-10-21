---
tags: [herramienta, explotacion, ntlm, responder]
tipo: herramienta
categoria: explotacion
ultima_revision: 2026-10-20
---

# Responder

Captura hashes NetNTLM(v1/v2) envenenando LLMNR/NBT-NS/SMB.

## Uso típico

```bash
sudo responder -I vboxnet0 -wv
```

## Notas / lecciones

- Necesita un «cebo» que fuerce la autenticación (LNK/SCF/URL en share escribible).

## Técnicas relacionadas

- [[Responder-NTLM-Theft]], [[Cracking]]
