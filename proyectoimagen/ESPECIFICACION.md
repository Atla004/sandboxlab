# Especificación Técnica — Editor de Imágenes PPM

Este documento detalla las especificaciones técnicas concretas que deben seguir **todas las entregas**. Es el contrato de diseño del proyecto.

---

## 1. 🧬 Formato PPM

### P3 (Texto) — E2
```
P3
ancho alto
255
r g b r g b r g b ...
```

- Cada píxel son 3 números (0-255) separados por espacios
- 1 píxel por línea recomendado (no obligatorio)
- Comentarios con `#` al inicio de línea
- Tamaño: ~3x el tamaño del binario

### P6 (Binario) — E3
```
P6
ancho alto
255
[datos binarios: ancho * alto * 3 bytes]
```

- Cabecera igual que P3, pero terminada en '\n'
- Datos: RGBRGBRGB... en bruto, 3 bytes por píxel
- Tamaño: exactamente `ancho * alto * 3 + tamaño_cabecera`

### Validación de cabecera
```
P3\n ancho alto\n 255\n     → texto
P6\n ancho alto\n 255\n     → binario
```

---

## 2. 🎨 Sistema de color

### Estructura Pixel
```cpp
struct Pixel {
    unsigned char r, g, b;
};
```

### Fórmula de brillo (para ASCII art)
```
brillo = (r + g + b) / 3

Mapeo a caracteres (10 niveles):
0-25:   ' '
26-51:  '.'
52-76:  ':'
77-102: '-'
103-127: '='
128-153: '+'
154-178: '*'
179-204: '#'
205-229: '%'
230-255: '@'
```

---

## 3. ⚙️ Filtros

### Filtros básicos (E1)
| Filtro | Operación |
|--------|-----------|
| Invertir | `r = 255 - r`, igual para g, b |
| Brillo +N | `r = min(r + N, 255)` |
| Brillo -N | `r = max(r - N, 0)` |
| Umbral | Si brillo < límite → negro, sino blanco |
| Mezclar | `pixel = a * peso + b * (1-peso)` |

### Filtros avanzados (E4)
| Filtro | Operación |
|--------|-----------|
| Sepia | `r = min(r*0.393 + g*0.769 + b*0.189, 255)` etc. |
| Gauss 3x3 | Convolución con kernel [[1,2,1],[2,4,2],[1,2,1]] / 16 |
| Sobel | Convolución con kernels Gx, Gy, magnitud |
| Pixelado | Promedio de bloques NxN |

---

## 4. 🔄 Transformaciones geométricas (E3)

| Transformación | Descripción |
|----------------|-------------|
| Espejo horizontal | `pixel[fila][col] ↔ pixel[fila][ancho-1-col]` |
| Espejo vertical | `pixel[fila][col] ↔ pixel[alto-1-fila][col]` |
| Rotar 90° | Nueva imagen de `alto x ancho` |
| Redimensionar | Nearest neighbor: `srcX = x * anchoSrc / anchoDst` |
| Recortar | Copiar submatriz desde (x,y) con (w,h) |

---

## 5. 📐 Filtros de convolución (E4)

Kernel genérico 3x3:
```
k00 k01 k02
k10 k11 k12
k20 k21 k22
```

Aplicación:
```
nuevoPixel = sumatoria(k[i][j] * pixel[fila-1+i][col-1+j]) / divisor
```

Kernel Gauss:
```
1 2 1
2 4 2   / 16
1 2 1
```

Kernel Sobel X:
```
-1 0 1
-2 0 2
-1 0 1
```

---

## 6. 🏗️ Normas de codificación

| Norma | Detalle |
|-------|---------|
| Lenguaje | C++11 o superior |
| Compilación | `g++ -std=c++11 -Wall -Wextra -I include` |
| Prohibido | `std::string` como atributo, `std::vector`, librerías externas |
| Memoria | Cada `new` debe tener su `delete`. Matriz: `new Pixel*[alto]`, cada fila `new Pixel[ancho]` |
| Nombres | `snake_case` funciones/variables, `PascalCase` clases |
| Archivos | Un `.h` y un `.cpp` por clase principal |
| Makefile | Targets: `entrega1`, `entrega2`, `entrega3`, `entrega4`, `clean` |

---

## 7. 📤 Entregas y evaluación progresiva

| Entrega | Temas | El usuario... |
|---------|-------|---------------|
| E1 | Punteros y memoria dinámica | Juega con imágenes hardcodeadas en ASCII |
| E2 | Ficheros de texto | Carga/guarda imágenes PPM reales editables |
| E3 | Ficheros binarios | Carga/guarda PPM binario, transforma geométricamente |
| E4 | POO avanzado | Filtros polimórficos, operadores, Factory, excepciones |

---

## 8. 🖼️ Ejemplo de datos hardcodeados (E1)

```cpp
// main.cpp
int ancho = 20, alto = 10;
Imagen degradado(ancho, alto);
for(int f = 0; f < alto; f++)
    for(int c = 0; c < ancho; c++) {
        unsigned char v = (f * 255 / alto + c * 255 / ancho) / 2;
        degradado.setPixel(f, c, {v, v, v});
    }

Imagen ajedrez(8, 8);
for(int f = 0; f < 8; f++)
    for(int c = 0; c < 8; c++) {
        if((f + c) % 2 == 0)
            ajedrez.setPixel(f, c, {255, 255, 255}); // blanco
        else
            ajedrez.setPixel(f, c, {0, 0, 0});       // negro
    }
```

---

## 9. 🔍 Verificación con Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all ./programa
```

Salida esperada:
```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
All heap blocks were freed -- no leaks are possible
```

---

## 10. 📐 Reparto de trabajo sugerido (equipo de 3-4 personas)

| Persona | E1 | E2 | E3 | E4 |
|---------|----|----|----|----|
| **A** | `Imagen` + Pixel | `GestorPPMTexto` (carga) | `GestorPPMBinario` (guardado) | Refactor `ImagenColor` + `ImagenBase` |
| **B** | `Filtro` (funciones) + mostrarASCII | `GestorPPMTexto` (guardado) | `GestorPPMBinario` (carga) | Jerarquía `Filtro` + `ConvolucionFiltro` |
| **C** | `main` + menú + Makefile | Generar pruebas + menú | Transformaciones geométricas | `GestorArchivo` interfaz + `ImagenFactory` |
| **D** | Pruebas + patrón ajedrez/degradado | Validación errores | Verificación integridad | Excepciones + namespaces + operadores |
