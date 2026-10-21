---
tags: [tecnica, web, upload, php]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [Securitron]
---

# File Upload

## 1. Qué es

Subida de ficheros que permite introducir código ejecutable en el servidor (p.ej. PHP/ASP/JSP) por validaciones débiles.

## 2. Cómo funciona

- Se sube un fichero con extensión/contenido ejecutable.
- La validación se hace por **extensión** o **Content-Type** (cliente), no por contenido real.

## 3. Cuándo sospecharla

- Formularios de **avatar**, documentos, imágenes.
- Carpetas de subida servidas por Apache/Nginx con un intérprete activo.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
curl -s -X POST http://host/upload.php -F "avatar=@shell.php;type=image/png"
curl -s "http://host/uploads/shell.php?c=id"
# variantes de evasión: .php5, .phtml, mayúsculas, doble extensión, magic bytes
```

## 5. Herramientas

- `curl`, [[Burp Suite]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Securitron]] | Subida de avatar con PHP y `Content-Type: image/png` | RCE `www-data` |

## 7. Falsos positivos / pruebas fallidas

- Extensiones/MIME bloqueados requirieron variantes (`.php5`, `.phtml`, cambios de mayúsculas).

## 8. Prevención y mitigación

- Validar **contenido real** (magic bytes), renombrar ficheros.
- Almacenar fuera del webroot y **sin** ejecución.
- Servir desde dominio/almacenamiento separado.

## 9. Técnicas relacionadas

- [[Path-Traversal]], [[Command-Injection]]
