---
tags: [playbook, ad, active-directory]
tipo: playbook
estado: activo
ultima_revision: 2026-10-20
---

# Playbook — Active Directory

> Basado en Phantom y Elevator. **Regla:** BloodHound es una **guía**, no una garantía.

## 1. Enumeración inicial

```bash
nmap -Pn -sCV -p- <DC>
netexec smb <DC> -u '' -p ''                     # null auth
netexec smb <DC> -u <user> -p '<pw>' --users --shares --rid-brute
```

## 2. Recolección BloodHound

```bash
bloodhound-python -u <user> -p '<pw>' -d <dom> -dc <dc> -ns <DC> -c All --zip
```

## 3. Rutas de ataque típicas

| Vector | Técnica |
|---|---|
| ACLs delegadas | [[AD-Abuso-de-ACLs]] |
| NTLM theft (LNK/SCF) | [[Responder-NTLM-Theft]] |
| Crackeo de hashes | [[Cracking]] |
| Objetos eliminados | [[AD-Tombstones]] |
| AD CS | [[ADCS-ESC3]] |
| Certificado → hash | [[PKINIT-Pass-The-Hash]] |

## 4. Validación obligatoria

Antes de asumir que una ruta «funciona»:

- [ ] Comprobar **ACEs salientes** del objeto controlado.
- [ ] Comprobar **tokenGroups** / anidamiento.
- [ ] Probar **DRSUAPI (DCSync)** explícitamente.
- [ ] Probar acceso a **shares**.

> Elevator demostró que una cadena puede **terminar** sin privilegios.

## 5. Contingencias

- Sin ruta AD: revisar **discos auxiliares**, `.BEK`, BitLocker ([[BitLocker-BEK]]).
- SAM local ≠ cuentas de dominio (**NTDS.dit**).
