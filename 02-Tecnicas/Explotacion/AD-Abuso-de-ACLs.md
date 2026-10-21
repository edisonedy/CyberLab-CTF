---
tags: [tecnica, explotacion, ad, acl, active-directory]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Phantom, Elevator]
---

# AD — Abuso de ACLs

## 1. Qué es

Uso de permisos delegados en objetos de Active Directory (`GenericAll`, `WriteDacl`, `WriteOwner`, `ForceChangePassword`, `AddSelf`, `WriteMembers`) para tomar control de cuentas/grupos.

## 2. Cómo funciona

- Cada ACE define quién puede hacer qué sobre un objeto.
- Una cadena de ACEs permite escalar de un usuario a otro, o a un grupo, hasta privilegios altos.

## 3. Cuándo sospecharla

- Tras recolectar **BloodHound**, cuando aparecen aristas hacia objetos de mayor privilegio.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
bloodhound-python -u <u> -p '<pw>' -d <dominio> -dc <dc> -ns <ip> -c All --zip
net rpc password <target> '<NuevaPw!>' -U '<dom>/<u>%<pw>' -S <ip>   # ForceChangePassword
# ldapmodify / Impacket para WriteDacl / AddSelf / WriteMembers
```

## 5. Herramientas

- [[BloodHound]], [[NetExec]], Impacket, `ldapmodify`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Phantom]] | mark→bob→frank (GenericAll, ForceChangePassword) | WinRM `frank` |
| [[Elevator]] | cadena hasta `OPERACIONES` (sin privilegios AD) | cadena **terminó**; se usó BitLocker |

## 7. Falsos positivos / pruebas fallidas

- **Elevator**: BloodHound mostraba una cadena, pero `OPERACIONES` **no** daba privilegios → hay que validar DACLs reales.

## 8. Prevención y mitigación

- Minimizar/auditar ACLs delegadas; revisar `nTSecurityDescriptor` periódicamente.

## 9. Técnicas relacionadas

- [[BloodHound]], [[ADCS-ESC3]], [[AD-Tombstones]]
