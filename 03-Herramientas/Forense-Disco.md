---
tags: [herramienta, forense, disco]
tipo: herramienta
categoria: forense
ultima_revision: 2026-10-20
---

# fls / icat / ntfs-3g / 7z (forense de disco)

Extracción **offline** de ficheros de imágenes de disco sin montar el sistema operativo.

## Uso típico

```bash
7z x "disco.vdi"                                   # extraer particiones de un VDI
fls -o <offset> -r -p imagen.raw | grep -i -E 'user.txt|root.txt'
icat -o <offset> imagen.raw <inode>
ntfs-3g -o ro,recover,norecover 1.ntfs /mnt/disk   # NTFS "sucio"
```

## Notas / lecciones

- `fls` requiere el **offset** correcto de la partición.
- NTFS marcado como sucio puede montar vacío sin `-o recover,norecover`; alternativa `fls`/`icat`.
- Un `vulnserver`/binario de prueba muestra pistas del reto.

## Técnicas relacionadas

- [[Sleuthkit]], [[BitLocker-BEK]]
