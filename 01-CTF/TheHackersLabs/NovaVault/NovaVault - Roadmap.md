---
tags: [ctf, thehackerslabs, linux, roadmap, ssrf, reversing]
tipo: roadmap
estado: resuelto
ip: 192.168.56.13
fecha: 2026-10-20
---

# NovaVault — Roadmap

> [!abstract] Cadena de explotación
> Reconocimiento → **API Gateway Flask** → **SSRF** en `/api/v1/vault/inspect` → bypass de validación loopback con **`127.1`** → identificación de servicios internos → descubrimiento de **`/api/v1/docs`** → **SSRF POST** en `/api/v1/vault/sync` → despacho al servicio interno de tareas → **command execution** como `www-data` → grupo **`crypto-operators`** → lectura de **user flag** → análisis del binario **SUID `vault-guard`** → desensamblado de `verify_recovery_token` → decodificación **XOR `0x5A`** → mecanismo de recuperación del laboratorio → **root**.

## Diagrama

```mermaid
flowchart TD
    A[Recon] --> B[API Gateway Flask api.novavault.thl]
    B --> C[SSRF /api/v1/vault/inspect]
    C --> D[Bypass loopback 127.1]
    D --> E[Servicios internos]
    E --> F[/api/v1/docs]
    F --> G[SSRF POST /api/v1/vault/sync]
    G --> H[Command exec www-data]
    H --> I[grupo crypto-operators]
    I --> J[User flag]
    J --> K[SUID vault-guard]
    K --> L[objdump verify_recovery_token]
    L --> M[XOR 0x5A]
    M --> N[root]
```

## Etapas

### 1. Reconocimiento → API Gateway
- **Objetivo:** identificar superficie.
- **Evidencia:** API Flask en `api.novavault.thl`.

### 2. SSRF en `/api/v1/vault/inspect` + bypass `127.1`
- **Objetivo:** alcanzar servicios internos.
- **Evidencia:** el endpoint hace peticiones a URLs; la validación de loopback se evade con **`127.1`**.
- **Decisión:** probar representaciones alternativas de loopback cuando se bloquea `127.0.0.1`.

### 3. `/api/v1/docs` → SSRF POST `/api/v1/vault/sync`
- **Objetivo:** encontrar un sink con más poder.
- **Evidencia:** `/docs` revela `/api/v1/vault/sync`, que despacha al servicio interno de tareas.
- **Decisión:** encadenar **SSRF POST** (no solo GET).

### 4. Command execution → `www-data` → `crypto-operators` → user flag
- **Objetivo:** RCE y leer la flag.
- **Evidencia:** el servicio de tareas ejecuta comandos; el usuario `www-data` pertenece al grupo `crypto-operators`.
- **Decisión:** revisar **grupos** del usuario para acceder a la flag.

### 5. SUID `vault-guard` → reversing → XOR → root
- **Objetivo:** escalada.
- **Evidencia:** binario SUID `vault-guard` con `verify_recovery_token`; el token se valida tras **XOR 0x5A**.
- **Decisión:** `strings`/`file`/`objdump` para entender la validación y usar el mecanismo de recuperación del laboratorio.

## Kill chain resumida

```text
Flask API → SSRF /vault/inspect → 127.1 bypass → internal services → /docs
  → SSRF POST /vault/sync → command exec www-data → crypto-operators → USER
  → SUID vault-guard → objdump verify_recovery_token → XOR 0x5A → ROOT
```

## Enlaces

- [[NovaVault - Walkthrough]]
- [[NovaVault - Evidencias]]
- [[NovaVault - Lecciones]]
- [[NovaVault - Defensas]]
