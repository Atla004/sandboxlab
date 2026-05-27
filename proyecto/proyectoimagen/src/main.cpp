#include <iostream>
#include <limits>
#include "Imagen.h"
#include "Filtro.h"

static void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void esperarEnter() {
    std::cout << "\nPresiona Enter para volver...";
    limpiarBuffer();
    std::cin.get();
}

int main() {
    // Crear imagen A: degradado 20x10
    Imagen imgA(20, 10);
    for (int f = 0; f < 10; f++) {
        for (int c = 0; c < 20; c++) {
            unsigned char brillo = (unsigned char)((f * 255 / 10 + c * 255 / 20) / 2);
            imgA.setPixel(f, c, {brillo, brillo, brillo});
        }
    }

    // Crear imagen B: ajedrez 8x8
    Imagen imgB(8, 8);
    for (int f = 0; f < 8; f++) {
        for (int c = 0; c < 8; c++) {
            if ((f + c) % 2 == 0)
                imgB.setPixel(f, c, {255, 255, 255});
            else
                imgB.setPixel(f, c, {0, 0, 0});
        }
    }

    int opcion;
    do {
        std::cout << "\n=== EDITOR DE IMAGENES PPM ===" << std::endl;
        std::cout << "1. Ver imagen A" << std::endl;
        std::cout << "2. Ver imagen B" << std::endl;
        std::cout << "3. Aplicar filtro a imagen A" << std::endl;
        std::cout << "4. Aplicar filtro a imagen B" << std::endl;
        std::cout << "5. Mezclar imagenes A y B" << std::endl;
        std::cout << "6. Salir" << std::endl;
        std::cout << "> ";
        std::cin >> opcion;
        limpiarBuffer();

        Imagen* imgSeleccionada = nullptr;
        const char* nombre = "";

        if (opcion == 1) {
            std::cout << "\n--- IMAGEN A (" << imgA.getAncho() << "x" << imgA.getAlto() << ") ---" << std::endl;
            imgA.mostrarASCII();
            esperarEnter();
        } else if (opcion == 2) {
            std::cout << "\n--- IMAGEN B (" << imgB.getAncho() << "x" << imgB.getAlto() << ") ---" << std::endl;
            imgB.mostrarASCII();
            esperarEnter();
        } else if (opcion == 3 || opcion == 4) {
            imgSeleccionada = (opcion == 3) ? &imgA : &imgB;
            nombre = (opcion == 3) ? "A" : "B";

            int filtro;
            std::cout << "\n--- FILTROS DISPONIBLES ---" << std::endl;
            std::cout << "1. Invertir colores" << std::endl;
            std::cout << "2. Ajustar brillo (+50)" << std::endl;
            std::cout << "3. Ajustar brillo (-50)" << std::endl;
            std::cout << "4. Umbral (blanco y negro)" << std::endl;
            std::cout << "> ";
            std::cin >> filtro;
            limpiarBuffer();

            switch (filtro) {
                case 1:
                    Filtro::invertir(*imgSeleccionada);
                    std::cout << "Filtro aplicado: Invertir colores" << std::endl;
                    break;
                case 2:
                    Filtro::brillo(*imgSeleccionada, 50);
                    std::cout << "Filtro aplicado: Brillo +50" << std::endl;
                    break;
                case 3:
                    Filtro::brillo(*imgSeleccionada, -50);
                    std::cout << "Filtro aplicado: Brillo -50" << std::endl;
                    break;
                case 4:
                    Filtro::umbral(*imgSeleccionada, 128);
                    std::cout << "Filtro aplicado: Umbral" << std::endl;
                    break;
                default:
                    std::cout << "Opcion invalida" << std::endl;
                    continue;
            }

            std::cout << "\n--- IMAGEN " << nombre << " (resultado) ---" << std::endl;
            imgSeleccionada->mostrarASCII();
            esperarEnter();
        } else if (opcion == 5) {
            Imagen mezclada(imgA);
            Filtro::mezclar(mezclada, imgB, 0.5f);
            std::cout << "\nMezclando A + B al 50%..." << std::endl;
            std::cout << "Imagen mezclada de " << mezclada.getAncho() << "x" << mezclada.getAlto() << " creada." << std::endl;
            std::cout << "\n--- IMAGEN MEZCLADA ---" << std::endl;
            mezclada.mostrarASCII();
            esperarEnter();
        }
    } while (opcion != 6);

    std::cout << "Saliendo..." << std::endl;
    return 0;
}
