---
tags: [tecnica, explotacion, password, spraying]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Folclore, Phantom]
---

# Password Spraying

## 1. Qué es

Probar **pocas contraseñas** contra **muchos usuarios** (al revés que la fuerza bruta), minimizando bloqueos.

## 2. Cómo funciona

- Se usa una contraseña candidata (filtrada/reutilizada) sobre la lista de usuarios.
- Ideal cuando hay indicios de **reutilización**.

## 3. Cuándo sospecharla

- Contraseña obtenida en un contexto (share, tombstone) que podría reutilizarse.
- Política sin bloqueo agresivo.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
netexec smb <ip> -u users.txt -p 'Candidata' --continue-on-success
```

## 5. Herramientas

- [[NetExec]], [[CrackMapExec]] (referencia).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Folclore]] | `45yD#k7` reusada → `La_mulata_de_Cordoba` | credencial |
| [[Phantom]] | lista de usuarios con contraseña de tombstone | `tomas` |

## 7. Falsos positivos / pruebas fallidas

- Un `--continue-on-success` mal usado corta al primer acierto y **oculta** otros válidos.

## 8. Prevención y mitigación

- Bloqueo por intentos, MFA, contraseñas únicas, detección de spraying.

## 9. Técnicas relacionadas

- [[Cracking]], [[AD-Tombstones]]
