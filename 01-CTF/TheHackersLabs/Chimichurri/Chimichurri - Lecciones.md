---
tags: [ctf, thehackerslabs, windows, lecciones, active-directory]
tipo: lecciones
---

# Chimichurri — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Asumir que el login de dominio se podía cambiar desde el SAM local | `chntpw` no afecta a cuentas de dominio | En un **DC** las cuentas están en **`NTDS.dit`** |
| Confundir `THL{9A88909B-…}` con un flag | Falso positivo | Era nombre de GPO + GUID de registro |

## Tiempo perdido y por qué

- Resolver el arranque sin DHCP/APIPA y el `NS_ERROR_FAILURE` del VDI (UUID duplicado).

## Conceptos aprendidos

- **SAM local vs `NTDS.dit`** en controladores de dominio.
- Credenciales en ficheros de disco.
- Corrección del **UUID del VDI** (`sethduuid`).
- Lectura **offline** del NTFS con [[Sleuthkit]].

## Señales para reconocer en otra máquina

- [ ] DC de Active Directory (puertos 88/389/445…).
- [ ] Ficheros con pistas (`credenciales.txt`, `perico.txt`).
- [ ] Cadenas que **parecen** flags (`THL{…}`) pero son GUIDs.

## Preguntas de autoevaluación

1. ¿Dónde están las cuentas de dominio frente a las locales?
2. ¿Cómo distingues un flag real de un GUID/identificador?

## Técnicas que necesitan más práctica

- [[Sleuthkit]]
- Enumeración y ataque de AD (para Chimichurri como DC)
