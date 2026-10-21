---
tags: [tecnica, web, lfi, path-traversal]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [Securitron, BabyCorp, AllSafe]
---

# Path Traversal / LFI

## 1. Qué es

Acceso a ficheros fuera del directorio previsto, manipulando rutas (`../`) o parámetros que el servidor usa para abrir/leer ficheros.

## 2. Cómo funciona

- El código usa entrada del usuario para construir una ruta: `open(filename)`, `include($_GET['page'])`, `readFile(params.fileName)`.
- Sin **normalización** ni **allow-list**, se puede escapar del directorio base.

## 3. Cuándo sospecharla

- Parámetros `file=`, `page=`, `doc=`, `path=`, `fileName`.
- Endpoints tipo *load-file*, *download*, *view*.
- Rutas que devuelven contenido de ficheros.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
curl -s "http://host/api/load-file" --data-urlencode "file=/etc/passwd"
# variantes de codificación
curl -s --path-as-is "http://host/api/models/..%2f..%2findex.js"
# wrappers PHP
curl -s "http://host/index.php?page=php://filter/convert.base64-encode/resource=config.php" | base64 -d
```

## 5. Herramientas

- `curl --path-as-is`, [[Burp Suite]], [[ffuf]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[BabyCorp]] | `POST /api/load-file` en Flask **como root** | lectura arbitraria root → flags |
| [[Securitron]] | Path traversal en `/api/models/:fileName` (Node) | lectura de `index.js` |
| [[AllSafe]] | Posible LFI vía LaTeX (`\lstinputlisting`) | ❌ **NO confirmado** (solo HTTP 200) |

## 7. Falsos positivos / pruebas fallidas

- **AllSafe**: un PDF con HTTP 200 **no** demuestra LFI; el contenido salió como **texto literal**.
- En Node/Express, `curl` normaliza `../` salvo que uses `--path-as-is`.

## 8. Prevención y mitigación

- Normalizar y validar rutas; **allow-list** de directorios/ficheros.
- Servir ficheros desde almacenamiento controlado (no rutas del usuario).
- Autenticación en endpoints de lectura.

## 9. Técnicas relacionadas

- [[PHP-Filters]], [[SSTI-Jinja2]], [[SQL-Injection]]
