---
tags: [ctf, thehackerslabs, windows, walkthrough, active-directory]
tipo: walkthrough
estado: resuelto
---

# Chimichurri — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Pasos del roadmap de Chimichurri.

## Paso 1 — Diagnóstico de red (DHCP/APIPA)

**Comando**
```bash
VBoxManage list dhcpservers
VBoxManage dhcpserver modify --netname HostInterfaceNetworking-vboxnet0 --ip 192.168.56.2
```

**Interpretación / por qué**
- La VM arrancaba **sin lease DHCP / con APIPA**. Ajustar el DHCP de VirtualBox la hace visible en la red host-only.

## Paso 2 — Corregir el UUID del VDI (si aplica)

**Comando**
```bash
VBoxManage internalcommands sethduuid "<vdi>" c9fd0695-6ab4-4e18-8c7f-443d746f1a65
```

**Interpretación / por qué**
- Tras restaurar el VDI, VirtualBox falla con `NS_ERROR_FAILURE` si el UUID del disco ya existe. Fijar un UUID propio lo resuelve.

## Paso 3 — Pista de credenciales

**Ruta**
```text
C:\drogas\credenciales.txt   →  pista: usar el usuario "hacker"
```

**Interpretación / por qué**
- Las pistas en disco (`credenciales.txt`) orientan al usuario objetivo.

## Paso 4 — Credenciales

**Ruta**
```text
C:\Users\hacker\Desktop\perico.txt  →  hacker:Perico69
```

**Interpretación / por qué**
- Credenciales de laboratorio confirmadas en fichero.

## Paso 5 — User flag

**Ruta**
```text
C:\Users\hacker\Desktop\user.txt   →  acrsgvs6edr8f5vaw9a8eadv6fa9b
```

## Paso 6 — Escalada y root flag

**Ruta**
```text
C:\Users\Administrador\Desktop\root.txt  →  hjafcdv8a75e3cvsdfg6asd4f9vbsf9sa
```

## Paso 6b — Alternativa: extracción offline del NTFS

**Comando**
```bash
fls -o <offset> -r -p chimichurri.raw | grep -i -E 'user.txt|root.txt|perico.txt|credenciales.txt'
icat -o <offset> chimichurri.raw <inode>
```

**Interpretación / por qué**
- Permite obtener los flags **sin login**; se documenta como camino alternativo.

## Advertencia importante (DC)

- En un **Controlador de Dominio** las cuentas están en **`NTDS.dit`**, **no** en el SAM local:
  `chntpw` sobre el SAM **no** cambia el login de dominio.

## Qué funcionó

- Seguir pistas en disco.
- Lectura offline con [[Sleuthkit]].

## Qué falló (y por qué)

- Modificar contraseñas del SAM local en un DC (irrelevante para el dominio).

## Qué aprendí

- Diagnóstico de red VirtualBox y UUID de discos.
- SAM local vs `NTDS.dit`.
- Identificar un **falso positivo** (`THL{9A88909B-…}` era nombre de GPO + GUID de registro, no un flag).
