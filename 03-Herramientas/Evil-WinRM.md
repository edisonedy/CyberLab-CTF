---
tags: [herramienta, windows, reverse-shell]
tipo: herramienta
categoria: windows
ultima_revision: 2026-10-20
---

# Evil-WinRM

Shell interactiva sobre WinRM (5985) con soporte de password, hash y certificados.

## Uso típico

```bash
evil-winrm -i <ip> -u <user> -p '<pw>'
evil-winrm -i <ip> -u Administrador -H <hash_NT>
```

## Notas / lecciones

- Funciona cuando hay WinRM habilitado (5985) y el usuario tiene permiso remoto.

## Técnicas relacionadas

- [[AD-Abuso-de-ACLs]], [[PKINIT-Pass-The-Hash]]
