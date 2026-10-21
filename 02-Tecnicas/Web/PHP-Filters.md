---
tags: [tecnica, web, php, wrappers]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [THLCPPTV16]
---

# PHP Filters / WrapWrap

## 1. Qué es

Uso de **wrappers** de PHP (`php://filter`, `php://input`, `data://`, …) para leer/transformar ficheros y, encadenándolos, evadir filtros de LFI.

## 2. Cómo funciona

- `php://filter/convert.base64-encode/resource=config.php` devuelve el fichero codificado (eludiendo ejecución/parseo).
- **WrapWrap** encadena wrappers para leer ficheros que, de otro modo, se incluyen como PHP.

## 3. Cuándo sospecharla

- Parámetro tipo `page=`, `include=`, `view=` en apps PHP.
- LFI que no muestra el código fuente (se «ejecuta»).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
curl -s 'http://host/index.php?page=php://filter/convert.base64-encode/resource=config.php' | base64 -d
# filtros encadenados (WrapWrap)
```

## 5. Herramientas

- `curl`, [[Burp Suite]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | Segundo vhost, PHP filters + WrapWrap | lectura de configuración → credenciales |

## 7. Falsos positivos / pruebas fallidas

- Un `include` que **ejecuta** el PHP no muestra el código: hay que usar `php://filter` para leerlo.

## 8. Prevención y mitigación

- Deshabilitar wrappers peligrosos (`allow_url_include=Off`).
- Validar rutas incluidas con **allow-list**.

## 9. Técnicas relacionadas

- [[Path-Traversal]], [[LFI]]
