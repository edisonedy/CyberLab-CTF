---
tags: [ctf, thehackerslabs, lecciones]
tipo: lecciones
---

# Securitron — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Asumir que `localhost` era solo `127.0.0.1` | API interna invisible | Probar **IPv6** `[::1]` |
| Usar curl sin `--path-as-is` | `../` normalizado, traversal fallaba | Usar `--path-as-is` |
| Probar SQLi solo con payloads visibles | Sin señal | Usar **time-based** (`SLEEP`) |

## Tiempo perdido y por qué

- Encontrar la variante de upload que evadía la validación MIME/extensión.

## Conceptos aprendidos

- [[Fuga-Info-IA]].
- [[SQL-Injection]] boolean vs time-based.
- [[File-Upload]] + validación MIME.
- IPv4 vs IPv6 en localhost.
- [[Path-Traversal]] en Node/Express.
- [[Sudo-Ar]] y [[Path-Hijacking]].

## Señales para reconocer en otra máquina

- [ ] App con "IA" que devuelve contexto interno.
- [ ] Formularios de subida (avatar) con validación laxa.
- [ ] Servicios que escuchan en `::1`.
- [ ] `sudo -l` con `ar`, `tar`, pagers, etc.
- [ ] Crons que invocan binarios sin ruta absoluta.

## Preguntas de autoevaluación

1. ¿Cuándo eliges SQLi time-based sobre boolean-based?
2. ¿Por qué `curl --path-as-is` es clave en traversal?
3. ¿Qué hace peligroso a `sudo ar`?

## Técnicas que necesitan más práctica

- [[SQL-Injection]] (time-based a ciegas).
- [[Path-Traversal]]
- [[Path-Hijacking]]
