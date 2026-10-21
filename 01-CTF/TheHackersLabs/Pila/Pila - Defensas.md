---
tags: [ctf, thehackerslabs, windows, defensas, buffer-overflow]
tipo: defensas
---

# Pila — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Stack Buffer Overflow | `strcpy(Buffer2S, Input)` sin comprobar longitud |
| 2 | Ausencia de mitigaciones | Windows Server 2008 x86 sin ASLR/DEP efectivos en el binario |

## Impacto

- Ejecución de código con los privilegios del proceso (y acceso a ficheros de Administrador).

## Prevención

- Sustituir `strcpy` por funciones con límite (`strncpy_s`, `snprintf`).
- Compilar con **ASLR**, **DEP** y **/GS** (stack cookies).
- No ejecutar servicios vulnerables como Administrador.

## Detección

- Conexiones al puerto `9999` (servicio de prueba no productivo).
- Crashes repetidos del proceso del servidor.
- Patrones cíclicos (`Aa0Aa1…`) en memoria/logs.

## Hardening recomendado

- [ ] Retirar binarios de prueba (`vulnserver`) de sistemas productivos.
- [ ] Habilitar mitigaciones del sistema operativo.
- [ ] Principio de mínimo privilegio para servicios.
