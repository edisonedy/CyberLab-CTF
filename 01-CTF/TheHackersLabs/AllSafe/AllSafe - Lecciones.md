---
tags: [ctf, thehackerslabs, linux, lecciones, en-progreso]
tipo: lecciones
estado: en-progreso
---

# AllSafe — Lecciones aprendidas (parcial)

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Interpretar **HTTP 200** como explotación | Falsa sensación de éxito con LFI/LaTeX | Verificar el **contenido** real del PDF (byte a byte) |
| Probar varias variantes a la vez | No se sabe qué causó el resultado | Cambiar **una sola** variable por prueba |
| Asumir que el backend es LaTeX | Se probaron payloads posiblemente irrelevantes | Confirmar primero el motor/sink |

## Tiempo perdido y por qué

- Iterar payloads LaTeX sin confirmar antes el **sink** (`compile.php`) ni el motor de PDF.

## Conceptos a reforzar

- Diferencia entre **generar un PDF** y **lograr LFI/ejecución**.
- Metodología de hipótesis → payload → respuesta → **resultado real**.

## Señales para reconocer en otra máquina

- [ ] Formularios que **generan documentos** (PDF, facturas, contratos).
- [ ] Parámetros que puedan entrar a LaTeX/plantillas del lado servidor.
- [ ] Respuestas 200 que **no** contienen los datos esperados.

## Preguntas de autoevaluación

1. ¿Cómo demuestras de forma **inequívoca** un LFI a través de un PDF?
2. ¿Qué pasa si LaTeX no tiene habilitado `shell-escape`?
3. ¿Por qué comparar el PDF byte a byte ayuda a distinguir "literal" de "incluido"?

## Técnicas que necesitan más práctica

- [[LaTeX-Injection]]
- [[LFI]]
- Verificación de explotación sin autoengaño
