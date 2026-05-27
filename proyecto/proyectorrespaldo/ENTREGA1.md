# Entrega 1 — Punteros y Memoria Dinámica

## 🎯 Objetivo

Implementar un **gestor de paquetes/archivos** en memoria que permita almacenar, listar y extraer "archivos" simulados (buffers de bytes) usando **exclusivamente** punteros y memoria dinámica. Sin ficheros reales en disco.

**Temas evaluados:** punteros, buffers dinámicos (`char*`), arrays dinámicos de estructuras, constructores copia, destructores, `operator=`, deep copy.

---

## 📋 Especificaciones

### 1. Representación de un archivo en memoria

```cpp
struct Entrada {
    char* nombre;       // Nombre del archivo (cadena dinámica)
    char* datos;        // Contenido del archivo (buffer dinámico)
    int tamano;         // Tamaño en bytes
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
- **`operator=`** (deep copy)
- **Destructor** (libera todas las entradas y sus contenidos)
- `agregar(const char* nombre, const char* datos, int tamano)`: añade un archivo al paquete (copia profunda del nombre y los datos)
- `extraer(int indice)`: devuelve puntero a `Entrada` o `nullptr`
- `eliminar(int indice)`: elimina una entrada y compacta
- `listar()`: muestra nombre y tamaño de cada archivo
- `getNumEntradas()`: devuelve cuántos archivos hay

### 3. Clase `Compresor` (versión procedural, sin herencia)

```cpp
class Compresor {
public:
    static char* comprimirRLE(const char* datos, int tamOriginal, int& tamComprimido);
    static char* descomprimirRLE(const char* datos, int tamComprimido, int& tamOriginal);
};
```

**RLE simple:** secuencias del mismo byte se codifican como `[byte][contador]`.
- Si un byte aparece 3+ veces seguidas: `[byte][0xFF][contador]`
- Si aparece 1-2 veces: se deja igual

Ejemplo: `AAAAABBBCC` → `A[0xFF][5]B[0xFF][3]CC`

### 4. Función `main` — Experiencia de usuario

#### 🎮 ¿Qué hace el usuario?

El programa arranca con **archivos hardcodeados** creados en memoria:

- `documento.txt`: "Hola mundo, este es un archivo de prueba."
- `datos.bin`: 100 bytes con valores incrementales (0,1,2...99)
- `imagen.img`: 50 bytes con patrón alternado (0xAA, 0x55)

#### Flujo completo:

```
$ ./programa

=== GESTOR DE PAQUETES (MEMORIA) ===
1. Listar contenido del paquete
2. Extraer y ver archivo
3. Eliminar archivo del paquete
4. Comprimir archivo (RLE)
5. Descomprimir archivo (RLE)
6. Salir
> 1

--- CONTENIDO DEL PAQUETE ---
Índice | Nombre           | Tamaño (bytes)
-------|------------------|---------------
0      | documento.txt    | 41
1      | datos.bin        | 100
2      | imagen.img       | 50

Total: 3 archivos (191 bytes)
```

Al **extraer y ver** un archivo:

```
> 2

Índice del archivo: 0

--- documento.txt (41 bytes) ---
Hola mundo, este es un archivo de prueba.
```

Al **comprimir**:

```
> 4

Índice del archivo: 1

Comprimiendo datos.bin con RLE...
Original: 100 bytes
Comprimido: 68 bytes
Ratio: 68.0%
```

Al **listar después de comprimir**:

```
Índice | Nombre                  | Tamaño (bytes)
-------|-------------------------|---------------
0      | documento.txt           | 41
1      | datos.bin               | 68   ← comprimido
2      | datos.bin.rle           | 68   ← archivo nuevo
3      | imagen.img              | 50
```

#### ‼️ Limitaciones de E1:
- No hay persistencia en disco
- Los archivos están hardcodeados en el código
- No se pueden importar archivos reales del sistema
- El paquete existe solo en memoria RAM
