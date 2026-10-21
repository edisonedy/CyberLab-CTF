---
tags: [tecnica, escalada-linux, grupos, permisos]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [NovaVault]
---

# Grupos Linux (acceso por grupo)

## 1. Qué es

Pertenencia a un **grupo** que otorga lectura/escritura sobre ficheros protegidos (p.ej. la flag), como control de acceso.

## 2. Cómo funciona

- El servicio corre con un usuario que pertenece a un grupo con permisos sobre rutas concretas.

## 3. Cuándo sospecharla

- Tras obtener RCE, revisar `id`/`groups` y permisos de ficheros «interesantes».

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
id; groups
ls -l /ruta/a/user.txt
find / -group <grupo> 2>/dev/null
```

## 5. Herramientas

- `id`, `find`, `ls`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[NovaVault]] | `www-data` ∈ `crypto-operators` | lectura de la **user flag** |

## 7. Falsos positivos / pruebas fallidas

- Ignorar los grupos del usuario hace que se pase por alto el acceso a la flag.

## 8. Prevención y mitigación

- Mínimo privilegio en grupos; no proteger secretos solo por grupo de un usuario de servicio.

## 9. Técnicas relacionadas

- [[SUID]], [[Linux-Enum]]
