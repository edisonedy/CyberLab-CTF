---
tags: [ctf, thehackerslabs, linux, lecciones]
tipo: lecciones
---

# Sulaco — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Asumir conectividad sin revisar el NIC | VM invisible en `192.168.56.x` | Revisar **bridged vs host-only** y el DHCP |
| Usar `curl` con UA por defecto | Petición bloqueada | Enviar **User-Agent de navegador** |
| Intentar `admin_flag.php` directamente | Puerta + CSRF | Entender `inc_gate.php` y el doble-submit |

## Tiempo perdido y por qué

- Diagnóstico del adaptador de red y del DHCP de VirtualBox.

## Conceptos aprendidos

- Diagnóstico de red VirtualBox (bridged/host-only, DHCP).
- Enumeración web (`ffuf`), `robots.txt`, panel oculto.
- Autenticación por **doble-submit de nonce** (cookie == header).
- **UA gating** y **CSRF** (`md5('sulaco-secret-'.$id)`).
- Lectura offline con [[Sleuthkit]].

## Señales para reconocer en otra máquina

- [ ] `robots.txt` con rutas «prohibidas» interesantes.
- [ ] Endpoints que exigen UA concreto o nonce en cookie+header.
- [ ] Ficheros PHP con la flag embebida (posible lectura offline).

## Preguntas de autoevaluación

1. ¿Qué es un doble-submit de nonce y qué ataque mitiga?
2. ¿Cómo detectas un gating por User-Agent?
3. ¿Cuándo conviene la lectura offline del disco?

## Técnicas que necesitan más práctica

- [[Sleuthkit]]
- Mecanismos de nonce/CSRF en apps PHP
