---
tags: [ctf, thehackerslabs, linux, evidencias, en-progreso]
tipo: evidencias
estado: en-progreso
---

# AllSafe — Evidencias

> [!warning] Regla de oro
> **No** marcar LFI o ejecución LaTeX como confirmados solo por obtener un PDF con HTTP 200. Distinguir evidencia original, reconstrucción e inferencia.

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| — | — | (no recuperadas en esta importación) | — | sesión original |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Dos vhosts: `allsafe.thl`, `intranet.allsafe.thl` | registros |
| 2 | Funcionalidad de contratos PDF + `compile.php` | registros |
| 3 | Prueba de entradas LaTeX (`\lstinputlisting`) | registros |
| 4 | Intentos SSH sin éxito | registros |

## Registro de pruebas (hipótesis → payload → respuesta → resultado)

| # | Hipótesis | Payload | Respuesta | Resultado real |
|---|---|---|---|---|
| 1 | Inclusión de fichero vía LaTeX | `\lstinputlisting{/etc/passwd}` | HTTP 200 + PDF | ❌ texto literal, no confirmado |
| 2 | Ejecución LaTeX (`\write18`) | (variante) | fallo | ❌ no confirmado |

## Inferencias (no verificadas)

- Que el backend sea LaTeX y no otro motor de PDF: **inferido**, no probado.
- Que `compile.php` sea el sink real de la entrada: **no confirmado**.

## Comandos clave reproducibles

```bash
curl -s -X POST http://intranet.allsafe.thl/compile.php -d 'campo=<valor>' -o contrato.pdf
strings contrato.pdf | head
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `contrato.pdf` | local | comparar byte a byte entre pruebas |
