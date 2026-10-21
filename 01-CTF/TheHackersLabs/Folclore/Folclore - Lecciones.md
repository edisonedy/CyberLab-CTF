---
tags: [ctf, thehackerslabs, windows, lecciones, smb, esteganografia]
tipo: lecciones
---

# Folclore — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Intentar cambiar la contraseña **expirada por SMB** | Bloqueo (`STATUS_USER_SESSION_DELETED`, `samr` → `ACCESS_DENIED`) | El cambio se hace por **GUI/consola**; en SMB es inviable en 19041 |
| Buscar solo credenciales «normales» | Sin avance | Leer **metadatos** (EXIF), audio/imagen (steganografía) y ZIP |
| Fuerza bruta masiva | Lento/ruidoso | **Spraying dirigido** y wordlists por **patrón** |

## Conceptos aprendidos

- [[RID-Cycling]] sobre SMB sin credenciales.
- Interpretación de `STATUS_PASSWORD_EXPIRED`.
- [[Exif-Metadata]] (`exiftool`).
- [[Esteganografia-LSB]] con passphrase (`stegseek` + rockyou).
- [[Password-Spraying]] + crackeo de **ZIP** (`zip2john`+john).
- Wordlist por **patrón conocido** (hashcat `-a 3 --stdout`).
- Share escribible + **tarea programada** → `.ps1` → reverse shell.
- Credenciales en claro en el perfil → [[PsExec]] (SYSTEM).

## Señales para reconocer en otra máquina

- [ ] SMB con signing no requerido y sesión guest activa.
- [ ] Shares «temáticos» con imágenes/textos/ZIP.
- [ ] Estado `PASSWORD_EXPIRED` en cuentas.
- [ ] Shares escribibles con tareas programadas.
- [ ] Credenciales en claro en el perfil del usuario.

## Preguntas de autoevaluación

1. ¿Por qué el cambio de contraseña expirada falla por SMB y no por GUI?
2. ¿Cómo decides entre spraying y fuerza bruta?
3. ¿Cómo construyes una wordlist cuando conoces el **patrón**?

## Técnicas que necesitan más práctica

- [[Esteganografia-LSB]]
- [[RID-Cycling]]
- Crackeo de ZIP y wordlists por patrón
