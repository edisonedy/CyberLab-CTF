---
tags: [reporte, importacion, metadatos]
tipo: reporte
fecha: 2026-10-20
---

# 📦 Reporte de Importación — Base de conocimiento CTF

> [!info] Alcance
> Consolidación de todos los CTF, roadmaps y walkthroughs en el Vault de Obsidian **`/home/edison/CyberLab`** como base de conocimiento navegable. **No** se realizaron ataques ni escaneos nuevos: es una tarea de organización de documentación histórica.

## 1. Resumen ejecutivo

| Métrica | Valor |
|---|---|
| Notas Markdown en el Vault | **238** |
| Máquinas documentadas (TheHackersLabs) | **15** |
| Máquinas resueltas (evidencia propia) | **13** |
| Máquinas en progreso | **2** |
| Referencias externas | **1** (HappyJump) |
| Técnicas documentadas | 70 notas (`02-Tecnicas/`) |
| Herramientas documentadas | 36 notas (`03-Herramientas/`) |
| Playbooks | 5 (`04-Playbooks/`) |
| Cheatsheets | 6 (`06-Cheatsheets/`) |
| Plantillas | 9 (`08-Plantillas/`) |
| Enlaces internos rotos | **0** |
| Errores de YAML | **0** |

## 2. Estructura creada

```
CyberLab/
├── 00-Dashboard/            Dashboard + índice de máquinas + progreso
├── 00-Inicio/Dashboard.md   redirección al dashboard nuevo
├── 01-Maquinas/             índices: Resueltas, En-Progreso, Referencias
├── 01-CTF/TheHackersLabs/   15 carpetas de máquina (notas completas)
├── 02-Tecnicas/             8 categorías, 70 notas
├── 03-Herramientas/         36 notas
├── 04-Playbooks/            5 playbooks (método, AD, Linux, laboratorio, reporte)
├── 05-Roadmap-Aprendizaje/  roadmap maestro + plan 12 semanas
├── 06-Cheatsheets/          5 cheatsheets + índice
├── 07-Evidencias/           (reservado)
├── 08-Plantillas/           7 plantillas + índice
├── 09-Lecciones-Aprendidas/ lecciones globales
├── .backups-importacion-20261020-195335/   backup de originales
└── (carpetas heredadas vacías: 02-Roadmaps, 04-Tecnicas, 05-Writeups,
    06-Evidencias, 07-Plantillas — cada una con un README que redirige
    a la ubicación real del contenido)
```

> [!important] Estructura por máquina
> `Ficha` (`<Máquina>.md`) · `Roadmap` · `Walkthrough` · `Evidencias` · `Lecciones` · `Defensas` · (`Pendientes` solo si no está resuelta)

> Los nombres siguen el patrón `<Máquina> - <Tipo>.md` (por ejemplo `AllSafe - Roadmap.md`).

## 3. Máquinas importadas

### Resueltas (13)

| Máquina | OS | Técnica destacada |
|---|---|---|
| Phantom | Windows Server 2022 (AD) | ADCS ESC3 + PKINIT + PtH |
| Chimichurri | Windows Server 2016 (DC) | Credenciales en disco + sleuthkit |
| Elevator | Windows Server 2022 (DC) | BitLocker `.BEK` |
| Pila | Windows Server 2008 x86 | Stack Buffer Overflow (Vulnserver) |
| Folclore | Windows 10 (19041) | Cadena SMB + psexec |
| Sulaco | Linux (Debian) | Nonce doble-submit + CSRF |
| BabyCorp | Linux (Debian 12) | LFI con servicio como root |
| El Baifo | Linux | Redis → authorized_keys + SUID |
| Cold Fear | Linux | Esteganografía en audio + GTFOBins |
| SuraxCorp | Linux | SSTI Jinja2 + Rsync IPv6 |
| Securitron | Linux | SQLi time-based + PATH hijacking |
| THLCPPTV16 | Linux (multi-host) | Pivoting Ligolo/WireGuard |
| NovaVault | Linux | SSRF + reversing de token SUID |

### En progreso (2)

| Máquina | Estado real | Pendiente |
|---|---|---|
| Company1 | SSH como `marco.silva`; escalada fallida | Timers, SUID, capabilities, `~/.bash_history` |
| AllSafe | Web + intranet + PDFs; **LFI/LaTeX NO confirmado** | Confirmar motor PDF y comparar PDFs |

### Referencias (1)

| Recurso | Estado |
|---|---|
| HappyJump | Write-up **externo** — no es logro propio ([[HappyJump - Referencia]]) |

## 4. Decisiones de organización

1. **Vault único:** se usó `/home/edison/CyberLab` (repo git). No se creó otro Vault.
2. **Ubicación de máquinas:** se mantienen en `01-CTF/TheHackersLabs/` para no romper wikilinks; `01-Maquinas/` actúa como índice/hub.
3. **Nombres de fichero:** `0X-Tipo.md` → `"<Máquina> - Tipo.md"` y la ficha a `"<Máquina>.md"`, para que los enlaces por nombre de nota (p. ej. `Máquina - Roadmap` o `Máquina`) resuelvan en Obsidian.
4. **Legacy preservado:** las máquinas con roadmap detallado previo conservan `"<Máquina> - Roadmap.md"`; el resumen nuevo es `"<Máquina> - Roadmap (resumen).md"`.
5. **Ficha legacy:** para Phantom/Folclore/Chimichurri/Elevator/Pila la nota principal `<Máquina>.md` actúa como ficha (se le añadió YAML y "Notas relacionadas"); se eliminó el `- Ficha.md` duplicado.

## 5. Reglas seguidas

- ✅ **No** se inventaron flags, contraseñas ni salidas de herramientas.
- ✅ Se distingue **evidencia original**, **reconstrucción** e **inferencia**.
- ✅ **No** se marcó LFI/LaTeX de AllSafe como confirmado por un HTTP 200.
- ✅ **No** se copiaron claves privadas SSH/WireGuard, tokens ni `.env`.
- ✅ **No** se realizaron ataques ni escaneos nuevos.
- ✅ Los write-ups de terceros van como **Referencias**, no como máquinas resueltas.
- ✅ Se preservaron los originales y se creó backup previo.

## 6. Entregables de conocimiento

| Elemento | Ruta |
|---|---|
| Dashboard | [[Dashboard]] |
| Índice por categoría | [[Indice-Maquinas]] |
| Método de resolución | [[Metodo-de-Resolucion]] |
| Roadmap de aprendizaje | [[Roadmap-Maestro]] |
| Plan de estudio | [[Plan-12-Semanas]] |
| Índice de técnicas | [[Indice-Tecnicas]] |
| Índice de herramientas | [[Indice-Herramientas]] |
| Lecciones globales | [[Lecciones-Globales]] |
| Índice de cheatsheets | [[Indice-Cheatsheets]] |
| Índice de plantillas | [[Indice-Plantillas]] |

## 7. Validación

| Comprobación | Resultado |
|---|---|
| Enlaces internos (wikilinks) rotos | **0** |
| Frontmatter YAML con claves duplicadas o sin cierre | **0** |
| Claves privadas / tokens / `.env` en el Vault | **0** |
| Máquinas con estructura completa | **15 / 15** |
| Notas huérfanas / duplicadas de ficha | Resueltas (consolidadas) |

## 8. Limitaciones y notas honestas

- Parte de las notas se basan en **roadmaps de estudio** (documentos), no en capturas en vivo; en esos casos las salidas se marcan como **reconstrucción**.
- `~/ctf/novavault/SOLVED.md` **no existe**: documentado como fuente no localizada.
- Algunas IPs no quedaron registradas en las fuentes y se dejan como `—` en [[Resueltas]].
- El puerto `9000` de BabyCorp y otros cabos sueltos quedan en su nota `Pendientes`, **sin** inventar resultados.

## 9. Estado del repositorio

- **Cambios sin commitear:** sí (nuevas carpetas y notas). Backup de originales en `.backups-importacion-20261020-195335/`.
- **`.gitignore` respeta** exclusiones de secretos (`.env`, `*.key`, `*.pem`, `*credentials*`, `*password*`, `*secret*`).

---
*Reporte generado el 2026-10-20 durante la importación al Vault.*
