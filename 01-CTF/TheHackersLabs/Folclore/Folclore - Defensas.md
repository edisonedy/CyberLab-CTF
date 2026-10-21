---
tags: [ctf, thehackerslabs, windows, defensas, smb]
tipo: defensas
---

# Folclore — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Enumeración de usuarios | Guest/nulls session y **RID cycling** habilitados |
| 2 | Secretos en metadatos/imágenes | Pistas y contraseñas embebidas en EXIF y esteganografía |
| 3 | Password spraying viable | Contraseñas débiles/compartidas y política laxa |
| 4 | Share escribible con tarea programada | Ejecución de `.ps1` desde share por el sistema |
| 5 | Credenciales en claro en el perfil | Contraseña de Administrador guardada en un `.txt` |

## Impacto

- Compromiso total hasta **SYSTEM** en un host standalone.

## Prevención

- Deshabilitar guest/mull sessions y restringir RID enumeration.
- No almacenar secretos en imágenes, metadatos ni ficheros de perfil.
- Política de contraseñas fuerte, sin reutilización; bloquear spraying (lockout).
- Revisar shares escribibles y tareas programadas que ejecutan scripts.
- No guardar credenciales privilegiadas en claro (usar LAPS/gestor).

## Detección

- RID cycling (muchos lookups de SIDs) en logs SMB.
- Escritura de `.ps1`/`.lnk` en shares.
- Ejecución de tareas programadas inesperadas.
- Accesos con `psexec` (servicio remoto creado).

## Hardening recomendado

- [ ] `RestrictAnonymous` y sin sesión guest.
- [ ] Auditoría de shares y tareas programadas.
- [ ] Lockout + MFA donde aplique.
- [ ] Búsqueda de secretos en perfiles y metadatos.
