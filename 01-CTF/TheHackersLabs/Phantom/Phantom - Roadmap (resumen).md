---
tags: [ctf, thehackerslabs, windows, roadmap, active-directory, adcs]
tipo: roadmap
estado: resuelto
ip: 192.168.56.100
fecha: 2026-10-16
---

# Phantom — Roadmap (resumen)

> [!tip] Documento principal
> El detalle completo está en [[Phantom - Roadmap]] (writeup paso a paso). Esta nota ofrece el **resumen** y el diagrama.

> [!abstract] Cadena de explotación
> `mark` →(GenericAll)→ `bob` →(HELPDESK ForceChangePassword)→ `frank` (WinRM) → LNK en share "Dev Tools" + Responder → hash de `robert` → crack (bLink182) → **USER** → tombstone `dev_test` (password en `description`) → reutilización → `tomas` → **ADCS ESC3** (Phantom-DevAuth) → cert de Administrador → **PKINIT** → NT hash → **PtH** → **ADMIN**.

## Diagrama

```mermaid
flowchart TD
    A["mark (dado)"] -->|GenericAll| B[bob]
    B -->|HELPDESK: ForceChangePassword| C["frank (WinRM)"]
    C -->|"share Dev Tools escribible + LNK + Responder"| D["hash de robert"]
    D -->|"crack rockyou -> bLink182"| E["robert = USER FLAG"]
    E -->|"tombstone dev_test (description)"| F["password -> reutilizacion"]
    F --> G[tomas]
    G -->|"ADCS ESC3 (Phantom-DevAuth)"| H["cert de Administrador"]
    H -->|PKINIT| I["NT hash"]
    I -->|Pass-the-Hash| J["Administrador = ADMIN FLAG"]
```

## Kill chain resumida

```text
mark —GenericAll→ bob —ForceChangePassword→ frank (WinRM)
  → LNK + Responder → hash robert → crack → robert (USER FLAG)
  → tombstone dev_test → tomas → ADCS ESC3 → cert Administrador
  → PKINIT → NT hash → PtH → ADMIN FLAG
```

## Enlaces

- [[Phantom - Roadmap]] (detalle completo)
- [[Phantom - Walkthrough]]
- [[Phantom - Evidencias]]
