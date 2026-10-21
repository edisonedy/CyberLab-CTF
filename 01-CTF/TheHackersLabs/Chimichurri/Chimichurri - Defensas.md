---
tags: [ctf, thehackerslabs, windows, defensas, active-directory]
tipo: defensas
---

# Chimichurri — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Credenciales en ficheros | Contraseñas en texto claro (`perico.txt`) |
| 2 | Pistas de usuario accesibles | Configuración/higiene deficiente en el DC |

## Impacto

- Acceso como usuario y escalada a Administrador del dominio.

## Prevención

- Prohibir credenciales en ficheros de disco/compartidos.
- Aplicar mínimo privilegio y **LAPS** para cuentas administradoras.
- Gestor de secretos y rotación periódica.

## Detección

- Ficheros con contraseñas en texto claro (DLP/escaneo).
- Inicios de sesión con cuentas de servicio desde ubicaciones inusuales.

## Hardening recomendado

- [ ] Auditoría de ficheros con credenciales.
- [ ] Endurecer cuentas privilegiadas (LAPS, MFA).
- [ ] Monitorizar accesos anómalos al DC.
