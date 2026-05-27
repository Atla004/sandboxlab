#ifndef BATALLA_H
#define BATALLA_H

#include "Entrenador.h"
#include "Mochila.h"

class Batalla {
private:
    Entrenador& e1;
    Entrenador& e2;
    int turno;

    int calcularDano(Pokemon& atacante, Pokemon& defensor);

public:
    Batalla(Entrenador& e1, Entrenador& e2);
    void iniciarBatalla(Mochila& mochila);
};

#endif
