

# Punteros y Memoria Dinámica en C++

Guía teórico-práctica para Programación 2.

---

## Índice

1. [¿Qué es un puntero?](#1-qué-es-un-puntero)
2. [Aritmética de punteros](#2-aritmética-de-punteros)
3. [Punteros y arrays](#3-punteros-y-arrays)
4. [Cadenas con char*](#4-cadenas-con-char)
5. [Memoria dinámica: Stack vs Heap](#5-memoria-dinámica-stack-vs-heap)
6. [Arrays dinámicos 1D](#6-arrays-dinámicos-1d)
7. [Arrays dinámicos 2D (matrices)](#7-arrays-dinámicos-2d-matrices)
8. [Punteros dobles (Tipo**)](#8-punteros-dobles-tipo)
9. [Punteros y referencias como parámetros](#9-punteros-y-referencias-como-parámetros)
10. [La Regla de 3](#10-la-regla-de-3)
11. [Deep Copy vs Shallow Copy](#11-deep-copy-vs-shallow-copy)
12. [Errores comunes](#12-errores-comunes)
13. [Valgrind: cómo depurar memoria](#13-valgrind-cómo-depurar-memoria)
14. [Ejercicios resueltos](#14-ejercicios-resueltos)

---

## 1. ¿Qué es un puntero?

### 1.1. La memoria del ordenador

La RAM de tu ordenador es como un **enorme armario con casillas numeradas**. Cada casilla tiene una **dirección** (un número) y puede guardar un valor.

```
Dirección    Contenido
0x1000      →  42
0x1004      →  7
0x1008      →  'A'
...
```

Cuando declaras una variable normal:

```cpp
int edad = 25;
```

El compilador:
1. Elige una casilla libre en la RAM
2. Guarda el valor `25` en esa casilla
3. Asocia el nombre `edad` con esa dirección

Tú trabajas con el nombre (`edad`), pero el ordenador trabaja con direcciones.

### 1.2. ¿Qué es un puntero entonces?

**Un puntero es una variable que guarda direcciones de memoria, no valores normales.**

```cpp
int edad = 25;    // Variable normal: guarda el número 25
int* p = &edad;   // Puntero: guarda la DIRECCIÓN donde está 'edad'
```

Visualmente:

```
Variable 'edad' (int)         Variable 'p' (int*)
┌──────────────┐              ┌──────────────┐
│     25       │              │   0x1000     │
└──────────────┘              └──────────────┘
  Dirección: 0x1000            Dirección: 0x2000
```

### 1.3. Los dos operadores fundamentales

| Operador | Nombre | ¿Qué hace? |
|----------|--------|------------|
| `&variable` | Dirección de | Pregunta: "¿En qué dirección de memoria está `variable`?" |
| `*puntero` | Desreferencia | Pregunta: "Vete a la dirección que guarda `puntero` y dime qué hay allí" |

```cpp
int a = 10;
int* ptr = &a;     // ptr guarda la dirección de a

cout << a;          // 10    (el valor de a)
cout << &a;         // 0x1000  (la dirección de a en memoria)
cout << ptr;        // 0x1000  (lo que guarda el puntero: la dirección de a)
cout << *ptr;       // 10    (va a 0x1000 y lee el valor que hay)
```

**Analogía:** `&` es como preguntar "¿cuál es tu dirección postal?". `*` es como ir a esa dirección y ver quién vive allí.

### 1.4. Declaración de punteros

```cpp
int* p;         // Puntero a entero
char* c;        // Puntero a carácter
double* d;      // Puntero a double
float* f;       // Puntero a float
bool* b;        // Puntero a booleano
```

El `*` en la declaración significa: "esto no guarda un `int`, guarda una **dirección donde hay un `int`**".

### 1.5. nullptr: el puntero que no apunta a nada

Un puntero puede no apuntar a ningún sitio válido. Para eso está `nullptr`:

```cpp
int* p = nullptr;  // "Este puntero no apunta a nada"

// Siempre verificar antes de usar:
if(p != nullptr) {
    *p = 5;  // Seguro: solo ejecuta si p NO es nulo
}
```

**¿Qué pasa si desreferencias nullptr?** Tu programa **crash** con `segfault` (segmentation fault). El sistema operativo dice: "estás intentando acceder a memoria que no te pertenece".

```
nullptr → dirección 0 → el SO bloquea el acceso → CRASH
```

Por eso la verificación `if(p != nullptr)` es obligatoria.

---

## 2. Aritmética de punteros

### 2.1. ¿Por qué p + 1 no suma 1?

Cuando sumas 1 a un puntero, **no avanzas 1 byte**. Avanzas `sizeof(tipo)` bytes.

```cpp
int arr[3] = {10, 20, 30};
// Cada int ocupa 4 bytes

int* p = arr;   // p apunta al primer elemento

cout << p;      // 0x1000
cout << p + 1;  // 0x1004  (NO 0x1001)
cout << p + 2;  // 0x1008
```

**¿Por qué?** Porque el puntero "sabe" qué tipo de dato apunta. Si es `int*`, sabe que cada elemento está a 4 bytes de distancia. Si es `double*`, cada elemento está a 8 bytes.

```cpp
double d[3] = {1.0, 2.0, 3.0};
double* pd = d;

cout << pd;     // 0x2000
cout << pd + 1; // 0x2008  (cada double son 8 bytes)
```

### 2.2. Operaciones permitidas

| Operación | Ejemplo | Significado |
|-----------|---------|-------------|
| Suma | `p + n` | Avanza `n * sizeof(tipo)` bytes |
| Resta | `p - n` | Retrocede `n * sizeof(tipo)` bytes |
| Incremento | `p++` | Avanza al siguiente elemento |
| Decremento | `p--` | Retrocede al elemento anterior |
| Diferencia | `p2 - p1` | Número de elementos entre ellos |
| Comparación | `p1 == p2` | ¿Apuntan al mismo sitio? |

### 2.3. Diferencia entre direcciones vs diferencia entre elementos

```cpp
int arr[5];
int* inicio = &arr[0];  // 0x1000
int* final  = &arr[3];  // 0x100C (3 * 4 bytes = 12 bytes después)

cout << (final - inicio);  // 3  (¡no 12!)
```

**¿Por qué da 3 y no 12?** Porque cuando restas punteros, el resultado es en **elementos**, no en bytes. Es decir: `(dirección_final - dirección_inicial) / sizeof(tipo)`.

---

## 3. Punteros y arrays

### 3.1. La relación clave

**El nombre de un array es un puntero al primer elemento.**

```cpp
int arr[5] = {10, 20, 30, 40, 50};

cout << arr;    // 0x1000  (dirección de arr[0])
cout << &arr[0];// 0x1000  (es exactamente lo mismo)

cout << arr[2];    // 30  (sintaxis de array)
cout << *(arr+2);  // 30  (sintaxis de puntero, exactamente igual)
```

### 3.2. La diferencia crucial: puntero constante

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int* p = arr;

p++;     // OK: p es variable, ahora apunta a arr[1]
arr++;   // ERROR: arr es constante, no se puede reasignar

cout << sizeof(arr);  // 20 (5 * 4 bytes: el array completo)
cout << sizeof(p);    // 8  (solo el tamaño del puntero, siempre 8 en 64 bits)
```

**`arr` es un puntero constante.** Puedes leer su dirección pero no cambiarla. Es como una flecha clavada en el suelo que siempre apunta al primer elemento.

### 3.3. Pasar arrays a funciones (degradación a puntero)

```cpp
void imprimir(int* arr, int n) {
    for(int i = 0; i < n; i++)
        cout << arr[i] << " ";  // o *(arr + i)
}

int main() {
    int datos[5] = {1, 2, 3, 4, 5};
    imprimir(datos, 5);
}
```

**¿Por qué pasar el tamaño `n` como parámetro extra?** Porque cuando el array "entra" en la función, se **degrada a puntero** y se pierde la información de cuántos elementos tiene:

```cpp
void f(int arr[]) {   // int arr[] es equivalente a int* arr
    cout << sizeof(arr);  // 8 (tamaño del puntero, NO del array)
}
```

Por eso siempre necesitas pasar el tamaño aparte: `f(int* arr, int n)`.

---

## 4. Cadenas con char*

### 4.1. ¿Qué es una cadena en C++?

**Una cadena es un array de caracteres que termina en `'\0'` (carácter nulo).**

```
"Hola" en memoria:

'H' 'o' 'l' 'a' '\0'
65  111 108 97  0
```

El `'\0'` marca el final. Sin él, el ordenador no sabría dónde termina la cadena.

### 4.2. Las 3 formas de declarar una cadena

```cpp
// FORMA 1: Array estático en el stack
char saludo1[20] = "Hola";   // Reserva 20 bytes en el stack
                             // Copia 'H','o','l','a','\0' al inicio
                             // Los otros 15 bytes quedan sin usar

// FORMA 2: Puntero a literal (¡cuidado! solo lectura)
const char* saludo2 = "Hola";  // Apunta a una zona de solo lectura
// saludo2[0] = 'M';           // ERROR: no se puede modificar

// FORMA 3: Memoria dinámica (la que usaremos para atributos de clase)
char* saludo3 = new char[20];  // Reserva 20 bytes en el heap
strcpy(saludo3, "Hola");       // Copia la cadena a ese espacio
delete[] saludo3;              // Hay que liberarlo manualmente
```

### 4.3. ¿Por qué no usar `std::string`?

En los proyectos se pide usar `char*` porque:
1. **Aprendes cómo funciona la memoria por debajo** (cuando usas `std::string`, todo eso ocurre automágicamente)
2. **Practicas gestión manual de memoria** (new/delete, constructor copia, etc.)
3. **`std::string` es `std::vector<char>`** — si no puedes usar `std::vector`, tampoco deberías usar `std::string`

### 4.4. Funciones de <cstring> (heredadas de C)

```cpp
#include <cstring>

char* s1 = new char[20];
char* s2 = new char[20];

strcpy(s1, "Hola");        // Copia: destino ← origen
strcat(s1, " Mundo");      // Concatena: "Hola Mundo"
int len = strlen(s1);      // Longitud: 10 (sin contar '\0')

if(strcmp(s1, s2) == 0)    // Compara: 0 si iguales, <0 si s1 < s2
    cout << "Iguales";

delete[] s1;
delete[] s2;
```

### 4.5. El patrón de copia dinámica

Siempre que necesites copiar una cadena a memoria dinámica:

```cpp
char* copiarCadena(const char* original) {
    // 1. Calcular cuánto mide (sin contar '\0')
    int len = strlen(original);

    // 2. Reservar espacio: len + 1 (para el '\0')
    char* copia = new char[len + 1];

    // 3. Copiar el contenido (incluyendo el '\0')
    strcpy(copia, original);

    return copia;
}
```

**¿Por qué `+ 1`?** `strlen` no cuenta el `'\0'` final. Si la cadena mide 5 caracteres, necesitas 6 bytes: 5 para los caracteres + 1 para el terminador nulo.

---

## 5. Memoria dinámica: Stack vs Heap

### 5.1. ¿Qué es el stack?

El **stack** o **pila** es la memoria donde se guardan las variables locales. Cuando llamas a una función, se "apila" un marco con sus variables. Cuando la función termina, ese marco se "desapila" y la memoria se libera automáticamente.

```cpp
void funcion() {
    int x = 5;       // x vive aquí, en el stack
    char c = 'A';    // c también
}   // al salir, x y c se destruyen solas
```

**Stack: automático, rápido, pequeño (pocos MB).**

### 5.2. ¿Qué es el heap?

El **heap** o **montón** es una zona de memoria global que tú gestionas manualmente. Puedes reservar memoria cuando quieras y liberarla cuando quieras.

```cpp
int* p = new int;    // Pides memoria al heap. Te devuelve la dirección.
*p = 5;              // Usas esa memoria.
delete p;            // Debes DEVOLVER la memoria al heap.
```

**Heap: manual, más lento, grande (toda la RAM disponible).**

### 5.3. ¿Por qué necesitamos el heap?

**El stack tiene un problema:** todo lo que se crea dentro de una función se destruye al salir. Si necesitas que un dato **viva más que la función que lo creó**, necesitas el heap:

```cpp
int* crearArray(int n) {
    // Esto vuelve basura al salir de la función:
    int arrStack[10];       // NO se puede devolver

    // Esto SÍ vive aunque salgamos de la función:
    int* arrHeap = new int[n];  // Se puede devolver
    return arrHeap;
}

int main() {
    int* datos = crearArray(100);
    // datos sigue siendo válido aquí
    delete[] datos;  // Hay que liberarlo manualmente
}
```

### 5.4. new: ¿qué pasa realmente?

```cpp
int* p = new int;
```

Cuando escribes `new int`, el ordenador:
1. Busca un espacio libre en el heap del tamaño de un `int` (4 bytes)
2. Si lo encuentra, anota en sus registros que ese espacio está ocupado
3. Te devuelve la dirección de ese espacio (un `int*`)

```cpp
delete p;
```

Cuando escribes `delete p`:
1. El ordenador mira sus registros y encuentra el bloque que empieza en `p`
2. Anota que ese bloque vuelve a estar libre
3. **NO borra el valor** (el `42` puede seguir ahí hasta que otro lo sobreescriba)

### 5.5. new[] vs new (la diferencia)

```cpp
int* p1 = new int;      // Reserva 1 entero (4 bytes)
int* p2 = new int[10];  // Reserva 10 enteros (40 bytes)

delete p1;              // Libera 1 entero
delete[] p2;            // Libera 10 enteros
```

**¿Por qué hay dos formas distintas?** El sistema necesita saber cuánto liberar. Con `new[]`, guarda internamente el contador de elementos. Si usas `delete` (sin `[]`) en algo creado con `new[]`, no sabe cuánto liberar → **comportamiento indefinido** (probable crash).

### 5.6. Regla inquebrantable

```
Cada new     →  un delete
Cada new[]   →  un delete[]
```

Mezclarlos produce **comportamiento indefinido** (desde fuga de memoria hasta crash).

### 5.7. Comparativa visual

```
STACK                          HEAP
┌─────────────────┐           ┌─────────────────┐
│ main():          │           │                 │
│   int a = 5      │           │   (espacio      │
│   char c = 'X'   │           │    reservado)   │
├─────────────────┤           │                 │
│ funcion():       │           │  p ─────► [42]  │
│   int x = 10    │           │                 │
│   int y = 20    │           │   (espacio      │
├─────────────────┤           │    libre)       │
│ ...              │           │                 │
└─────────────────┘           └─────────────────┘
```

El stack se organiza en **capas** (como platos apilados). Al salir de una función, la capa se retira.

El heap es como un **tablón de anuncios**: cualquiera puede clavar un papel (new) y cualquiera puede retirarlo (delete). Si no lo retiras, queda ahí para siempre (memory leak).

---

## 6. Arrays dinámicos 1D

### 6.1. ¿Por qué dinámicos?

Los arrays estáticos tienen un problema: su tamaño se fija en compilación.

```cpp
int estatico[100];  // Si necesito 101, no puedo.
```

Los arrays dinámicos permiten decidir el tamaño **en ejecución**:

```cpp
int n;
cout << "¿Cuántos números?";
cin >> n;

int* dinamico = new int[n];  // Tamaño decidido en ejecución
delete[] dinamico;
```

### 6.2. Patrón básico

```cpp
// CREAR
int* arr = new int[tamano];

// USAR (exactamente igual que un array estático)
for(int i = 0; i < tamano; i++)
    arr[i] = i * 2;

// LIBERAR (IMPRESCINDIBLE)
delete[] arr;
```

**¿Por qué `arr[i]` funciona si `arr` es un puntero?** Porque el compilador traduce `arr[i]` a `*(arr + i)`. Como vimos en la sección 3, array y puntero son equivalentes en el acceso.

### 6.3. Redimensionar un array dinámico

Los arrays dinámicos NO se redimensionan automáticamente. Hay que hacerlo a mano:

```cpp
void redimensionar(int*& arr, int& capacidad, int nuevoTam) {
    // 1. Crear array nuevo con el tamaño deseado
    int* nuevo = new int[nuevoTam];

    // 2. Copiar los datos que nos interesan
    int min = (capacidad < nuevoTam) ? capacidad : nuevoTam;
    for(int i = 0; i < min; i++)
        nuevo[i] = arr[i];

    // 3. Liberar el array antiguo
    delete[] arr;

    // 4. Apuntar al nuevo
    arr = nuevo;
    capacidad = nuevoTam;
}
```

**Nota:** el parámetro es `int*& arr` (puntero por referencia). Necesitamos modificar el puntero en sí (no solo lo que apunta) para que `arr` apunte al nuevo bloque.

---

## 7. Arrays dinámicos 2D (matrices)

### 7.1. Concepto

Una matriz 2D dinámica es un **array de punteros**, donde cada puntero apunta a una fila:

```
int** matriz
│
├───► [int] [int] [int] ...  (fila 0)
├───► [int] [int] [int] ...  (fila 1)
└───► [int] [int] [int] ...  (fila 2)
```

### 7.2. Creación (el porqué de cada paso)

```cpp
int** crearMatriz(int filas, int columnas) {
    // PASO 1: Array de punteros (uno por cada fila)
    // Cada elemento será un int* (puntero a una fila)
    int** m = new int*[filas];

    // PASO 2: Por cada fila, crear un array de enteros
    for(int i = 0; i < filas; i++)
        m[i] = new int[columnas];

    return m;
}
```

**¿Por qué `int**`?** Porque queremos un array donde cada elemento sea `int*`. Un array de `int*` se escribe `int**`.

**¿Por qué dos pasos y no uno solo?** Porque necesitamos `filas` arrays independientes, cada uno con su propio ciclo de vida. La memoria del array de punteros y la memoria de cada fila son reservas separadas.

### 7.3. Destrucción

```cpp
void liberarMatriz(int** m, int filas) {
    // 1. Primero liberar cada fila
    for(int i = 0; i < filas; i++)
        delete[] m[i];  // cada fila es un int*

    // 2. Luego liberar el array de punteros
    delete[] m;         // m es un int**
}
```

**¿Por qué primero las filas y luego el array?** Si liberases el array de punteros primero, perderías las direcciones de las filas y no podrías liberarlas después (memory leak).

### 7.4. Acceso

```cpp
int** matriz = crearMatriz(3, 4);

matriz[0][0] = 10;  // Primera fila, primera columna
matriz[1][2] = 20;  // Segunda fila, tercera columna
matriz[2][3] = 30;  // Tercera fila, cuarta columna

// mat[i][j] se traduce como *(*(mat + i) + j)
//           = *(mat[i] + j)
//           = valor en fila i, columna j

liberarMatriz(matriz, 3);
```

### 7.5. Copia profunda de matriz

```cpp
int** copiarMatriz(int** original, int filas, int columnas) {
    int** copia = new int*[filas];

    for(int i = 0; i < filas; i++) {
        copia[i] = new int[columnas];           // nueva fila
        for(int j = 0; j < columnas; j++)
            copia[i][j] = original[i][j];       // copiar elemento
    }

    return copia;
}
```

---

## 8. Punteros dobles (Tipo**)

### 8.1. ¿Qué es un puntero doble?

Un puntero doble (`Tipo**`) es una variable que guarda **la dirección de un puntero**.

```cpp
int valor = 42;
int* ptr = &valor;    // ptr guarda la dirección de valor
int** ptr2 = &ptr;    // ptr2 guarda la dirección de ptr
```

```
Valor (int)       ptr (int*)       ptr2 (int**)
┌────────┐       ┌────────┐       ┌────────┐
│   42   │ ◄──── │ 0x1000 │ ◄──── │ 0x2000 │
└────────┘       └────────┘       └────────┘
 0x1000           0x2000           0x3000
```

### 8.2. ¿Para qué se usan?

Los usos principales son:

1. **Arrays de punteros** (el caso de la matriz 2D, sección 7)
2. **Arrays de cadenas** (cada cadena es un `char*`, el array es `char**`)
3. **Modificar un puntero desde una función** (necesitas `Tipo**` o `Tipo*&`)

### 8.3. Array de cadenas con char**

```cpp
// 3 nombres, cada uno es un char* (cadena dinámica)
char** nombres = new char*[3];

// Cada elemento del array es una cadena independiente
nombres[0] = new char[10];
strcpy(nombres[0], "Ana");

nombres[1] = new char[10];
strcpy(nombres[1], "Luis");

nombres[2] = new char[10];
strcpy(nombres[2], "Eva");

// Liberar
for(int i = 0; i < 3; i++)
    delete[] nombres[i];  // cada cadena
delete[] nombres;          // el array de punteros
```

### 8.4. Modificar un puntero desde una función

```cpp
// Sin doble puntero: la función recibe una COPIA del puntero
void asignarMal(int* p) {
    p = new int(5);   // Solo modifica la copia local
}                     // La copia se destruye: LEAK

// Con doble puntero: la función recibe la DIRECCIÓN del puntero
void asignarBien(int** p) {
    *p = new int(5);  // Modifica el puntero original
}

int* ptr = nullptr;
asignarBien(&ptr);    // Ahora ptr apunta a un int con valor 5
delete ptr;
```

---

## 9. Punteros y referencias como parámetros

### 9.1. Las 3 formas de pasar un parámetro

```cpp
// 1. POR VALOR: se COPIA el argumento
void porValor(int x) {
    x = 10;  // Solo modifica la copia
}            // La copia se destruye al salir

// 2. POR PUNTERO: se pasa la dirección
void porPuntero(int* p) {
    *p = 10;  // Modifica el original (desreferencia)
}

// 3. POR REFERENCIA: alias del original
void porReferencia(int& r) {
    r = 10;   // Modifica el original directamente
}

int main() {
    int a = 5;

    porValor(a);       // a = 5  (no cambia)
    porPuntero(&a);    // a = 10 (cambia)
    porReferencia(a);  // a = 10 (cambia)

    return 0;
}
```

### 9.2. ¿Cuál usar cuándo?

| Situación | Solución | ¿Por qué? |
|-----------|----------|-----------|
| Modificar el original | `T&` o `T*` | Necesitas acceso al original |
| Objeto grande, solo lectura | `const T&` | Evitas copiar, no puedes modificar |
| Puede ser nulo | `T*` | `T&` no puede ser nulo, `T*` sí |
| No quiero copias, lectura sola | `const T&` | Lo más eficiente y seguro |

### 9.3. Puntero por defecto (nullptr opcional)

```cpp
void procesar(int* datos = nullptr) {
    if(datos == nullptr) {
        cout << "No hay datos" << endl;
        return;
    }
    // ... procesar datos ...
}

procesar();           // nullptr por defecto
procesar(&algo);      // le paso algo real
```

---

## 10. La Regla de 3

### 10.1. El problema

Cuando una clase tiene **atributos que son punteros con memoria dinámica**, el compilador genera código automático que hace **shallow copy** (copia superficial). Eso causa problemas.

Si no escribes estos 3 métodos, el compilador los crea por ti, pero mal:

```cpp
class Ejemplo {
    char* datos;  // atributo dinámico
    // Si no escribo nada, el compilador genera:
    // 1. Destructor: no hace nada → LEAK
    // 2. Copia: copia el puntero (shallow) → DOUBLE DELETE
    // 3. Asignación: copia el puntero (shallow) → DOUBLE DELETE
};
```

### 10.2. Los 3 métodos que debes implementar

```cpp
class Ejemplo {
    char* datos;  // Puntero a memoria dinámica
    int tamano;   // Atributo normal (no puntero)
public:
    // 1. CONSTRUCTOR (con parámetros)
    Ejemplo(const char* d) {
        tamano = strlen(d) + 1;
        datos = new char[tamano];
        strcpy(datos, d);
    }

    // 2. DESTRUCTOR
    ~Ejemplo() {
        delete[] datos;  // Libera la memoria dinámica
    }

    // 3. CONSTRUCTOR COPIA
    Ejemplo(const Ejemplo& otro) {
        tamano = otro.tamano;
        datos = new char[tamano];
        strcpy(datos, otro.datos);
    }

    // 4. OPERADOR ASIGNACIÓN
    Ejemplo& operator=(const Ejemplo& otro) {
        if(this != &otro) {            // Protección autoasignación
            delete[] datos;             // Liberar lo que tenía
            tamano = otro.tamano;
            datos = new char[tamano];
            strcpy(datos, otro.datos);
        }
        return *this;
    }
};
```

### 10.3. El patrón común (copia y pega esto adaptándolo)

Para cualquier clase con un puntero como atributo:

```
Constructor:  new
Destructor:   delete
Copia:        new + copiar contenido
Asignación:   delete + new + copiar contenido + protección autoasignación
```

### 10.4. Protección contra autoasignación (importantísima)

```cpp
// Sin protección:
Ejemplo& operator=(const Ejemplo& otro) {
    delete[] datos;                          // ¡Me borro a mí mismo!
    datos = new char[strlen(otro.datos) + 1];
    strcpy(datos, otro.datos);               // CRASH: otro.datos ya no existe
    return *this;
}

// Con protección:
Ejemplo& operator=(const Ejemplo& otro) {
    if(this != &otro) {                      // "¿Soy yo mismo?"
        delete[] datos;
        datos = new char[strlen(otro.datos) + 1];
        strcpy(datos, otro.datos);
    }
    return *this;
}

// Caso que activa la protección:
Ejemplo e("test");
e = e;  // Autoasignación: sin protección → CRASH
```

**`this`** es un puntero especial que apunta al propio objeto. Si `this` y `&otro` son la misma dirección, es que te estás asignando a ti mismo.

---

## 11. Deep Copy vs Shallow Copy

### 11.1. Shallow copy (lo que hace el compilador por defecto)

Cuando copias un objeto con punteros, el compilador copia **los punteros**, no los datos apuntados.

```cpp
class SinCopia {
    char* texto;
public:
    SinCopia(const char* t) {
        texto = new char[strlen(t) + 1];
        strcpy(texto, t);
    }
    // FALTA: destructor, copia, asignación
};

SinCopia a("Hola");
SinCopia b = a;  // Shallow copy: b.texto = a.texto (MISMA dirección)
```

Resultado en memoria:

```
Antes de copiar:
a.texto ───→ [H][o][l][a][\0]

Después de shallow copy:
a.texto ───→ [H][o][l][a][\0]   ← ambos apuntan al mismo sitio
b.texto ────┘

Cuando a se destruye: delete[] a.texto → la memoria se libera
Cuando b se destruye: delete[] b.texto → DOUBLE DELETE → CRASH
```

### 11.2. Deep copy (la correcta)

Cada objeto tiene su propia copia independiente de los datos:

```cpp
class ConCopia {
    char* texto;
public:
    ConCopia(const char* t) {
        texto = new char[strlen(t) + 1];
        strcpy(texto, t);
    }
    ConCopia(const ConCopia& otro) {   // Constructor copia
        texto = new char[strlen(otro.texto) + 1];
        strcpy(texto, otro.texto);
    }
    ~ConCopia() {
        delete[] texto;
    }
    ConCopia& operator=(const ConCopia& otro) {
        if(this != &otro) {
            delete[] texto;
            texto = new char[strlen(otro.texto) + 1];
            strcpy(texto, otro.texto);
        }
        return *this;
    }
};

ConCopia a("Hola");
ConCopia b = a;  // Deep copy: b tiene su PROPIA copia
```

Resultado:

```
a.texto ───→ [H][o][l][a][\0]   (copia independiente)
b.texto ───→ [H][o][l][a][\0]   (copia independiente)

Cuando a se destruye: libera su copia
Cuando b se destruye: libera su copia
NO HAY PROBLEMA
```

### 11.3. Analogía

**Shallow copy** = darle a alguien una foto de tu casa. Si la casa se quema, él también se queda sin casa.

**Deep copy** = construirle una casa exactamente igual a la tuya. Cada uno tiene la suya, independiente.

---

## 12. Errores comunes

### 12.1. Memory leak (olvidar delete)

```cpp
void funcion() {
    int* p = new int[1000];
    // ... usar p ...
    // olvidé delete[] p → 1000 bytes perdidos para siempre
}
```

**Cada llamada a esta función pierde 1000 bytes.** Si se llama 1000 veces, pierdes 1MB.

**Síntoma:** el programa consume cada vez más RAM hasta que el sistema se ralentiza o crashea.

### 12.2. Double delete

```cpp
int* p = new int(5);
delete p;
delete p;  // CRASH: la memoria ya fue liberada
```

**¿Por qué crash?** El sistema lleva un registro de qué memoria está ocupada. Cuando llamas a `delete` dos veces, intentas liberar algo ya libre. El sistema operativo lo detecta y mata tu programa.

### 12.3. Dangling pointer (puntero colgante)

```cpp
int* p = new int(5);
delete p;
*p = 10;  // CRASH: estás escribiendo en memoria ya liberada
```

**¿Por qué crash?** El bloque de memoria fue devuelto al sistema. Puede que ya esté siendo usado por otro programa, o puede que esté marcado como inválido. Escribir allí es ilegal.

### 12.4. Usar delete en vez de delete[]

```cpp
int* arr = new int[100];
delete arr;   // ERROR: solo libera el primero de los 100
```

**¿Qué pasa?** `delete` sin `[]` asume que es un solo objeto. Libera la dirección base pero no sabe que hay 100 enteros. Los otros 99 quedan ocupados (leak).

### 12.5. No implementar la Regla de 3

```cpp
class Mal {
    char* nombre;
public:
    Mal(const char* n) { nombre = new char[strlen(n)+1]; strcpy(nombre, n); }
    ~Mal() { delete[] nombre; }
    // FALTAN: constructor copia y operator=
};

Mal a("test");
Mal b(a);   // Shallow copy → ambos comparten el mismo puntero
// Al salir del ámbito: se llama al destructor de a y de b
// Segundo delete → CRASH
```

### 12.6. Desreferenciar nullptr

```cpp
int* p = nullptr;
*p = 5;  // CRASH: segmentation fault
```

**Siempre verificar:**
```cpp
if(p != nullptr) {
    *p = 5;  // Seguro
}
```

### 12.7. Modificar un string literal

```cpp
char* s = "Hola";   // "Hola" está en memoria de solo lectura
s[0] = 'M';         // CRASH en algunos sistemas (comportamiento indefinido)
```

La forma correcta:

```cpp
char* s = new char[20];  // Memoria dinámica modificable
strcpy(s, "Hola");
s[0] = 'M';              // OK: s ahora es "Mola"
```

---

## 13. Valgrind: cómo depurar memoria

### 13.1. ¿Qué es Valgrind?

Valgrind es una herramienta que ejecuta tu programa y **detecta errores de memoria**: leaks, accesos inválidos, double frees, etc.

```bash
# Instalar
sudo apt install valgrind
```

### 13.2. Compilar para depuración

```bash
g++ -g -O0 -Wall -Wextra programa.cpp -o programa
```

- `-g`: incluye información de depuración (líneas exactas del error)
- `-O0`: sin optimizaciones (código más legible para depurar)
- `-Wall -Wextra`: todos los warnings

### 13.3. Ejecutar

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./programa
```

### 13.4. Interpretar la salida

**Salida con leak:**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 100 bytes in 1 blocks
==12345==
==12345== 100 bytes in 1 blocks are DEFINITELY LOST
==12345==    at 0x...: operator new[](unsigned long)
==12345==    by 0x...: main (programa.cpp:15)
```

Te dice: "en la línea 15 del programa.cpp hay un `new[]` que nunca se liberó".

**Salida limpia (sin errores):**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==
==12345== All heap blocks were freed -- no leaks are possible
```

### 13.5. Qué significa cada mensaje

| Mensaje | ¿Qué es? |
|---------|----------|
| `definitely lost` | **Leak real.** Reservaste memoria y nunca la liberaste |
| `indirectly lost` | **Leak dentro de un leak.** Perdiste el puntero a una estructura que contenía más punteros |
| `possibly lost` | **Puede ser leak.** Valgrind no está seguro. Investiga |
| `still reachable` | No liberaste pero aún tienes el puntero (menos grave, pero limpialo) |
| `Invalid read/write` | **MUY GRAVE.** Lees o escribes en memoria que no te pertenece |
| `Conditional jump depends on uninitialized value` | Usas una variable sin inicializar |

### 13.6. Tu objetivo

Siempre que ejecutes tu programa con valgrind, la salida debe terminar con:

```
All heap blocks were freed -- no leaks are possible
```

---

## 14. Ejercicios resueltos

### Ejercicio 1: Array dinámico de enteros

Crear un array de enteros, llenarlo con los primeros `n` números pares, imprimirlos, liberarlo.

```cpp
#include <iostream>
using namespace std;

int main() {
    int n = 10;
    int* arr = new int[n];

    for(int i = 0; i < n; i++)
        arr[i] = (i + 1) * 2;

    for(int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    delete[] arr;
    return 0;
}
```

### Ejercicio 2: Clase con char* (Regla de 3)

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Estudiante {
    char* nombre;
    int edad;
public:
    Estudiante(const char* n, int e) : edad(e) {
        nombre = new char[strlen(n) + 1];
        strcpy(nombre, n);
    }

    ~Estudiante() {
        delete[] nombre;
    }

    Estudiante(const Estudiante& otro) : edad(otro.edad) {
        nombre = new char[strlen(otro.nombre) + 1];
        strcpy(nombre, otro.nombre);
    }

    Estudiante& operator=(const Estudiante& otro) {
        if(this != &otro) {
            delete[] nombre;
            edad = otro.edad;
            nombre = new char[strlen(otro.nombre) + 1];
            strcpy(nombre, otro.nombre);
        }
        return *this;
    }

    void mostrar() const {
        cout << nombre << " (" << edad << " anios)" << endl;
    }
};

int main() {
    Estudiante e1("Ana", 20);
    Estudiante e2 = e1;      // Constructor copia
    Estudiante e3("Luis", 22);
    e3 = e1;                 // operator=
    

    e1.mostrar();
    e2.mostrar();
    e3.mostrar();

    return 0;
}
```

### Ejercicio 3: Matriz 2D dinámica

```cpp
#include <iostream>
using namespace std;

int** crearMatriz(int filas, int cols) {
    int** m = new int*[filas];
    for(int i = 0; i < filas; i++)
        m[i] = new int[cols];
    return m;
}

void llenar(int** m, int filas, int cols) {
    for(int i = 0; i < filas; i++)
        for(int j = 0; j < cols; j++)
            m[i][j] = i * cols + j;
}

void mostrar(int** m, int filas, int cols) {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < cols; j++)
            cout << m[i][j] << "\t";
        cout << endl;
    }
}

void liberar(int** m, int filas) {
    for(int i = 0; i < filas; i++)
        delete[] m[i];
    delete[] m;
}

int main() {
    int filas = 3, cols = 4;
    int** mat = crearMatriz(filas, cols);
    llenar(mat, filas, cols);
    mostrar(mat, filas, cols);
    liberar(mat, filas);
    return 0;
}
```

### Ejercicio 4: Array dinámico de objetos

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Producto {
    char* nombre;
    double precio;
public:
    Producto() : nombre(nullptr), precio(0) {}
    Producto(const char* n, double p) : precio(p) {
        nombre = new char[strlen(n) + 1];
        strcpy(nombre, n);
    }
    ~Producto() { delete[] nombre; }

    Producto(const Producto& otro) : precio(otro.precio) {
        nombre = new char[strlen(otro.nombre) + 1];
        strcpy(nombre, otro.nombre);
    }

    Producto& operator=(const Producto& otro) {
        if(this != &otro) {
            delete[] nombre;
            precio = otro.precio;
            nombre = new char[strlen(otro.nombre) + 1];
            strcpy(nombre, otro.nombre);
        }
        return *this;
    }

    void mostrar() const {
        cout << nombre << " - $" << precio << endl;
    }
};

class Catalogo {
    Producto* productos;
    int cantidad;
    int capacidad;
public:
    Catalogo() : cantidad(0), capacidad(5) {
        productos = new Producto[capacidad];
    }

    ~Catalogo() { delete[] productos; }

    void agregar(const Producto& p) {
        if(cantidad >= capacidad) {
            Producto* nuevo = new Producto[capacidad * 2];
            for(int i = 0; i < cantidad; i++)
                nuevo[i] = productos[i];
            delete[] productos;
            productos = nuevo;
            capacidad *= 2;
        }
        productos[cantidad] = p;
        cantidad++;
    }

    void listar() const {
        for(int i = 0; i < cantidad; i++)
            productos[i].mostrar();
    }
};

int main() {
    Catalogo cat;
    cat.agregar(Producto("Laptop", 899.99));
    cat.agregar(Producto("Mouse", 25.50));
    cat.agregar(Producto("Teclado", 45.00));
    cat.listar();
    return 0;
}
```

---

## Resumen visual

```
┌────────────────────────────────────────────────────────┐
│                    PUNTEROS                            │
├────────────────────────────────────────────────────────┤
│                                                        │
│  int* p = &x;     → p guarda la dirección de x        │
│  *p               → el VALOR que hay en esa dirección  │
│                                                        │
│  p + 1            → avanza sizeof(*p) bytes           │
│                                                        │
│  arr[n] ≡ *(arr + n)   (todo array se convierte en     │
│                         puntero al pasar a función)    │
│                                                        │
│  STACK (automático)   vs  HEAP (manual)                │
│  int x = 5;                  int* p = new int;        │
│  Se libera solo              delete p;                │
│                                                        │
│  REGLA DE ORO:                                          │
│  new → delete                                          │
│  new[] → delete[]                                      │
│                                                        │
│  REGLA DE 3:                                            │
│  Si hay punteros en la clase:                           │
│  1. ~Destructor()  → libera                            │
│  2. Constructor copia → deep copy                      │
│  3. operator= → deep copy + autoasignación             │
│                                                        │
│  DEBUG: valgrind --leak-check=full ./programa          │
│                                                        │
└────────────────────────────────────────────────────────┘
```
