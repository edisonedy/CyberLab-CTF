---
tags: [tecnica, reversing, xor, criptografia]
tipo: tecnica
categoria: reversing
estado: practicado
maquinas: [NovaVault]
---

# XOR

## 1. Qué es

Operación lógica `⊕` usada (mal) como «cifrado» simple. Repetida con una constante o clave, es reversible.

## 2. Cómo funciona

- `cifrado = claro ⊕ clave`; `claro = cifrado ⊕ clave`.

## 3. Cuándo sospecharla

- Byte «raro» comparado en el reversing; cadenas con XOR evidente.
- Blobs que no son Base64/hex pero parecen ofuscados.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# XOR con 0x5A (ejemplo NovaVault)
python3 - <<'PY'
data = bytes.fromhex('...')
print(bytes(b ^ 0x5A for b in data))
PY
```

## 5. Herramientas

- Python, CyberChef (referencia).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[NovaVault]] | Token de recuperación con **XOR `0x5A`** | token válido → root |

## 7. Falsos positivos / pruebas fallidas

- Asumir XOR cuando en realidad es otra transformación (sumas, ROT) da basura.

## 8. Prevención y mitigación

- No usar XOR como seguridad; emplear criptografía autenticada (HMAC/AES-GCM).

## 9. Técnicas relacionadas

- [[Reversing-Token-Validation]], [[Codificaciones]]
