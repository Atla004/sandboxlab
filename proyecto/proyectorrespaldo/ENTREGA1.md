# Entrega 1 — Punteros y Memoria Dinámica

## 🎯 Objetivo

Implementar un **gestor de paquetes/archivos** en memoria que permita almacenar, listar, extraer, comprimir y verificar la integridad de "archivos" simulados (buffers de bytes) usando **exclusivamente** punteros y memoria dinámica. Sin ficheros reales en disco.

**Temas evaluados:** punteros, buffers dinámicos (`char*`), arrays dinámicos de estructuras, constructores copia, destructores, `operator=`, deep copy.

---

## 📋 Especificaciones

### 1. Representación de un archivo en memoria

```cpp
struct Entrada {
    char* nombre;       // Nombre del archivo (cadena dinámica)
    char* datos;        // Contenido del archivo (buffer dinámico)
    int tamano;         // Tamaño en bytes
    unsigned int checksum; // Checksum para verificar integridad
};
```

### 2. Clase `Paquete`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `entradas` | `Entrada*` | Array dinámico de entradas |
| `numEntradas` | `int` | Número de archivos en el paquete |
| `capacidad` | `int` | Capacidad máxima del array |

Métodos obligatorios:
- Constructor por defecto (paquete vacío)
- **Constructor de copia** (deep copy)
- **`operator=`** (deep copy, asignación segura)
- **Destructor** (libera todas las entradas y sus contenidos)
- `agregar(const char* nombre, const char* datos, int tamano)`: añade un archivo al paquete (copia profunda del nombre y los datos)
- `extraer(int indice)`: devuelve puntero a `Entrada` o `nullptr`
- `eliminar(int indice)`: elimina una entrada y compacta el array
- `listar()`: muestra nombre, tamaño y checksum de cada archivo
- `buscar(const char* patron)`: busca archivos cuyo nombre contenga el patron (case-insensitive), devuelve un array dinámico de índices encontrados
- `tamanoTotal()`: suma los tamaños de todos los archivos
- `getNumEntradas()`: devuelve cuántos archivos hay
- `actualizarChecksum(int indice)`: recalcula el checksum de una entrada

### 3. Clase `Compresor` (versión procedural, sin herencia)

```cpp
class Compresor {
public:
    static char* comprimirRLE(const char* datos, int tamOriginal, int& tamComprimido);
    static char* descomprimirRLE(const char* datos, int tamComprimido, int& tamOriginal);
    static float ratioCompresion(int tamOriginal, int tamComprimido);
};
```

**RLE simple:** secuencias del mismo byte se codifican como `[byte][0xFF][contador]`.
- Si un byte aparece 3+ veces seguidas: `[byte][0xFF][contador]`
- Si aparece 1-2 veces: se deja igual

Ejemplo: `AAAAABBBCC` → `A[0xFF][5]B[0xFF][3]CC`

### 4. Clase `Integridad`

```cpp
class Integridad {
public:
    static unsigned int calcularChecksum(const char* datos, int tamano);
    static bool verificar(const Entrada& entrada);
    static char* cifrarXOR(const char* datos, int tamano, char clave, int& tamSalida);
    static char* descifrarXOR(const char* datos, int tamano, char clave, int& tamSalida);
    static bool comparar(const Entrada& e1, const Entrada& e2);
};
```

- **calcularChecksum**: suma acumulativa con rotación: `checksum = (checksum << 5) + byte`
- **verificar**: compara el checksum almacenado con el calculado, devuelve `true` si coinciden
- **cifrarXOR**: aplica XOR a cada byte con la clave dada (el tamaño no cambia)
- **descifrarXOR**: misma operacion que cifrar (XOR es simétrico)
- **comparar**: dos entradas son iguales si tienen el mismo checksum y mismo tamaño

### 5. Clase `GestorPaquetes`

Maneja múltiples paquetes simultáneamente.

| Método | Descripción |
|--------|-------------|
| `GestorPaquetes(int maxPaquetes)` | Constructor, crea array dinámico de paquetes |
| `~GestorPaquetes()` | Destructor, libera todos los paquetes |
| `crearPaquete(const char* nombre)` | Crea un nuevo paquete vacío |
| `fusionar(int idx1, int idx2)` | Crea un nuevo paquete con todas las entradas de ambos |
| `separar(int idxOrigen, int* indices, int numIndices)` | Crea un nuevo paquete solo con las entradas seleccionadas |
| `buscarEnTodos(const char* patron)` | Busca un patron en todos los paquetes, devuelve resultados con su ubicación |
| `listarTodo()` | Muestra todos los paquetes y su contenido |
| `eliminarPaquete(int indice)` | Elimina un paquete y compacta el array |

### 6. Función `main` — Experiencia de usuario

#### 🎮 ¿Qué hace el usuario?

El programa arranca con **2 paquetes pre-creados** con archivos hardcodeados en memoria:

- **Paquete A** (documentos):
  - `documento.txt`: "Hola mundo, este es un archivo de prueba."
  - `datos.bin`: 100 bytes con valores incrementales (0,1,2...99)
  - `imagen.img`: 50 bytes con patrón alternado (0xAA, 0x55)

- **Paquete B** (respaldo):
  - `notas.txt`: "Esto es una nota de respaldo."
  - `datos.bin`: copia idéntica al de Paquete A (para probar detección de duplicados)

#### Flujo completo:

```
$ ./programa

=== GESTOR DE PAQUETES (MEMORIA) ===
1. Ver todos los paquetes
2. Ver contenido de un paquete
3. Extraer y ver archivo
4. Eliminar archivo del paquete
5. Buscar archivo por nombre
6. Comprimir archivo (RLE)
7. Descomprimir archivo (RLE)
8. Cifrar archivo (XOR)
9. Verificar integridad de archivo
10. Fusionar dos paquetes
11. Comparar archivos entre paquetes
12. Salir
> 1

--- PAQUETES DISPONIBLES ---
Índice | Nombre        | Archivos | Tamaño total
-------|---------------|----------|-------------
0      | documentos    | 3        | 191 bytes
1      | respaldo      | 2        | 78 bytes

Total: 2 paquetes (269 bytes)
```

Al **buscar** un archivo:

```
> 5

Buscar: datos
Resultados:
  Paquete [0] documentos: datos.bin (100 bytes)
  Paquete [1] respaldo: datos.bin (100 bytes)

Se encontraron 2 coincidencias.
```

Al **verificar integridad**:

```
> 9

Índice del paquete: 0
Índice del archivo: 0

--- documento.txt ---
Checksum almacenado:  0xA3F2B1C0
Checksum calculado:   0xA3F2B1C0
Estado: INTEGRO
```

Al **fusionar paquetes**:

```
> 10

Fusionando paquete 0 (documentos) + paquete 1 (respaldo)...
Nuevo paquete creado: fusion_documentos_respaldo
Contiene 5 archivos (269 bytes)
```

Al **comparar archivos** (detecta duplicados):

```
> 11

Buscando archivos duplicados entre paquetes...
Encontrado: datos.bin en paquete [0] == datos.bin en paquete [1]
Checksum: 0xB7E4D2A1 | Tamaño: 100 bytes
```

#### ‼️ Limitaciones de E1:
- No hay persistencia en disco
- Los archivos están hardcodeados en el código
- No se pueden importar archivos reales del sistema
- Los paquetes existen solo en memoria RAM

---

## 🧠 Requisitos técnicos obligatorios

- **No usar `std::string`** en atributos de clases (solo `char*`)
- **No usar `std::vector`** ni contenedores STL
- **No usar ficheros** (ni lectura ni escritura)
- Toda memoria dinámica debe liberarse correctamente (sin leaks)
- Constructor copia, `operator=` y destructor implementados **explícitamente**
- Deep copy en todas las copias

---

## 📁 Estructura de archivos esperada

```
include/
  Entrada.h
  Paquete.h
  Compresor.h
  Integridad.h
  GestorPaquetes.h
src/
  main.cpp
  Paquete.cpp
  Compresor.cpp
  Integridad.cpp
  GestorPaquetes.cpp
Makefile
```

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Funcionalidad (menú, compresión, integridad, gestor) | 30% |
| Gestión correcta de memoria dinámica (sin leaks, sin doble delete) | 25% |
| Constructor copia, `operator=` y destructor correctos | 20% |
| Deep copy vs shallow copy | 10% |
| Calidad del código (nombres, indentación, comentarios) | 10% |
| Makefile funcional | 5% |

---

## 🚀 Cómo compilar y ejecutar

```bash
make entrega1
./programa
```

O manualmente:

```bash
g++ -std=c++11 -I include src/*.cpp -o programa -Wall -Wextra
./programa
```

---

## 📤 Entrega

- Subir a repositorio GitHub en una carpeta `Entrega1/`
- Incluir `Makefile` funcional
- El programa debe compilar sin warnings con `-Wall -Wextra`
- Incluir captura de pantalla del programa funcionando
