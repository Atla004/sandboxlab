# Entrega 4 — POO Estructura y Diseño

## 🎯 Objetivo

Refactorizar **todo el código** de las entregas anteriores aplicando principios avanzados de POO con especial énfasis en **interfaces de compresión y persistencia, manejo de excepciones, y operadores sobrecargados**. La funcionalidad visible se amplía con nuevos algoritmos de compresión y un sistema de plugins simple.

---

## 🎮 Experiencia de usuario — ¿Qué cambia para el jugador?

### 1. Sistema de compresión extensible

En E1-E3 solo había RLE. En E4 se pueden elegir entre múltiples algoritmos:

```
> 7 (Comprimir archivo)

Índice del archivo: 1

--- MÉTODOS DE COMPRESIÓN ---
1. Sin compresión (copiar)
2. RLE (Run-Length Encoding)
3. RLE Mejorado (RLE con marcador de longitud) ← NUEVO
4. Comprobar cuál comprime más (automático)    ← NUEVO
> 4

Probando métodos...
- Sin compresión: 100 bytes
- RLE básico: 68 bytes
- RLE mejorado: 62 bytes ← GANADOR

Aplicando RLE mejorado...
Comprimido: 100 → 62 bytes (62%)
```

### 2. Operadores sobrecargados

```cpp
Paquete p1;
p1.agregar("a.txt", "Hola", 4);
p1.agregar("b.txt", "Mundo", 5);

Paquete p2;
p2.agregar("c.txt", "Hello", 5);

Paquete total = p1 + p2;        // Fusionar dos paquetes
cout << total;                   // Mostrar info completa del paquete
total += entradaNueva;           // Añadir entrada con +=
Entrada* e = total[0];           // Acceder por índice con []
cout << total[0];                // Mostrar entrada individual
total - "a.txt";                 // Eliminar archivo por nombre con operador -

if (total > 1024)               // Comparar tamaño total
    cout << "Paquete grande!";
```

### 3. Clases refactorizadas

**`class Entrada`**:
```cpp
class Entrada {
    char* nombre;
    char* datos;
    int tamano;
public:
    Entrada(const char* nombre, const char* datos, int tamano);
    Entrada(const Entrada& otra);    // deep copy
    ~Entrada();
    Entrada& operator=(const Entrada& otra);
    friend ostream& operator<<(ostream& os, const Entrada& e);
    // Getters, setters
};
```

**`class Paquete`** con iterador interno:
```cpp
class Paquete {
    Entrada** entradas;  // Array de punteros
    int numEntradas;
    int capacidad;
public:
    Paquete();
    Paquete(const Paquete& otro);
    ~Paquete();
    Paquete& operator=(const Paquete& otro);

    void agregar(const Entrada& e);
    bool eliminar(const char* nombre);
    Entrada* extraer(int indice);

    Paquete operator+(const Paquete& otro) const;  // Fusión
    Paquete& operator+=(const Entrada& e);          // Añadir
    Entrada* operator[](int indice) const;          // Acceso
    Paquete operator-(const char* nombre) const;    // Sin ese archivo
    bool operator>(int tamano) const;                // Comparar tamaño
    friend ostream& operator<<(ostream& os, const Paquete& p);
    friend istream& operator>>(istream& is, Paquete& p);  // Cargar desde texto
};
```

### 4. Interfaz de compresión polimórfica

```cpp
class AlgoritmoCompresion {
protected:
    char nombre[50];
public:
    virtual ~AlgoritmoCompresion() {}
    virtual char* comprimir(const char* datos, int tamIn, int& tamOut) = 0;
    virtual char* descomprimir(const char* datos, int tamIn, int& tamOut) = 0;
    virtual const char* getNombre() const;
};

class CompresionRLE : public AlgoritmoCompresion { ... };
class CompresionRLEMejorado : public AlgoritmoCompresion {  // RLE con contador de 2 bytes
    char* comprimir(const char* datos, int tamIn, int& tamOut) override;
    char* descomprimir(const char* datos, int tamIn, int& tamOut) override;
};
class CompresionNula : public AlgoritmoCompresion {  // Sin compresión
    char* comprimir(const char* datos, int tamIn, int& tamOut) override;
    char* descomprimir(const char* datos, int tamIn, int& tamOut) override;
};
```

Uso polimórfico:
```cpp
AlgoritmoCompresion* algo = nullptr;
if (metodo == "RLE")
    algo = new CompresionRLE();
else if (metodo == "RLE+")
    algo = new CompresionRLEMejorado();

int tamOut;
char* comp = algo->comprimir(datos, tamIn, tamOut);
delete algo;
```

### 5. Interfaz de persistencia polimórfica

```cpp
class GestorPersistencia {
public:
    virtual ~GestorPersistencia() {}
    virtual bool guardar(const Paquete& pkg, const char* path) = 0;
    virtual Paquete* cargar(const char* path) = 0;
    virtual const char* formato() const = 0;
};

class GestorTextoPersistencia : public GestorPersistencia { ... };  // TXT
class GestorBinarioPersistencia : public GestorPersistencia { ... }; // PKG
```

### 6. Excepciones personalizadas

```cpp
class ErrorPaquete {
protected:
    char* mensaje;
public:
    ErrorPaquete(const char* msg);
    virtual const char* what() const;
    virtual ~ErrorPaquete();
};

class ErrorArchivoNoEncontrado : public ErrorPaquete { ... };
class ErrorPaqueteCorrupto : public ErrorPaquete { ... };
class ErrorCompresion : public ErrorPaquete { ... };
class ErrorIndiceInvalido : public ErrorPaquete { ... };
```

**Uso obligatorio:**
- Extraer archivo con índice fuera de rango → `ErrorIndiceInvalido`
- Cargar .pkg corrupto → `ErrorPaqueteCorrupto`
- Archivo a importar no existe en disco → `ErrorArchivoNoEncontrado`

### 7. Namespaces

```cpp
namespace PaqueteTool {
    namespace Core {
        class Entrada;
        class Paquete;
    }
    namespace Compresion {
        class AlgoritmoCompresion;
        class CompresionRLE;
        class CompresionRLEMejorado;
    }
    namespace IO {
        class GestorPersistencia;
        class GestorTextoPersistencia;
        class GestorBinarioPersistencia;
        class UI;
    }
    namespace Exceptions {
        class ErrorPaquete;
        class ErrorArchivoNoEncontrado;
        class ErrorPaqueteCorrupto;
    }
}
```

### 8. Factory Method

```cpp
class EntradaFactory {
public:
    static Entrada* desdeTexto(const char* nombre, const char* contenido);
    static Entrada* desdeArchivo(const char* rutaDisco);
    static Entrada* crearAleatorio(const char* nombre, int tamano);
};

class CompresionFactory {
public:
    static AlgoritmoCompresion* crear(const char* tipo);
    static AlgoritmoCompresion* mejorPara(Entrada& e);  // Prueba todos
};

class PersistenciaFactory {
public:
    static GestorPersistencia* crearParaFormato(const char* formato);
};
```

### 9. Estructura de archivos

```
include/
  Core/
    Entrada.h
    Paquete.h
  Compresion/
    AlgoritmoCompresion.h
    CompresionRLE.h
    CompresionRLEMejorado.h
  IO/
    GestorPersistencia.h
    GestorTextoPersistencia.h
    GestorBinarioPersistencia.h
    UI.h
  Exceptions/
    ErrorPaquete.h
  Factory/
    EntradaFactory.h
    CompresionFactory.h
    PersistenciaFactory.h
src/
  Core/
    Entrada.cpp
    Paquete.cpp
  Compresion/
    AlgoritmoCompresion.cpp
    CompresionRLE.cpp
    CompresionRLEMejorado.cpp
  IO/
    GestorTextoPersistencia.cpp
    GestorBinarioPersistencia.cpp
    UI.cpp
  Exceptions/
    ErrorPaquete.cpp
  Factory/
    EntradaFactory.cpp
    CompresionFactory.cpp
    PersistenciaFactory.cpp
  main.cpp
Makefile
```

---

## 🔄 Cambios vs E3

| Aspecto | E3 | E4 |
|---------|----|----|
| Compresión | Solo RLE básico | RLE + RLE Mejorado + selección automática |
| Operadores | ❌ | `+`, `+=`, `[]`, `-`, `>`, `<<`, `>>` |
| Excepciones | if/else | Jerarquía `ErrorPaquete` |
| Persistencia | Clases sueltas | Interfaz polimórfica común |
| Namespaces | ❌ | `PaqueteTool::` |
| Factory | ❌ | 3 factories distintas |
| Arquitectura | Monolítica | Modular con carpetas |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Jerarquía `AlgoritmoCompresion` con polimorfismo | 15% |
| Interfaz `GestorPersistencia` con herencia | 10% |
| Sobrecarga de operadores (mín. 5 operadores) | 20% |
| Excepciones personalizadas | 15% |
| Factory Methods (mín. 2) | 10% |
| E1-E3 siguen funcionando | 10% |
| Namespaces y estructura modular | 10% |
| Calidad del código | 10% |

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
- Demostrar funcionamiento de todos los operadores
- Demostrar selección automática del mejor algoritmo de compresión
- El paquete generado debe ser compatible con la carga de E3
