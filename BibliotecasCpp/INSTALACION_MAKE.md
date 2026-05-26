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

![[Pasted image 20260526000925.png]]
1. Haz clic en el boton **"Variables de entorno..."** en la parte inferior.

2. En la seccion **"Variables del sistema"** (abajo), busca la variable llamada `Path`, selecciona y haz clic en **"Editar"**.
![[Pasted image 20260526001524.png]]
3. Haz clic en **"Nuevo"** y agrega la ruta a la carpeta `bin` de tu compilador.
   ![[Pasted image 20260526001637.png]]
   - **Dev-C++:** El compilador viene incluido dentro de la carpeta de instalacion de Dev-C++. La ruta generalmente es:
     - `C:\Program Files (x86)\Dev-Cpp\MinGW64\bin`
     - Si no la encuentras ahi, abre Dev-C++, ve a **Herramientas > Opciones del compilador** y copia la ruta que aparece en "Directorio de binarios".
   - **MinGW independiente:** Generalmente es `C:\MinGW\bin`
   - **MSYS2:** Generalmente es `C:\msys64\mingw64\bin`

   > **Tip:** Si no estas seguro de donde esta tu compilador, puedes buscar `g++.exe` en el explorador de archivos. La carpeta que lo contiene es la que debes agregar al PATH.


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
