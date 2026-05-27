# Especificación Técnica — Gestor de Paquetes / Respaldos

Este documento detalla las especificaciones técnicas concretas que deben seguir **todas las entregas**. Es el contrato de diseño del proyecto.

---

## 1. 📦 Estructura de entrada

```cpp
struct Entrada {
    char* nombre;
    char* datos;
    int tamano;
};
```

Tanto `nombre` como `datos` son **cadenas/buffers dinámicos** asignados con `new char[]`.

---

## 2. 🗜️ Compresión RLE

### RLE básico (E1)
- Secuencias de 3+ bytes iguales: `[byte][0xFF][contador]`
- Secuencias de 1-2 bytes: se dejan igual
- Ejemplo: `AAAAABBBCC` → `A[0xFF][5]B[0xFF][3]CC`

### RLE Mejorado (E4)
- Usa contador de 2 bytes (little-endian) para secuencias > 255
- Marcador: `[0x00]` (byte de escape)
- Formato: `[0x00][byte][contador_2bytes]`
- Ejemplo: `5000xA` → `[0x00]['A'][0x1388]`

---

## 3. 💾 Formato binario .pkg (E3)

### Cabecera (16 bytes)
```
Offset | Tamaño | Campo
0      | 4      | Magic: "PKG1" (0x504B4731)
4      | 4      | Versión: int (actual: 1)
8      | 4      | Nº entradas: int
12     | 4      | Tamaño índice: int (bytes)
```

### Índice
Por cada entrada:
```
4 bytes: longitud del nombre (int)
N bytes: nombre (char*, sin '\0' añadido)
4 bytes: tamaño de datos (int)
4 bytes: offset de datos desde inicio archivo (int)
```

### Datos
Los datos de cada entrada se concatenan secuencialmente.

### Checksum
XOR byte a byte desde el primer byte de la cabecera hasta el último byte de datos.

---

## 4. 📄 Formato texto .txt / .csv (E2)

### índice.csv
```
# NOMBRE,TAMAÑO,TIPO
foto.jpg,45200,imagen
documento.txt,280,texto
```

### paquete.txt
```
=== PAQUETE: nombre ===
Fecha: DD/MM/AAAA
Archivos: N

--- INICIO ARCHIVO: nombre_archivo ---
Tamaño: X bytes
Contenido:
... datos textuales o hex ...
--- FIN ARCHIVO: nombre_archivo ---
```

Para archivos binarios, el contenido se muestra en **hexadecimal**:
```
Contenido (hex):
00 01 02 03
0A 0B 0C 0D
```

---

## 5. ⚙️ Compresión

### Algoritmos disponibles

| Algoritmo | Ratio típico | Velocidad | Cuándo usar |
|-----------|-------------|-----------|-------------|
| `CompresionNula` | 100% | Instantánea | Archivos ya comprimidos (jpg, zip) |
| `CompresionRLE` | 60-80% | Rápida | Archivos con patrones repetitivos (imágenes simples, datos binarios) |
| `CompresionRLEMejorado` | 50-75% | Rápida | Archivos con secuencias largas (>255 repeticiones) |

---

## 6. 🏗️ Normas de codificación

| Norma | Detalle |
|-------|---------|
| Lenguaje | C++11 o superior |
| Compilación | `g++ -std=c++11 -Wall -Wextra -I include` |
| Prohibido | `std::string` como atributo, `std::vector`, librerías externas |
| Memoria | Cada `new[]` con su `delete[]`, cada `new` con su `delete` |
| Nombres | `snake_case` funciones/variables, `PascalCase` clases |
| Archivos | Un `.h` y un `.cpp` por clase principal |
| Makefile | Targets: `entrega1`, `entrega2`, `entrega3`, `entrega4`, `clean` |
| Lectura ficheros (E3+) | `ifstream` con `ios::binary` para archivos reales. En E2 solo modo texto (`getline`, `<<`, `>>`) |

---

## 7. 📤 Entregas y evaluación progresiva

| Entrega | Temas | El usuario... |
|---------|-------|---------------|
| E1 | Punteros y memoria dinámica | Crea paquetes en memoria con datos hardcodeados |
| E2 | Ficheros de texto | Importa archivos reales, exporta a CSV/TXT |
| E3 | Ficheros binarios | Guarda/carga .pkg binario con índice y checksum |
| E4 | POO avanzado | Compresión polimórfica, operadores, Factory, excepciones |

---

## 8. 🧪 Ejemplo de datos hardcodeados (E1)

```cpp
// main.cpp
Paquete pkg;

const char* contenido1 = "Hola mundo, este es un archivo de prueba.";
pkg.agregar("documento.txt", contenido1, strlen(contenido1) + 1);

char* datosBin = new char[100];
for(int i = 0; i < 100; i++) datosBin[i] = i;
pkg.agregar("datos.bin", datosBin, 100);
delete[] datosBin;

char* datosImg = new char[50];
for(int i = 0; i < 50; i += 2) {
    datosImg[i] = 0xAA;
    datosImg[i+1] = 0x55;
}
pkg.agregar("imagen.img", datosImg, 50);
delete[] datosImg;
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
| **A** | `Entrada` + array dinámico | `GestorTexto` (exportar) | `GestorBinario` (guardado) | Refactor `Entrada` + `Paquete` |
| **B** | `CompresorRLE` + pruebas | `GestorTexto` (importar) | `GestorBinario` (carga) | Jerarquía `AlgoritmoCompresion` |
| **C** | `main` + menú + Makefile | Importar/extraer archivos | Magic number + checksum | `GestorPersistencia` interfaz + operadores |
| **D** | Tests manuales + datos hardcode | Validación errores + CSV | Extracción rápida + listado rápido | Excepciones + Factory + namespaces |
