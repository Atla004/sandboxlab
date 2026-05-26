# Como instalar CMake

CMake es un sistema de compilacion multiplataforma que genera archivos de proyecto para diferentes compiladores.

---

## Windows

### Opcion 1: Instalador oficial (recomendado)

1. Descarga el instalador desde [cmake.org/download](https://cmake.org/download/)
2. Ejecuta el instalador y selecciona **"Add CMake to the system PATH for all users"**
3. Reinicia tu terminal

Verifica la instalacion:

```cmd
cmake --version
```

### Opcion 2: Winget

```powershell
winget install Kitware.CMake
```

### Opcion 3: Chocolatey

```powershell
choco install cmake
```

---

## Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install cmake
```

Verifica la instalacion:

```bash
cmake --version
```

---

## Linux (Fedora / RHEL / CentOS)

```bash
sudo dnf install cmake
```

## macOS

Con Homebrew:

```bash
brew install cmake
```

O descarga el instalador desde [cmake.org](https://cmake.org/download/)

Verifica la instalacion:

```bash
cmake --version
```

---

## Compilador C++ requerido

CMake necesita un compilador instalado para funcionar:

- **Linux:** `sudo apt install build-essential`
- **macOS:** `xcode-select --install`
- **Windows:** El compilador viene con Visual Studio Build Tools o MinGW

---

## Verificar que funciona

Desde la carpeta del proyecto, ejecuta:

```bash
mkdir build
cd build
cmake ..
make
./programa
```

Si ves la salida del programa, CMake esta instalado correctamente.
