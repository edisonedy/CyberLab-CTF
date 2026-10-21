---
tags: [cheatsheet, forense, offline]
tipo: cheatsheet
ultima_revision: 2026-10-20
---

# Cheatsheet — Forense / Extracción offline

```bash
# Extraer particiones de un VDI
7z x "The Hackers Labs - <Maquina>-disk001.vdi"

# Leer NTFS con sleuthkit
fls -o <offset> -r -p imagen.raw | grep -i -E 'user.txt|root.txt|flag'
icat -o <offset> imagen.raw <inode>

# Montar NTFS "sucio"
ntfs-3g -o ro,recover,norecover 1.ntfs /mnt/disk

# BitLocker
fls -r -p aux.ntfs; icat aux.ntfs <inode>        # extraer .BEK
cryptsetup bitlkDump disco.raw                    # identificar protectores
dislocker -V <vol> -k <archivo.bek> -- /mnt/clear
icat /mnt/clear/dislocker-file <inode>

# Metadatos / esteganografía
exiftool imagen.jpg
stegseek imagen.jpg /usr/share/wordlists/rockyou.txt
steghide extract -sf imagen.jpg
zsteg imagen.png

# Codificaciones
echo '<b64>' | base64 -d
echo '<texto>' | tr 'A-Za-z' 'N-ZA-Mn-za-m'      # ROT13
```

## Técnicas

- [[Sleuthkit]], [[BitLocker-BEK]], [[Exif-Metadata]], [[Esteganografia-LSB]], [[Esteganografia-WAV-LSB]], [[Codificaciones]]
