---
tags: [ctf, thehackerslabs, linux, lecciones]
tipo: lecciones
---

# El Baifo — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Considerar un **nombre de usuario** (visto en MQTT) como si fuera una credencial | Expectativa de acceso que no existía | Un usuario filtrado ≠ acceso; hay que lograr una credencial/clave válida |
| Ir directo a la web sin probar antes los servicios de infraestructura | Tiempo perdido | MQTT/Redis sin auth son de bajo coste y alto valor: probarlos temprano |
| No anotar el estado de Redis (dir/dbfilename) antes de modificarlo | Difícil restaurar | Guardar siempre el estado original antes de mutar un servicio |

## Tiempo perdido y por qué

- En explorar la web cuando el camino estaba en MQTT/Redis.

## Conceptos aprendidos

- **MQTT anónimo**: `mosquitto_sub -t '#'` puede filtrar información.
- **Redis sin `AUTH`** → escritura arbitraria de archivos vía persistencia RDB.
- **Escritura de `authorized_keys`** como acceso directo por SSH.
- **Cron de root** con script escribible como escalada clásica.
- **SUID + `bash -p`** y verificación de `EUID`.

## Señales para reconocer en otra máquina

- [ ] Puerto `1883` (MQTT) o `6379` (Redis) abiertos.
- [ ] Broker MQTT que acepta conexiones anónimas.
- [ ] `redis-cli` que no pide `AUTH`.
- [ ] Scripts de cron en rutas escribibles por el usuario actual.
- [ ] Binarios SUID fuera de los habituales.

## Preguntas de autoevaluación

1. ¿Cómo distingues entre "credencial filtrada" y "acceso confirmado"?
2. ¿Qué tres comandos de Redis permiten escribir un archivo arbitrario?
3. ¿Qué revisas primero al encontrar un script de cron escribible?

## Técnicas que necesitan más práctica

- [[Redis-Sin-Autenticacion]]
- [[Cron-Privilege-Escalation]]
- [[SUID]]

## Limpieza y buenas prácticas

- Restaurar `CONFIG SET dir`/`dbfilename` y quitar la clave añadida.
- Usar **snapshots** de la VM antes de modificar servicios de persistencia.
