---
tags: [tecnica, forense, sleuthkit, ntfs]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Pila, Chimichurri, Sulaco, Folclore, Elevator]
---

# Sleuthkit (lectura offline de NTFS/ext4)

## 1. Qué es

Conjunto de herramientas forenses para listar y extraer ficheros de una imagen de disco **sin** montarla ni explotar la máquina.

## 2. Cómo funciona

- `fls` lista entradas de directorio (inodos); `icat` extrae el contenido de un inodo.

## 3. Cuándo sospecharla

- Cuando se dispone de un **VDI/imagen** (VM apagada) y se quieren los flags offline.
- NTFS «sucio» que no monta bien.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
fls -o <offset> -r -p imagen.raw | grep -i -E 'user.txt|root.txt'
icat -o <offset> imagen.raw <inode>
# alternativa de montaje (NTFS sucio)
ntfs-3g -o ro,recover,norecover 1.ntfs /mnt/disk
```

## 5. Herramientas

- `fls`, `icat`, `ntfs-3g`, `7z` (extraer particiones de un VDI).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Pila]] | Extracción offline de flags | user/root |
| [[Chimichurri]] | Extracción offline en DC | user/root |
| [[Sulaco]] | `icat` de `admin_flag.php` | admin flag |
| [[Folclore]] | Particiones extraídas del VDI con `7z` | user/root |
| [[Elevator]] | `.BEK` + NTFS descifrado | user/root |

## 7. Falsos positivos / pruebas fallidas

- `fls` requiere el **offset** correcto de la partición.
- NTFS marcado «sucio» monta vacío sin `-o recover,norecover`; alternativa: `fls`/`icat`.

## 8. Prevención y mitigación

- (Defensa) cifrado y control de acceso a los discos virtuales; escrow de claves.

## 9. Técnicas relacionadas

- [[BitLocker-BEK]]
