---
tags: [herramienta, virtualizacion, virtualbox]
tipo: herramienta
categoria: laboratorio
ultima_revision: 2026-10-20
---

# VBoxManage

Gestión de VirtualBox por CLI. Fundamental para arreglar problemas de laboratorio (redes, discos).

## Uso típico

```bash
# Red: poner NIC en host-only
VBoxManage controlvm "The Hackers Labs - Sulaco" nic1 hostonly vboxnet0
VBoxManage modifyvm "The Hackers Labs - Folclore" --nic1 hostonly --hostonlyadapter1 vboxnet0 --cableconnected1 on
# DHCP: evitar conflicto con IP fija de la VM
VBoxManage dhcpserver modify --network=HostInterfaceNetworking-vboxnet0 \
  --server-ip=192.168.56.2 --lower-ip=192.168.56.101 --upper-ip=192.168.56.254 \
  --netmask=255.255.255.0 --enable
# UUID de disco duplicado
VBoxManage internalcommands sethduuid "<vdi>" <uuid-nuevo>
```

## Notas / lecciones

- APIPA (`169.254.x.x`) casi siempre = **conflicto de DHCP** con la IP fija de la VM.
- Bridged sobre `wlan0` deja la VM fuera de `192.168.56.x`.
- `NS_ERROR_FAILURE` al importar VDI → UUID duplicado.

## Técnicas relacionadas

- [[Phantom - Roadmap]], [[Chimichurri - Roadmap]], [[Sulaco - Roadmap]], [[Folclore - Roadmap]]
