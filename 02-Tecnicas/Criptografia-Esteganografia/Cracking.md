---
tags: [tecnica, criptografia, cracking, hashcat, john]
tipo: tecnica
categoria: criptografia-esteganografia
estado: practicado
maquinas: [Folclore, Phantom, THLCPPTV16, Cold Fear]
---

# Crackeo de contraseñas (John / hashcat)

## 1. Qué es

Recuperación de contraseñas a partir de hashes o valores capturados (NetNTLMv2, phpass, ZIP), por diccionario o máscara.

## 2. Cómo funciona

- Se identifica el tipo de hash y se usa un **modo**/formato adecuado.
- Diccionario (rockyou) para passwords comunes; **máscara** cuando se conoce el patrón.

## 3. Cuándo sospecharla

- Capturas NetNTLMv2 (Responder), hashes de WordPress (phpass), ZIP protegidos, pistas de «patrón».

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# NetNTLMv2
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt hash.txt
# ZIP
zip2john archivo.zip > zip.hash && john --wordlist=rockyou.txt zip.hash
# wordlist por patron
hashcat -a 3 --stdout 'J9c?u?l?d?l?d%/' > wordlist.txt
```

## 5. Herramientas

- [[John-The-Ripper]], [[Hashcat]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Folclore]] | ZIP `Pacto.zip` (`celina`) + wordlist por patrón | credenciales |
| [[Phantom]] | NetNTLMv2 de `robert` (`bLink182`) | acceso WinRM |
| [[THLCPPTV16]] | hash **phpass** de Tom | acceso WordPress |
| [[Cold Fear]] | crackeo asociado a pistas | credenciales |

## 7. Falsos positivos / pruebas fallidas

- Fuerza bruta online en aplicaciones es lenta/ruidosa; preferir **offline**.
- Aplicar el formato incorrecto (p.ej. `-m 1000` NTLM en lugar de `5600` NetNTLMv2) no encuentra nada.

## 8. Prevención y mitigación

- Contraseñas largas/únicas, MFA, bloqueo por intentos; evitar NTLMv2 capturable (SMB signing, LLMNR off).

## 9. Técnicas relacionadas

- [[Responder]], [[Password-Spraying]]
