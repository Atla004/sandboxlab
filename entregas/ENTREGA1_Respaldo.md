# Entrega 1 — Gestor de Paquetes y Respaldo

##  Objetivo

Crear un gestor de paquetes/archivos que opera completamente en memoria. El programa inicia con paquetes predefinidos que contienen "archivos" simulados. El usuario puede listar, buscar, extraer, comprimir, cifrar, verificar integridad, fusionar y comparar archivos entre paquetes.

**Tema central:** punteros y memoria dinámica.

---

##  Funcionalidad esperada

### Menú principal

- Ver todos los paquetes 
- Extraer y visualizar un archivo
- Eliminar archivos de un paquete
- Buscar archivos por nombre
- Comprimir y descomprimir archivos
- Verificar integridad de un archivo
- Fusionar dos paquetes en uno nuevo
- Comparar archivos entre paquetes para detectar duplicados
- Salir

### ¿Qué es un "paquete"?

Un paquete es un **contenedor simulado en memoria**. No son archivos reales de tu disco. Al ejecutar el programa, el código crea en RAM estructuras que imitan archivos con nombre, contenido (buffer de bytes), tamaño y checksum.

**Ejemplo de lo que hay en memoria al arrancar:**

```
Paquete "documentos":
  → "documento.txt"  →  "Hola mundo, este es un archivo de prueba."
  → "datos.bin"      →  100 bytes con valores 0, 1, 2, ... 99
  → "imagen.img"     →  50 bytes con patrón alternado 0xAA, 0x55

Paquete "respaldo":
  → "notas.txt"      →  "Esto es una nota de respaldo."
  → "datos.bin"      →  copia idéntica al de arriba (para probar duplicados)
```

**"Listar paquetes"** simplemente muestra al usuario estos contenedores:

```
=== PAQUETES DISPONIBLES ===
[0] documentos    — 3 archivos
[1] respaldo      — 2 archivos
```

El usuario elige un paquete por su índice (`0` o `1`) y luego puede operar sobre sus archivos simulados: ver contenido, comprimir, cifrar, etc.

### Datos iniciales

- 2 paquetes predefinidos con archivos hardcodeados en el código
- Al menos un archivo duplicado entre ambos paquetes (para probar detección)

### Compresión

- Algoritmo RLE: secuencias de bytes repetidos se codifican de forma compacta
- El programa muestra el ratio de compresión obtenido

### Integridad

- Cada archivo posee un checksum calculado al crearlo
- La verificación recalcula el checksum y lo compara con el almacenado

### Fusión y comparación

- Se pueden detectar archivos idénticos entre paquetes distintos

---

## Restricciones


- NO usar archivos (ni lectura ni escritura en disco)
- Toda memoria dinámica debe liberarse correctamente
- Constructor de copia, `operator=` y destructor obligatorios en clases con memoria dinámica
- Deep copy siempre, nunca shallow copy

---

##  Entrega

- Carpeta `Entrega1/` en tu repositorio GitHub
- `Makefile` funcional

---

##  Creatividad

Las mejoras opcionales (otro algoritmo de compresión, más operaciones entre paquetes, mejor interfaz) pueden sumar puntos extra. Tú decides qué tan lejos quieres llegar.
