---
tags: [tecnica, explotacion, latex]
tipo: tecnica
categoria: explotacion
estado: sin-evidencia
maquinas: [AllSafe]
---

# LaTeX Injection

## 1. Qué es

Inyección de LaTeX cuando la entrada del usuario se compila en un documento, permitiendo **leer ficheros** (`\input`, `\lstinputlisting`) o **ejecutar comandos** (`\write18` con shell-escape).

## 2. Cómo funciona

- El backend compila LaTeX con la entrada del usuario.
- Directivas de inclusión leen ficheros; con `shell-escape` habilitado, `\write18` ejecuta comandos.

## 3. Cuándo sospecharla

- Formularios que **generan PDFs** (contratos, facturas).

## 4. Cómo comprobarla (laboratorio autorizado)

```latex
\lstinputlisting{/etc/passwd}
\input{/etc/passwd}
```

## 5. Herramientas

- `curl`, [[Burp Suite]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[AllSafe]] | Entradas LaTeX en `compile.php` | ❌ **NO confirmado** (PDF con texto literal) |

## 7. Falsos positivos / pruebas fallidas

> [!warning] Lección clave
> **HTTP 200 + PDF generado ≠ explotación.** En AllSafe el contenido apareció como **texto literal**; no hubo evidencia de lectura de ficheros. Sin confirmar el **motor** ni `shell-escape`, la hipótesis queda abierta.

## 8. Prevención y mitigación

- Sanitizar entradas; deshabilitar `shell-escape`; compilar en sandbox sin acceso a ficheros sensibles.

## 9. Técnicas relacionadas

- [[Path-Traversal]], [[LFI]]
