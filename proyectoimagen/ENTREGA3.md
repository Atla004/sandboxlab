# Entrega 3 — Ficheros Binarios

## 🎯 Objetivo

Ampliar el editor de imágenes de la Entrega 2 añadiendo **carga y guardado en formato PPM binario (P6)** y **transformaciones geométricas**. Se mantiene toda la funcionalidad de E1 y E2.

**Temas evaluados:** ficheros binarios (`read`/`write`), serialización binaria, modos `ios::binary`, transformaciones de matrices.

---

## 📋 Especificaciones

### 1. Formato PPM Binario (P6)

El **PPM binario** (P6) tiene la misma cabecera que P3, pero los datos de píxel se almacenan en **binario raw**:

```
P6
ancho alto
255
[datos binarios: ancho * alto * 3 bytes]
```

Donde cada píxel son 3 bytes consecutivos (R, G, B) sin espacios ni saltos de línea.

**No es legible** por humanos, pero ocupa ~3x menos que P3 y se carga/guarda mucho más rápido.

### 2. Clase `GestorPPMBinario` (nueva)

```cpp
class GestorPPMBinario {
public:
    static bool guardar(const Imagen& img, const char* filename);
    static Imagen* cargar(const char* filename);
    static bool verificarIntegridad(const char* filename); // comprueba cabecera y tamaño
};
```

- `guardar`: escribe cabecera P6 + datos binarios
- `cargar`: lee cabecera, reserva memoria, lee datos en bruto
- `verificarIntegridad`: comprueba que el magic number "P6" es correcto y que el tamaño del archivo coincide con `ancho * alto * 3 + cabecera`

### 3. Transformaciones geométricas (nuevas)

```cpp
class Transformacion {
public:
    static void espejarHorizontal(Imagen& img);
    static void espejarVertical(Imagen& img);
    static void rotar90(Imagen& img);
    static void rotar180(Imagen& img);
    static void redimensionar(Imagen& img, int nuevoAncho, int nuevoAlto); // nearest neighbor
    static void recortar(Imagen& img, int x, int y, int w, int h);
};
```

**Detalle de cada una:**
- **espejarHorizontal**: refleja la imagen horizontalmente (efecto espejo)
- **espejarVertical**: refleja verticalmente
- **rotar90**: rota 90° en sentido horario (cambian ancho y alto)
- **redimensionar**: escala usando **vecino más cercano** (simple pero funcional)
- **recortar**: extrae una sub-imagen

### 4. Menú ampliado

```
=== EDITOR DE IMÁGENES PPM ===
1. Ver imagen actual
2. Aplicar filtro (E1+E2)
3. Transformaciones geométricas       ← NUEVO
4. Gestionar archivos (TEXTO P3)      ← E2
5. Gestionar archivos (BINARIO P6)    ← NUEVO
6. Información de la imagen
7. Salir

--- TRANSFORMACIONES ---
1. Espejo horizontal
2. Espejo vertical
3. Rotar 90°
4. Rotar 180°
5. Redimensionar
6. Recortar

--- ARCHIVOS BINARIOS ---
1. Cargar .ppm (P6)
2. Guardar .ppm (P6)
3. Verificar integridad
4. Comparar tamaño P3 vs P6
```

### 5. Experiencia de usuario

#### 🎮 ¿Qué hace el usuario en E3?

Ahora el usuario puede **trabajar con imágenes reales en binario**, más rápidas y compactas.

**Escenario típico:**

1. Carga una imagen PPM P3 (E2) o P6 (E3)
2. La previsualiza en ASCII
3. Aplica una transformación geométrica:
   ```
   > 3.1 (Espejo horizontal)
   Aplicando espejo horizontal...

   ANTES:          DESPUÉS:
   @@@###          ###@@@
   @@@###          ###@@@
   @@@###          ###@@@
   ```
4. Aplica filtros adicionales (E1+E2)
5. Guarda el resultado en P6 (más rápido, menos espacio)
6. **Compara tamaños**: el programa muestra cuánto ahorró el formato binario
   ```
   --- COMPARATIVA ---
   P3 (texto):   97.2 KB
   P6 (binario): 32.4 KB
   Ahorro: 66.7%
   ```

**Verificación de integridad:**

```
> 5.3

Archivo: fotillo.ppm
Verificando integridad...
Magic number: P6 ✅
Dimensiones: 100x80
Tamaño esperado: 24039 bytes
Tamaño real:     24039 bytes ✅
El archivo es válido.
```

**Ejemplo de rotación:**

```
> 3.3

Rotando 90°...
Las dimensiones pasan de 20x10 a 10x20

ANTES (20x10):    DESPUÉS (10x20):
@@@@@%%%%#...     @@@@@@@@@@
@@@@@%%%%#...     @@@@@...@@
...               ...
                  ...
```

#### ‼️ Limitaciones de E3 (para E4):
- Los filtros son funciones sueltas, no hay jerarquía de clases
- No hay sistema de plugins ni extensibilidad
- Mucho código repetido entre formatos de archivo

---

## ✅ Criterios de evaluación

| Criterio | Peso |
|----------|------|
| Carga/guardado P6 correcto | 25% |
| Transformaciones geométricas (espejo, rotar, redimensionar) | 25% |
| Verificación de integridad | 10% |
| E1 + E2 siguen funcionando | 15% |
| Manejo de errores | 10% |
| Calidad del código | 15% |

---

## 🚀 Compilación

```bash
make entrega3
./programa
```

---

## 📤 Entrega

- Subir a GitHub en carpeta `Entrega3/`
- Incluir imágenes de ejemplo en ambos formatos (`.ppm` P3 y P6)
- El programa debe poder leer cualquiera de los dos formatos
- Guardar una misma imagen en P3 y P6 y mostrar la diferencia de tamaño
