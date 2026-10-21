---
tags: [ctf, thehackerslabs, windows, defensas, active-directory, adcs]
tipo: defensas
---

# Phantom — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | ACLs delegadas peligrosas | `GenericAll`/`ForceChangePassword` innecesarios |
| 2 | NTLM theft | LLMNR/NBT-NS activos, SMB sin firmar, share escribible |
| 3 | Secretos en tombstones | Contraseña en `description`; papelera de AD sin purgar |
| 4 | Reutilización de credenciales | Contraseñas compartidas entre cuentas |
| 5 | ADCS ESC3 | Plantilla con EKU *Certificate Request Agent* mal restringida |

## Impacto

- Compromiso progresivo hasta **Administrador del dominio**.

## Prevención

- Minimizar y auditar ACLs delegadas.
- Deshabilitar **LLMNR/NBT-NS** y exigir **SMB signing**.
- No dejar shares escribibles por todos; monitorizar escritura de `.lnk/.scf`.
- Purgar cuentas de prueba y **no** guardar secretos en `description`; vaciar la papelera de AD.
- Endurecer plantillas de certificados (quitar EKU *Certificate Request Agent*, restringir *enroll*).
- Rotar el hash de `krbtgt` y de cuentas comprometidas.

## Detección

- Capturas NTLMv2 anómalas / autenticaciones LLMNR.
- Escritura de `.lnk`/`.scf` en shares.
- Peticiones de certificado con *on-behalf-of*.
- Uso de Pass-the-Hash (mismo hash, distintos hosts).

## Hardening recomendado

- [ ] Revisión de ACLs y de rights delegados.
- [ ] SMB signing obligatorio + LLMNR/NBT-NS off.
- [ ] Auditoría de plantillas ADCS.
- [ ] Rotación de secretos de dominio.
