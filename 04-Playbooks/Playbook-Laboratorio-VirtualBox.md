---
tags: [playbook, laboratorio, virtualbox]
tipo: playbook
estado: activo
ultima_revision: 2026-10-20
---

# Playbook — Diagnóstico de laboratorio (VirtualBox)

> Problemas recurrentes que **parecen** ataques y son de entorno.

## Síntomas y soluciones

| Síntoma | Causa probable | Solución |
|---|---|---|
| VM con APIPA `169.254.x.x` | Conflicto DHCP con IP fija de la VM | Mover DHCP ([[VBoxManage]]) |
| VM no aparece en `192.168.56.x` | NIC en **bridged** sobre `wlan0` | `nic1 hostonly vboxnet0` |
| NIC en `NatNetwork` inexistente | Config errónea | `--nic1 hostonly --hostonlyadapter1 vboxnet0 --cableconnected1 on` |
| `NS_ERROR_FAILURE` al abrir VM | UUID de disco duplicado | `VBoxManage internalcommands sethduuid` |
| NTFS no monta | Volumen «sucio» | `ntfs-3g -o ro,recover,norecover` o `fls`/`icat` |
| `cryptography` roto en Python | Paquetes de `~/.local` | `PYTHONNOUSERSITE=1` |

## Checklist

- [ ] `VBoxManage list dhcpservers`
- [ ] `VBoxManage showvminfo "<VM>" | grep -i nic`
- [ ] `ip a` en el atacante (interfaz host-only presente?)
- [ ] `nmap -sn 192.168.56.0/24`
