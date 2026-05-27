# Entrega 2 — Ficheros de Texto

## 🎯 Objetivo

Ampliar el simulador de la Entrega 1 añadiendo **persistencia mediante ficheros de texto**. El programa debe seguir compilando y ejecutándose sin necesidad de ficheros binarios. Todo el código de la Entrega 1 debe mantenerse funcional.

**Temas evaluados:** flujos de ficheros (`ifstream`/`ofstream`), formato CSV/TXT, serialización textual, parsing.

---

## 📋 Especificaciones

### 1. Clase `PersistenciaTexto` (nueva)

Clase con **métodos estáticos** para toda la E/S de texto:

```cpp
class PersistenciaTexto {
public:
    static void guardarPokemon(const Pokemon& p, ofstream& archivo);
    static Pokemon cargarPokemon(ifstream& archivo);
    static void guardarEquipo(const Entrenador& e, const char* filename);
    static Entrenador* cargarEquipo(const char* filename);
    static void guardarMochila(const Mochila& m, const char* filename);
    static Mochila* cargarMochila(const char* filename);
    static void guardarHistorialBatalla(const char* mensaje, const char* filename);
    static void mostrarArchivo(const char* filename);
};
```

### 2. Formatos de archivo

**`pokemon.txt`** — definición de especies disponibles:
```
# ESPECIE,TIPO,VIDA_MAX,ATAQUE,DEFENSA,VELOCIDAD
Pikachu,Eléctrico,110,55,40,90
Charmander,Fuego,100,52,43,65
Squirtle,Agua,110,48,65,43
Bulbasaur,Planta,120,49,49,45
```

**`equipo.txt`** — equipo de un entrenador:
```
# Entrenador: Ash
# NOMBRE,ESPECIE,TIPO,NIVEL,VIDA,VIDA_MAX,ATAQUE,DEFENSA,VELOCIDAD
Pika,Pikachu,Eléctrico,25,110,110,55,40,90
Charizard,Charmander,Fuego,36,100,100,52,43,65
```

**`historial.txt`** — log de batallas (formato legible):
```
=== BATALLA: Ash vs Brock ===
22/05/2026 10:30:00
Pika ataca a Onix: 45 de daño
Onix ataca a Pika: 22 de daño
Pika derrota a Onix
--- GANADOR: Ash ---
```

### 3. Funcionalidad nueva (sin perder la E1)

El menú principal se amplía:

```
=== POKEMON BATTLE SIMULATOR ===
1. Ver equipo del entrenador 1
2. Ver equipo del entrenador 2
3. Iniciar batalla
4. Gestionar ficheros
5. Salir

--- GESTIÓN DE FICHEROS ---
1. Cargar equipo desde .txt
2. Guardar equipo a .txt
3. Ver listado de Pokémon disponibles (pokemon.txt)
4. Ver historial de batallas
5. Exportar batalla a historial.txt
6. Volver
```

### 4. Experiencia de usuario — ¿Qué cambia para el jugador?

#### 🎮 ¿Qué hace el usuario en E2?

Ahora el usuario **no está limitado a los entrenadores hardcodeados**. Puede:

**A. Crear sus propios Pokémon editando `pokemon.txt`**
```
# ESPECIE,TIPO,VIDA_MAX,ATAQUE,DEFENSA,VELOCIDAD
Pikachu,Eléctrico,110,55,40,90
Charmander,Fuego,100,52,43,65
Squirtle,Agua,110,48,65,43
Bulbasaur,Planta,120,49,49,45
Gengar,Fantasma,100,65,60,110  ← el usuario añade este
```

**B. Crear su propio entrenador con equipo personalizado en `equipo.txt`**
```
# Entrenador: Misty
# NOMBRE,ESPECIE,TIPO,NIVEL,VIDA,VIDA_MAX,ATAQUE,DEFENSA,VELOCIDAD
Staryu,Staryu,Agua,30,100,100,45,55,85
Goldeen,Goldeen,Agua,25,90,90,40,50,63
```

**C. Cada batalla queda registrada en `historial.txt`**
```
=== BATALLA: Ash vs Brock ===
22/05/2026 10:30:00
Pika ataca a Onix: 45 de daño
...
--- GANADOR: Ash ---
```

#### Flujo completo del menú ampliado:

```
=== POKEMON BATTLE SIMULATOR ===
1. Ver equipo del entrenador 1
2. Ver equipo del entrenador 2
3. Iniciar batalla
4. Gestionar ficheros
5. Salir
> 4

--- GESTIÓN DE FICHEROS ---
1. Cargar equipo desde .txt       ← NUEVO
2. Guardar equipo a .txt           ← NUEVO
3. Ver listado de Pokémon disponibles (pokemon.txt)  ← NUEVO
4. Ver historial de batallas       ← NUEVO
5. Exportar batalla a historial.txt ← NUEVO
6. Volver
```

**Escenario típico de uso:**
1. El usuario edita `pokemon.txt` con un editor externo (Bloc de notas, VS Code)
2. Abre el programa, selecciona "Cargar equipo desde .txt" y elige su `equipo.txt`
3. Pelea contra el otro entrenador
4. Al terminar, el programa pregunta: `¿Guardar batalla en historial.txt? (s/n)`
5. Sale del programa. Al cerrar, los Pokémon vuelven a su estado inicial (no hay persistencia de estado, solo de configuración) ← importante para que E3 tenga sentido

#### ‼️ Limitaciones de E2 (para que se vea el crecimiento en E3):
- Los Pokémon vuelven a su estado inicial al cerrar el programa
- No hay guardado de partida (progreso, objetos, estado actual)
- No hay sprites ni representación visual

### 5. Requisitos sobre ficheros

Se debe usar **exclusivamente**:
- `#include <fstream>` (`ifstream`, `ofstream`, `fstream`)
- Formato **texto plano** (ASCII/UTF-8)
- Delimitación por comas (CSV-like)
- Comentarios con `#` al inicio de línea

No se permite usar:
- Ficheros binarios (`ios::binary`)
- `std::vector` u otros contenedores STL
- Librerías externas de serialización

---

## 🔄 Cambios vs Entrega 1

| Aspecto | E1 | E2 |
|---------|----|----|
| Datos iniciales | Hardcodeados | Cargados desde TXT o hardcode |
| Persistencia | No existe | Guardar/cargar en texto |
| Equipos | Fijos, 2 entrenadores | Configurables desde archivos |
| Historial | No | Log de batallas en txt |

El código de la E1 puede modificarse, pero la funcionalidad E1 debe seguir funcionando.

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Funcionalidad de guardado y carga correcta | 25% |
| Formato de archivos correcto y consistente | 15% |
| Manejo de errores (archivo no existe, formato inválido) | 20% |
| Historial de batallas funcionando | 15% |
| Todo lo de E1 sigue funcionando | 15% |
| Calidad del código | 10% |

---

## 🚀 Compilación

```bash
make entrega2
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega2/`
- Incluir `pokemon.txt` de ejemplo (mín. 6 especies distintas)
- Incluir `equipo.txt` de ejemplo (mín. 2 equipos distintos)
- El programa debe funcionar con y sin archivos presentes
