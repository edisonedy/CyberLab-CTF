---
tags: [ctf, thehackerslabs, windows, active-directory, bitlocker, acls, dislocker, resuelta]
tipo: ficha
plataforma: TheHackersLabs
os: Windows Server 2022 (DC — Active Directory)
dificultad: media-alta
estado: resuelto
ip: 192.168.56.104
fecha: 2026-10-20
user: true
root: true
tecnicas: [bloodhound, acl-abuse, addself, genericall, forcechangepassword, writedacl, writeowner, bitlocker, bek, dislocker, sleuthkit, cryptsetup]
fuente: Elevator.md
ultima_revision: 2026-10-20
---

# Elevator — The Hackers Labs

## Estado
✅ Resuelto (2026-10-20)

> [!tip] Writeup completo
> Ver [[Elevator - Roadmap]]

## Plataforma
The Hackers Labs

## Sistema operativo
**Windows Server 2022** — Domain Controller

## Entorno
- Hipervisor: VirtualBox
- Red: Host-Only `vboxnet0`
- DC: `ELEVATOR` (`elevator.bloodhound.thl`)
- IP: `192.168.56.104`
- Dominio: `bloodhound.thl`
- Disco principal: BitLocker (imagen `elev.raw`, 50 GB)
- Disco auxiliar: NTFS de 10 MB etiquetado `Bitlocker` (`elev_seed.ntfs`) con 3 archivos `.BEK`

## Credencial inicial
| Usuario | Contraseña |
|---|---|
| `john.smith` | `Rk436\#Z4&` |

> La barra invertida es literal. En shell: `'Rk436\#Z4&'`.

## Flags

| Flag | Valor | Ubicación |
|---|---|---|
| **USER** | `becbd588e130ecd0faea547fec3ab81e` | `C:\Users\Administrador\Desktop\user.txt` |
| **ROOT** | `a6fe6dcd34b4978ec1f3b8450aa3c886` | `C:\Users\Administrador\Desktop\root.txt` |

## Cadena AD identificada

```text
JOHN.SMITH  --AddSelf-->            FINANZAS
FINANZAS    --GenericAll-->         MARY.JOHNSON
MARY.JOHNSON --ForceChangePassword-> ROBERT.WILLIAMS
ROBERT.WILLIAMS (MARKETING) --WriteDacl--> PATRICIA.BROWN
PATRICIA.BROWN --WriteOwner-->      OPERACIONES (grupo)
PATRICIA.BROWN (owner) --WriteDacl/WriteMembers--> OPERACIONES
```

- El grupo `OPERACIONES` es `CN=OPERACIONES,OU=OPERACIONES,DC=bloodhound,DC=thl`, SID `S-1-5-21-3580157585-956322742-780763674-1115`.
- Se completó la cadena hasta añadir a `patricia.brown` y `john.smith` al grupo `OPERACIONES`.
- **Pero `OPERACIONES` no tiene ACEs salientes, no está anidado en ningún grupo privilegiado y no otorga DCSync ni acceso a shares.** La cadena AD termina ahí; no llega a `michael.jones` (Builtin Administrators) ni a DRSUAPI.

## Vulnerabilidad principal
1. **Abuso de ACLs en AD**: `AddSelf` → `GenericAll` → `ForceChangePassword` → `WriteDacl` → `WriteOwner` → `WriteMembers`.
2. **BitLocker sin protección efectiva**: las *startup keys* `.BEK` están en un disco auxiliar de 10 MB, accesible offline. Con ellas se descifra el volumen `C:` y se leen las flags sin necesidad de DCSync.

## Ruta
1. Autenticarse como `john.smith` y recolectar BloodHound.
2. Ejecutar la cadena AD hasta controlar el grupo `OPERACIONES`.
3. Comprobar que `OPERACIONES` no da privilegios AD (DACLs reales, `tokenGroups`, DRSUAPI y shares).
4. Extraer los `.BEK` del disco auxiliar con sleuthkit.
5. Identificar los protectores BitLocker del disco principal con `cryptsetup bitlkDump`.
6. Montar el volumen con `dislocker` + `.BEK` vía FUSE (sin root).
7. Leer `user.txt` y `root.txt` del NTFS descifrado con `fls`/`icat`.

## Lecciones aprendidas
- BloodHound es una guía, pero hay que validar las DACLs reales (`nTSecurityDescriptor` + control `SD_FLAGS`): no toda cadena de ACL llega a DCSync.
- `AddSelf`, `GenericAll`, `ForceChangePassword`, `WriteDacl`, `WriteOwner` y `WriteMembers` con `ldapmodify` e Impacket.
- Las *startup keys* `.BEK` de BitLocker permiten montar el volumen con `dislocker` sin recovery password ni root.
- `cryptsetup bitlkDump` sobre una copia sparse (primer/último tramo) identifica los protectores sin copiar los 50 GB.
- `dislocker` se puede extraer de un `.deb` y ejecutar en espacio de usuario con FUSE.
- `PYTHONNOUSERSITE=1` evita el conflicto de `cryptography` instalado en `~/.local` con las herramientas del sistema.

Detalle paso a paso y comandos: [[Elevator - Roadmap]]

## Notas relacionadas

- [[Elevator - Roadmap]]
- [[Elevator - Walkthrough]]
- [[Elevator - Evidencias]]
- [[Elevator - Lecciones]]
- [[Elevator - Defensas]]
