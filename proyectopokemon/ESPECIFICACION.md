# Especificación Técnica del Proyecto Pokémon Battle Simulator

Este documento detalla las especificaciones técnicas concretas que deben seguir **todas las entregas**. Es el contrato de diseño del proyecto.

---

## 1. 🧬 Sistema de Tipos

### Tipos de Pokémon (mínimo 6)

| Tipo | Símbolo | Ventaja vs | Debilidad vs |
|------|---------|------------|--------------|
| Fuego | 🔥 | Planta, Hielo | Agua, Tierra |
| Agua | 💧 | Fuego, Tierra | Planta, Eléctrico |
| Planta | 🌿 | Agua, Tierra | Fuego, Volador |
| Eléctrico | ⚡ | Agua, Volador | Tierra, Planta |
| Normal | ⚪ | Ninguno | Lucha |
| Tierra | 🏔️ | Fuego, Eléctrico | Agua, Planta |

### Tabla de multiplicadores completa

```
     FUE AGU PLA ELE NOR TIE
FUE  0.5 0.5 2.0 1.0 1.0 1.0
AGU  2.0 0.5 0.5 2.0 1.0 2.0
PLA  0.5 2.0 0.5 1.0 1.0 0.5
ELE  1.0 2.0 0.5 0.5 1.0 0.0
NOR  1.0 1.0 1.0 1.0 1.0 1.0
TIE  2.0 1.0 1.0 2.0 1.0 1.0
```

---

## 2. ⚔️ Sistema de Batalla

### Fórmula de daño
```
dañoBase = ((2 * nivel / 5 + 2) * ataque / defensa / 50 + 2)
dañoFinal = dañoBase * multiplicadorTipo
```

### Flujo de un turno
1. Mostrar estado actual (vida de ambos)
2. Mostrar opciones:
   ```
   1. Atacar
   2. Usar objeto
   3. Cambiar Pokémon
   ```
3. Si ataca, mostrar opciones de movimiento (si E4) o ataque básico
4. Calcular daño según tipo y stats
5. Aplicar daño al defensor
6. Turno de la IA (misma lógica, elección aleatoria)
7. Comprobar si alguien ganó
8. Si no, siguiente turno

### AI del oponente
- 70% atacar
- 15% usar objeto (si tiene)
- 15% cambiar Pokémon (si tiene más de 1 vivo)
- Prioriza ataques supereficaces si hay tabla de tipos

---

## 3. 📦 Sistema de Objetos (Mochila)

| Objeto | Efecto |
|--------|--------|
| Poción | Recupera 20 PS |
| Superpoción | Recupera 50 PS |
| Antídoto | Cura envenenamiento (E4) |
| MT Fuego | Enseña movimiento de fuego (E4) |
| Baya | Recupera 10 PS, se usa automática al caer a 0 |

---

## 4. 💾 Persistencia

### Texto (E2)
- **Pokémon**: `nombre,especie,tipo,nivel,vida,vidaMax,ataque,defensa,velocidad`
- **Equipo**: primera línea `# Entrenador: <nombre>`, luego un Pokémon por línea
- **Historial**: formato libre pero legible, con fecha/hora

### Binario (E3)
- Magic number: `0x504B4231` ("PKB1" en ASCII)
- Checksum: XOR byte a byte de todo el contenido
- Los `int` se guardan en 4 bytes (little-endian nativo)
- Las cadenas se guardan como: `[4 bytes: longitud]` + `[N bytes: datos]`

---

## 5. 🖼️ Sprites ASCII

Cada sprite se define en una cuadrícula de caracteres de ancho y alto variable:
```
    ╔═══════╗
    ║ ◕ω◕ ║
    ╚═╤═══╤═╝
     ║   ║
    ╱ ╲ ╱ ╲
```

En binario (.spr): `[ancho: int][alto: int][datos: char[ancho*alto]]`

---

## 6. 🏗️ Normas de codificación comunes a todas las entregas

| Norma | Detalle |
|-------|---------|
| Lenguaje | C++11 o superior |
| Compilación | `g++ -std=c++11 -Wall -Wextra -I include` |
| Prohibido | `std::string` como atributo (salvo E4 si se justifica), `std::vector`, librerías externas |
| Memoria | Cada `new` debe tener su `delete`. Prohibido `malloc`/`free` |
| Nombres | `snake_case` para funciones y variables, `PascalCase` para clases |
| Archivos | Un `.h` y un `.cpp` por clase (salvo structs pequeños) |
| Makefile | Targets: `entrega1`, `entrega2`, `entrega3`, `entrega4`, `clean` |
| Git | Una rama `entrega-N` por entrega, tags `v1.0`, `v2.0`, etc. |

---

## 7. 📤 Entregas y evaluación progresiva

| Entrega | Temas | Depende de | Usuario pelea contra | Novedad para el usuario |
|---------|-------|------------|----------------------|-------------------------|
| E1 | Punteros y memoria dinámica | Nada | Brock (CPU hardcodeado) | Batalla 1vs1 básica con ataque genérico |
| E2 | Ficheros de texto | E1 | Quien él quiera (edita TXT) | Personaliza equipos editando archivos .txt |
| E3 | Ficheros binarios | E1 + E2 | El mismo rival que antes | Guarda/carga partida, ve sprites ASCII |
| E4 | POO y diseño avanzado | E1+E2+E3 (refactor) | El mismo rival + mejoras | Ataques con nombre real, efectos, Factory |

**Regla fundamental:** cada entrega debe compilar y ejecutarse por separado. Se permite modificar código de entregas anteriores solo si es necesario para la nueva funcionalidad, pero el comportamiento observable desde el menú debe ser compatible hacia atrás.

---

## 8. 📁 Ejemplo de datos iniciales (para E1)

```cpp
// main.cpp — Datos hardcodeados para E1
Pokemon* p1 = new Pokemon("Pika", "Pikachu", "Electrico", 25, 110, 55, 40, 90);
Pokemon* p2 = new Pokemon("Char", "Charmander", "Fuego", 28, 100, 52, 43, 65);
Pokemon* p3 = new Pokemon("Squirtle", "Squirtle", "Agua", 22, 110, 48, 65, 43);

Entrenador ash("Ash");
ash.agregarPokemon(*p1);
ash.agregarPokemon(*p2);
// ... delete p1, p2, p3 despues de agregar (el entrenador hace copia)
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
| **A** | `Pokemon` + `Batalla` | `PersistenciaTexto` (carga) | `PersistenciaBinario` (guardado) | Refactor jerarquía `Entidad` |
| **B** | `Entrenador` + `Mochila` | `PersistenciaTexto` (guardado) | `PersistenciaBinario` (carga) + sprites | Refactor `Persistencia` interfaz |
| **C** | `UI` + `main` + Makefile | Historial + integración | Checksum + magic number + tests | `Movimiento` jerarquía + excepciones |
| **D** | Tabla tipos + tests manual | Validación errores + tests | Sprites + tests | Factory + namespaces + UML |
