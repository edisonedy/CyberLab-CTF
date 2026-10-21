---
tags: [ctf, thehackerslabs, windows, roadmap, active-directory, bitlocker]
tipo: roadmap
estado: resuelto
ip: 192.168.56.104
fecha: 2026-10-20
---

# Elevator — Roadmap (resumen)

> [!tip] Documento principal
> El detalle completo está en [[Elevator - Roadmap]]. Esta nota resume la cadena y añade el diagrama.

> [!abstract] Cadena de explotación
> Autenticación como `john.smith` → recolección **BloodHound** → cadena de **ACLs** hasta controlar `OPERACIONES` (que **no** da privilegios AD) → extraer `.BEK` del disco auxiliar → identificar protectores **BitLocker** → montar con **dislocker** → leer las flags.

## Diagrama

```mermaid
flowchart TD
    A["john.smith (credencial inicial)"] --> B[BloodHound]
    B --> C["AddSelf -> FINANZAS"]
    C --> D["GenericAll -> MARY.JOHNSON"]
    D --> E["ForceChangePassword -> ROBERT.WILLIAMS"]
    E --> F["WriteDacl -> PATRICIA.BROWN"]
    F --> G["WriteOwner -> OPERACIONES (grupo)"]
    G --> H["WriteDacl/WriteMembers -> control de OPERACIONES"]
    H --> I{"OPERACIONES da privilegios AD?"}
    I -->|"NO (sin ACEs salientes)"| J["Cadena AD termina"]
    J --> K["Extraer .BEK del disco auxiliar (sleuthkit)"]
    K --> L["Identificar protectores BitLocker (bitlkDump)"]
    L --> M["Montar con dislocker + .BEK (FUSE)"]
    M --> N["Leer user.txt y root.txt (fls/icat)"]
```

## Kill chain resumida

```text
john.smith → BloodHound → AddSelf→FINANZAS → GenericAll→MARY → ForceChangePassword→ROBERT
  → WriteDacl→PATRICIA → WriteOwner→OPERACIONES → (sin privilegios AD)
  → .BEK del disco auxiliar → bitlkDump → dislocker → flags (USER/ROOT)
```

## Nota clave

> [!warning] La cadena AD engaña
> BloodHound es una **guía**, no una garantía. Hay que validar las **DACLs reales** (`nTSecurityDescriptor` + control `SD_FLAGS`): no toda cadena de ACL llega a DCSync.

## Enlaces

- [[Elevator - Roadmap]] (detalle completo)
- [[Elevator - Walkthrough]]
- [[Elevator - Evidencias]]
