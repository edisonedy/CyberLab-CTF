---
tags: [tecnica, web, command-injection, rce]
tipo: tecnica
categoria: web
estado: practicado
maquinas: [SuraxCorp, NovaVault]
---

# Command Injection

## 1. Qué es

Ejecución de comandos del sistema a través de una entrada del usuario que se concatena en una llamada a shell.

## 2. Cómo funciona

- `os.system("ping " + user)`, `subprocess.call(cmd, shell=True)`, `exec()` en servicios internos.
- Se inyectan operadores (`;`, `|`, `&&`, `` ` ``, `$()`) para añadir comandos.

## 3. Cuándo sospecharla

- Funcionalidades que «ejecutan» algo (diagnóstico, ping, conversión, tareas).
- Parámetros `cmd=`, `host=`, `action=`, campos enviados a *jobs* internos.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# prueba mínima
curl -s 'http://host/?cmd=;id'
# bypass de filtros
curl -s 'http://host/?cmd=;cat${IFS}/etc/passwd'
curl -s 'http://host/?cmd=;/bin/ca?+/etc/passwd'   # wildcards
```

## 5. Herramientas

- `curl`, [[Burp Suite]], `nc` (reverse shell).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[SuraxCorp]] | Vhost `adminis-surapharm…` con filtros | reverse shell `www-data` |
| [[NovaVault]] | Servicio interno de tareas (vía SSRF POST) | command exec `www-data` |

## 7. Falsos positivos / pruebas fallidas

- Filtros que bloquean `;`, `|`, espacios → hubo que usar `${IFS}` y **wildcards** en SuraxCorp.

## 8. Prevención y mitigación

- No usar shell; usar APIs con lista de argumentos (`subprocess.run([...])`).
- Validación estricta con allow-list; `shlex.quote` si es inevitable.

## 9. Técnicas relacionadas

- [[SSTI-Jinja2]], [[SSRF]]
