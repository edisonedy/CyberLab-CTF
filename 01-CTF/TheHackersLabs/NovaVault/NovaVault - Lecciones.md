---
tags: [ctf, thehackerslabs, lecciones]
tipo: lecciones
---

# NovaVault — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Asumir que `127.0.0.1` era el único loopback | SSRF bloqueado | Probar `127.1`, `[::1]`, decimal/hex |
| Ignorar los **grupos** del usuario | No leer la flag | `id`/`groups` tras cada acceso |
| Errores de **quoting** en JSON | Peticiones inválidas | Usar `--data` con comillas correctas |

## Conceptos aprendidos

- [[SSRF]] y bypasses de validación de loopback.
- Descubrimiento de endpoints (`/docs`).
- SSRF **POST**.
- [[Grupos-Linux]] como control de acceso.
- [[SUID]] + [[Reversing-Token-Validation]] + [[XOR]].

## Señales para reconocer en otra máquina

- [ ] Endpoint que acepta una **URL** para "inspect"/"fetch".
- [ ] Validación de IP por lista negra (bypasseable).
- [ ] Documentación de API expuesta (`/docs`, `/openapi.json`).
- [ ] Binarios SUID propios (`vault-*`).
- [ ] Flags protegidas por **grupo**.

## Preguntas de autoevaluación

1. ¿Qué representaciones alternativas de loopback conoces?
2. ¿Cómo pasas de un SSRF GET a un SSRF POST?
3. ¿Cómo revviertes una validación de token con `objdump`?

## Técnicas que necesitan más práctica

- [[SSRF]]
- [[Reversing-Token-Validation]]
- [[XOR]]
