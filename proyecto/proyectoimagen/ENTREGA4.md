# Entrega 4 — POO Estructura y Diseño

## 🎯 Objetivo

Refactorizar **todo el código** de las entregas anteriores aplicando principios avanzados de POO con especial énfasis en **herencia y polimorfismo aplicados a filtros y gestores de archivo**. La funcionalidad visible se amplía con nuevos filtros y operadores.

---

## 🎮 Experiencia de usuario — ¿Qué cambia para el jugador?

El usuario percibe los siguientes cambios visibles respecto a E3:

### 1. Sistema de filtros polimórfico (catálogo extensible)

En E1-E3 los filtros eran funciones estáticas. En E4 son **objetos** que se pueden listar, combinar y aplicar en cadena:

```
--- SELECCIONA FILTRO ---
1. Invertir colores (InvertirFiltro)
2. Blanco y negro (BinarizarFiltro)
3. Brillo (+50) (BrilloFiltro)
4. Sepia (SepiaFiltro) ← NUEVO
5. Gauss 3x3 (GaussFiltro) ← NUEVO
6. Detectar bordes (SobelFiltro) ← NUEVO
7. Aplicar filtro personalizado en cadena ← NUEVO
> 7

--- CADENA DE FILTROS ---
Filtros disponibles: Invertir, BN, Brillo, Sepia, Gauss
Orden de aplicación (separados por coma): BN, Sepia, Brillo

Aplicando: BN → Sepia → Brillo...
Hecho. Imagen transformada.
```

### 2. Nuevos filtros avanzados

| Filtro | Efecto |
|--------|--------|
| `SepiaFiltro` | Tono sepia clásico fotográfico |
| `GaussFiltro` | Desenfoque gaussiano 3x3 |
| `SobelFiltro` | Detección de bordes (gradiente) |
| `PixeladoFiltro` | Efecto mosaico |

### 3. Operadores sobrecargados

```cpp
Imagen a(100, 100, rojo);
Imagen b(100, 100, azul);
Imagen c = a + b;           // Mezcla al 50%
Imagen d = a * 1.5;         // Escala brillo x1.5
Imagen e = -a;              // Negativo
cout << a;                  // Muestra info de la imagen
a[10][5] = pixelAzul;       // Acceso con doble operator[]
Pixel p = a(10, 5);         // Acceso con operator()
```

### 4. Clases refactorizadas

**`class Imagen`** con herencia:
```cpp
class ImagenBase {
protected:
    int ancho, alto;
public:
    virtual ~ImagenBase() {}
    virtual void mostrar() const = 0;
    virtual ImagenBase* clonar() const = 0;
};

class ImagenColor : public ImagenBase {
    Pixel** pixeles;
    // ... implementación completa
};

class ImagenGris : public ImagenBase {  // opcional
    unsigned char** grises;
    // ... versión en escala de grises (1 canal)
};
```

**`class Filtro`** con herencia polimórfica:
```cpp
class Filtro {
protected:
    char nombre[50];
public:
    virtual ~Filtro() {}
    virtual void aplicar(ImagenColor& img) = 0;
    virtual const char* getNombre() const;
};

class InvertirFiltro : public Filtro { ... };
class BinarizarFiltro : public Filtro { ... };
class BrilloFiltro : public Filtro { ... };
class SepiaFiltro : public Filtro { ... };
class ConvolucionFiltro : public Filtro {   // base para filtros de matriz
protected:
    float kernel[3][3];
public:
    ConvolucionFiltro(const float k[3][3]);
};
class GaussFiltro : public ConvolucionFiltro { ... };
class SobelFiltro : public ConvolucionFiltro { ... };
```

**`class GestorArchivo`** interfaz polimórfica:
```cpp
class GestorArchivo {
public:
    virtual ~GestorArchivo() {}
    virtual bool guardar(const ImagenColor& img, const char* path) = 0;
    virtual ImagenColor* cargar(const char* path) = 0;
    virtual const char* formato() const = 0;
};

class GestorPPMTexto : public GestorArchivo { ... };   // P3
class GestorPPMBinario : public GestorArchivo { ... };  // P6
```

Uso:
```cpp
GestorArchivo* gestor = nullptr;
if (formato == "P3")
    gestor = new GestorPPMTexto();
else
    gestor = new GestorPPMBinario();
gestor->guardar(img, "salida.ppm");
delete gestor;
```

### 5. Namespaces

```cpp
namespace ImagenEditor {
    namespace Core {
        class ImagenColor;
        class Pixel;
    }
    namespace Filtros {
        class Filtro;
        class InvertirFiltro;
        class BrilloFiltro;
        class SepiaFiltro;
        class ConvolucionFiltro;
    }
    namespace IO {
        class GestorArchivo;
        class GestorPPMTexto;
        class GestorPPMBinario;
    }
    namespace Exceptions {
        class ErrorImagen;
        class ErrorCarga;
        class ErrorDimension;
    }
}
```

### 6. Excepciones personalizadas

```cpp
class ErrorImagen {
    char* mensaje;
public:
    ErrorImagen(const char* msg);
    virtual const char* what() const;
    virtual ~ErrorImagen();
};

class ErrorCarga : public ErrorImagen {
    const char* archivo;
    // ...
};

class ErrorDimension : public ErrorImagen { ... };
class ErrorFormato : public ErrorImagen { ... };
```

**Uso obligatorio:**
- Cargar un archivo que no existe → `ErrorCarga`
- Redimensionar a dimensiones negativas → `ErrorDimension`
- Cargar un archivo con cabecera inválida → `ErrorFormato`

### 7. Factory Method para crear imágenes de prueba

```cpp
class ImagenFactory {
public:
    static ImagenColor* crearDegradado(int w, int h);
    static ImagenColor* crearAjedrez(int size);
    static ImagenColor* crearBandera(const char* pais);
    static ImagenColor* crearSolido(int w, int h, Pixel color);
    static ImagenColor* desdeArchivo(const char* path); // auto-detecta P3/P6
};
```

### 8. Estructura de archivos

```
include/
  Core/
    ImagenBase.h
    ImagenColor.h
    Pixel.h
  Filtros/
    Filtro.h
    InvertirFiltro.h
    BrilloFiltro.h
    SepiaFiltro.h
    BinarizarFiltro.h
    ConvolucionFiltro.h
    GaussFiltro.h
    SobelFiltro.h
  IO/
    GestorArchivo.h
    GestorPPMTexto.h
    GestorPPMBinario.h
    UI.h
  Exceptions/
    ErrorImagen.h
  Factory/
    ImagenFactory.h
src/
  Core/
    ImagenColor.cpp
  Filtros/
    Filtro.cpp
    InvertirFiltro.cpp
    BrilloFiltro.cpp
    SepiaFiltro.cpp
    BinarizarFiltro.cpp
    ConvolucionFiltro.cpp
    GaussFiltro.cpp
    SobelFiltro.cpp
  IO/
    GestorPPMTexto.cpp
    GestorPPMBinario.cpp
    UI.cpp
  Exceptions/
    ErrorImagen.cpp
  Factory/
    ImagenFactory.cpp
  main.cpp
Makefile
```

---

## 🔄 Cambios vs E3

| Aspecto | E3 | E4 |
|---------|----|----|
| Filtros | Funciones sueltas | Jerarquía polimórfica `Filtro` |
| Gestores | Clases independientes | Interfaz `GestorArchivo` |
| Operadores | ❌ | `+`, `*`, `-`, `<<`, `[]`, `()` |
| Excepciones | if/else | `ErrorImagen`, `ErrorCarga`, etc. |
| Namespaces | ❌ | `ImagenEditor::Core`, `::Filtros`, `::IO` |
| Creación | Manual | `ImagenFactory` |
| Nuevos filtros | ❌ | Sepia, Gauss, Sobel, Pixelado |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Jerarquía `Filtro` con polimorfismo | 20% |
| Interfaz `GestorArchivo` con herencia | 15% |
| Sobrecarga de operadores (mín. 4) | 15% |
| Excepciones personalizadas | 15% |
| Factory Method | 10% |
| E1-E3 siguen funcionando | 10% |
| Namespaces y estructura modular | 10% |
| Calidad del código | 5% |

---

## 🚀 Compilación

```bash
make entrega4
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega4/`
- Incluir `DIAGRAMA.md` con UML de clases
- Las imágenes PPM generadas deben poder verse en cualquier visor compatible
- Demostrar cadena de filtros y operadores sobrecargados
