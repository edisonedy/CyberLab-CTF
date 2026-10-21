---
tags: [ctf, thehackerslabs, linux, defensas]
tipo: defensas
---

# Sulaco — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Endpoints sensibles listados en `robots.txt` | Divulgación de rutas internas |
| 2 | Gating débil (UA + nonce predecible/duplicable) | Controles de seguridad insuficientes |
| 3 | Flag embebida en `admin_flag.php` | Secreto en código protegido solo por lógica |
| 4 | CSRF con secreto estático (`md5('sulaco-secret-'.$id)`) | Construcción criptográficamente débil |

## Impacto

- Divulgación de flags y de la lógica de protección.

## Prevención

- No listar rutas sensibles en `robots.txt`.
- No basar el control de acceso en `User-Agent` o nonces duplicables.
- No embeber secretos en el código; usar almacén seguro.
- Sustituir MD5 de secreto+id por **HMAC** con clave y comparación en tiempo constante.

## Detección

- Peticiones con UA de navegador falso a APIs internas.
- Accesos a `/api/admin_flag.php` fuera de la app.

## Hardening recomendado

- [ ] Retirar rutas sensibles de `robots.txt`.
- [ ] Control de acceso por sesión autenticada real.
- [ ] HMAC en lugar de MD5 para CSRF.
