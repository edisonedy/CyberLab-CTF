---
tags: [dashboard, indice, inicio]
tipo: dashboard
ultima_revision: 2026-10-20
---

# 🛡️ CyberLab — Dashboard

> [!info] Base de conocimiento CTF
> Documentación estructurada de máquinas de **TheHackersLabs**, técnicas, herramientas, playbooks y roadmap de aprendizaje.

## 🗺️ Navegación principal

| Sección | Enlace |
|---|---|
| Máquinas resueltas | [[Resueltas]] |
| Máquinas en progreso | [[En-Progreso]] |
| Referencias externas | [[Referencias]] |
| Técnicas | [[Indice-Tecnicas]] |
| Herramientas | [[Indice-Herramientas]] |
| Playbooks | [[Metodo-de-Resolucion]] |
| Roadmap de aprendizaje | [[Roadmap-Maestro]] |
| Cheatsheets | [[Indice-Cheatsheets]] |
| Lecciones aprendidas | [[Lecciones-Globales]] |
| Plantillas | [[Indice-Plantillas]] |

## 🖥️ Máquinas (TheHackersLabs)

### ✅ Resueltas

- [[Phantom]] — AD / ADCS ESC3
- [[Chimichurri]] — DC / credenciales en disco
- [[Elevator]] — AD / BitLocker (.BEK)
- [[Pila]] — Windows / Buffer Overflow
- [[Sulaco]] — Web / nonce + CSRF
- [[Folclore]] — Windows / cadena SMB
- [[BabyCorp]] — Web / LFI como root
- [[El Baifo]] — Linux / Redis + SUID
- [[Cold Fear]] — Linux / audio + GTFOBins
- [[SuraxCorp]] — Linux / SSTI + IPv6
- [[Securitron]] — Linux / SQLi + cron
- [[THLCPPTV16]] — Multi-host / pivoting
- [[NovaVault]] — Linux / SSRF + SUID reversing

### 🟡 En progreso

- [[Company1]] — Linux / escalada pendiente
- [[AllSafe]] — Web / LaTeX-LFI **sin confirmar**

### 📚 Referencias

- [[HappyJump - Referencia]] — write-up externo (no propio)

## 📈 Progreso

| Métrica | Valor |
|---|---|
| Máquinas resueltas (propias) | 13 |
| Máquinas en progreso | 2 |
| Referencias externas | 1 |
| Técnicas documentadas | 44 |
| Herramientas documentadas | 21 |
| Playbooks | 5 |
| Cheatsheets | 5 |

## 🎯 Objetivos próximos

- [ ] Terminar **Company1** (escalada).
- [ ] Confirmar o descartar **AllSafe** (LaTeX/LFI).
- [ ] Ampliar **AD CS** (ESC1/ESC4/ESC8).
- [ ] Reversing moderno (ROP, format strings).

## ⚠️ Principios de esta base

- **No** inventar flags, salidas ni resultados.
- Distinguir **evidencia original**, **reconstrucción** e **inferencia**.
- **Nunca** marcar explotación como confirmada sin prueba (HTTP 200 ≠ éxito).
- No incluir claves privadas, tokens ni `.env` en el Vault.

---
*Última revisión: 2026-10-20*
