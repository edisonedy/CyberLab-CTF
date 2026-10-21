---
tags: [tecnica, web, sqli, inyeccion]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [Securitron, THLCPPTV16]
---

# SQL Injection

## 1. Qué es

Inyección de código SQL en consultas construidas a partir de entrada del usuario, permitiendo alterar la lógica de la base de datos (autenticación, extracción de datos, etc.).

## 2. Cómo funciona

- La aplicación **concatena** la entrada en la consulta: `"SELECT * FROM users WHERE user='" + user + "'"`.
- Insertando `'`, `OR`, `-- -`, `UNION`, etc., se rompe/reescribe la consulta.

## 3. Cuándo sospecharla

- Formularios de login, filtros, búsquedas con parámetros (`id=`, `user=`).
- Errores SQL visibles o comportamientos distintos con `'`.
- Respuestas que cambian de forma booleana o **en tiempo**.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# boolean-based
curl -s -X POST http://host/Admin/login.php --data "user=admin' AND '1'='1&pass=x"
# time-based (ciega)
curl -s -X POST http://host/Admin/login.php --data "user=admin' AND SLEEP(3)-- -&pass=x" -w '%{time_total}\n'
# sqlmap
sqlmap -u "http://host/Admin/login.php" --data="user=a&pass=b" --technique=BT --dbs
```

## 5. Herramientas

- [[SQLMap]], [[Burp Suite]], `curl`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Securitron]] | SQLi en `Admin/login.php` (boolean/time-based) | login admin → RCE |
| [[THLCPPTV16]] | SQLi ciega time-based en plugin **My Calendar** | hash de Tom → john |

## 7. Falsos positivos / pruebas fallidas

- Un error SQL **no** siempre implica explotabilidad (puede ser un mensaje genérico).
- En Securitron no había diferencia **visible** en boolean-based: fue necesario **time-based**.

## 8. Prevención y mitigación

- **Consultas parametrizadas**/prepared statements.
- ORM con parámetros; validación de entrada.
- Mínimo privilegio en la cuenta de BD.

## 9. Técnicas relacionadas

- [[LFI]], [[Path-Traversal]], [[PHP-Filters]]
