---
tags: [ctf, thehackerslabs, lecciones, pivoting]
tipo: lecciones
---

# THLCPPTV16 — Lecciones aprendidas

## Errores cometidos

| Error | Consecuencia | Corrección |
|---|---|---|
| Fuerza bruta online en WordPress | Lento y ruidoso | Crackear el hash **offline** (phpass + John) |
| No documentar la topología de red desde el principio | Confusión al pivote | Mapa de red **antes** de saltar de host |
| Confundir acceso inicial con intermedios | Pérdida de contexto | Etiquetar cada host: inicial/intermedio/objetivo |

## Tiempo perdido y por qué

- Descifrar la secuencia de wrappers PHP hasta llegar a `WrapWrap`.

## Conceptos aprendidos

- [[SQL-Injection]] ciega (time-based) en **My Calendar**.
- Hash **phpass** + [[John-The-Ripper]].
- [[PHP-Filters]] + [[WrapWrap]].
- Alias SSH como movimiento lateral.
- [[Sudo-Vim]].
- [[Pivoting-Ligolo]] + [[WireGuard]].
- [[APT-Pre-Invoke]] y [[Sudo-Nginx]].

## Señales para reconocer en otra máquina

- [ ] WordPress con plugins desactualizados (inventario `wpscan`).
- [ ] Segundo vhost con parámetros tipo `page=`.
- [ ] `~/.ssh/config` con alias a otros hosts.
- [ ] Redes internas inaccesibles → pivoting.
- [ ] Hooks de APT o `sudo` sobre servidores (nginx) con config escribible.

## Preguntas de autoevaluación

1. ¿Cómo montas un pivote con Ligolo-ng y por qué WireGuard?
2. ¿Qué requisito debe cumplir un fichero de config de APT para abusar de Pre-Invoke?
3. ¿Por qué `sudo nginx -c` es peligroso?

## Técnicas que necesitan más práctica

- [[Pivoting-Ligolo]]
- [[PHP-Filters]] / [[WrapWrap]]
- [[APT-Pre-Invoke]]
