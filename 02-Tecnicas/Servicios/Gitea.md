---
tags: [tecnica, servicios, gitea, fuga-codigo]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [BabyCorp]
---

# Gitea / Fuga de código fuente

## 1. Qué es

Gitea (servidor Git self-hosted, puerto típico `3000`) puede exponer repositorios **públicos** con el código del backend, revelando vulnerabilidades.

## 2. Cómo funciona

- La API `/api/v1/repos/search` lista repos sin autenticación.
- Clonar el repo da acceso al código (sinks, credenciales, rutas).

## 3. Cuándo sospecharla

- Puerto `3000` con `i_like_gitea` (cookie) o título Gitea.
- Repos visibles por API/explorador.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
curl -s "http://<host>:3000/api/v1/repos/search?limit=50"
git clone http://<host>:3000/<user>/<repo>.git
```

## 5. Herramientas

- `curl`, `git`, [[Gobuster]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[BabyCorp]] | Repo público `wvverez/BabyCorp-Web-Project` | `app.py` → endpoint vuln |

## 7. Falsos positivos / pruebas fallidas

- Un repo «de prácticas» puede contener código **no** desplegado.

## 8. Prevención y mitigación

- Repos privados; minimizar exposición; no incluir secretos en código.

## 9. Técnicas relacionadas

- [[Path-Traversal]], [[LFI]]
