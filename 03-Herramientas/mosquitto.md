---
tags: [herramienta, servicios, mqtt]
tipo: herramienta
categoria: servicios
ultima_revision: 2026-10-20
---

# mosquitto_sub / mosquitto_pub

Clientes MQTT para suscribirse y publicar en un broker.

## Uso típico

```bash
mosquitto_sub -h <host> -t '#' -v
mosquitto_pub -h <host> -t 'test' -m 'hola'
```

## Notas / lecciones

- Un usuario visto en la telemetría **no** es una credencial válida.
- Comprobar si el broker acepta conexión anónima.

## Técnicas relacionadas

- [[MQTT]]
