# Entrega 2 — Ficheros de Texto

##  Objetivo

Ampliar el gestor de paquetes Torrespaldo de la Entrega 1 añadiendo persistencia mediante ficheros de texto. el usuario podrá agregar archivos reales del disco a un paquete en memoria, listarlos, verlos, extraerlos, y guardar/cargar el paquete completo a un archivo de texto. Se mantiene toda la funcionalidad de la Entrega 1.

**Tema central:** flujos de ficheros, importación de archivos reales.

---

## 📋 Funcionalidad esperada

### Menú principal

- Listar contenido del paquete
- Ver archivo del paquete
- Eliminar archivo del paquete
- Agregar archivo real al paquete
- Extraer archivo del paquete
- Gestionar ficheros
  - Exportar índice del paquete a archivo
  - Cargar índice desde archivo
  - Guardar paquete a archivo TXT
  - Cargar paquete desde archivo TXT
  - Ver información del paquete
  - Volver
- Salir

### Agregar archivos reales

- El usuario puede agregar archivos de texto reales del disco al paquete
- Los datos se almacenan en un buffer dinámico igual que en Entrega 1
- Solo archivos de texto (.txt, .md, .cpp, etc.)
- Si el archivo no existe o no se puede abrir, se muestra mensaje de error

### Extraer archivos

- El usuario puede extraer archivos del paquete y guardarlos en el disco real

### Gestión de ficheros

- Exportar el índice del paquete a archivo (se puede abrir con Excel/LibreOffice)
- Cargar el índice desde un archivo guardado previamente
- Guardar el paquete completo a un archivo TXT (para compartir o transportar)
- Cargar un paquete completo desde un TXT guardado previamente
- Ver información resumida del paquete (cantidad de archivos, tamaños)

### Datos

- Archivo con el índice del paquete
- Archivo TXT con el paquete completo exportado
- Al menos 2 archivos de prueba reales para agregar al paquete
- El paquete guardado debe poder cargarse en otra ejecución del programa

---

##  Restricciones

- NO usar archivos binarios
- NO usar std::vector ni otros contenedores STL
- NO usar librerías externas de serialización

---

##  Entrega

- Carpeta `Entrega2/` en tu repositorio GitHub
- `Makefile` funcional
- Incluir al menos 2 archivos de prueba reales
- Incluir ejemplo de paquete exportado (.txt)


