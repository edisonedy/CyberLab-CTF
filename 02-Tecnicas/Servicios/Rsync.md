---
tags: [tecnica, servicios, rsync, ipv6]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [SuraxCorp]
---

# Rsync (IPv6)

## 1. Qué es

Servicio de sincronización de ficheros. Puerto `873`. Puede escuchar **solo en IPv6**, pasando desapercibido en escaneos IPv4.

## 2. Cómo funciona

- Módulos publicados permiten `--list-only` y copia de ficheros.
- Módulos sin auth exponen backups/configuraciones.

## 3. Cuándo sospecharla

- Puerto `873` (revisar **IPv6**).
- Servidores con IPv6 `-6` no escaneados.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
nmap -6 -p- --open -sS <IPv6>
rsync -av --list-only rsync://[<IPv6>]:873/
rsync -av rsync://[<IPv6>]:873/backup/ ./backup/
```

## 5. Herramientas

- `rsync`, [[Nmap]] (`-6`).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[SuraxCorp]] | Módulo `backup` por IPv6 con config de vhost | pista de vhost |

## 7. Falsos positivos / pruebas fallidas

- Escanear solo IPv4 hace que Rsync **no aparezca**.

## 8. Prevención y mitigación

- `auth users` + `secrets file`, `hosts allow`, no exponer por IPv6.

## 9. Técnicas relacionadas

- [[Analisis-Paquetes-Deb]], [[Reconocimiento]] (IPv6)
