---
tags: [ctf, thehackerslabs, defensas]
tipo: defensas
---

# THLCPPTV16 — Defensas

## Causa raíz

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | My Calendar SQLi | Plugin sin actualizar / consulta sin parametrizar |
| 2 | PHP filters/WrapWrap | LFI con wrappers habilitados |
| 3 | Reutilización de credenciales | Misma clave en varios hosts/servicios |
| 4 | `sudo vim` | Editor con escape a shell |
| 5 | Segmentación débil | Redes internas alcanzables tras un pivote |
| 6 | APT Pre-Invoke | Config de APT escribible por el usuario |
| 7 | `sudo nginx` | Servidor con config cargable arbitraria |

## Impacto

- Compromiso de **varios hosts** en cadena y root final.

## Prevención

- Mantener WordPress/plugins al día; WAF; parametrizar consultas.
- Deshabilitar wrappers peligrosos; validar rutas incluidas.
- **No** reutilizar credenciales; gestor de secretos.
- `sudoers`: sin editores/pagers.
- Microsegmentación + monitoreo de movimientos laterales.
- Config de APT y de nginx propiedad de root, sin escritura para otros.

## Detección

- Consultas con `SLEEP`/retardos anómalos.
- Uso de `php://filter` en logs web.
- Autenticaciones SSH entre hosts internos inesperadas.
- `apt update` disparado por procesos no habituales.
- `nginx -c` con rutas en `/tmp`.

## Hardening recomendado

- [ ] Parcheo de plugins y SQL parametrizado.
- [ ] Rotación de credenciales por host/servicio.
- [ ] `visudo` mínimo privilegio.
- [ ] Segmentación de red y alertas EDR en pivotes.
