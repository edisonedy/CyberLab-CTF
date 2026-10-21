---
tags: [ctf, thehackerslabs, windows, lecciones, active-directory, adcs]
tipo: lecciones
---

# Phantom — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Interpretar APIPA como «VM rota» | Pérdida de tiempo | Revisar **conflicto de IP** entre DHCP de VirtualBox y la IP fija de la VM |
| Esperar enumerar shares con null session | Sin resultados | Usar credenciales válidas (`mark`) |
| Usar contraseñas débiles al cambiar (`net rpc password`) | Rechazadas por política | Contraseñas largas y complejas |

## Conceptos aprendidos

- Abuso de **ACLs** en AD (`GenericAll`/`WriteDACL`/`ForceChangePassword`/`AddSelf`).
- **NTLM theft** con LNK en share escribible + [[Responder]].
- Crackeo **NetNTLMv2** con `john --format=netntlmv2`.
- **Tombstones / Deleted Objects** (control LDAP show-deleted).
- Credenciales en `description` y **reutilización** / password spraying.
- [[ADCS-ESC3]] (Enrollment Agent on-behalf-of) + [[PKINIT]] + [[Pass-The-Hash]].

## Señales para reconocer en otra máquina

- [ ] DC con Null Auth pero sin listar shares.
- [ ] Shares escribibles visitados por tareas de otros usuarios.
- [ ] Pistas que mencionan «cuentas de prueba» y «certificados internos».
- [ ] Plantillas de certificado con EKU *Certificate Request Agent* (ESC3).
- [ ] `description` de cuentas con aspecto de contraseña.

## Preguntas de autoevaluación

1. ¿Por qué un LNK en un share escribible permite robar un hash NTLM?
2. ¿Qué diferencia hay entre GenericAll y ForceChangePassword?
3. ¿Qué hace exactamente ESC3 y por qué PKINIT da el hash NT?

## Técnicas que necesitan más práctica

- [[ADCS-ESC3]]
- [[Responder]] / [[NTLM-Theft]]
- [[AD-Tombstones]]
