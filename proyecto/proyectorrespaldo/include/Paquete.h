#ifndef PAQUETE_H
#define PAQUETE_H

#include "Entrada.h"

class Paquete {
private:
    Entrada* entradas;
    int numEntradas;
    int capacidad;

    void liberarEntrada(Entrada& e);
    void copiarEntrada(Entrada& dest, const Entrada& src);

public:
    Paquete();
    Paquete(const char* nombre);
    Paquete(const Paquete& otro);
    Paquete& operator=(const Paquete& otro);
    ~Paquete();

    void agregar(const char* nombre, const char* datos, int tamano);
    Entrada* extraer(int indice);
    void eliminar(int indice);
    void listar() const;
    int* buscar(const char* patron, int& numResultados) const;
    int tamanoTotal() const;
    int getNumEntradas() const;
    void actualizarChecksum(int indice);
    const char* getNombre() const;
    void setNombre(const char* nombre);
};

#endif
