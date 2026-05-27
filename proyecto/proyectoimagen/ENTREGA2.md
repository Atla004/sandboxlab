# Entrega 2 — Ficheros de Texto

## 🎯 Objetivo

Ampliar el editor de imágenes de la Entrega 1 añadiendo **carga y guardado en formato PPM texto (P3)**. El programa debe seguir compilando y ejecutándose sin binarios. Se mantiene toda la funcionalidad de E1.

**Temas evaluados:** flujos de ficheros (`ifstream`/`ofstream`), parsing de texto, formato PPM ASCII.

---

## 📋 Especificaciones

### 1. Formato PPM Texto (P3)

El formato **PPM ASCII** (P3) tiene esta estructura:

```
P3
ancho alto
valor_maximo
r g b r g b r g b ...
r g b r g b r g b ...
```

Es **legible por humanos** y se puede abrir en cualquier editor de texto o visor de imágenes.

Ejemplo de un cuadrado 3x3 rojo:
```
P3
3 3
255
255 0 0 255 0 0 255 0 0
255 0 0 255 0 0 255 0 0
255 0 0 255 0 0 255 0 0
```

### 2. Clase `GestorPPMTexto` (nueva)

```cpp
class GestorPPMTexto {
public:
    static bool guardar(const Imagen& img, const char* filename);
    static Imagen* cargar(const char* filename);
    static void mostrarInfo(const char* filename); // cabecera + stats
};
```

- `guardar`: escribe la imagen en formato P3
- `cargar`: lee un archivo P3 y devuelve un `Imagen*` (nuevo en heap)
- **Validación**: si el archivo no existe o tiene formato inválido, devuelve `nullptr` y muestra error

### 3. Nueva funcionalidad en el menú

```
=== EDITOR DE IMÁGENES PPM ===
1. Ver imagen A
2. Ver imagen B
3. Aplicar filtro
4. Gestionar archivos (TEXTO)        ← NUEVO
5. Mezclar imágenes
6. Salir

--- GESTIÓN DE ARCHIVOS (TEXTO) ---
1. Cargar imagen desde .ppm (P3)
2. Guardar imagen actual a .ppm (P3)
3. Ver información de archivo
4. Generar imagen de prueba (degradado, ajedrez, bandera)
5. Volver
```

### 4. Imágenes de prueba generables

El usuario puede generar imágenes desde el menú:

```
> 4

--- GENERAR IMAGEN DE PRUEBA ---
1. Degradado (20x10)
2. Ajedrez (8x8)
3. Bandera de España (30x20)
4. Bandera de Francia (30x20)
5. Personalizada (especificar ancho, alto y patrón)
> 3

Bandera de España generada y guardada en imagenes/bandera_espana.ppm
```

Esto reemplaza/amplía las imágenes hardcodeadas de E1.

### 5. Experiencia de usuario

#### 🎮 ¿Qué hace el usuario en E2?

Ahora el usuario puede **trabajar con imágenes reales** que puede ver fuera del programa.

**Escenario típico:**

1. El usuario descarga o crea una imagen PPM P3 (puede generarla con el programa o buscarla en internet)
2. Abre el programa y selecciona "Cargar imagen desde .ppm"
3. El programa la carga y muestra su previsualización ASCII
4. Aplica filtros (invertir, brillo, umbral)
5. Guarda el resultado en `resultado.ppm`
6. Abre `resultado.ppm` en un visor de imágenes (GIMP, Visual Studio Code con extensión, etc.) → **ve su trabajo**

**Ejemplo de flujo:**

```
> 4.1

Nombre del archivo: fotillo.ppm
Cargando fotillo.ppm...
Formato: P3
Dimensiones: 100x80
Valor máximo: 255
Imagen cargada correctamente.

¿Deseas ver previsualización ASCII? (s/n) > s

(fotillo.ppm mostrada en ASCII art, 100 caracteres de ancho)

¿Aplicar filtro ahora? (s/n) > s
1. Invertir
2. Brillo +50
3. Brillo -50
4. Umbral
> 1

Filtro aplicado. ¿Guardar? (s/n) > s
Nombre: fotillo_invertido.ppm
Guardado en fotillo_invertido.ppm
```

### 6. Formatos de archivo compatibles

| Formato | Extensión | Tipo | Lectura | Escritura |
|---------|-----------|------|---------|-----------|
| PPM P3 | `.ppm` | Texto | ✅ | ✅ |

Solo texto. No se permite `ios::binary` en esta entrega.

---

## 🔄 Cambios vs Entrega 1

| Aspecto | E1 | E2 |
|---------|----|----|
| Imágenes | Hardcodeadas (degradado, ajedrez) | Cargadas desde .ppm o generadas |
| Guardado | ❌ | ✅ Exportar a .ppm (P3) |
| Persistencia | No existe | Archivos .ppm editables externamente |
| Visor externo | ❌ | ✅ Se puede abrir en GIMP, visores |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Carga de PPM P3 correcta | 25% |
| Guardado de PPM P3 correcto | 20% |
| E1 sigue funcionando | 15% |
| Manejo de errores (archivo inexistente, formato inválido) | 20% |
| Generación de imágenes de prueba | 10% |
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
- Incluir al menos 3 `.ppm` de ejemplo (degradado, ajedrez, un patrón propio)
- Los archivos .ppm deben poder abrirse con GIMP o visores online
