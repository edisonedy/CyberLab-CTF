---
tags: [ctf, thehackerslabs, linux, defensas, en-progreso]
tipo: defensas
estado: en-progreso
---

# AllSafe — Defensas (preliminar)

> [!note] Preliminar
> Sin causa raíz confirmada (máquina sin resolver). Se listan buenas prácticas aplicables a generación documental.

## Causa raíz

- Pendiente de confirmar (sin explotación documentada).

## Impacto

- Pendiente de determinar. Riesgo potencial: lectura de ficheros o ejecución si la entrada llega a LaTeX con `shell-escape`.

## Prevención (recomendaciones generales)

- **Nunca** pasar entrada del usuario directamente a LaTeX/plantillas sin sanitizar.
- Deshabilitar `\write18`/`shell-escape` en el motor LaTeX.
- Renderizar en un **sandbox**/contenedor sin acceso al sistema de ficheros sensible.
- Limitar el acceso al binario LaTeX con permisos mínimos.

## Detección

- Patrones LaTeX sospechosos (`\input`, `\lstinputlisting`, `\write18`) en logs.
- Generación anómala de PDFs con rutas del sistema.

## Hardening recomendado

- [ ] Sanitizar entradas de formularios documentales.
- [ ] `shell-escape` deshabilitado.
- [ ] Sandbox de compilación sin secretos accesibles.
