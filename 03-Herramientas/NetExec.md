---
tags: [herramienta, ad, smb, winrm]
tipo: herramienta
categoria: ad
ultima_revision: 2026-10-20
---

# NetExec (nxc)

Framework de enumeración y explotación de redes Windows/AD.

## Uso típico

```bash
netexec smb <ip> -u guest -p '' --rid-brute --shares
netexec smb <ip> -u <user> -p '<pw>' --users
netexec winrm <ip> -u <user> -p '<pw>'
netexec winrm <ip> -u Administrador -H <hash_NT>
```

## Notas / lecciones

- Requiere `PYTHONNOUSERSITE=1` en este entorno para evitar conflictos.
- `--continue-on-success` para spraying.
- No todo `Pwn3d!` implica privilegios completos: comprobar contexto.

## Técnicas relacionadas

- [[SMB-Enum]], [[Password-Spraying]], [[PKINIT-Pass-The-Hash]]
