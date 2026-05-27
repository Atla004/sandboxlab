#include "Integridad.h"

unsigned int Integridad::calcularChecksum(const char* datos, int tamano) {
    unsigned int cs = 0;
    for (int i = 0; i < tamano; i++) {
        cs = (cs << 5) + (unsigned char)datos[i];
    }
    return cs;
}

bool Integridad::verificar(const Entrada& entrada) {
    unsigned int calculado = calcularChecksum(entrada.datos, entrada.tamano);
    return calculado == entrada.checksum;
}

char* Integridad::cifrarXOR(const char* datos, int tamano, char clave, int& tamSalida) {
    tamSalida = tamano;
    char* salida = new char[tamano];
    for (int i = 0; i < tamano; i++) {
        salida[i] = datos[i] ^ clave;
    }
    return salida;
}

char* Integridad::descifrarXOR(const char* datos, int tamano, char clave, int& tamSalida) {
    return cifrarXOR(datos, tamano, clave, tamSalida);
}

bool Integridad::comparar(const Entrada& e1, const Entrada& e2) {
    return e1.checksum == e2.checksum && e1.tamano == e2.tamano;
}
