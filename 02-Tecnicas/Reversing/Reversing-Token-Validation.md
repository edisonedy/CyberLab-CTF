---
tags: [tecnica, reversing, suid, objdump, xor]
tipo: tecnica
categoria: reversing
estado: practicado
maquinas: [NovaVault, Pila]
---

# Reversing de validación de tokens (SUID)

## 1. Qué es

Análisis de un binario (a menudo SUID) para entender cómo valida un **token/credencial** y construir uno válido.

## 2. Cómo funciona

- La validación suele ser una comparación tras una transformación simple (p.ej. **XOR** con una constante).
- Desensamblando la función de verificación se deduce el algoritmo.

## 3. Cuándo sospecharla

- Binarios **SUID** propios (`vault-*`, `*-guard`).
- Strings que insinúan un «token»/«recovery».

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
find / -perm -4000 -type f 2>/dev/null
file /usr/local/bin/vault-guard
strings -a /usr/local/bin/vault-guard | less
objdump -d -M intel /usr/local/bin/vault-guard | sed -n '/verify_recovery_token/,/ret/p'
```

## 5. Herramientas

- `file`, `strings`, [[Objdump]], Ghidra/radare2 (referencia).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[NovaVault]] | `verify_recovery_token` valida tras **XOR 0x5A** | token válido → root |
| [[Pila]] | ANÁLISIS DE `vulnserver.c` (BOF) | shellcode → admin |

## 7. Falsos positivos / pruebas fallidas

- Una shell root **dentro** del binario no siempre deja shell persistente: distinguir ejecución puntual de shell.

## 8. Prevención y mitigación

- Sustituir XOR por **HMAC** con clave; eliminar SUID si no es imprescindible.

## 9. Técnicas relacionadas

- [[SUID]], [[XOR]], [[Buffer-Overflow]]
