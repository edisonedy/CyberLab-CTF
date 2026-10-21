---
tags: [ctf, thehackerslabs, lecciones]
tipo: lecciones
---

# SuraxCorp — Lecciones aprendidas

## Error / acierto central

> **Seguir la pista más fuerte y cambiar una sola variable por prueba.**

Cambiar varias cosas a la vez impide saber qué causó el resultado. En SuraxCorp, mantener el resto fijo permitió aislar el `sink` de la SSTI.

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| No escanear IPv6 inicialmente | Rsync quedó oculto | Escanear siempre IPv4 **e** IPv6 |
| Tratar el `.deb` como caja negra | Se pierde el vhost alternativo | Descomprimir e inspeccionar scripts/binarios |
| Probar SSTI sin confirmar el sink | Tiempo perdido | Buscar la función vulnerable (`Template(description)`) |

## Conceptos aprendidos

- [[Rsync]] (IPv6).
- [[Analisis-Paquetes-Deb]].
- [[Command-Injection]] con `${IFS}`/wildcards.
- [[SSTI-Jinja2]] y tracebacks de Werkzeug.
- Enumeración de servicios en **localhost**.
- [[Sudo-Telnet]].

## Señales para reconocer en otra máquina

- [ ] Puerto `873` (Rsync) abierto (revisar IPv6).
- [ ] Paneles con contraseña vacía o por defecto.
- [ ] Paquetes `.deb`/`.rpm` descargables.
- [ ] Servicios Flask/Werkzeug con parámetros que entran a plantillas.
- [ ] `sudo` sobre clientes interactivos (`telnet`, `ftp`, `less`).

## Preguntas de autoevaluación

1. ¿Cómo confirmas que un parámetro entra a `Template()`/`render_template_string`?
2. ¿Qué tres bypasses de filtros usaste en la command injection?
3. ¿Por qué enumerar `127.0.0.1` cuando ya tienes shell de `www-data`?

## Técnicas que necesitan más práctica

- [[SSTI-Jinja2]]
- [[Command-Injection]]
