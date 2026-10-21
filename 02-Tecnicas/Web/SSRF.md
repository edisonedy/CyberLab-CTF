---
tags: [tecnica, web, ssrf]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [NovaVault]
---

# SSRF

## 1. Qué es

Server-Side Request Forgery: el servidor realiza peticiones a un destino **controlado por el atacante**, permitiendo alcanzar servicios internos.

## 2. Cómo funciona

- Un endpoint acepta una **URL** y hace una petición (`inspect`, `fetch`, `sync`).
- Si la validación es débil (lista negra), se accede a `127.0.0.1`, `169.254.169.254`, redes internas, etc.

## 3. Cuándo sospecharla

- Parámetros `url=`, `target=`, `host=`, `webhook=`, `callback=`.
- Funciones de «inspección», «vista previa», «sincronización» o integraciones.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# destino normal (bloqueado si es loopback)
curl -s -X POST http://api.novavault.thl/api/v1/vault/inspect \
  -H 'Content-Type: application/json' -d '{"url":"http://127.0.0.1:5000/"}'
# bypass de validación de loopback
curl -s -X POST http://api.novavault.thl/api/v1/vault/inspect \
  -H 'Content-Type: application/json' -d '{"url":"http://127.1:5000/"}'
```

## 5. Herramientas

- `curl`, [[Burp Suite]] (Collaborator/repeater).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[NovaVault]] | `/api/v1/vault/inspect` + bypass `127.1`; `/vault/sync` (POST) | access a servicios internos → command exec |

## 7. Falsos positivos / pruebas fallidas

- `127.0.0.1` bloqueado por **lista negra** → se evadió con `127.1` (otra representación de loopback).
- El **quoting** de JSON mal formado provoca errores que parecen bloqueos.

## 8. Prevención y mitigación

- **Allow-list** de destinos; resolver y validar la **IP final** (todas las representaciones).
- No seguir redirecciones; bloquear metadatos cloud.
- Segmentación y autenticación de servicios internos.

## 9. Técnicas relacionadas

- [[Command-Injection]], [[Path-Traversal]]
