---
tags: [ctf, thehackerslabs, linux, lecciones, lfi]
tipo: lecciones
---

# BabyCorp — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| (no documentado) | — | — |

> No se registran errores relevantes en el material original. Se conserva la sección para mantener el formato.

## Conceptos aprendidos

- [[Gitea]] expuesta con repos **públicos** ⇒ fuga de código fuente.
- [[LFI]] / [[Path-Traversal]] por `open()` sin sanitizar.
- Respuesta en [[Base64]]: hay que decodificar.
- Un servicio web ejecutándose como **root** convierte un LFI en compromiso total.

## Señales para reconocer en otra máquina

- [ ] Gitea/plataforma de repos en un puerto no estándar (`:3000`).
- [ ] Endpoints que **abren rutas** indicadas por el usuario (`load-file`, `download`, `view`).
- [ ] Servicios web con `Uid: 0` (`/proc/self/status`).

## Preguntas de autoevaluación

1. ¿Cómo confirmas que el servicio corre como root sin explotar nada aún?
2. ¿Qué diferencias hay entre LFI y Path Traversal en este endpoint?
3. ¿Por qué un repo público acelera tanto la explotación?

## Técnicas que necesitan más práctica

- [[LFI]]
- Revisión de código Python/Flask para localizar sinks.
- [[Base64]] en respuestas JSON.
