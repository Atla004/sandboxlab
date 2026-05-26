# Entrega 2 — Ficheros de Texto

## 🎯 Objetivo

Ampliar el gestor de paquetes de la Entrega 1 añadiendo **persistencia mediante ficheros de texto**: exportar el índice del paquete, importar archivos reales del disco, y guardar/cargar paquetes en formato texto. Se mantiene toda la funcionalidad de E1.

**Temas evaluados:** flujos de ficheros (`ifstream`/`ofstream`), formato CSV, importación de archivos reales.

---

## 📋 Especificaciones

### 1. Clase `GestorTexto` (nueva)

```cpp
class GestorTexto {
public:
    static bool guardarIndice(const Paquete& pkg, const char* filename);
    static bool cargarIndice(Paquete& pkg, const char* filename);
    static bool exportarPaquete(const Paquete& pkg, const char* dirSalida);
    static Paquete* importarDirectorio(const char* dirEntrada);
    static bool guardarInfo(const Paquete& pkg, const char* filename);
};
```

### 2. Formatos de archivo

**`indice.csv`** — listado del contenido del paquete:
```
# NOMBRE,TAMAÑO,TIPO
documento.txt,41,texto
datos.bin,100,binario
imagen.img,50,imagen
```

**`paquete.txt`** — exportación completa (metadatos + contenido en Base64-like simplificado):
```
=== PAQUETE: backup_2026 ===
Fecha: 22/05/2026
Archivos: 3

--- INICIO ARCHIVO: documento.txt ---
Tamaño: 41 bytes
Contenido:
Hola mundo, este es un archivo de prueba.
--- FIN ARCHIVO: documento.txt ---

--- INICIO ARCHIVO: datos.bin ---
Tamaño: 100 bytes
Contenido (hex):
00 01 02 03 04 05 06 07 08 09
0A 0B 0C 0D 0E 0F 10 11 12 13
...
--- FIN ARCHIVO: datos.bin ---
```

**`info.txt`** — resumen del paquete:
```
Paquete: backup_2026
Nº archivos: 3
Tamaño total: 191 bytes
Tamaño comprimido: 159 bytes (ratio: 83.2%)
```

### 3. Importar archivos de texto

El usuario puede **importar archivos de texto reales del disco** al paquete usando solo modo texto:

```
> 4 (Importar archivo de texto)

Ruta del archivo: /home/usuario/notas.txt
Nombre en el paquete: notas.txt
Importando... 280 bytes leídos (12 líneas).
Archivo importado correctamente.
```

- Se usa `ifstream` en modo texto con `getline()` para leer línea a línea
- Los datos se almacenan en un buffer dinámico (`char*`) igual que en E1
- Solo archivos de texto (.txt, .csv, .md, .cpp, etc.) — **sin `ios::binary`**
- Si el archivo no existe o no se puede abrir, se muestra mensaje de error

**Restricción:** en E2 solo se importan archivos de texto. Los archivos binarios (.bin, .jpg, .exe) se importarán en E3.

### 4. Exportar archivos de texto a disco

El usuario puede **extraer archivos del paquete y guardarlos en el disco real** usando solo modo texto:

```
> 5 (Extraer archivo a disco)

Índice del archivo: 0
Ruta de destino: /home/usuario/extraido.txt
Exportando... 41 bytes escritos.
Archivo extraído correctamente.
```

- Se usa `ofstream` en modo texto con `<<`
- Solo apto para archivos de texto
- Si el archivo contiene bytes binarios, se exportarán igual pero **pueden no ser legibles**

### 5. Menú ampliado

```
=== GESTOR DE PAQUETES ===
1. Listar contenido del paquete
2. Extraer y ver archivo
3. Eliminar archivo del paquete
4. Importar archivo de texto           ← NUEVO (solo .txt)
5. Extraer archivo a disco             ← NUEVO (solo texto)
6. Gestionar ficheros de texto         ← NUEVO
7. Comprimir/Descomprimir (RLE)
8. Salir

--- GESTIÓN DE FICHEROS (TEXTO) ---
1. Guardar índice a CSV
2. Cargar índice desde CSV
3. Exportar paquete completo a TXT
4. Cargar paquete desde TXT
5. Ver información del paquete
6. Volver
```

### 6. Experiencia de usuario

#### 🎮 ¿Qué hace el usuario en E2?

Ahora el usuario puede **importar archivos reales** de su ordenador y **exportar el contenido del paquete** a texto legible.

**Escenario típico:**

1. Crea un `notas.txt` con VS Code o bloc de notas
2. Abre el programa con los archivos hardcodeados (E1)
3. Importa el archivo de texto real:
   ```
   Ruta del archivo: /home/usuario/notas.txt
   Importando... 280 bytes leídos.
   ```
4. Lista: ahora tiene 4 archivos en el paquete
5. Exporta el índice a `indice.csv` → lo abre en Excel/LibreOffice
6. Exporta el paquete completo a `paquete.txt` → lo envía por email
7. En otro ordenador, abren el programa, cargan `paquete.txt`
8. Extraen los archivos de texto al disco real

**Ejemplo de paquete.txt exportado:**

```
=== PAQUETE: backup_2026 ===
Fecha: 22/05/2026
Archivos: 4

--- INICIO ARCHIVO: documento.txt ---
Tamaño: 41 bytes
Contenido:
Hola mundo, este es un archivo de prueba.
--- FIN ARCHIVO: documento.txt ---

--- INICIO ARCHIVO: notas.txt ---
Tamaño: 280 bytes
Contenido:
Apuntes de Programación 2
- Tema 1: Punteros
- Tema 2: Ficheros de texto
...
--- FIN ARCHIVO: notas.txt ---
```

---

## 🔄 Cambios vs Entrega 1

| Aspecto | E1 | E2 |
|---------|----|----|
| Archivos | Hardcodeados | + Importación real del disco |
| Persistencia | Solo memoria | Índice CSV, paquete TXT |
| Exportación | Ver en pantalla | Guardar a archivos reales |
| Intercambio | ❌ | Puedes enviar el .txt por email |

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Importación de archivos reales | 25% |
| Exportación de archivos a disco | 15% |
| Formato índice CSV correcto | 15% |
| Formato paquete TXT correcto | 15% |
| Carga de paquete desde TXT | 15% |
| E1 sigue funcionando | 10% |
| Calidad del código | 5% |

---

## 🚀 Compilación

```bash
make entrega2
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega2/`
- Incluir al menos 2 archivos de prueba reales para importar
- Incluir `indice.csv` y `paquete.txt` de ejemplo generados por el programa
- Demostrar que un `paquete.txt` generado puede cargarse en otra ejecución
