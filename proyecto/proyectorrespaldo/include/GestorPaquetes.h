#ifndef GESTORPAQUETES_H
#define GESTORPAQUETES_H

#include "Paquete.h"

struct ResultadoBusqueda {
    int paqueteIdx;
    int entradaIdx;
};

class GestorPaquetes {
private:
    Paquete* paquetes;
    int numPaquetes;
    int capacidad;

public:
    GestorPaquetes(int maxPaquetes);
    ~GestorPaquetes();

    void crearPaquete(const char* nombre);
    Paquete* fusionar(int idx1, int idx2);
    Paquete* separar(int idxOrigen, int* indices, int numIndices);
    ResultadoBusqueda* buscarEnTodos(const char* patron, int& numResultados);
    void listarTodo() const;
    void eliminarPaquete(int indice);
    Paquete* getPaquete(int indice);
    int getNumPaquetes() const;
};

#endif
