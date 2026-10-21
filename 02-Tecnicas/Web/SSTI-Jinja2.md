---
tags: [tecnica, web, ssti, jinja2, flask]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [SuraxCorp]
---

# SSTI (Jinja2)

## 1. Qué es

Server-Side Template Injection: la entrada del usuario se **evalúa como plantilla** en el servidor, permitiendo ejecutar expresiones y, finalmente, código.

## 2. Cómo funciona

- El backend usa `Template(user_input)` o `render_template_string(user_input)` en vez de plantillas estáticas.
- `{{7*7}}` se evalúa a `49` → confirma la inyección.

## 3. Cuándo sospecharla

- Campos que se «renderizan» (nombres, descripciones, correos).
- Aplicaciones **Flask/Jinja2**; tracebacks de **Werkzeug**.
- Respuestas que reflejan expresiones.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
curl -s -X POST http://127.0.0.1:5000/event --data 'description={{7*7}}'
# -> 49 confirma SSTI; escalar a RCE Jinja2 (p.ej. via config/globals)
```

## 5. Herramientas

- [[Burp Suite]], [[tplmap]] (referencia), `curl`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[SuraxCorp]] | `/event` procesa `description` con `Template(description)` | RCE como `suraxddq` |

## 7. Falsos positivos / pruebas fallidas

- Probar SSTI **sin confirmar el sink**: en SuraxCorp hubo que localizar `Template(description)` y habilitar la ruta vía `/surax`.

## 8. Prevención y mitigación

- **Nunca** pasar entrada del usuario a `Template()`/`render_template_string`.
- Usar plantillas estáticas con variables escapadas; sandbox si es imprescindible.

## 9. Técnicas relacionadas

- [[Command-Injection]], [[LFI]], [[PHP-Filters]]
