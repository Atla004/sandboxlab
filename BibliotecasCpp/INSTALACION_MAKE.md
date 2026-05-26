# Como instalar Make

Make es una herramienta que automatiza la compilacion de proyectos usando un archivo `Makefile`.

---

## Windows

### Si ya tienes un compilador (Dev-C++, MinGW, etc.)

Si ya puedes compilar programas pero el comando `make` no funciona, probablemente solo necesitas agregar tu compilador al PATH de Windows.

**Paso 1: Verifica si g++ esta en el PATH**

Abre una terminal (CMD o PowerShell) y ejecuta:

```cmd
g++ --version
```

Si ves informacion de la version, tu compilador esta accesible. Si dice "no se reconoce", necesitas instalar un compilador primero.

**Paso 2: Verifica si make esta disponible**

Ejecuta:

```cmd
make --version
```

> **Nota:** Algunos compiladores usan `mingw32-make` en lugar de `make`. Prueba tambien:
> ```cmd
> mingw32-make --version
> ```

Si `g++` funciona pero `make` (o `mingw32-make`) no, sigue los pasos para agregarlo al PATH.

### Como agregar el compilador al PATH

1. Abre el menu de inicio y busca **"Variables de entorno"**. Selecciona **"Editar las variables de entorno del sistema"**.

   > ![Paso 1: Buscar variables de entorno](IMAGEN_PASO_1_AQUI)

2. Haz clic en el boton **"Variables de entorno..."** en la parte inferior.

   > ![Paso 2: Boton variables de entorno](IMAGEN_PASO_2_AQUI)

3. En la seccion **"Variables del sistema"** (abajo), busca la variable llamada `Path`, selecciona y haz clic en **"Editar"**.

   > ![Paso 3: Editar Path](IMAGEN_PASO_3_AQUI)

4. Haz clic en **"Nuevo"** y agrega la ruta a la carpeta `bin` de tu compilador.
   
   - **Dev-C++:** Generalmente es `C:\Program Files (x86)\Dev-Cpp\MinGW64\bin`
   - **MinGW:** Generalmente es `C:\MinGW\bin`
   - **MSYS2:** Generalmente es `C:\msys64\mingw64\bin`

   > ![Paso 4: Agregar ruta al PATH](IMAGEN_PASO_4_AQUI)

5. Haz clic en **"Aceptar"** en todas las ventanas y **reinicia tu terminal** para que los cambios surtan efecto.

6. Verifica nuevamente:

```cmd
make --version
```

---

## Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install build-essential
```

Verifica la instalacion:

```bash
make --version
```

---

## Linux (Fedora / RHEL / CentOS)

```bash
sudo dnf install make gcc gcc-c++
```

O en versiones antiguas:

```bash
sudo yum install make gcc gcc-c++
```

---

## macOS

Make viene incluido con las herramientas de desarrollo de Xcode:

```bash
xcode-select --install
```

Verifica la instalacion:

```bash
make --version
```

---

## Verificar que funciona

Desde la carpeta del proyecto, ejecuta:

```bash
make
make run
```

Si ves la salida del programa, Make esta instalado correctamente.
