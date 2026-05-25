# Entrega 3 — Ficheros Binarios

## 🎯 Objetivo

Ampliar el simulador de la Entrega 2 añadiendo **persistencia mediante ficheros binarios** para guardado/recuperación de partidas completas. El programa debe conservar toda la funcionalidad de E1 y E2.

**Temas evaluados:** ficheros binarios (`read`/`write`), serialización binaria, `struct` packing, modos `ios::binary`, gestión de memoria en buffers binarios.

---

## 📋 Especificaciones

### 1. Clase `PersistenciaBinario` (nueva)

Clase con métodos estáticos para E/S binaria:

```cpp
class PersistenciaBinario {
public:
    static void guardarPartida(const Entrenador& e1, const Entrenador& e2,
                                const Mochila& m, const char* filename);
    static bool cargarPartida(Entrenador*& e1, Entrenador*& e2,
                               Mochila*& m, const char* filename);
    static void guardarSprite(const char* pokemon, const char* filename);
    static char* cargarSprite(const char* filename, int& size);

    // Para datos estructurados
    static void guardarPokemonBinario(const Pokemon& p, ofstream& archivo);
    static Pokemon* cargarPokemonBinario(ifstream& archivo);
};
```

### 2. Formato del fichero binario `partida.bin`

Estructura binaria secuencial:

```
[CABECERA]      -> 4 bytes: magic number "PKB1"
[NUM_ENTREN]    -> 4 bytes: int (siempre 2)
[ENTRENADOR_1]  -> datos del entrenador 1
[ENTRENADOR_2]  -> datos del entrenador 2
[MOCHILA]       -> datos de la mochila
[CHECKSUM]      -> 4 bytes: XOR de todos los bytes anteriores
```

Cada `Entrenador` se serializa como:
```
[nombre_len]    -> 4 bytes: int (incluye '\0')
[nombre]        -> nombre_len bytes: char[]
[num_pokemon]   -> 4 bytes: int
[capacidad]     -> 4 bytes: int
[POKEMON_1]     -> estructura del pokémon 1
[...]
[POKEMON_N]     -> estructura del pokémon N
```

Cada `Pokemon` se serializa como:
```
[nombre_len]    -> 4 bytes: int
[nombre]        -> nombre_len bytes
[especie_len]   -> 4 bytes: int
[especie]       -> especie_len bytes
[tipo_len]      -> 4 bytes: int
[tipo]          -> tipo_len bytes
[vida]          -> 4 bytes: int
[vidaMax]       -> 4 bytes: int
[ataque]        -> 4 bytes: int
[defensa]       -> 4 bytes: int
[velocidad]     -> 4 bytes: int
[nivel]         -> 4 bytes: int
```

### 3. Sprites ASCII en binario

Crear sprites ASCII para cada especie y almacenarlos en archivos binarios `.spr`:

```
fichero.spr (binario):
- 4 bytes: ancho del sprite (int)
- 4 bytes: alto del sprite (int)
- N bytes: datos del sprite en raw
```

Ejemplo de sprite para `Pikachu.spr` (guardado en binario, pero conceptualmente):
```
  ╔════╗
  ║❚◕ω◕❚║
  ╚═╤══╤═╝
   ║  ║
  ╱ ╲╱ ╲
```

Los sprites se muestran durante la batalla o al inspeccionar un Pokémon.

### 4. Experiencia de usuario — ¿Qué cambia para el jugador?

#### 🎮 ¿Qué hace el usuario en E3?

Ahora el usuario puede **guardar su progreso** y **ver sprites ASCII** de los Pokémon durante la batalla. La diferencia clave con E2: en E2 se guardaba la *configuración* (equipos), en E3 se guarda el *estado actual* (vida, objetos, progreso).

#### Flujo de guardado de partida:

```
--- GESTIÓN DE PARTIDAS ---
1. Guardar partida actual (partida.bin)
2. Cargar partida guardada (partida.bin)
3. Ver sprites de Pokémon
4. Volver
> 1

Partida guardada correctamente en partida.bin
```

#### Flujo de carga de partida:

```
> 2

Cargando partida.bin...
Magic number OK ✓
Checksum OK ✓
Partida cargada: Ash vs Brock. Todo listo para continuar!
```

#### Batalla con sprites:

Al iniciar batalla, ahora se ven los sprites:

```
      ╔═══════╗                    ╔════════╗
      ║ ◕ω◕  ║                    ║  •_•  ║
      ╚═╤═══╤═╝                    ╚═╤══╤══╝
       ║   ║                       ║  ║
      ╱ ╲ ╱ ╲                     ╱ ╲╱ ╲

Pika (Pikachu)                    Rocky (Geodude)
❤️ 85/110                           ❤️ 45/120
Nv.25                              Nv.22
```

#### El guardado binario captura el estado completo:

Lo que se guarda en `partida.bin` (que NO se puede abrir en bloc de notas):
- Nombre y estado de ambos entrenadores
- **Vida actual de cada Pokémon** (no la inicial)
- **Estado de la mochila** (objetos restantes)
- Progreso de la batalla actual (si está a medias)

#### Escenario típico de uso:

1. El usuario carga sus equipos desde TXT (E2)
2. Personaliza con `pokemon.txt` (E2)
3. Inicia una batalla, lucha 3 turnos
4. Guarda la partida (`partida.bin`)
5. Cierra el programa, va a comer
6. Vuelve, abre el programa
7. Carga `partida.bin` → la batalla sigue exactamente donde la dejó, con la vida que tenían
8. Termina la batalla y puede guardar el historial en TXT (E2)

#### El menú completo en E3:

```
=== POKEMON BATTLE SIMULATOR ===
1. Ver equipo del entrenador 1
2. Ver equipo del entrenador 2
3. Iniciar batalla
4. Gestionar ficheros de texto     ← E2
5. Gestionar partidas (binario)     ← NUEVO
6. Salir
```

#### ‼️ Limitaciones de E3 (para que se vea el crecimiento en E4):
- Los ataques siguen siendo genéricos ("Atacar")
- No hay tipos de movimientos, ni efectos especiales
- El código empieza a ser difícil de mantener (clases planas, lógica mezclada)

### 5. Comprobación de integridad

Al cargar una partida binaria:
1. Verificar **magic number** (si no coincide, rechazar archivo)
2. Opcional: verificar **checksum** (XOR de todos los bytes)
3. Si el archivo está corrupto, mostrar error y continuar con datos por defecto

### 6. Requisitos sobre ficheros binarios

Se debe usar:
- `ios::binary` en los modos de apertura
- `archivo.write(reinterpret_cast<const char*>(&dato), sizeof(dato))`
- `archivo.read(reinterpret_cast<char*>(&dato), sizeof(dato))`
- `archivo.write(cadena, longitud)` para cadenas dinámicas

El fichero binario **no debe ser legible** como texto plano.

---

## 🔄 Cambios vs E1 y E2

| Aspecto | E2 | E3 |
|---------|----|----|
| Persistencia principal | Texto (CSV) | Binario (partidas) |
| Texto | Obligatorio | Se mantiene para config/export |
| Velocidad | Lenta en datos grandes | Rápida |
| Visibilidad | Legible por humanos | Ilegible |
| Nuevo | - | Sprites ASCII, checksum, magic number |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Guardado binario correcto | 20% |
| Carga binaria correcta con reconstrucción de objetos | 20% |
| Magic number y checksum | 10% |
| Sprites ASCII desde binario | 15% |
| E1 y E2 siguen funcionando | 15% |
| Manejo de errores (archivo corrupto, no existe) | 10% |
| Calidad del código y gestión de memoria | 10% |

---

## 🚀 Compilación

```bash
make entrega3
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega3/`
- Incluir al menos un `partida.bin` de ejemplo generado por el programa
- Incluir al menos 3 sprites `.spr` (Pikachu, Charmander, Squirtle)
- El programa debe ser capaz de generar `partida.bin` desde cero
- La funcionalidad de E1 (sin ficheros) y E2 (texto) debe seguir intacta
