# Entrega 1 — Punteros y Memoria Dinámica

## 🎯 Objetivo

Implementar un editor de imágenes en consola que trabaje con el formato **PPM (Portable Pixel Map)** utilizando **exclusivamente** punteros, memoria dinámica (`new`/`delete`) y arrays bidimensionales dinámicos. Sin ficheros externos.

**Temas evaluados:** punteros, punteros dobles (matriz dinámica), memoria dinámica, constructores copia, destructores, `operator=`, deep copy.

---

## 📋 Especificaciones

### 1. Representación de la imagen

Cada píxel tiene 3 componentes (RGB):

```cpp
struct Pixel {
    unsigned char r; // Rojo (0-255)
    unsigned char g; // Verde (0-255)
    unsigned char b; // Azul (0-255)
};
```

La imagen se representa como una **matriz dinámica** de píxeles:

```cpp
Pixel** pixeles; // [filas][columnas]
int ancho;
int alto;
```

### 2. Clase `Imagen`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `pixeles` | `Pixel**` | Matriz bidimensional dinámica |
| `ancho` | `int` | Ancho en píxeles |
| `alto` | `int` | Alto en píxeles |

Métodos obligatorios:
- Constructor por defecto (imagen 1x1 negra)
- Constructor con parámetros (ancho, alto, color de relleno opcional)
- **Constructor de copia** (deep copy)
- **`operator=`** (deep copy, asignación segura)
- **Destructor** (libera toda la memoria)
- `getPixel(int fila, int col)`: devuelve el píxel en esa posición
- `setPixel(int fila, int col, Pixel color)`: modifica un píxel
- `mostrarASCII()`: imprime la imagen en consola usando caracteres de densidad
- `rellenar(Pixel color)`: llena toda la imagen de un color

### 3. Visualización ASCII

La imagen se muestra en consola mapeando brillo a caracteres:

```
@%#*+=-:.       (caracteres de más brillante a menos brillante)
```

Fórmula de brillo de un píxel: `brillo = (r + g + b) / 3`

Ejemplo de salida para una imagen 10x5:
```
@@@@@%%%%#*+=-:.
@@@@@%%%%#*+=-:.
@@@@@%%%%#*+=-:.
@@@@@%%%%#*+=-:.
@@@@@%%%%#*+=-:.
```

### 4. ¿Cómo se manejan las imágenes si no hay ficheros?

En E1 las imágenes **no vienen de ningún archivo**. Se crean directamente en memoria usando bucles que rellenan píxel a píxel:

```cpp
// Crear imagen degradado 20x10: más claro en esquina sup-izq, más oscuro en inf-der
Imagen degradado(20, 10);
for(int f = 0; f < 10; f++) {
    for(int c = 0; c < 20; c++) {
        unsigned char brillo = (f * 255 / 10 + c * 255 / 20) / 2;
        degradado.setPixel(f, c, {brillo, brillo, brillo}); // escala de grises
    }
}

// Crear imagen ajedrez 8x8: cuadrados blancos y negros alternados
Imagen ajedrez(8, 8);
for(int f = 0; f < 8; f++) {
    for(int c = 0; c < 8; c++) {
        if((f + c) % 2 == 0)
            ajedrez.setPixel(f, c, {255, 255, 255}); // blanco
        else
            ajedrez.setPixel(f, c, {0, 0, 0});       // negro
    }
}
```

**Flujo de visualización sin ficheros:**
```
Memoria (Pixel**)  →  mostrarASCII()  →  Consola

[Píxeles en RAM]  →  [brillo→carácter]  →  [@@@%%%%###...]
```

1. Los píxeles existen solo como `Pixel**` (matriz dinámica en RAM)
2. `mostrarASCII()` recorre la matriz, calcula brillo = `(r+g+b)/3` y lo mapea a caracteres `@%#*+=-:.`
3. Imprime fila por fila en la consola
4. Cuando se cierra el programa, toda la memoria se libera con `delete[]`

**Ningún archivo externo interviene** — todo existe y muere dentro de la ejecución del programa.

### 5. Clase `Filtro` (versión procedural, sin herencia aún)

Métodos estáticos que trabajan sobre `Imagen`:

```cpp
class Filtro {
public:
    static void invertir(Imagen& img);
    static void brillo(Imagen& img, int ajuste);   // +- brillo
    static void umbral(Imagen& img, int limite);    // blanco o negro según umbral
    static void mezclar(Imagen& img1, const Imagen& img2, float peso);
};
```

- **invertir**: cada canal se convierte en `255 - valor`
- **brillo**: suma/resta a cada canal, saturando entre 0 y 255
- **umbral**: si brillo < límite → negro, si no → blanco
- **mezclar**: combina dos imágenes (alpha blending)

### 6. Función `main` — Experiencia de usuario

#### 🎮 ¿Qué hace el usuario?

El programa arranca con **2 imágenes hardcodeadas** creadas en código:

- **Imagen A**: degradado de 20x10 (esquina sup-izq clara a inf-der oscura)
- **Imagen B**: patrón de ajedrez de 8x8 (blanco y negro alternados)

#### Flujo completo:

```
$ ./programa

=== EDITOR DE IMÁGENES PPM ===
1. Ver imagen A
2. Ver imagen B
3. Aplicar filtro a imagen A
4. Aplicar filtro a imagen B
5. Mezclar imágenes A y B
6. Salir
> 1

--- IMAGEN A (20x10) ---
@@@@@%%%%#*+=-:.@@@@@%%%%
%%%#*+=-:.@@@@@%%%%#*+=-:
.@@@@@%%%%#*+=-:.@@@@@%%%
#*+=-:.@@@@@%%%%#*+=-:.@@
@@%%%%#*+=-:.@@@@@%%%%#*+
=-:.@@@@@%%%%#*+=-:.@@@@@
%%#*+=-:.@@@@@%%%%#*+=-:.
@@@@@%%%%#*+=-:.@@@@@%%%%
*+=-:.@@@@@%%%%#*+=-:.@@@
@%%%%#*+=-:.@@@@@%%%%#*+=

Presiona Enter para volver...
```

Al elegir **aplicar filtro**:

```
--- FILTROS DISPONIBLES ---
1. Invertir colores
2. Ajustar brillo (+50)
3. Ajustar brillo (-50)
4. Umbral (blanco y negro)
> 1

Filtro aplicado: Invertir colores
Mostrando resultado... (Enter para volver)
```

Al mezclar imágenes:

```
> 5

Mezclando A + B al 50%...
Imagen mezclada de 20x10 creada.

1. Ver resultado
2. Volver
```

#### ‼️ Resumen visual del flujo de E1:

```
                    ┌─────────────────────┐
                    │  PROGRAMA INICIA    │
                    │                     │
                    │  new Pixel**        │
                    │  Bucles: rellenar   │
                    │  píxel a píxel      │
                    └────────┬────────────┘
                             │
                             ▼
                    ┌─────────────────────┐
                    │  Imagen en memoria   │
                    │  (matriz Pixel**)    │
                    └────────┬────────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
     ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
     │ Ver en ASCII│ │Aplicar filtro│ │ Mezclar     │
     │ (consola)   │ │ (modifica   │ │ (crea nueva │
     │             │ │  la matriz) │ │  matriz)    │
     └─────────────┘ └─────────────┘ └─────────────┘
                             │
                             ▼
                    ┌─────────────────────┐
                    │  PROGRAMA TERMINA   │
                    │  delete[] pixeles   │
                    │  Todo se pierde     │
                    └─────────────────────┘
```

#### ‼️ Limitaciones de E1:
- No se pueden cargar imágenes reales (solo las hardcodeadas)
- No se puede guardar el resultado a un archivo
- Al cerrar el programa, todo se pierde
- No hay formato PPM real todavía
