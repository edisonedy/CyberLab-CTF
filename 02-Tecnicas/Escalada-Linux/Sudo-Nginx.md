---
tags: [tecnica, escalada-linux, sudo, nginx]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [THLCPPTV16]
---

# Sudo Nginx

## 1. Qué es

`nginx` con `sudo` permite cargar una **configuración propia**, abusando de directivas (`root`, `error_log`, `load_module`, SSL) para leer/escribir ficheros o ejecutar código como root.

## 2. Cómo funciona

- `sudo nginx -c /tmp/nginx.conf` carga una config controlada.
- Directivas como `error_log /root/.ssh/authorized_keys` o `root /` permiten escritura/lectura privilegiada.

## 3. Cuándo sospecharla

- `sudo -l` incluye `nginx` (con `-c`).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
sudo -l
# config propia con directivas abusables
sudo nginx -c /tmp/nginx.conf
```

## 5. Herramientas

- [[GTFOBins]], documentación de directivas nginx.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[THLCPPTV16]] | `sudo nginx` en el host final | root final |

## 7. Falsos positivos / pruebas fallidas

- Sin permiso de `-c` (o con config fija) no aplica.

## 8. Prevención y mitigación

- No conceder `nginx` con `sudo`; si es necesario, config fija y sin `-c` arbitrario.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[SUID]]
