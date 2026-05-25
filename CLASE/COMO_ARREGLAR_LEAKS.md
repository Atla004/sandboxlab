# Cómo arreglar memory leaks

Guía práctica paso a paso.

---

## Paso 0: Confirmar que hay leak

```bash
valgrind --leak-check=full --show-leak-kinds=all ./programa
```

Si ves "definitely lost: X bytes in N blocks" → hay leak.

---

## Paso 1: Localizar el origen

Valgrind te dice la línea exacta del `new` que no tiene `delete`:

```
==12345== 100 bytes in 1 blocks are DEFINITELY LOST
==12345==    at 0x...: operator new[](unsigned long)
==12345==    by 0x...: main (archivo.cpp:15)
```

El problema está en **archivo.cpp, línea 15**.

---

## Paso 2: Buscar el `new` y seguir el código

Ve a la línea que indica valgrind y pregúntate:

- ¿Este `new` está en un **constructor**? → el `delete` va en el **destructor**
- ¿Este `new` está en una **función normal**? → el `delete` va donde la variable deje de usarse
- ¿Este `new` se asigna a un **atributo de clase**? → el `delete` va en el **destructor**

---

## Paso 3: Aplicar la solución según el caso

### Caso A: El `new` está en el constructor → poner `delete` en el destructor

**Antes (con leak):**
```cpp
class Persona {
    char* nombre;
public:
    Persona(const char* n) {
        nombre = new char[strlen(n) + 1];  // ← línea 15
        strcpy(nombre, n);
    }
    // ¡FALTA EL DESTRUCTOR!
};
```

**Después (sin leak):**
```cpp
class Persona {
    char* nombre;
public:
    Persona(const char* n) {
        nombre = new char[strlen(n) + 1];
        strcpy(nombre, n);
    }
    ~Persona() {
        delete[] nombre;  // ← se libera al destruir el objeto
    }
};
```

### Caso B: El `new` está en una función → poner `delete` al final

**Antes (con leak):**
```cpp
void procesar() {
    int* arr = new int[100];
    // ... usar arr ...
    // Falta delete[]
}
```

**Después (sin leak):**
```cpp
void procesar() {
    int* arr = new int[100];
    // ... usar arr ...
    delete[] arr;  // ← liberar antes de salir
}
```

### Caso C: El `new` está en una función que devuelve el dato

**Antes (con leak en quien la llama):**
```cpp
int* crearArray(int n) {
    return new int[n];  // La función crea, otro debe liberar
}

int main() {
    int* datos = crearArray(100);
    // ... usar datos ...
    // Falta delete[] → LEAK
}
```

**Después (sin leak):**
```cpp
int* crearArray(int n) {
    return new int[n];
}

int main() {
    int* datos = crearArray(100);
    // ... usar datos ...
    delete[] datos;  // ← el que recibe es responsable de liberar
}
```

### Caso D: Array de punteros (Tipo**)

**Antes (con leak):**
```cpp
int** matriz = new int*[filas];
for(int i = 0; i < filas; i++)
    matriz[i] = new int[cols];
// Uso...
// FALTA: liberar cada fila y el array de punteros
```

**Después (sin leak):**
```cpp
int** matriz = new int*[filas];
for(int i = 0; i < filas; i++)
    matriz[i] = new int[cols];

// ... usar ...

// Liberar en orden INVERSO al de creación:
for(int i = 0; i < filas; i++)
    delete[] matriz[i];  // 1º cada fila
delete[] matriz;          // 2º el array de punteros
```

### Caso E: Clase con array dinámico de punteros a objetos

**Antes (con leak):**
```cpp
class Catalogo {
    Producto** items;
    int num;
public:
    Catalogo() {
        items = new Producto*[10];
        num = 0;
    }

    void agregar(const Producto& p) {
        items[num] = new Producto(p);  // new aquí
        num++;
    }

    ~Catalogo() {
        delete[] items;  // ❌ solo libera el array, NO los objetos
    }
};
```

**Después (sin leak):**
```cpp
    ~Catalogo() {
        for(int i = 0; i < num; i++)
            delete items[i];     // 1º cada objeto
        delete[] items;          // 2º el array de punteros
    }
```

---

## Paso 4: Verificar que se arregló

```bash
valgrind --leak-check=full --show-leak-kinds=all ./programa
```

Busca:

```
==12345== All heap blocks were freed -- no leaks are possible
```

---

## Patrones rápidos de memoria

| Código | Liberación |
|--------|-----------|
| `new int` | `delete p;` |
| `new int[n]` | `delete[] p;` |
| `new Producto("x", 5)` | `delete p;` |
| `new int*[filas]` + cada fila `new int[cols]` | `for each: delete[] fila[i];` luego `delete[] p;` |
| `new Producto*[n]` + cada uno `new Producto(...)` | `for each: delete items[i];` luego `delete[] items;` |

---

## Checklist para prevenir leaks

- [ ] Cada `new` tiene su `delete` correspondiente
- [ ] Cada `new[]` tiene su `delete[]` correspondiente (no mezclar)
- [ ] El destructor libera todos los atributos dinámicos
- [ ] El constructor copia hace deep copy (no shallow)
- [ ] El `operator=` libera lo antiguo ANTES de asignar lo nuevo
- [ ] Las matrices 2D se liberan en orden inverso (filas primero, luego array)
- [ ] Los arrays de punteros liberan cada objeto ANTES que el array
- [ ] `valgrind` confirma: "no leaks are possible"
