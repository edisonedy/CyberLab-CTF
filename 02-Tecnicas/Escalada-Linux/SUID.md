---
tags: [tecnica, escalada-linux, sudo, suid, gtfobins]
tipo: tecnica
categoria: escalada-linux
estado: practicado
maquinas: [El Baifo, NovaVault, Cold Fear, SuraxCorp, Securitron]
---

# SUID

## 1. Qué es

Binarios con el bit **SUID** se ejecutan con los privilegios del propietario (a menudo root). Si permiten ejecutar código/comandos, otorgan escalada.

## 2. Cómo funciona

- `chmod u+s binario` → al ejecutarlo, el **EUID** es del propietario.
- Si el binario lanza una shell o permite `-exec`/`!sh`, esa shell hereda los privilegios.

## 3. Cuándo sospecharla

- Tras conseguir acceso, al enumerar binarios SUID.
- Binarios **no habituales** o propios.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
find / -perm -4000 -type f 2>/dev/null
# consultar GTFOBins para cada binario
bash -p        # si bash tiene SUID -> EUID 0
id
```

## 5. Herramientas

- [[GTFOBins]], `find`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[El Baifo]] | Escalada final con SUID | root |
| [[NovaVault]] | SUID `vault-guard` (reversing del token) | root |
| [[Pila]] | Escalada vía shellcode tras BOF (contexto admin) | root |
| [[Securitron]] | Escalada SUID tras PATH hijacking | root |

## 7. Falsos positivos / pruebas fallidas

- Un SUID que **no** permite ejecución arbitraria no sirve solo; hay que analizarlo.
- Verificar siempre el **EUID** real (`id`, `echo $EUID`), no asumir.

## 8. Prevención y mitigación

- Eliminar SUID innecesarios; auditar y usar alternativas (capabilities, sudoers).
- Montar particiones con `nosuid` cuando aplique.

## 9. Técnicas relacionadas

- [[GTFOBins]], [[Path-Hijacking]], [[Cron-Privilege-Escalation]], [[Reversing-Token-Validation]]
