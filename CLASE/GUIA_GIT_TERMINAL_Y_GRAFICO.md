# Guía de Git: Terminal y Gráfico

Guía básica para Programación 2.

---

## 1. ¿Qué es Git?

Git es un **sistema de control de versiones**. ¿Qué significa eso?

Imagina que estás haciendo un trabajo en equipo:

- Sin Git: os pasáis archivos por WhatsApp, cada uno tiene una copia diferente, al final no sabes cuál es la versión final, alguien borra algo importante y no hay forma de recuperarlo.
- Con Git: todos trabajan sobre el mismo repositorio, cada cambio queda registrado con fecha, autor y motivo. Si algo se rompe, puedes volver a cualquier versión anterior. Si dos personas trabajan a la vez, Git ayuda a fusionar los cambios.

### Conceptos fundamentales

| Concepto | ¿Qué es? |
|----------|----------|
| **Repositorio** | Carpeta donde Git guarda el historial de cambios (la carpeta `.git/`) |
| **Commit** | Una "foto" del estado de tu proyecto en un momento concreto. Cada commit tiene un identificador único (hash) |
| **Rama (branch)** | Una línea de desarrollo independiente. La rama principal se llama `main` |
| **Working directory** | Tus archivos actuales, los que estás editando ahora |
| **Staging area** | Zona intermedia donde preparas qué cambios incluir en el próximo commit |
| **Repositorio remoto** | Una copia del repositorio alojada en un servidor (GitHub, GitLab, etc.) |

### Flujo básico de trabajo

Tu código pasa por 3 estados antes de guardarse:

```
Working Directory  →  git add  →  Staging Area  →  git commit  →  Repositorio Local
(archivos que          (preparar        (foto lista        (guardado en
 editas)                cambios)         para guardar)      .git/)
```

Y luego, si quieres compartirlo:

```
Repositorio Local  →  git push  →  GitHub/GitLab
(guardado en tu PC)                (en la nube)
```

Para traer cambios de otros:

```
GitHub/GitLab  →  git pull  →  Repositorio Local
                               (tu PC actualizado)
```

---

## 2. GitHub y GitLab

Git es el programa que corre en tu máquina. **GitHub** y **GitLab** son servicios web que alojan repositorios Git en la nube.

| Plataforma | Descripción |
|------------|-------------|
| **GitHub** | El más popular. Gratuito para repositorios públicos y privados. Tiene herramientas como Pull Requests, Issues, Actions |
| **GitLab** | Similar a GitHub. Ofrece CI/CD integrado. Usado en empresas |
| **Bitbucket** | De Atlassian. Integración con Jira. Usado en entornos corporativos |

En esta guía usaremos **GitHub**, pero los comandos de Git son los mismos para todas.

---

## 3. Instalación y configuración

### Instalar Git

```bash
# Ubuntu/Debian
sudo apt install git

# macOS (con Homebrew)
brew install git

# Windows
# Descargar desde https://git-scm.com/ e instalar
# Marcar "Git Bash" durante la instalación
```

### Configurar por primera vez (solo una vez por máquina)

```bash
git config --global user.name "Tu Nombre"
git config --global user.email "tuemail@ejemplo.com"
git config --global init.defaultBranch main
```

**¿Por qué?** Cada commit queda marcado con tu nombre y email. Así se sabe quién hizo cada cambio.

Verificar configuración:
```bash
git config --list
```

---

## 4. Comandos básicos (terminal)

Todos los comandos de Git empiezan con `git` seguido de una acción.

### 4.1. Iniciar un repositorio

```bash
# Dentro de la carpeta de tu proyecto
cd ruta/de/tu/proyecto

# Inicializar Git en esta carpeta
git init
```

Esto crea la carpeta oculta `.git/`. Ahí dentro Git guardará TODO el historial. No la borres ni la toques.

### 4.2. Ver el estado

```bash
git status
```

**Es el comando que más vas a usar.** Te dice:
- En qué rama estás
- Qué archivos has modificado
- Qué archivos están preparados para commit
- Qué archivos nuevos (untracked) has creado

### 4.3. Añadir archivos al staging

```bash
# Un archivo específico
git add main.cpp

# Varios archivos
git add main.cpp persona.h persona.cpp

# Todos los archivos cambiados
git add .
```

**¿Por qué existe el staging?** Para que puedas elegir QUÉ cambios incluir en cada commit. Puedes tener 5 archivos modificados y solo commiteear 2 de ellos.

### 4.4. Hacer un commit

```bash
git commit -m "Añadida validación de datos de entrada"
```

Cada commit debe tener un mensaje **descriptivo**. En FUTURO, cuando mires atrás, el mensaje te dirá qué cambió sin tener que leer el código.

**Buenos mensajes:**
- `"Corregido error al calcular descuento"`
- `"Añadida clase Persona con memoria dinámica"`
- `"Refactorizado menú principal"`

**Malos mensajes:**
- `"fix"`
- `"cosas"`
- `"cambios"`

### 4.5. Ver el historial

```bash
git log

# Más compacto (una línea por commit)
git log --oneline

# Con gráfico de ramas
git log --oneline --graph --all
```

Cada commit tiene un hash (identificador único). Ejemplo:
```
abc1234 (HEAD -> main) Corregido error al calcular descuento
def5678 Añadida clase Persona con memoria dinámica
ghi9012 Commit inicial
```

### 4.6. Ver diferencias

```bash
# Cambios sin staging (en working directory)
git diff

# Cambios ya en staging
git diff --staged
```

Muestra línea por línea qué se añadió (+) y qué se borró (-).

### 4.7. Deshacer cambios

```bash
# Quitar archivo del staging (sin borrar lo editado)
git reset HEAD main.cpp

# También válido (Git >= 2.23):
git restore --staged main.cpp

# Descartar cambios de un archivo (VOLVER ATRÁS, no recuperable)
git checkout -- main.cpp
# O:
git restore main.cpp

# Deshacer el último commit (manteniendo los cambios en disco)
git reset --soft HEAD~1
```

---

## 5. Ramas (branches)

### ¿Qué es una rama?

Una rama es una **línea de desarrollo independiente**. Piensa en ello como un "universo paralelo" de tu código.

```
main ──●────●────●──────────────────
              \
feature-xyz    ●────●────●
```

**¿Para qué sirven?**
- Trabajar en una nueva funcionalidad sin romper el código estable
- Cada miembro del equipo trabaja en su propia rama
- Experimentar sin miedo a dañar el proyecto principal

### Comandos de ramas

```bash
# Listar ramas (la que tiene * es la actual)
git branch

# Crear una rama nueva
git branch nombre-de-la-rama

# Cambiar a otra rama
git checkout nombre-de-la-rama
# O:
git switch nombre-de-la-rama

# Crear y cambiar en un solo paso
git checkout -b nombre-de-la-rama
# O:
git switch -c nombre-de-la-rama

# Fusionar una rama en la actual
git checkout main        # ve a main
git merge nombre-rama    # trae los cambios de nombre-rama aquí

# Borrar una rama (después de fusionarla)
git branch -d nombre-rama
```

### Tags (para marcar entregas)

Los tags son puntos fijos en el historial. Útiles para marcar versiones o entregas.

```bash
# Crear tag
git tag -a v1.0 -m "Entrega 1 completa"

# Listar tags
git tag

# Subir tags a GitHub
git push origin --tags
```

---

## 6. Trabajo remoto (GitHub)

### 6.1. Conectar un repositorio local con GitHub

```bash
# 1. Crear un repositorio VACÍO en GitHub (web)
#    NO marcar "Initialize with README"

# 2. Enlazar tu local con el remoto
git remote add origin https://github.com/tuusuario/nombre-repo.git

# 3. Subir todo
git push -u origin main
```

### 6.2. Clonar un repositorio existente

```bash
git clone https://github.com/tuusuario/nombre-repo.git
```

Esto descarga todo el proyecto a tu máquina, incluyendo TODO el historial de commits (no solo la última versión).

### 6.3. Subir cambios (push)

```bash
git push origin main
```

Sube tus commits locales a GitHub. **Hazlo después de cada sesión de trabajo** para no perder nada.

### 6.4. Bajar cambios (pull)

```bash
git pull origin main
```

Trae los cambios que otros hayan subido a GitHub. **Hazlo al empezar a trabajar** para asegurarte de que tienes lo último.

### 6.5. Ver remotos configurados

```bash
git remote -v
```

Muestra la URL del repositorio remoto al que está conectado tu local.

### Resumen de conexión local-remoto

```
LOCAL (tu PC)                    GITHUB (nube)
┌─────────────────┐    push     ┌─────────────────┐
│ Repositorio Git  │ ────────→  │ Repositorio      │
│ (commits locales)│            │ (copia remota)   │
│                  │ ←────────  │                  │
│                  │    pull    │                  │
└─────────────────┘            └─────────────────┘
```

---

## 7. Resolver conflictos

### ¿Qué es un conflicto?

Ocurre cuando Git **no puede fusionar automáticamente** dos cambios porque dos personas modificaron las mismas líneas del mismo archivo.

### Escenario típico

```bash
# Persona A hace commit y push
git add .
git commit -m "Cambio A"
git push

# Persona B (sin hacer pull) intenta push
git add .
git commit -m "Cambio B"
git push  # ERROR: rejected!
```

### Cómo resolverlo

```bash
# 1. Traer cambios de GitHub
git pull origin main

# 2. Git marca los archivos conflictivos con este aspecto:
```

El archivo conflictivo mostrará:

```cpp
<<<<<<< HEAD
int precio = 100;      // Tu cambio (local)
=======
int precio = 150;      // Cambio de otro (remoto)
>>>>>>> main
```

```bash
# 3. TÚ decides qué queda. Editas el archivo y borras los marcadores:

int precio = 100;      // Decides mantener tu valor
```

```bash
# 4. Añadir el archivo resuelto y hacer commit
git add archivo.cpp
git commit -m "Resuelto conflicto en archivo.cpp"

# 5. Subir
git push
```

**Regla de oro:** haz `git pull` antes de empezar a trabajar así reduces drásticamente los conflictos.

---

## 8. .gitignore

El archivo `.gitignore` le dice a Git qué archivos NO debe rastrear.

### ¿Por qué es necesario?

Hay archivos que no deberían estar en el repositorio:
- Ejecutables compilados (.exe, .out, programa)
- Archivos temporales del compilador (.o, .obj)
- Carpetas de build (build/, output/)
- Configuración del IDE (.vscode/, .idea/)
- Archivos del sistema (.DS_Store, Thumbs.db)
- Archivos generados por el programa (datos creados por el usuario)

### Ejemplo mínimo

```gitignore
# Ejecutables
*.exe
programa

# Archivos objeto
*.o
*.obj

# Carpeta de build
build/
output/

# IDE
.vscode/

# Sistema
.DS_Store
```

Crear el archivo `gitignore` antes del primer commit o con:
```bash
echo "programa" >> .gitignore
echo "*.o" >> .gitignore
```

---

## 9. Git gráfico con VSCode

VSCode tiene Git integrado sin instalar nada extra.

### Panel de Control de Código Fuente

1. Abre tu proyecto en VSCode
2. Haz clic en el icono **Control de código fuente** (tercer icono, parece una "Y"):
   ```
   ┌──────────────────────┐
   │ ◉ SOURCE CONTROL     │
   │ ──────────────────   │
   │                       │
   │ Changes (3)           │
   │  M main.cpp      [+] │  ← modificado
   │  U nuevo.h       [+] │  ← nuevo (untracked)
   │  D viejo.h       [+] │  ← borrado
   │                       │
   │ [✓] Commit           │
   │ ...                   │
   └──────────────────────┘
   ```

### Pasos en VSCode

| Acción | Cómo hacerlo |
|--------|-------------|
| Ver qué cambió | Clic en el archivo → se abre el diff |
| Añadir a staging | Clic en `+` junto al archivo |
| Hacer commit | Escribe mensaje → clic en ✓ |
| Push | `...` → "Push" |
| Pull | `...` → "Pull" |
| Crear rama | Click en main (abajo izq) → "Create new branch" |

### Atajos útiles

- `Ctrl + Shift + G`: abrir panel de Git
- Clic en el nombre de la rama en la barra inferior para cambiar de rama

---

## 10. Git gráfico con GitHub Desktop

GitHub Desktop es una aplicación con interfaz visual para Git. Ideal si la terminal te resulta incómoda.

### Instalación

- **Windows/macOS**: descargar de https://desktop.github.com/
- **Linux**: está disponible pero con menos soporte oficial

### Flujo visual

```
┌─────────────────────────────────────────────────┐
│ GitHub Desktop                                   │
│                                                  │
│  Current Repo: proyecto-pokemon                  │
│  Current Branch: main                            │
│                                                  │
│  ┌─────────────┐   ┌────────────────────────┐   │
│  │ Changes      │   │ Diff                   │   │
│  │ ─────────    │   │ ──────                 │   │
│  │ ☑ pokemon.. │   │ + void setNombre(...)  │   │
│  │ ☐ main.cpp  │   │ +     delete[] nombre; │   │
│  │              │   │ +     nombre = new... │   │
│  └─────────────┘   └────────────────────────┘   │
│                                                  │
│  Summary: "Implementado setNombre"               │
│  Description: ...                                │
│                                                  │
│  ┌──────────────────────────────────────────┐    │
│  │           Commit to main                 │    │
│  └──────────────────────────────────────────┘    │
│                                                  │
│  [Fetch]  [Pull]  [Push]  [Branch]              │
└─────────────────────────────────────────────────┘
```

**Pasos básicos:**
1. Ves los cambios en el panel izquierdo
2. Seleccionas qué archivos incluir (checkbox)
3. Escribes un mensaje descriptivo
4. Clic en "Commit to main"
5. Clic en "Push origin" para subir a GitHub

---

## 11. Flujo diario típico

### Trabajo individual

```bash
# AL EMPEZAR: traer lo último
git pull origin main

# ... programar, modificar archivos ...

# Ver qué cambió
git status

# Preparar cambios
git add .

# Guardar cambios
git commit -m "Descripción de lo que hice"

# Subir a GitHub
git push origin main
```

### Trabajo en equipo

```bash
# Cada miembro en su propia rama durante el desarrollo:

# 1. Crear rama para mi funcionalidad
git checkout -b mi-feature

# 2. Programar y commiter en mi rama
git add .
git commit -m "Añadida funcionalidad X"

# 3. Traer cambios de main
git checkout main
git pull origin main

# 4. Fusionar mi feature en main
git merge mi-feature

# 5. Subir
git push origin main
```

### Al entregar

```bash
# Verificar que todo está commiteado
git status

# Ver historial
git log --oneline

# Crear tag de la entrega
git tag -a v1.0 -m "Entrega 1 completa"

# Subir tags
git push origin --tags
```

---

## Resumen: Los 10 comandos que usarás siempre

| Comando | ¿Qué hace? | ¿Cuándo? |
|---------|------------|----------|
| `git status` | Ver estado actual | Cada vez que dudes |
| `git add .` | Preparar todos los cambios | Antes de commit |
| `git commit -m "msg"` | Guardar cambios | Cuando completes algo |
| `git push origin main` | Subir a GitHub | Al final de la sesión |
| `git pull origin main` | Bajar de GitHub | Al empezar la sesión |
| `git log --oneline` | Ver historial | Para recordar qué hiciste |
| `git branch` | Listar ramas | Para ver dónde estás |
| `git checkout -b nombre` | Crear y cambiar rama | Para empezar algo nuevo |
| `git checkout main` | Volver a main | Antes de fusionar |
| `git merge nombre` | Fusionar rama | Para integrar cambios |

### Errores comunes y solución rápida

```
"Please tell me who you are"
  → git config --global user.name/email

"rejected: push failed"
  → git pull primero

"Merge conflict in archivo.cpp"
  → Editar archivo, borrar <<<<< ===== >>>>>, git add, git commit

"fatal: not a git repository"
  → Falta git init o estás en la carpeta equivocada

"Cannot pull: uncommitted changes"
  → git add + git commit o git stash
```
