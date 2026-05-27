#include "GestorPaquetes.h"
#include <iostream>
#include <cstring>

GestorPaquetes::GestorPaquetes(int maxPaquetes) : paquetes(nullptr), numPaquetes(0), capacidad(maxPaquetes) {
    paquetes = new Paquete[capacidad];
}

GestorPaquetes::~GestorPaquetes() {
    delete[] paquetes;
}

void GestorPaquetes::crearPaquete(const char* nombre) {
    if (numPaquetes < capacidad) {
        paquetes[numPaquetes] = Paquete();
        numPaquetes++;
    }
}

Paquete* GestorPaquetes::fusionar(int idx1, int idx2) {
    if (idx1 < 0 || idx1 >= numPaquetes || idx2 < 0 || idx2 >= numPaquetes) return nullptr;
    Paquete fusionado;
    for (int i = 0; i < paquetes[idx1].getNumEntradas(); i++) {
        Entrada* e = paquetes[idx1].extraer(i);
        fusionado.agregar(e->nombre, e->datos, e->tamano);
    }
    for (int i = 0; i < paquetes[idx2].getNumEntradas(); i++) {
        Entrada* e = paquetes[idx2].extraer(i);
        fusionado.agregar(e->nombre, e->datos, e->tamano);
    }
    Paquete* resultado = new Paquete(fusionado);
    return resultado;
}

Paquete* GestorPaquetes::separar(int idxOrigen, int* indices, int numIndices) {
    if (idxOrigen < 0 || idxOrigen >= numPaquetes) return nullptr;
    Paquete* separado = new Paquete();
    for (int i = 0; i < numIndices; i++) {
        Entrada* e = paquetes[idxOrigen].extraer(indices[i]);
        if (e) separado->agregar(e->nombre, e->datos, e->tamano);
    }
    return separado;
}

ResultadoBusqueda* GestorPaquetes::buscarEnTodos(const char* patron, int& numResultados) {
    ResultadoBusqueda* resultados = new ResultadoBusqueda[numPaquetes * 10];
    numResultados = 0;
    for (int p = 0; p < numPaquetes; p++) {
        int numEnPaquete = 0;
        int* indices = paquetes[p].buscar(patron, numEnPaquete);
        for (int i = 0; i < numEnPaquete; i++) {
            resultados[numResultados].paqueteIdx = p;
            resultados[numResultados].entradaIdx = indices[i];
            numResultados++;
        }
        delete[] indices;
    }
    return resultados;
}

void GestorPaquetes::listarTodo() const {
    for (int i = 0; i < numPaquetes; i++) {
        std::cout << "\n--- Paquete " << i << " ---" << std::endl;
        paquetes[i].listar();
    }
}

void GestorPaquetes::eliminarPaquete(int indice) {
    if (indice < 0 || indice >= numPaquetes) return;
    for (int i = indice; i < numPaquetes - 1; i++) {
        paquetes[i] = paquetes[i + 1];
    }
    numPaquetes--;
}

Paquete* GestorPaquetes::getPaquete(int indice) {
    if (indice >= 0 && indice < numPaquetes) return &paquetes[indice];
    return nullptr;
}

int GestorPaquetes::getNumPaquetes() const { return numPaquetes; }
