---
tags: [ctf, thehackerslabs, linux, latex, lfi, en-progreso]
tipo: ficha
plataforma: TheHackersLabs
os: Linux
estado: en-progreso
ip: 192.168.56.159
fecha: 2026-10-20
user: null
root: null
tecnicas: [vhost, php, latex, lfi, contratos-pdf, ssh]
fuente: registros locales / sesión
ultima_revision: 2026-10-20
---

# AllSafe — Ficha

> [!warning] Estado NO confirmado
> Hay exploración web, intranet y generación de PDFs, pero **ni LFI ni ejecución LaTeX están confirmados**: obtener un PDF con HTTP 200 **no** es prueba de explotación. Ver [[AllSafe - Pendientes]].

## Identificación

| Campo | Valor |
|---|---|
| Nombre | AllSafe |
| Plataforma | TheHackersLabs |
| Sistema operativo | Linux |
| Estado | 🟡 En investigación |
| Fecha documentada | 2026-10-20 |
| IP | `192.168.56.159` |
| Virtual hosts | `allsafe.thl`, `intranet.allsafe.thl` |

## Puertos / Servicios

| Puerto | Servicio | Notas |
|---|---|---|
| 80 | HTTP | sitio `allsafe.thl` |
| 80 | HTTP (vhost) | intranet `intranet.allsafe.thl` |
| 22 | SSH | probado sin acceso exitoso en los registros consultados |

## Resultado

- **User:** ⬜ no confirmado
- **Root:** ⬜ no confirmado

## Trabajo realizado

- Navegación web e **intranet**; generación de **contratos PDF**.
- Se investigaron: documentos, claves SSH, formularios, cookies y el proceso `compile.php`.
- Se probaron entradas **LaTeX**, incluida `\lstinputlisting`.
- Se generaron PDFs correctamente, pero **algunas entradas aparecieron como texto literal** y otras pruebas fallaron.
- Intentos **SSH sin acceso exitoso** en los registros consultados.

## Notas relacionadas

- [[AllSafe - Roadmap]]
- [[AllSafe - Walkthrough]]
- [[AllSafe - Evidencias]]
- [[AllSafe - Lecciones]]
- [[AllSafe - Defensas]]
- [[AllSafe - Pendientes]]
