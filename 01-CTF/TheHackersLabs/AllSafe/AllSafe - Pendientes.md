---
tags: [ctf, thehackerslabs, linux, pendientes, en-progreso]
tipo: pendientes
estado: en-progreso
ip: 192.168.56.159
---

# AllSafe — Pendientes

> [!important] Solo hechos confirmados
> Estado real: dos vhosts y una funcionalidad de contratos PDF. **LFI y ejecución LaTeX NO están confirmados.**

## Último punto confirmado

- Vhosts `allsafe.thl` e `intranet.allsafe.thl` activos.
- Intranet con generación de **contratos PDF** (`compile.php`).
- Pruebas LaTeX generan PDFs, pero el contenido apareció **como texto literal**.
- SSH: **sin acceso exitoso** en los registros.

## Hipótesis priorizadas

| # | Hipótesis | Fundamento | Comando de comprobación (no destructivo) |
|---|---|---|---|
| 1 | El backend usa LaTeX con `shell-escape` deshabilitado | Los payloads salen como texto literal | Enviar `\input{/etc/passwd}` y **comparar el PDF** con el de control |
| 2 | LFI por otra ruta (no LaTeX), p.ej. descarga de documentos | Existen documentos/claves SSH revisados | Revisar endpoints de descarga y parámetros tipo `file=`/`doc=` |
| 3 | Reutilización de credenciales de la intranet para SSH | Se revisaron claves SSH y formularios | Revisar configs/`.bash_history` si se obtiene acceso |
| 4 | Vulnerabilidad en la subida/manejo de documentos | Hay formularios | Probar nombres de fichero con traversal controlado |

## Próximo punto de reanudación

1. Enumerar endpoints de `compile.php` y de descarga de documentos.
2. Confirmar el **motor** de PDF (LaTeX u otro) y si hay `shell-escape`.
3. Repetir **una** prueba LaTeX, **comparando el PDF** con la versión de control.
4. Si no hay confirmación, pivotar a LFI por descarga de documentos.

## Hipótesis descartadas (documentadas)

| Hipótesis | Por qué se descartó |
|---|---|
| LFI confirmado vía `\lstinputlisting` | El contenido salió como texto literal; no hubo evidencia de lectura |
| Acceso SSH durante la sesión | Sin acceso exitoso en los registros |
