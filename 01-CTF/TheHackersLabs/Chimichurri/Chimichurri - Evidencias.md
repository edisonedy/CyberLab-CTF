---
tags: [ctf, thehackerslabs, windows, evidencias]
tipo: evidencias
---

# Chimichurri — Evidencias

> [!warning] Origen
> Basado en [[Chimichurri]] y [[Chimichurri - Roadmap]].

## Originales

| # | Tipo | Descripción | Ruta | Origen |
|---|---|---|---|---|
| 1 | md | Nota principal | `01-CTF/TheHackersLabs/Chimichurri/Chimichurri.md` | sesión original |
| 2 | fichero | `credenciales.txt`, `perico.txt` | `C:\drogas\` y `C:\Users\hacker\Desktop\` | laboratorio |
| 3 | disco | `Chimichurri.thl-disk001.vdi` (30 GB) | VirtualBox VMs | laboratorio |

## Reconstrucciones

| # | Qué | Base |
|---|---|---|
| 1 | Credenciales `hacker:Perico69` | contenido de `perico.txt` |
| 2 | Flags user/root | ficheros vía offline (sleuthkit) |

## Falsos positivos identificados

| Elemento | Por qué NO era un flag |
|---|---|
| `THL{9A88909B-…}` | Nombre de GPO (`CHIMICHURRI.THL`) + GUID del registro |

## Comandos clave reproducibles

```bash
VBoxManage internalcommands sethduuid "<vdi>" <uuid-nuevo>
fls -o <offset> -r -p chimichurri.raw | grep -i -E 'user.txt|root.txt'
icat -o <offset> chimichurri.raw <inode>
```

## Artefactos generados

| Artefacto | Ruta | Notas |
|---|---|---|
| `chimichurri.raw` | local | imagen para extracción offline |
