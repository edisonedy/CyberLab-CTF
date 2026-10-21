---
tags: [lessons, indice]
tipo: lecciones
ultima_revision: 2026-10-20
---

# 🧠 Lecciones aprendidas (global)

> Errores transversales detectados al documentar las máquinas. Complementa las notas `04-Lecciones` de cada máquina.

## Metodología

| Lección | Origen |
|---|---|
| **HTTP 200 ≠ explotación** | [[AllSafe]] |
| Una prueba = **una** variable cambiada | [[AllSafe]] |
| Distinguir **hipótesis / payload / respuesta / resultado real** | todas |
| No confundir **nombre de usuario** con **credencial** | [[El Baifo]] |

## AD / Windows

| Lección | Origen |
|---|---|
| **BloodHound es guía, no garantía**: validar DACLs reales | [[Elevator]] |
| SAM local ≠ cuentas de dominio (**NTDS.dit**) | [[Chimichurri]] |
| Cadenas de credenciales se construyen eslabón a eslabón | [[Folclore]] |
| Cuidado con cadenas que **parecen** flags (GUID/GPO) | [[Chimichurri]] |
| `STATUS_PASSWORD_EXPIRED` = contraseña correcta pero debe cambiarse | [[Folclore]] |

## Linux / Web

| Lección | Origen |
|---|---|
| Un servicio web como **root** convierte un LFI en compromiso total | [[BabyCorp]] |
| Detectar **IPv6** evita perder servicios (Rsync) | [[SuraxCorp]] |
| Crackeo **offline** > fuerza bruta **online** | varias |
| No asumir el vector: en Cold Fear fue **audio**, no imagen | [[Cold Fear]] |

## Entorno / laboratorio

| Lección | Origen |
|---|---|
| APIPA = conflicto de **DHCP** con IP fija | [[Phantom]], [[Chimichurri]] |
| Bridged sobre `wlan0` deja la VM fuera de la red | [[Sulaco]] |
| `NS_ERROR_FAILURE` = **UUID de disco** duplicado | [[Chimichurri]] |
| `PYTHONNOUSERSITE=1` evita conflictos de `cryptography` | [[Elevator]] |

## Reglas de oro

1. Documentar **siempre**, incluso lo que falla.
2. No inventar resultados.
3. Preservar el material original.
4. Una técnica aprendida se convierte en nota de [[Indice-Tecnicas]].
