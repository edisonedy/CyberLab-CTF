---
tags: [tecnica, explotacion, ad, pass-the-hash, pkinit]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Phantom]
---

# PKINIT y Pass-the-Hash

## 1. Qué es

- **PKINIT**: autenticación Kerberos con **certificado** → obtención de TGT y, con la clave derivada, del **hash NT**.
- **Pass-the-Hash (PtH)**: autenticarse usando el **hash NT** sin conocer la contraseña.

## 2. Cómo funciona

- Con un certificado válido de un usuario, `certipy auth` obtiene su hash NT.
- Con el hash, herramientas SMB/WinRM se autentican directamente.

## 3. Cuándo sospecharla

- Tras obtener un certificado (p.ej. ADCS ESC3).
- Cuando se dispone de un hash NT (DCSync, PKINIT, dump).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
certipy-ad auth -pfx admin.pfx -dc-ip <ip>          # -> hash NT
netexec winrm <ip> -u Administrador -H <hash_NT>
evil-winrm -i <ip> -u Administrador -H <hash_NT>
```

## 5. Herramientas

- [[Certipy]], [[NetExec]], Evil-WinRM, Impacket.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Phantom]] | PKINIT con cert ESC3 → hash NT → PtH | **ADMIN FLAG** |

## 7. Falsos positivos / pruebas fallidas

- Un certificado **sin** EKU de client-auth no sirve para PKINIT.

## 8. Prevención y mitigación

- Proteger cuentas privilegiadas (LAPS, tiering), detectar PtH (mismo hash, distintos orígenes), rotar `krbtgt`.

## 9. Técnicas relacionadas

- [[ADCS-ESC3]], [[SMB-Enum]]
