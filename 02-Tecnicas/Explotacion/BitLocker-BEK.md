---
tags: [tecnica, explotacion, bitlocker, forense]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Elevator]
---

# BitLocker y startup keys (.BEK)

## 1. Qué es

Abuso de **BitLocker** cuando las *startup keys* `.BEK` son accesibles: permiten montar el volumen cifrado sin recovery password ni root.

## 2. Cómo funciona

- Los protectores (`TPM`, `Numerical Password`, `External Key`) desbloquean el volumen.
- Con la `.BEK` (External Key), `dislocker` monta el contenido.

## 3. Cuándo sospecharla

- Volúmenes BitLocker y discos auxiliares con `.BEK`.
- Discos virtuales accesibles **offline**.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
# extraer .BEK de un disco auxiliar
fls -r -p aux.ntfs; icat aux.ntfs <inode>
# identificar protectores sin copiar todo el disco
cryptsetup bitlkDump disco.raw
# montar con la startup key (dislocker via FUSE, sin root)
dislocker -V <vol> -k <archivo.bek> -- /mnt/clear
icat /mnt/clear/dislocker-file <inode>
```

## 5. Herramientas

- `dislocker`, `cryptsetup`, [[Sleuthkit]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Elevator]] | `.BEK` en disco auxiliar de 10 MB | volumen descifrado → flags |

## 7. Falsos positivos / pruebas fallidas

- Sin la `.BEK`/recovery password correcta no se monta.
- Copiar los 50 GB es innecesario: usar copia **sparse** para `bitlkDump`.

## 8. Prevención y mitigación

- TPM+PIN; no almacenar `.BEK` en el mismo equipo; escrow seguro de recovery keys.

## 9. Técnicas relacionadas

- [[Sleuthkit]]
