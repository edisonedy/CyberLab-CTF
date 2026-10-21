---
tags: [tecnica, criptografia, base64, rot13, codificaciones]
tipo: tecnica
categoria: criptografia-esteganografia
estado: practicado
maquinas: [Cold Fear, THLCPPTV16, BabyCorp]
---

# Codificaciones: Base64 y ROT13

## 1. Qué es

Codificaciones/cifrados simples muy usados para «ocultar» pistas: Base64 (codificación) y ROT13 (cifrado César con desplazamiento 13).

## 2. Cómo funciona

- Base64: representa bytes en ASCII; termina habitualmente en `=` o `==`.
- ROT13: rota las letras 13 posiciones (auto-inverso).

## 3. Cuándo sospecharla

- Cadenas largas alfanuméricas con `+`/`/`/`=`.
- Texto que «parece» leíble pero no lo es.

## 4. Cómo comprobarla (laboratorio autorizado)

```bash
echo '<b64>' | base64 -d
echo '<texto>' | tr 'A-Za-z' 'N-ZA-Mn-za-m'    # ROT13
# encadenar en el orden correcto: base64 -> ROT13
```

## 5. Herramientas

- `base64`, `tr`, CyberChef (referencia).

## 6. Ejemplos en mis CTF

| Máquina | Contexto | Resultado |
|---|---|---|
| [[Cold Fear]] | Base64 → ROT13 | pistas de Sergei |
| [[BabyCorp]] | Respuesta del LFI en base64 | contenido de ficheros |
| [[THLCPPTV16]] | Mensaje en ROT13 | credencial de Jerry |

## 7. Falsos positivos / pruebas fallidas

- Aplicar las transformaciones **en el orden equivocado** da texto ilegible.

## 8. Prevención y mitigación

- (Defensa) no usar codificaciones simples como protección; son trivialmente reversibles.

## 9. Técnicas relacionadas

- [[XOR]], [[Esteganografia-LSB]]
