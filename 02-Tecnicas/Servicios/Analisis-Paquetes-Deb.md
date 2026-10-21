---
tags: [tecnica, servicios, deb, empaquetado]
tipo: tecnica
categoria: servicios
estado: practicado
maquinas: [SuraxCorp, Elevator]
---

# Análisis de paquetes .deb

## 1. Qué es

Inspección del contenido de un paquete Debian (scripts, binarios, config) para descubrir vulnerabilidades o pistas.

## 2. Cómo funciona

- `dpkg-deb -e` extrae metadatos/scripts; `-x` extrae el sistema de ficheros.

## 3. Cuándo sospecharla

- Paquetes descargables desde una web/repo interno.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
dpkg-deb -e paquete.deb ./DEBIAN
dpkg-deb -x paquete.deb ./data
cat DEBIAN/postinst DEBIAN/prerm
strings data/usr/bin/*
```

## 5. Herramientas

- `dpkg-deb`, `strings`, `ar`.

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[SuraxCorp]] | `surapharm.deb` | vhost alternativo / command injection |
| [[Elevator]] | `dislocker` extraído de un `.deb` | herramienta ejecutable sin root |

## 7. Falsos positivos / pruebas fallidas

- Tratar el `.deb` como caja negra oculta el vhost/pista interna.

## 8. Prevención y mitigación

- (Defensa) firmar/verificar paquetes y no exponerlos públicamente.

## 9. Técnicas relacionadas

- [[Command-Injection]], [[Rsync]]
