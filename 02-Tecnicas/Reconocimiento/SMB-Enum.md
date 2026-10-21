---
tags: [tecnica, reconocimiento, smb, rid-cycling]
tipo: tecnica
categoria: reconocimiento
estado: practicado
maquinas: [Folclore, Phantom, Chimichurri]
---

# Enumeración SMB / RID Cycling

## 1. Qué es

Enumeración de recursos y **usuarios** SMB sin credenciales válidas (sesión guest/nula) mediante RID cycling.

## 2. Cómo funciona

- SMB permite listar SIDs por RID; probando un rango se obtienen nombres de cuentas.

## 3. Cuándo sospecharla

- Puertos `139/445`; signing no requerido; sesión guest/nula activa.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
netexec smb <IP> -u guest -p '' --rid-brute
netexec smb <IP> -u guest -p '' --shares
nmap -p445 --script smb-os-discovery <IP>
```

## 5. Herramientas

- [[NetExec]], `enum4linux-ng`, [[Nmap]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Folclore]] | RID cycling con `guest` | 6 usuarios |
| [[Phantom]] | Null Auth permitido pero **sin** listar shares | necesitó credenciales |
| [[Chimichurri]] | DC de AD | enumeraciónSMB |

## 7. Falsos positivos / pruebas fallidas

- En Phantom, Null Auth **conecta** pero **no** permite listar shares ni RID-brute → hubo que usar credenciales.

## 8. Prevención y mitigación

- Deshabilitar guest/nulls y restringir RID enumeration; SMB signing.

## 9. Técnicas relacionadas

- [[Nmap]], [[Pass-The-Hash]], [[Responder]]
