---
tags: [tecnica, reconocimiento, vhost, gobuster, ffuf]
tipo: tecnica
categoria: reconocimiento
estado: practicado
maquinas: [El Baifo, Securitron, Sulaco, THLCPPTV16]
---

# Virtual Hosts y fuzzing web

## 1. Qué es

Descubrimiento de **virtual hosts** (sitios servidos por nombre) y de rutas/ficheros ocultos mediante fuzzing.

## 2. Cómo funciona

- El servidor responde distinto según `Host:` → hay que fijar el vhost (`/etc/hosts`).
- Fuzzing de directorios/ficheros y de nombres de host.

## 3. Cuándo sospecharla

- Web que responde igual a cualquier IP pero distinto por nombre.
- Pistas de dominios en configs (`secure-*.thl`).

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
echo "<IP> vhost.thl" | sudo tee -a /etc/hosts
gobuster dir -u http://vhost.thl/ -w /usr/share/wordlists/dirb/common.txt
ffuf -u http://<IP>/ -H "Host: FUZZ.thl" -w subdomains.txt
ffuf -u http://vhost.thl/FUZZ -w /usr/share/seclists/Discovery/Web-Content/common.txt
```

## 5. Herramientas

- [[Gobuster]], [[ffuf]], [[Feroxbuster]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | VHost `elbaifo.thl` | contenido por nombre |
| [[Securitron]] | VHost `admin19-32.securitron.thl` | panel admin |
| [[Sulaco]] | Fuzzing + `robots.txt` | endpoints sensibles |
| [[THLCPPTV16]] | VHosts `examen`/`examendos` | segundo sitio vulnerable |

## 7. Falsos positivos / pruebas fallidas

- Fuzzing sin fijar el vhost devuelve resultados del sitio por defecto (irrelevantes).

## 8. Prevención y mitigación

- (Defensa) no exponer paneles internos; `robots.txt` sin rutas sensibles.

## 9. Técnicas relacionadas

- [[Nmap]], [[SQL-Injection]], [[Path-Traversal]]
