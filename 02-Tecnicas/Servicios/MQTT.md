---
tags: [tecnica, servicios, mqtt]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [El Baifo]
---

# MQTT

## 1. Qué es

Protocolo de mensajería ligero (broker + publicación/suscripción) usado en IoT/telemetría. Puerto típico `1883` (y `8883` TLS).

## 2. Cómo funciona

- Los clientes se **suscriben** a temas (`topic`) y reciben mensajes publicados.
- Sin autenticación ni ACLs, cualquiera puede suscribirse a `#` (todos los temas).

## 3. Cuándo sospecharla

- Puerto `1883`/`8883` abierto.
- Broker que acepta conexión **anónima**.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
mosquitto_sub -h <host> -t '#' -v
mosquitto_pub -h <host> -t 'test' -m 'hola'   # probar escritura si aplica
```

## 5. Herramientas

- [[Mosquitto-Clients]], `mqtt-pwn` (referencia).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | MQTT anónimo; telemetría filtra el usuario `pastor01` | usuario identificado |

## 7. Falsos positivos / pruebas fallidas

- Un **nombre de usuario** visto en telemetría **no** es una credencial: no implica acceso.

## 8. Prevención y mitigación

- `allow_anonymous false`, autenticación y **ACLs por tema**; TLS.

## 9. Técnicas relacionadas

- [[Redis-Sin-Autenticacion]], [[SSH-Keys]]
