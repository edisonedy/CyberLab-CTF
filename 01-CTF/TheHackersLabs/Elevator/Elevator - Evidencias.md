---
tags: [ctf, thehackerslabs, windows, evidencias, bitlocker]
tipo: evidencias
---

# Elevator — Evidencias

> [!warning] Origen
> Basado en [[Elevator]] y [[Elevator - Roadmap]].

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Elevator/Elevator.md` | sesión original |
| 2 | imagen | `elev.raw` (BitLocker, 50 GB) | local | laboratorio |
| 3 | imagen | `elev_seed.ntfs` (NTFS 10 MB, etiqueta `Bitlocker`, 3 `.BEK`) | local | laboratorio |
| 4 | VDI | `The Hackers Labs - Elevator-disk001.vdi` | VirtualBox VMs | laboratorio |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Cadena de ACLs | salida de BloodHound |
| 2 | Flags user/root | NTFS descifrado (fls/icat) |

## Comprobaciones de alcance (importante)

| Comprobación | Resultado |
|---|---|
| ACEs salientes de `OPERACIONES` | Sin ACEs salientes |
| Anidamiento en grupos privilegiados | No |
| DCSync (DRSUAPI) | No |
| Acceso a shares | No |

## Comandos clave reproducibles

```bash
bloodhound-python -u john.smith -p 'Rk436\#Z4&' -d bloodhound.thl -dc elevator.bloodhound.thl -ns 192.168.56.104 -c All --zip
fls -r -p elev_seed.ntfs
cryptsetup bitlkDump elev.raw
dislocker -V <vol> -k <archivo.bek> -- /mnt/elev_clear
icat /mnt/elev_clear/dislocker-file <inode>
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `.BEK` extraídos | local | startup keys |
| copia sparse de `elev.raw` | local | para `bitlkDump` |
