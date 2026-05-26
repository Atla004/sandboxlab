# Como instalar Make

Make es una herramienta que automatiza la compilacion de proyectos usando un archivo `Makefile`.

---

## Windows

### Opcion 1: MSYS2 (recomendado)

1. Descarga e instala [MSYS2](https://www.msys2.org/)
2. Abre MSYS2 y ejecuta:

```bash
pacman -Syu
pacman -S make
```

3. Agrega `C:\msys64\usr\bin` a tu PATH de Windows.

### Opcion 2: MinGW

1. Descarga [MinGW](https://www.mingw-w64.org/)
2. Durante la instalacion, asegurate de seleccionar `mingw32-make`
3. Agrega la carpeta `bin` de MinGW a tu PATH

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
