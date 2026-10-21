---
tags: [herramienta, ad, ldap]
tipo: herramienta
categoria: ad
ultima_revision: 2026-10-20
---

# ldap3 / ldapmodify

Consulta y modificación de directorios LDAP (Active Directory, OpenLDAP). Clave para leer tombstones y abusar de ACLs.

## Uso típico

```python
from ldap3 import Server, Connection, ALL, SUBTREE
c = Connection(Server('<ip>', get_info=ALL), user='DOM\\user', password='<pw>',
               authentication='NTLM', auto_bind=True)
c.search('DC=DOM,DC=THL', '(isDeleted=TRUE)', search_scope=SUBTREE,
         attributes=['sAMAccountName','description'],
         controls=[('1.2.840.113556.1.4.417', True, None)])
print(c.entries)
```

```bash
ldapmodify -x -H ldap://<ip> -D 'user@dom' -w '<pw>' -f cambios.ldif
```

## Notas / lecciones

- El control LDAP show-deleted `1.2.840.113556.1.4.417` es necesario para ver objetos eliminados.
- En AllSafe se revisaron anuarios/consultas LDAP de la intranet (sin explotación confirmada).

## Técnicas relacionadas

- [[AD-Tombstones]], [[AD-Abuso-de-ACLs]]
