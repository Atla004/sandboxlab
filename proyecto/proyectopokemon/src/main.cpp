#include <iostream>
#include <limits>
#include "Pokemon.h"
#include "Entrenador.h"
#include "Mochila.h"
#include "Batalla.h"

static void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void esperarEnter() {
    std::cout << "\nPresiona Enter para volver...";
    limpiarBuffer();
    std::cin.get();
}

int main() {
    // Crear Pokemon de Ash
    Pokemon pika("Pika", "Pikachu", "Electrico", 110, 55, 40, 90, 25);
    Pokemon charmander("Char", "Charmander", "Fuego", 100, 52, 43, 65, 28);
    Pokemon squirtle("Squirtle", "Squirtle", "Agua", 110, 48, 65, 43, 22);

    // Crear Pokemon de Brock
    Pokemon geodude("Rocky", "Geodude", "Normal", 120, 80, 100, 20, 20);
    Pokemon onix("Onix", "Onix", "Normal", 150, 45, 160, 30, 25);
    Pokemon rhyhorn("Rhy", "Rhyhorn", "Normal", 130, 85, 95, 25, 22);

    // Crear entrenadores
    Entrenador ash("Ash");
    ash.agregarPokemon(pika);
    ash.agregarPokemon(charmander);
    ash.agregarPokemon(squirtle);

    Entrenador brock("Brock");
    brock.agregarPokemon(geodude);
    brock.agregarPokemon(onix);
    brock.agregarPokemon(rhyhorn);

    // Crear mochila
    Mochila mochila;
    mochila.agregarItem("Pocion", Pocion, 3);
    mochila.agregarItem("Baya", Baya, 2);

    int opcion;
    do {
        std::cout << "\n=== POKEMON BATTLE SIMULATOR ===" << std::endl;
        std::cout << "1. Ver equipo de Ash" << std::endl;
        std::cout << "2. Ver equipo de Brock" << std::endl;
        std::cout << "3. Ver mochila" << std::endl;
        std::cout << "4. Iniciar batalla" << std::endl;
        std::cout << "5. Salir" << std::endl;
        std::cout << "> ";
        std::cin >> opcion;
        limpiarBuffer();

        if (opcion == 1) {
            ash.mostrar();
            esperarEnter();
        } else if (opcion == 2) {
            brock.mostrar();
            esperarEnter();
        } else if (opcion == 3) {
            mochila.mostrar();
            esperarEnter();
        } else if (opcion == 4) {
            Batalla batalla(ash, brock);
            batalla.iniciarBatalla(mochila);
            std::cout << "\nVolver al menu? (s/n): ";
            char resp;
            std::cin >> resp;
            limpiarBuffer();
            if (resp != 's' && resp != 'S') break;
        }
    } while (opcion != 5);

    std::cout << "Saliendo..." << std::endl;
    return 0;
}
