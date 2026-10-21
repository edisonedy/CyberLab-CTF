---
tags: [ctf, thehackerslabs, windows, defensas, iis, php]
tipo: defensas
---

# Welcome To The Jungle — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | **Webshell PHP en producción** (`shell_exec($_POST['cmd'])`) | Código de ejecución de comandos embebido en una aplicación publicada; además, la salida no se filtra, lo que facilita el **abuso silencioso** |
| 2 | **Credenciales en código fuente** (`$user='slash'; $pass='sweetjungle2025';`) | Credenciales hardcodeadas en un fichero servible por HTTP |
| 3 | **Credenciales de Windows expuestas en un binario descargable** (`commlink.exe`) | Strings en claro dentro del ejecutable, entregado al cliente |
| 4 | **App pool de IIS con `SeImpersonatePrivilege`** | Token de servicio con privilegios de suplantación habilitados → escalada trivial a SYSTEM con la familia *Potato* |
| 5 | **Secreto en un canal no cifrado** (HTTP) | Credenciales de sesión y material de stego servidos en claro |
| 6 | **Permisos excesivos en `C:\Program Files\HexGuns`** (`BUILTIN\Usuarios:(OI)(CI)(W)`) | Ruta de programa escribible por usuarios → vector clásico de DLL hijack (aún sin exploitabilidad, es una debilidad latente) |

## Impacto

- Ejecución de comandos arbitrarios en el servidor como `IIS AppPool\DefaultAppPool`.
- **Escalada a `NT AUTHORITY\SYSTEM`** trivial → lectura de `C:\Users\Administrador\Desktop\root.txt` y control total del host.
- Exposición de credenciales de Windows de la cuenta `axl` (y de la web `slash`).

## Prevención

- **Eliminar toda llamada a `shell_exec`/`system`/`passthru`/`popen`/`proc_open` en código expuesto al cliente.** Si hace falta automatizar, usar una cola de trabajos con proceso de negocio definido.
- **Credenciales fuera del código**: gestor de secretos/vault, variables de entorno inyectadas en despliegue, y **rotación** de cualquier secreto que haya estado en un fichero served.
- **Compilar los binarios cliente con ofuscación de strings** o, mejor, no incluir credenciales de servidor en el cliente: el cliente debería autenticarse contra la API, nunca portar el secreto.
- **Endurecer el token del app pool**:
  - Revisar y desactivar privilegios innecesarios del grupo de aplicaciones (en particular `SeImpersonatePrivilege`).
  - Ejecutar los app pools con una **identidad de servicio dedicada** y de privilegio mínimo, nunca `LocalSystem`.
  - Limitar `loadUserProfile` y deshabilitar opciones no requeridas.
- **Permisos correctos en `Program Files`**: solo `SYSTEM`, `Administradores` y `TrustedInstaller` con `(F)`; `Users` solo `(RX)`. Auditar rutas de programa escribibles.
- **TLS en todo el sitio** y autenticación por **formulario con cookies `HttpOnly`/`Secure`/`SameSite`** en lugar de credenciales en claro.
- **Monitorizar** la creación de ficheros en `Program Files` y la ejecución de binarios desde rutas de servicio.

## Detección

- Alertar sobre peticiones HTTP que incluyan `cmd=`, `exec=`, o parámetros típicos de RCE hacia ficheros PHP.
- Alertar sobre **descarga y ejecución** de binarios desde `inetpub\wwwroot` (cadena `download → execute`).
- Vigilar procesos descendientes de `w3wp.exe` que no sean el propio worker de PHP (indicador de RCE vía webshell).
- Detectar el uso de la familia *Potato*: creations de named pipes inusuales por `w3wp.exe` y conexiones RPC a `spoolss`/`System`.
- Revisar periódicamente el `LastWriteTime` de logs de servicios para detectar ejecuciones no previstas (el método que reveló el señuelo de esta máquina).

## Hardening recomendado

- [ ] Barrido de los ficheros PHP publicados buscando `shell_exec|system|passthru|popen|proc_open|eval|assert|create_function`.
- [ ] Revisar la configuración de todos los app pools: identidad, `logonMethod`, privilegios del token y `loadUserProfile`.
- [ ] Corregir las ACL de todas las carpetas bajo `C:\Program Files*` (ninguna escribible por `Users`/`Authenticated Users`).
- [ ] Externalizar y rotar los secretos de `index.php` y de cualquier binario distribuido.
- [ ] Forzar HTTPS y cabeceras seguras en el sitio.
- [ ] Retirar del repo y de los clientes cualquier credencial de laboratorio expuesta.
