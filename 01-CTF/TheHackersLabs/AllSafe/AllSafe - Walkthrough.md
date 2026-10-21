---
tags: [ctf, thehackerslabs, linux, walkthrough, latex, en-progreso]
tipo: walkthrough
estado: en-progreso
---

# AllSafe — Walkthrough (parcial)

> [!warning] Incompleto
> Se documenta el trabajo real. **No** se presentan como éxito pruebas que no lo fueron.

## Paso 1 — Enumeración web y vhosts

**Comando**
```bash
nmap -p- --open -sS --min-rate 5000 -n -Pn 192.168.56.159
echo "192.168.56.159 allsafe.thl intranet.allsafe.thl" | sudo tee -a /etc/hosts
gobuster dir -u http://intranet.allsafe.thl/ -w /usr/share/wordlists/dirb/common.txt
```

**Interpretación / por qué**
- Se descubre una **intranet** en un segundo vhost con funcionalidad propia (contratos).

## Paso 2 — Generación de contratos PDF

**Comando**
```bash
# Enviar un formulario de contrato y descargar el PDF generado
curl -s -X POST http://intranet.allsafe.thl/compile.php \
  -d 'campo=<valor>' -o contrato.pdf
file contrato.pdf
```

**Interpretación / por qué**
- `compile.php` genera PDFs a partir de la entrada: candidato a **inclusión/ejecución** si la entrada llega a LaTeX.

## Paso 3 — Pruebas LaTeX (inconcluyentes)

**Payload enviado (ejemplo)**
```latex
\lstinputlisting{/etc/passwd}
```

**Respuesta obtenida**
```text
HTTP 200 + contrato.pdf generado
```

**Resultado real**
- 🔴 El contenido apareció **como texto literal** (no se confirmó la lectura del fichero) y otras pruebas fallaron.

**Interpretación / por qué**
- Un **HTTP 200 no es explotación**. Sin ver el **contenido** real de un fichero objetivo en el PDF, la hipótesis queda **sin confirmar**.
- Próxima prueba justificada: variar **una sola** variable (ruta exacta, sintaxis de inclusión, permisos de lectura de LaTeX) y comparar el PDF byte a byte.

## Paso 4 — Intentos SSH

**Comando**
```bash
ssh <usuario>@192.168.56.159
```

**Resultado real**
- 🔴 **Sin acceso exitoso** en los registros consultados.

## Qué funcionó

- Descubrimiento de la **intranet** y del proceso de generación de PDFs.

## Qué falló (y por qué)

- Confirmar LFI/LaTeX: los PDFs se generaban pero el contenido no evidenciaba lectura de ficheros.

## Qué queda por aprender

- Ver [[AllSafe - Pendientes]].
