---
tags: [ctf, thehackerslabs, windows, lecciones, active-directory, bitlocker]
tipo: lecciones
---

# Elevator — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Confiar en BloodHound como «ruta garantizada» | Se creyó que `OPERACIONES` daría DCSync | Validar **DACLs reales** (`nTSecurityDescriptor` + `SD_FLAGS`), `tokenGroups`, DRSUAPI y shares |
| Suponer que la escalada debía ser AD | Se ignoró el disco auxiliar | Revisar **todo** el entorno (discos, `.BEK`, BitLocker) |
| Copiar los 50 GB del disco | Costoso | Usar copia **sparse** (primer/último tramo) para `bitlkDump` |

## Conceptos aprendidos

- [[BloodHound]] como **guía**, no como verdad absoluta.
- ACLs: `AddSelf`, `GenericAll`, `ForceChangePassword`, `WriteDacl`, `WriteOwner`, `WriteMembers` con `ldapmodify` e Impacket.
- **BitLocker**: las startup keys `.BEK` permiten montar con [[Dislocker]] **sin** recovery password ni root.
- `cryptsetup bitlkDump` identifica protectores sin copiar todo el disco.
- `PYTHONNOUSERSITE=1` evita conflictos de `cryptography` con herramientas del sistema.

## Señales para reconocer en otra máquina

- [ ] Cadenas de ACL que **terminan** en un grupo sin ACEs salientes.
- [ ] Discos **auxiliares** pequeños con `.BEK`.
- [ ] Volúmenes **BitLocker** con startup keys accesibles.
- [ ] Necesidad de alternativas a DCSync.

## Preguntas de autoevaluación

1. ¿Cómo compruebas si un grupo controlado otorga privilegios reales?
2. ¿Qué diferencia hay entre startup key, recovery password y TPM protector?
3. ¿Cómo montas un volumen BitLocker con `dislocker` sin root?

## Técnicas que necesitan más práctica

- [[BitLocker-BEK]] / [[Dislocker]]
- Validación de DACLs reales en AD
- [[Sleuthkit]]
