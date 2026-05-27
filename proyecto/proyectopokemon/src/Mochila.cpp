#include "Mochila.h"
#include <iostream>
#include <cstring>

Mochila::Mochila() : items(nullptr), numItems(0) {}

Mochila::~Mochila() {
    for (int i = 0; i < numItems; i++) {
        delete[] items[i].nombre;
    }
    delete[] items;
}

void Mochila::agregarItem(const char* nombre, TipoItem tipo, int cant) {
    Item* nuevo = new Item[numItems + 1];
    for (int i = 0; i < numItems; i++) {
        nuevo[i] = items[i];
    }
    nuevo[numItems].nombre = new char[strlen(nombre) + 1];
    strcpy(nuevo[numItems].nombre, nombre);
    nuevo[numItems].tipo = tipo;
    nuevo[numItems].cantidad = cant;
    delete[] items;
    items = nuevo;
    numItems++;
}

bool Mochila::usarItem(int indice, Pokemon& destino) {
    if (indice < 0 || indice >= numItems) return false;
    if (items[indice].cantidad <= 0) return false;

    switch (items[indice].tipo) {
        case Pocion:
            destino.curar();
            std::cout << "Usaste " << items[indice].nombre << "! " << destino.getNombre() << " recupero toda su vida!" << std::endl;
            break;
        case Baya:
            int curacion = 30;
            int nuevaVida = destino.getVida() + curacion;
            if (nuevaVida > destino.getVidaMax()) nuevaVida = destino.getVidaMax();
            destino.recibirDano(destino.getVida() - nuevaVida);
            std::cout << "Usaste " << items[indice].nombre << "! " << destino.getNombre() << " recupero " << curacion << " HP!" << std::endl;
            break;
        case MT:
            std::cout << "Usaste " << items[indice].nombre << "! (Efecto no implementado en E1)" << std::endl;
            break;
    }

    items[indice].cantidad--;
    if (items[indice].cantidad <= 0) {
        delete[] items[indice].nombre;
        for (int i = indice; i < numItems - 1; i++) {
            items[i] = items[i + 1];
        }
        numItems--;
    }
    return true;
}

void Mochila::mostrar() const {
    std::cout << "--- MOCHILA ---" << std::endl;
    for (int i = 0; i < numItems; i++) {
        std::cout << (i + 1) << ". " << items[i].nombre << " x" << items[i].cantidad << std::endl;
    }
}

int Mochila::getNumItems() const { return numItems; }
const char* Mochila::getNombreItem(int indice) const { return items[indice].nombre; }
int Mochila::getCantidadItem(int indice) const { return items[indice].cantidad; }
