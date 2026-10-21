---
tags: [tecnica, explotacion, ad, tombstones]
tipo: tecnica
categoria: explotacion
estado: practicado
maquinas: [Phantom]
---

# AD — Objetos eliminados (Tombstones)

## 1. Qué es

Objetos borrados de AD que permanecen un tiempo en **Deleted Objects** conservando atributos (incluido `description`), donde a veces quedan **contraseñas**.

## 2. Cómo funciona

- Con el control LDAP *show-deleted* (`1.2.840.113556.1.4.417`) y filtro `(isDeleted=TRUE)` se leen los tombstones.

## 3. Cuándo sospecharla

- Pistas que mencionan «cuentas de prueba temporales».
- Recolección con un usuario que puede leer objetos eliminados.

## 4. Cómo comprobarla (laboratorio autorizado)

```python
from ldap3 import Server, Connection, ALL, SUBTREE
c = Connection(Server('<ip>', get_info=ALL), user='DOM\\user', password='<pw>',
               authentication='NTLM', auto_bind=True)
c.search('DC=DOM,DC=THL', '(isDeleted=TRUE)', search_scope=SUBTREE,
         attributes=['sAMAccountName','description','isDeleted'],
         controls=[('1.2.840.113556.1.4.417', True, None)])
print(c.entries)
```

## 5. Herramientas

- `ldap3`, [[NetExec]], [[BloodHound]].

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Phantom]] | Tombstone `dev_test` con contraseña en `description` | reutilización → `tomas` |

## 7. Falsos positivos / pruebas fallidas

- Sin permisos para leer Deleted Objects no se obtienen resultados.

## 8. Prevención y mitigación

- No guardar secretos en `description`; **purgar** la papelera de AD; auditar atributos.

## 9. Técnicas relacionadas

- [[Password-Spraying]], [[AD-Abuso-de-ACLs]]
