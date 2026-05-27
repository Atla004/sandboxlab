# Entrega 3 — Ficheros Binarios

## 🎯 Objetivo

Ampliar el gestor de paquetes de la Entrega 2 añadiendo **persistencia en formato binario** (.pkg) con magic number, checksum e índice binario para acceso rápido. Se mantiene toda la funcionalidad de E1 y E2.

**Temas evaluados:** ficheros binarios (`read`/`write`), serialización binaria, magic number, checksum, acceso aleatorio.

---

## 📋 Especificaciones

### 1. Formato binario `.pkg`

Estructura del archivo binario:

```
[CABECERA]
  4 bytes: magic number "PKG1" (0x504B4731)
  4 bytes: versión del formato (int)
  4 bytes: número de entradas (int)
  4 bytes: tamaño del índice (bytes)

[ÍNDICE]
  Por cada entrada:
    4 bytes: longitud del nombre (int)
    N bytes: nombre del archivo (char*)
    4 bytes: tamaño de los datos (int)
    4 bytes: offset de los datos en el archivo (int)

[DATOS]
  Por cada entrada:
    N bytes: contenido del archivo (raw)

[CHECKSUM]
  4 bytes: XOR de todos los bytes anteriores
```

El **índice** permite acceder directamente a cualquier archivo sin recorrer todo el paquete.

### 2. Clase `GestorBinario` (nueva)

```cpp
class GestorBinario {
public:
    static bool guardarPaquete(const Paquete& pkg, const char* filename);
    static Paquete* cargarPaquete(const char* filename);
    static bool verificarIntegridad(const char* filename);
    static Entrada* extraerRapido(const char* pkgFile, const char* nombreArchivo);
    static void listarRapido(const char* pkgFile);  // solo lee la cabecera+índice

    // Importar/exportar archivos binarios del disco real (NUEVO en E3 con ios::binary)
    static Entrada* importarArchivoBinario(const char* rutaDisco);
    static bool exportarArchivoBinario(const Entrada& entrada, const char* rutaDestino);
};
```

- `guardarPaquete`: escribe cabecera + índice + datos + checksum
- `cargarPaquete`: lee todo el paquete en memoria
- `verificarIntegridad`: comprueba magic number y checksum
- `extraerRapido`: busca en el índice y extrae un archivo sin cargar todo el paquete
- `listarRapido`: solo lee cabecera e índice para mostrar contenido (ideal para paquetes grandes)
- `importarArchivoBinario`: importa **cualquier archivo** (.jpg, .bin, .exe, .pdf) usando `ifstream` con `ios::binary` — lee byte a byte sin interpretar el contenido
- `exportarArchivoBinario`: exporta cualquier entrada del paquete a un archivo real en disco usando `ofstream` con `ios::binary`

### 3. Comparativa texto vs binario

El programa debe poder mostrar una tabla comparativa:

```
--- COMPARATIVA INDICE ---
Formato | Tamaño  | Tiempo carga | Legible
TXT     | 1.2 KB  | 15 ms        | Sí
PKG     | 412 B   | 2 ms         | No

Ahorro de espacio: 65.7%
```

### 4. Menú ampliado

```
=== GESTOR DE PAQUETES ===
1. Listar contenido del paquete
2. Extraer y ver archivo
3. Eliminar archivo del paquete
4. Importar archivo de texto         ← E2 (solo .txt con modo texto)
5. Extraer archivo a disco           ← E2 (solo texto)
6. Gestionar ficheros de texto       ← E2
7. Importar/exportar archivos binarios ← NUEVO (cualquier archivo con ios::binary)
8. Gestionar paquetes binarios       ← NUEVO
9. Comprimir/Descomprimir (RLE)
0. Salir

--- IMPORTAR/EXPORTAR BINARIOS ---
1. Importar archivo binario del disco   ← NUEVO (.jpg, .pdf, .exe, etc.)
2. Exportar archivo a disco (binario)   ← NUEVO
3. Volver

--- PAQUETES BINARIOS (.pkg) ---
1. Guardar paquete a .pkg
2. Cargar paquete desde .pkg
3. Verificar integridad del .pkg
4. Extraer archivo rápido (sin cargar todo)
5. Listar contenido rápido (solo cabecera)
6. Comparar TXT vs PKG
7. Volver
```

### 5. Experiencia de usuario

#### 🎮 ¿Qué hace el usuario en E3?

Ahora el usuario puede **importar cualquier tipo de archivo** (no solo texto) y **guardar/cargar paquetes completos** en un formato binario eficiente.

**Escenario típico 0 — Importar archivo binario (NUEVO respecto a E2):**

```
> 7.1

Ruta del archivo: /home/usuario/foto.jpg
Nombre en el paquete: foto.jpg
Abriendo con ios::binary...
Importando... 45.2 KB leídos.
Archivo binario importado correctamente.
```

**Diferencia clave con E2**: en E2 solo se podían importar `.txt` usando `getline()` (modo texto). Ahora en E3 se usa `ios::binary` para leer cualquier archivo byte a byte sin interpretación.

**Escenario típico 1 — Guardado completo:****

```
1. Importas varios archivos al paquete
2. Aplicas compresión RLE a algunos
3. Guardas en .pkg:
   > 7.1

   Nombre del archivo: mis_datos.pkg
   Guardando paquete con 5 archivos...
   Magic number: PKG1
   Versión: 1
   Checksum: 0xA3F7
   ---
   Tamaño total: 1.2 MB
   Guardado correctamente.
```

**Escenario típico 2 — Carga y verificación:**

```
> 7.2

Nombre del archivo: mis_datos.pkg
Cargando...
Magic number: PKG1 ✅
Versión: 1 ✅
Checksum: 0xA3F7 ✅
Paquete cargado: 5 archivos (1.2 MB)
```

**Escenario típico 3 — Integridad corrupta:**

```
> 7.3

Verificando integridad...
Magic number: PKG1 ✅
Checksum: 0xA3F7 → 0xA3F2 ❌
ERROR: El archivo está corrupto. Checksum no coincide.
```

**Escenario típico 4 — Extracción rápida:**

```
> 7.4

Nombre del archivo a extraer: foto.jpg
Buscando en índice...
Offset encontrado: 1024 bytes
Extrayendo 45.2 KB...
Extraído correctamente (sin cargar todo el paquete).
```

**Escenario típico 5 — Listado rápido (paquete de 1GB):**

```
> 7.5

Listando contenido de respaldo_grande.pkg...
(Solo leo cabecera + índice - 2 KB en lugar de 1 GB)

Archivos en el paquete:
foto1.jpg (2.3 MB)
foto2.jpg (1.8 MB)
video.mp4 (450 MB)
documento.pdf (2.1 MB)
```

---

## 🔄 Cambios vs E2

| Aspecto | E2 | E3 |
|---------|----|----|
| Persistencia | Texto (legible) | Binario (eficiente) |
| Velocidad | Lenta en paquetes grandes | Rápida |
| Tamaño archivo | Grande (3x) | Compacto |
| Seguridad | Sin verificación | Magic number + checksum |
| Acceso | Todo en memoria | Extracción rápida por offset |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Formato binario .pkg correcto | 25% |
| Magic number y checksum funcionales | 15% |
| Extracción rápida por índice | 15% |
| Listado rápido sin cargar todo | 15% |
| E1 + E2 siguen funcionando | 15% |
| Manejo de errores (archivo corrupto) | 10% |
| Calidad del código | 5% |

---

## 🚀 Compilación

```bash
make entrega3
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega3/`
- Incluir al menos un `.pkg` de ejemplo generado por el programa
- Demostrar que el .pkg NO es legible como texto
- Demostrar extracción rápida y listado rápido
- Incluir un .pkg deliberadamente corrupto para probar la verificación
