---
tags: [tecnica, pivoting, wireguard, vpn]
tipo: tecnica
categoria: pivoting
estado: por-reforzar
maquinas: [THLCPPTV16]
---

# WireGuard (enrutado en pivoting)

## 1. Qué es

VPN ligera (UDP) usada en escenarios de pivoting para **enrutar** hacia redes internas.

## 2. Cómo funciona

- Interfaz `wg0` con `AllowedIPs` define qué destinos se enrutan por el túnel.
- Combinado con Ligolo-ng u otro pivote, da alcance a segmentos internos.

## 3. Cuándo sospecharla

- Necesidad de alcanzar varios hosts en una red interna (multi-host).
- Infra de laboratorio que expone un peer WireGuard.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
wg-quick up wg0
wg show
ip route
```

## 5. Herramientas

- `wg`, `wg-quick`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | Enrutado hacia redes del pivote | acceso a hosts internos |

## 7. Falsos positivos / pruebas fallidas

- `AllowedIPs` mal definido hace que el tráfico **no** se enrute por el túnel.

## 8. Prevención y mitigación

- Control de peers y claves; segmentación.

## 9. Técnicas relacionadas

- [[Pivoting-Ligolo]]
