---
tags: [tecnica, servicios, ftp]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [Cold Fear]
---

# FTP anónimo

## 1. Qué es

Servidor FTP que permite login **anónimo** (`anonymous`), exponiendo ficheros sin credenciales.

## 2. Cómo funciona

- `anonymous_enable=YES` → acceso de solo lectura (o más) al directorio raíz del FTP.

## 3. Cuándo sospecharla

- Puerto `21` abierto.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
ftp <host>          # usuario: anonymous / (vacío)
curl -s ftp://anonymous:anonymous@<host>/
wget -m ftp://anonymous:anonymous@<host>/
```

## 5. Herramientas

- `ftp`, `curl`, `wget`, [[Nmap]] (`ftp-anon`).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | FTP anónimo con `bitacora-01.txt` | pista inicial (`tripulanteX`) |

## 7. Falsos positivos / pruebas fallidas

- Login anónimo **permitido** no siempre implica ficheros sensibles.

## 8. Prevención y mitigación

- `anonymous_enable=NO`; si se necesita, directorio de solo lectura sin datos sensibles.

## 9. Técnicas relacionadas

- [[Rsync]], [[Exif-Metadata]]
