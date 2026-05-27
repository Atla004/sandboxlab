#ifndef INTEGRIDAD_H
#define INTEGRIDAD_H

#include "Entrada.h"

class Integridad {
public:
    static unsigned int calcularChecksum(const char* datos, int tamano);
    static bool verificar(const Entrada& entrada);
    static char* cifrarXOR(const char* datos, int tamano, char clave, int& tamSalida);
    static char* descifrarXOR(const char* datos, int tamano, char clave, int& tamSalida);
    static bool comparar(const Entrada& e1, const Entrada& e2);
};

#endif
