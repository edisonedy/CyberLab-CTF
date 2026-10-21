---
tags: [herramienta, ad, bloodhound]
tipo: herramienta
categoria: ad
ultima_revision: 2026-10-20
---

# BloodHound / bloodhound-python

Mapea relaciones y permisos en Active Directory para hallar rutas de ataque.

## Uso típico

```bash
bloodhound-python -u <user> -p '<pw>' -d <dom> -dc <dc> -ns <ip> -c All --zip
```

## Notas / lecciones

- **Guía, no garantía**: validar DACLs reales (Elevator: `OPERACIONES` no daba privilegios).
- Revisar edges ADCS (ESC*).

## Técnicas relacionadas

- [[AD-Abuso-de-ACLs]], [[ADCS-ESC3]]
