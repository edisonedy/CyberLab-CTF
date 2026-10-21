---
tags: [herramienta, web, sqli]
tipo: herramienta
categoria: web
ultima_revision: 2026-10-20
---

# SQLMap

Automatiza la detección y explotación de inyecciones SQL.

## Uso típico

```bash
sqlmap -u "http://host/login.php" --data="user=a&pass=b" --technique=BT --dbs
sqlmap -u "http://host/item.php?id=1" --batch --dump
```

## Notas / lecciones

- En Securitron no había diferencia **visible** boolean-based: fue necesario **time-based**.
- Confirmar manualmente antes de automatizar.

## Técnicas relacionadas

- [[SQL-Injection]]
