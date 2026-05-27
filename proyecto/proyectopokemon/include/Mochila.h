#ifndef MOCHILA_H
#define MOCHILA_H

#include "Pokemon.h"

enum TipoItem { Pocion, MT, Baya };

struct Item {
    char* nombre;
    TipoItem tipo;
    int cantidad;
};

class Mochila {
private:
    Item* items;
    int numItems;

public:
    Mochila();
    ~Mochila();

    void agregarItem(const char* nombre, TipoItem tipo, int cant);
    bool usarItem(int indice, Pokemon& destino);
    void mostrar() const;
    int getNumItems() const;
    const char* getNombreItem(int indice) const;
    int getCantidadItem(int indice) const;
};

#endif
