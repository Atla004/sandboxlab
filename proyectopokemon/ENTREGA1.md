# Entrega 1 — Punteros y Memoria Dinámica

## 🎯 Objetivo

Implementar un simulador de batalla Pokémon 1vs1 funcional en consola utilizando **exclusivamente** punteros, memoria dinámica (`new`/`delete`) y arrays dinámicos. El programa debe compilar y ejecutarse sin ficheros externos.

**Temas evaluados:** punteros, memoria dinámica, constructores copia, destructores, `operator=`, deep copy.

---

## 📋 Especificaciones

### 1. Clase `Pokemon`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `nombre` | `char*` | Nombre del Pokémon (cadena dinámica) |
| `especie` | `char*` | Especie (Pikachu, Charmander, etc.) |
| `tipo` | `char*` | Tipo: "Fuego", "Agua", "Planta", "Eléctrico", "Normal" |
| `vida` | `int` | Puntos de vida actuales |
| `vidaMax` | `int` | Vida máxima (para curar) |
| `ataque` | `int` | Estadística de ataque |
| `defensa` | `int` | Estadística de defensa |
| `velocidad` | `int` | Estadística de velocidad |
| `nivel` | `int` | Nivel (1-100) |

La clase debe implementar **obligatoriamente**:
- Constructor por defecto
- Constructor con parámetros (recibe `const char*` y los copia dinámicamente)
- **Constructor de copia** (deep copy)
- **`operator=`** (deep copy, asignación segura)
- **Destructor** (libera memoria)
- Getters y setters (consultar y modificar atributos)
- Método `recibirDaño(int)`: reduce vida, no menor que 0
- Método `curar()`: restaura vida al máximo
- Método `estaVivo()`: devuelve `true` si vida > 0
- Método `mostrar()`: imprime stats del Pokémon

### 2. Clase `Entrenador`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `nombre` | `char*` | Nombre del entrenador |
| `equipo` | `Pokemon**` | Array dinámico de punteros a Pokémon |
| `numPokemon` | `int` | Cantidad actual de Pokémon |
| `capacidad` | `int` | Capacidad máxima del array (crece dinámicamente) |

Debe implementar:
- Constructor con parámetros
- Constructor de copia (deep copy)
- `operator=` (deep copy)
- Destructor
- `agregarPokemon(const Pokemon&)`: añade al final, redimensiona si toca
- `eliminarPokemon(int indice)`: elimina y compacta el array
- `getPokemon(int)`: devuelve puntero al Pokémon en esa posición
- `mostrar()`: lista todos los Pokémon del equipo

### 3. Clase `Mochila`

Debe implementarse como una **lista simplemente enlazada** (o array dinámico) de objetos.

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `items` | `Item*` | Array dinámico de Items |
| `numItems` | `int` | Cantidad de items |

Cada `Item` debe ser un `struct` con: `nombre` (cadena dinámica), `tipo` (enum: Pocion, MT, Baya), `cantidad` (int).

Funcionalidad:
- `agregarItem(const char* nombre, TipoItem tipo, int cant)`
- `usarItem(int indice, Pokemon& destino)`: aplica efecto según tipo
- `mostrar()`

### 4. Clase `Batalla`

Gestiona el combate 1vs1.

| Método | Descripción |
|--------|-------------|
| `Batalla(Entrenador& e1, Entrenador& e2)` | Constructor con referencias |
| `iniciarBatalla()` | Bucle principal del combate |
| `calcularDaño(Pokemon& atacante, Pokemon& defensor)` | Fórmula de daño |

**Fórmula de daño:**
```
daño = ((2 * nivel / 5 + 2) * ataque / defensa / 50 + 2) * multiplicadorTipo
```

**Tabla de tipos** (simplificada, mínimo 5 tipos con al menos 10 relaciones):
| Ataque \ Defensa | Fuego | Agua | Planta | Eléctrico | Normal |
|------------------|-------|------|--------|-----------|--------|
| Fuego | 0.5 | 0.5 | 2.0 | 1.0 | 1.0 |
| Agua | 2.0 | 0.5 | 0.5 | 1.0 | 1.0 |
| Planta | 0.5 | 2.0 | 0.5 | 1.0 | 1.0 |
| Eléctrico | 1.0 | 2.0 | 0.5 | 0.5 | 1.0 |
| Normal | 1.0 | 1.0 | 1.0 | 1.0 | 1.0 |

El combate debe ser **por turnos**: jugador elige atacar, usar item o cambiar (entre los que tiene vivos). La IA del oponente elige aleatoriamente.

### 5. Función `main` — Experiencia de usuario

#### 🎮 ¿Qué hace el usuario?

El programa arranca con **2 entrenadores ya creados** con datos fijos hardcodeados en el código:

- **Ash** (tú): Pikachu, Charmander, Squirtle (3 Pokémon)
- **Brock** (CPU/rival): Geodude, Onix, Rhyhorn (3 Pokémon)

#### Flujo completo:

```
$ ./programa

=== POKEMON BATTLE SIMULATOR ===
1. Ver equipo de Ash
2. Ver equipo de Brock
3. Iniciar batalla
4. Salir
> 1

--- EQUIPO DE ASH ---
Pokémon 1: Pika (Pikachu) - Eléctrico - ❤️ 110/110 - Nv.25
Pokémon 2: Char (Charmander) - Fuego - ❤️ 100/100 - Nv.28
Pokémon 3: Squirtle (Squirtle) - Agua - ❤️ 110/110 - Nv.22

Presiona Enter para volver...
```

Al elegir **Iniciar batalla**:

```
¡Comienza la batalla entre ASH y BROCK!

Ash envía a Pika (Pikachu) - ❤️ 110/110
Brock envía a Rocky (Geodude) - ❤️ 120/120

===== TURNO 1 =====

[Turno de Ash - TÚ DECIDES]
Pika (Pikachu) - ❤️ 110/110   |   Rocky (Geodude) - ❤️ 120/120
1. Atacar
2. Usar objeto (tienes 3)
3. Cambiar Pokémon
> 1

Pika ataca! Causa 28 de daño!
Rocky - ❤️ 92/120

[Turno de Brock - IA]
Rocky ataca a Pika! Causa 18 de daño!
Pika - ❤️ 92/110

===== TURNO 2 =====
...
```

Cuando todos los Pokémon de un entrenador se debilitan (vida = 0):

```
¡Ash derrota a Brock!
¿Volver al menú? (s/n)
```

La IA de Brock elige entre atacar (70%), usar objeto (15%) o cambiar Pokémon (15%) de forma aleatoria.

#### ‼️ Limitaciones de E1 (para que se vea el crecimiento en E2):
- No puedes crear tus propios Pokémon ni entrenadores
- No puedes guardar nada: al cerrar el programa, todo se pierde
- No hay nombres de ataques (solo "Atacar")
- Sin sprites ni decoración visual

---

## 🧠 Requisitos técnicos obligatorios

- **No usar `std::string`** en atributos de clases (solo `char*`)
- **No usar `std::vector`** ni contenedores STL
- **No usar ficheros** (ni lectura ni escritura)
- Toda memoria dinámica debe liberarse correctamente (valgrind sin leaks)
- Constructor copia, `operator=` y destructor implementados **explícitamente**
- Deep copy en todas las copias

---

## 📁 Estructura de archivos esperada

```
include/
  Pokemon.h
  Entrenador.h
  Mochila.h
  Batalla.h
  UI.h
  Tipos.h          // enum TipoPokemon, tabla de multiplicadores
src/
  main.cpp
  Pokemon.cpp
  Entrenador.cpp
  Mochila.cpp
  Batalla.cpp
  UI.cpp
Makefile
```

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Funcionalidad (menú, batalla, items) | 30% |
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
