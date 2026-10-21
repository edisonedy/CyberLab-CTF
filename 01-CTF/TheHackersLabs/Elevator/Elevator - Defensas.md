---
tags: [ctf, thehackerslabs, windows, defensas, active-directory, bitlocker]
tipo: defensas
---

# Elevator — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | ACLs delegadas peligrosas | `AddSelf`/`GenericAll`/`ForceChangePassword`/`WriteDacl`/`WriteOwner`/`WriteMembers` |
| 2 | BitLocker inefectivo | Startup keys `.BEK` accesibles en disco auxiliar |
| 3 | Exposición de discos | Discos auxiliares con secretos, accesibles offline |

## Impacto

- Control de grupos de AD y lectura de las flags mediante descifrado **offline**.

## Prevención

- Minimizar y auditar ACLs delegadas en AD.
- Proteger BitLocker con **TPM + PIN**, no con startup keys almacenadas en el mismo equipo.
- Guardar `.BEK`/recovery passwords fuera del host (AD/escrow seguro) y no en discos locales.
- Cifrado a nivel de hipervisor y control de acceso a los discos virtuales.

## Detección

- Cambios en `nTSecurityDescriptor` de objetos sensibles.
- Uso de `ldapmodify`/Impacket para modificar ACLs.
- Acceso offline a discos virtuales.

## Hardening recomendado

- [ ] Auditoría continua de ACLs (BloodHound defensivo).
- [ ] BitLocker con TPM+PIN y escrow de claves.
- [ ] Retirar discos auxiliares con secretos.
- [ ] Control de acceso a los VDI/imágenes.
