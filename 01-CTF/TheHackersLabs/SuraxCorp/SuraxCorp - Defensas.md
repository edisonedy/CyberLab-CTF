---
tags: [ctf, thehackerslabs, defensas]
tipo: defensas
---

# SuraxCorp — Defensas

## Causa raíz de las vulnerabilidades

| # | Vulnerabilidad | Causa raíz |
|---|---|---|
| 1 | Rsync expuesto | Módulo `backup` sin auth/ACL, accesible por IPv6 |
| 2 | Login con contraseña vacía | Política de credenciales inexistente |
| 3 | Command injection | Concatenación de entrada del usuario en comandos |
| 4 | SSTI | `Template(description)` con datos no confiables |
| 5 | `sudo telnet` | Concesión de un cliente interactivo con escape a shell |

## Impacto

- Fuga de configuración interna y RCE como `www-data` y `suraxddq`, y escalada a **root**.

## Prevención

- Rsync: `auth users`, `secrets file`, `hosts allow`, no exponer por IPv6.
- Política de contraseñas: prohibir vacías/débiles, MFA en paneles.
- Command injection: APIs sin shell (`subprocess` con lista de args, `shlex`), validación estricta.
- SSTI: nunca `Template()`/`render_template_string` con entrada del usuario; usar plantillas estáticas.
- `sudoers`: nunca conceder pagers ni clientes con `!`.

## Detección

- Consultas Rsync desde IPs no esperadas.
- Patrones `${IFS}`, `{{...}}`, `../` en logs de aplicación.
- Uso anómalo de `sudo telnet`/`sudo less`.

## Hardening recomendado

- [ ] Restringir Rsync a red interna + auth.
- [ ] Eliminar contraseñas vacías y forzar complejidad.
- [ ] Revisar sinks de plantillas y sanitizar entradas.
- [ ] `visudo` mínimo privilegio.
