---
tags: [tecnica, servicios, ssh, claves]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [El Baifo, THLCPPTV16]
---

# SSH y gestión de claves

## 1. Qué es

Acceso a SSH con **claves** (pares público/privada) y movimiento lateral mediante **alias** en `~/.ssh/config`.

## 2. Cómo funciona

- La clave pública en `~/.ssh/authorized_keys` autoriza la privada correspondiente.
- Escribir una `authorized_keys` (p.ej. vía Redis/LFI) da acceso directo.

## 3. Cuándo sospecharla

- Sistemas con SSH donde se puede **escribir** en el home del objetivo.
- `~/.ssh/config` con **alias** hacia otros hosts (pivoting lateral).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
ssh-keygen -t ed25519 -f ./key -N ''
chmod 600 ./key
ssh -i ./key user@host
cat ~/.ssh/config        # buscar Host/alias hacia otros hosts
```

## 5. Herramientas

- `ssh`, `ssh-keygen`, `ssh-keyscan`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | Escritura de `authorized_keys` vía Redis | SSH como `pastor01` |
| [[THLCPPTV16]] | Alias SSH de Tom hacia Rafael | movimiento lateral |

## 7. Falsos positivos / pruebas fallidas

- Una clave privada **no** sirve si su pública no está autorizada.
- Permisos incorrectos (`chmod`) hacen que SSH rechace la clave.

## 8. Prevención y mitigación

- Restringir escritura en `~/.ssh`; `AuthorizedKeysFile` controlado.
- Rotación de claves; passphrase y agent forwarding con cuidado.

## 9. Técnicas relacionadas

- [[Redis-Sin-Autenticacion]], [[Pivoting-Ligolo]]
