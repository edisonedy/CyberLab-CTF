---
tags: [playbook, metodologia]
tipo: playbook
estado: activo
ultima_revision: 2026-10-20
---

# Método de Resolución (playbook)

> [!important] Regla de honestidad
> Registrar **hipótesis → payload → respuesta → resultado real**. Un HTTP 200, una conexión o un PDF **no** son prueba de explotación. No fabricar salidas.

## 0. Antes de empezar

- [ ] Confirmar **autorización** (laboratorio propio / CTF).
- [ ] Anotar IP, plataforma, credenciales dadas.
- [ ] Preparar carpeta de trabajo y de capturas.
- [ ] Comprobar conectividad de la VM ([[VBoxManage]]: host-only, DHCP, cable).

## 1. Reconocimiento

1. Descubrimiento de host: `nmap -sn`, `arp-scan -I <if> -g <rango>`.
2. Puertos: `nmap -p- --open -sS --min-rate 5000 -n -Pn <IP>`.
3. Servicios/versiones: `nmap -sCV -p<puertos> <IP>`.
4. **IPv6** si los resultados parecen incompletos ([[Nmap]]).
5. Web: vhosts y fuzzing ([[Virtual-Hosts]]).

**Salida esperada:** inventario de servicios + superficie de ataque priorizada.

## 2. Enumeración por servicio

| Servicio | Técnica |
|---|---|
| Web | [[Virtual-Hosts]], [[SQL-Injection]], [[Path-Traversal]], [[SSTI-Jinja2]] |
| SMB | [[SMB-Enum]] |
| FTP | [[FTP-Anonimo]] |
| Redis | [[Redis-Sin-Autenticacion]] |
| MQTT | [[MQTT]] |
| Rsync | [[Rsync]] |
| Gitea | [[Gitea]] |
| AD | [[AD-Abuso-de-ACLs]], [[AD-Tombstones]], [[ADCS-ESC3]] |

**Regla:** obtener el **código fuente** (Gitea, `.deb`, configs) suele ser más rápido que adivinar.

## 3. Acceso inicial

- Credenciales filtradas (ficheros, EXIF, metadatos, configs).
- Explotación web ([[Command-Injection]], [[File-Upload]], [[SSRF]]).
- Captura de hashes ([[Responder-NTLM-Theft]]) + crackeo ([[Cracking]]).
- Escritura de `authorized_keys` ([[Redis-Sin-Autenticacion]], [[SSH-Keys]]).

## 4. Enumeración post-acceso

- Linux: [[Linux-Enum]].
- Windows/AD: [[BloodHound]], shares, WinRM.

## 5. Escalada de privilegios

- Linux: [[SUID]], [[GTFOBins]], [[Cron-Privilege-Escalation]], [[Path-Hijacking]], [[Grupos-Linux]].
- AD: [[ADCS-ESC3]], [[AD-Abuso-de-ACLs]], [[PKINIT-Pass-The-Hash]].
- Fallback offline: [[Sleuthkit]], [[BitLocker-BEK]].

## 6. Pivoting

- [[Pivoting-Ligolo]], [[WireGuard]].

## 7. Documentación (obligatoria)

Crear en la carpeta de la máquina:

1. `00-Ficha` — identificación, puertos, resultado, técnicas.
2. `01-Roadmap` — cadena con diagrama mermaid.
3. `02-Walkthrough` — comando → salida → interpretación → por qué.
4. `03-Evidencias` — originales, reconstrucciones, inferencias.
5. `04-Lecciones` — errores, conceptos, señales.
6. `05-Defensas` — causa raíz, impacto, prevención, detección.
7. `06-Pendientes` — solo si **no** está resuelta.

## 8. Cierre

- Actualizar índices ([[Resueltas]], [[En-Progreso]]).
- Añadir técnicas nuevas a [[Indice-Tecnicas]].
- Registrar lecciones en [[Lecciones-Globales]].

## Errores frecuentes (recopilados)

| Error | Corrección |
|---|---|
| Confundir HTTP 200 con explotación | Verificar **contenido** (AllSafe) |
| Confiar en BloodHound como verdad | Validar DACLs reales (Elevator) |
| Escanear solo IPv4 | Incluir IPv6 (SuraxCorp) |
| Fuerza bruta online | Crackeo **offline** |
| Cambiar varias variables a la vez | Una por prueba |
| Perder tiempo buscando root en todas | A menudo hay **acceso offline a discos** |
