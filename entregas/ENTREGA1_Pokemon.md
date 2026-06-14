# Entrega 1 — Simulador de Batalla Pokémon

##  Objetivo

Crear un simulador de batalla Pokémon 1vs1 en consola. El programa inicia con dos entrenadores predefinidos, cada uno con un equipo de Pokémon. El usuario puede ver los equipos e iniciar batallas por turnos contra la CPU.

**Tema central:** punteros y memoria dinámica.

---

##  Funcionalidad esperada

### Menú principal

- Ver equipos de ambos entrenadores
- Iniciar batalla
- Salir

### Batalla

- Combate por turnos entre un Pokémon del jugador y uno de la CPU
- El jugador elige: atacar, usar objeto de la mochila, o cambiar Pokémon
- La CPU elige su acción de forma autónoma
- Existe una tabla de tipos con ventajas y desventajas (mínimo 5 tipos)
- Cuando un Pokémon llega a 0 de vida se debilita
- Gana el entrenador que deje sin Pokémon al rival

### Mochila

- Cada entrenador posee objetos consumibles que se usan durante la batalla
- Al menos un objeto debe restaurar vida

### Datos iniciales

- 2 entrenadores con 3 Pokémon cada uno, hardcodeados en el código
- Cada Pokémon tiene: nombre, especie, tipo, vida, ataque, defensa, velocidad, nivel

---

##  Restricciones

- NO usar archivos (ni lectura ni escritura)
- Toda memoria dinámica debe liberarse correctamente


---

##  Entrega

- Carpeta `Entrega1/` en tu repositorio GitHub
- `Makefile` funcional

---

##  Creatividad

Las mejoras opcionales (más tipos, efectos de objetos creativos, mejor IA, decoración visual) pueden sumar puntos extra. Tú decides qué tan lejos quieres llegar.
