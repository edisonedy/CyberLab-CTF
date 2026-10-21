---
tags: [ctf, thehackerslabs, windows, lecciones, iis, php, godpotato]
tipo: lecciones
---

# Welcome To The Jungle — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Construir el exploit de **DLL hijack** (`config.dll`) antes de buscar el *trigger* | Se perdió tiempo compilando y subiendo una DLL que nunca iba a ejecutarse | **Localizar primero qué ejecuta el binario**; si el log no se actualiza ni tras reiniciar, es señuelo |
| Confiar en `schtasks /query` **como usuario no privilegiado** | Falsa conclusión de que "no hay tareas" | Repetir la enumeración con el token del **app pool de IIS**: 159 tareas vs. 0 visibles |
| Buscar el disparador solo en `HKLM\...\Services` | Ningún servicio con `ImagePath` no estándar | Enumerar **servicios** con `Get-CimInstance Win32_Service` (ruta real) y no solo el registro |
| `PrintSpoofer` como primera opción | Se colgó sin generar salida y dejó procesos huérfanos | Usar `GodPotato -cmd "..."` (sintaxis obligatoria); matar procesos colgados antes de borrar binarios |
| `GodPotato` sin `-cmd` | Solo imprimió la ayuda y no escaló | Sintaxis real: `GodPotato -cmd "cmd /c whoami"` |
| `2>nul` en `cmd` a través de `evil-winrm` | Error `FileStream was asked to open a device that is not a file` | `2>&1` o redirección de PowerShell (`2>$null`) |
| `Invoke-WebRequest` con `Invoke-RestMethod` mal formado para exfiltrar | Salida vacía | `Invoke-WebRequest -Method POST -Body <bytes>` contra un listener local |

## Tiempo perdido y por qué

- **Contraseña expirada de `axl`**: el reloj del sistema estaba en 2026 → la contraseña de 2025 figuraba expirada. Los intentos de ajustar `biossystemtimeoffset` y `paravirt none` no sirvieron; hubo que ir a la **consola de VirtualBox** (VNC + `keyboardputscancode` + OCR) para cambiar la contraseña por el diálogo de Windows.
- **Búsqueda del trigger**: se sondearon servicios, tareas, claves `Run`, `Winlogon`, `AppInit_DLLs`, `IFEO`, suscripciones WMI, `BootExecute`, `GroupPolicy\Scripts`, `Startup` y el propio sistema de ficheros, sin encontrar disparador para `setlist_uploader.exe`.
- Descubrir que el **app pool de IIS** tenía permisos de enumeración distintos a `axl` (tareas, servicios) y que su salida había que exfiltrarla por un canal externo.

## Conceptos aprendidos

- **Webshell PHP con salida oculta**: `shell_exec($_POST['cmd'])` sin imprimir el resultado → hay que **exfiltrar la salida** (callback HTTP) para poder interpretar el entorno.
- **`IIS AppPool\DefaultAppPool`** tiene `SeImpersonatePrivilege` + `SeAssignPrimaryTokenPrivilege` + `SeCreateGlobalPrivilege` → vector directo a **SYSTEM** con la familia *Potato* (`GodPotato`, `PrintSpoofer`, `JuicyPotato`).
- **Un token de servicio ve más que un usuario de escritorio**: el app pool pudo listar 159 tareas y 69 servicios que a `axl` le negaban (`Access denied` en SCM, WMI y `TaskCache`).
- **Señuelos de DLL hijack**: un binario que carga `config.dll` desde una carpeta escribible **no es explotable sin un trigger**. La prueba decisive es el `LastWriteTime` de su log tras un reinicio.
- **Desfase de reloj**: en estos labs el reloj puede estar muy adelantado y caducar credenciales legítimas; la consola local es el recurso para resetear la contraseña.
- `stegseek` con passphrase deducida del contexto (nombre de la banda) como atajo frente aldictionary attacks.

## Señales para reconocer en otra máquina

- [ ] Aplicación web con `panel.php`/`cmd.php` que use `shell_exec`, `system`, `passthru` o `popen` → leer el **código fuente**, no solo el HTML renderizado.
- [ ] `whoami /priv` con `SeImpersonatePrivilege` habilitado → familia *Potato* viable.
- [ ] Servicios `Spooler` y `RpcSs` en ejecución → precondición de `GodPotato`/`PrintSpoofer`.
- [ ] Carpeta de `Program Files` con ACL `(OI)(CI)(W)` para `BUILTIN\Usuarios` + binario que carga DLLs → **verificar siempre el trigger antes de exploitear**.
- [ ] Credenciales de Windows reveladas en `strings` de un binario descargado de la web.
- [ ] Contraseña que falla con `STATUS_PASSWORD_EXPIRED` → problema de **reloj**, no de credencial.

## Preguntas de autoevaluación

1. ¿Cómo confirmas que un DLL hijack es explotable antes de escribir la DLL?
2. ¿Por qué un usuario de servicio puede enumerar más que un usuario de escritorio?
3. ¿Cómo exfiltras la salida de un `shell_exec` que no se imprime?
4. ¿Qué hacer si la contraseña de una cuenta de laboratorio aparece expirada?

## Técnicas que necesitan más práctica

- Familia *Potato* en Windows Server 2019/2022 (`GodPotato` vs `PrintSpoofer` vs `SweetPotato`) y sus diferencias de sintaxis.
- Automatización de la consola gráfica de VirtualBox (`keyboardputscancode` + OCR) para resolver dialogues de Windows.
- Escritura de **DLLs de hijack** en C con mingw (`LoadLibrary` vs link explícito) — aunque no fue necesario aquí.
- Análisis de PE más profundo (imports, secciones, `objdump -p`) para validar qué DLL carga un ejecutable.
