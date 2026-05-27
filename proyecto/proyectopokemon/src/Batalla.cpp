#include "Batalla.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

static void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int leerInt(int min, int max) {
    int val;
    while (true) {
        std::cin >> val;
        limpiarBuffer();
        if (val >= min && val <= max) return val;
        std::cout << "Opcion invalida. Ingresa un numero entre " << min << " y " << max << ": ";
    }
}

Batalla::Batalla(Entrenador& e1, Entrenador& e2) : e1(e1), e2(e2), turno(1) {}

int Batalla::calcularDano(Pokemon& atacante, Pokemon& defensor) {
    float nivel = atacante.getNivel();
    float mult = getMultiplicadorTipo(atacante.getTipo(), defensor.getTipo());
    int dano = (int)(((2 * nivel / 5 + 2) * atacante.getAtaque() / defensor.getDefensa() / 50 + 2) * mult);
    return dano;
}

void Batalla::iniciarBatalla(Mochila& mochila) {
    std::srand(std::time(nullptr));

    std::cout << "\n¡Comienza la batalla entre " << e1.getNombre() << " y " << e2.getNombre() << "!" << std::endl;

    int idx1 = 0, idx2 = 0;
    turno = 1;

    while (true) {
        Pokemon* p1 = e1.getPokemon(idx1);
        Pokemon* p2 = e2.getPokemon(idx2);

        if (!p1 || !p2 || !p1->estaVivo() || !p2->estaVivo()) {
            if (!p1 || !p1->estaVivo()) {
                for (int i = 0; i < e1.getNumPokemon(); i++) {
                    if (e1.getPokemon(i)->estaVivo()) { idx1 = i; p1 = e1.getPokemon(i); break; }
                }
                if (!p1 || !p1->estaVivo()) {
                    std::cout << "\n¡" << e2.getNombre() << " derrota a " << e1.getNombre() << "!" << std::endl;
                    break;
                }
                std::cout << e1.getNombre() << " envia a " << p1->getNombre() << "!" << std::endl;
            }
            if (!p2 || !p2->estaVivo()) {
                for (int i = 0; i < e2.getNumPokemon(); i++) {
                    if (e2.getPokemon(i)->estaVivo()) { idx2 = i; p2 = e2.getPokemon(i); break; }
                }
                if (!p2 || !p2->estaVivo()) {
                    std::cout << "\n¡" << e1.getNombre() << " derrota a " << e2.getNombre() << "!" << std::endl;
                    break;
                }
                std::cout << e2.getNombre() << " envia a " << p2->getNombre() << "!" << std::endl;
            }
        }

        std::cout << "\n===== TURNO " << turno << " =====" << std::endl;
        std::cout << p1->getNombre() << " - HP: " << p1->getVida() << "/" << p1->getVidaMax()
                  << "   |   " << p2->getNombre() << " - HP: " << p2->getVida() << "/" << p2->getVidaMax() << std::endl;

        std::cout << "\n[Turno de " << e1.getNombre() << " - TU DECIDES]" << std::endl;
        std::cout << "1. Atacar" << std::endl;
        std::cout << "2. Usar objeto" << std::endl;
        std::cout << "3. Cambiar Pokemon" << std::endl;
        std::cout << "> ";
        int accion = leerInt(1, 3);

        if (accion == 1) {
            int dano = calcularDano(*p1, *p2);
            p2->recibirDano(dano);
            std::cout << p1->getNombre() << " ataca! Causa " << dano << " de dano!" << std::endl;
            std::cout << p2->getNombre() << " - HP: " << p2->getVida() << "/" << p2->getVidaMax() << std::endl;
        } else if (accion == 2) {
            if (mochila.getNumItems() > 0) {
                mochila.mostrar();
                std::cout << "Que objeto usar? (1-" << mochila.getNumItems() << "): ";
                int itemIdx = leerInt(1, mochila.getNumItems()) - 1;
                mochila.usarItem(itemIdx, *p1);
            } else {
                std::cout << "No tienes objetos!" << std::endl;
                continue;
            }
        } else {
            std::cout << "Cambiar a que Pokemon?" << std::endl;
            for (int i = 0; i < e1.getNumPokemon(); i++) {
                std::cout << (i + 1) << ". ";
                e1.getPokemon(i)->mostrar();
            }
            std::cout << "> ";
            int nuevo = leerInt(1, e1.getNumPokemon()) - 1;
            if (nuevo != idx1) {
                idx1 = nuevo;
                p1 = e1.getPokemon(idx1);
                std::cout << e1.getNombre() << " envia a " << p1->getNombre() << "!" << std::endl;
            }
        }

        if (!p2->estaVivo()) { turno++; continue; }

        std::cout << "\n[Turno de " << e2.getNombre() << " - IA]" << std::endl;
        int accionIA = std::rand() % 10;
        if (accionIA < 7) {
            int dano = calcularDano(*p2, *p1);
            p1->recibirDano(dano);
            std::cout << p2->getNombre() << " ataca a " << p1->getNombre() << "! Causa " << dano << " de dano!" << std::endl;
            std::cout << p1->getNombre() << " - HP: " << p1->getVida() << "/" << p1->getVidaMax() << std::endl;
        } else {
            std::cout << p2->getNombre() << " usa un objeto! (IA no implementada en E1)" << std::endl;
        }

        turno++;
    }
}
