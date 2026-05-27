#include "Paquete.h"
#include <iostream>
#include <cstring>

Paquete::Paquete() : entradas(nullptr), numEntradas(0), capacidad(0) {}

Paquete::Paquete(const char* nombre) : entradas(nullptr), numEntradas(0), capacidad(0) {}

void Paquete::liberarEntrada(Entrada& e) {
    delete[] e.nombre;
    delete[] e.datos;
    e.nombre = nullptr;
    e.datos = nullptr;
}

void Paquete::copiarEntrada(Entrada& dest, const Entrada& src) {
    if (src.nombre) {
        dest.nombre = new char[strlen(src.nombre) + 1];
        strcpy(dest.nombre, src.nombre);
    } else {
        dest.nombre = nullptr;
    }
    if (src.datos && src.tamano > 0) {
        dest.datos = new char[src.tamano];
        memcpy(dest.datos, src.datos, src.tamano);
    } else {
        dest.datos = nullptr;
    }
    dest.tamano = src.tamano;
    dest.checksum = src.checksum;
}

Paquete::Paquete(const Paquete& otro) : entradas(nullptr), numEntradas(0), capacidad(0) {
    if (otro.capacidad > 0) {
        capacidad = otro.capacidad;
        numEntradas = otro.numEntradas;
        entradas = new Entrada[capacidad];
        for (int i = 0; i < numEntradas; i++) {
            copiarEntrada(entradas[i], otro.entradas[i]);
        }
    }
}

Paquete& Paquete::operator=(const Paquete& otro) {
    if (this != &otro) {
        for (int i = 0; i < numEntradas; i++) liberarEntrada(entradas[i]);
        delete[] entradas;
        capacidad = otro.capacidad;
        numEntradas = otro.numEntradas;
        if (capacidad > 0) {
            entradas = new Entrada[capacidad];
            for (int i = 0; i < numEntradas; i++) {
                copiarEntrada(entradas[i], otro.entradas[i]);
            }
        } else {
            entradas = nullptr;
        }
    }
    return *this;
}

Paquete::~Paquete() {
    for (int i = 0; i < numEntradas; i++) liberarEntrada(entradas[i]);
    delete[] entradas;
}

void Paquete::agregar(const char* nombre, const char* datos, int tamano) {
    if (numEntradas >= capacidad) {
        int nuevaCap = (capacidad == 0) ? 4 : capacidad * 2;
        Entrada* nuevo = new Entrada[nuevaCap];
        for (int i = 0; i < numEntradas; i++) {
            copiarEntrada(nuevo[i], entradas[i]);
            liberarEntrada(entradas[i]);
        }
        delete[] entradas;
        entradas = nuevo;
        capacidad = nuevaCap;
    }
    entradas[numEntradas].nombre = new char[strlen(nombre) + 1];
    strcpy(entradas[numEntradas].nombre, nombre);
    entradas[numEntradas].datos = new char[tamano];
    memcpy(entradas[numEntradas].datos, datos, tamano);
    entradas[numEntradas].tamano = tamano;
    entradas[numEntradas].checksum = 0;
    numEntradas++;
}

Entrada* Paquete::extraer(int indice) {
    if (indice >= 0 && indice < numEntradas) return &entradas[indice];
    return nullptr;
}

void Paquete::eliminar(int indice) {
    if (indice < 0 || indice >= numEntradas) return;
    liberarEntrada(entradas[indice]);
    for (int i = indice; i < numEntradas - 1; i++) {
        entradas[i] = entradas[i + 1];
    }
    numEntradas--;
}

void Paquete::listar() const {
    std::cout << "Indice | Nombre              | Tamano (bytes) | Checksum" << std::endl;
    std::cout << "-------|---------------------|----------------|----------" << std::endl;
    for (int i = 0; i < numEntradas; i++) {
        printf("%-7d| %-20s| %-15d| 0x%08X\n", i, entradas[i].nombre, entradas[i].tamano, entradas[i].checksum);
    }
    std::cout << "\nTotal: " << numEntradas << " archivos (" << tamanoTotal() << " bytes)" << std::endl;
}

int* Paquete::buscar(const char* patron, int& numResultados) const {
    int* resultados = new int[numEntradas];
    numResultados = 0;
    for (int i = 0; i < numEntradas; i++) {
        if (strstr(entradas[i].nombre, patron) != nullptr) {
            resultados[numResultados++] = i;
        }
    }
    return resultados;
}

int Paquete::tamanoTotal() const {
    int total = 0;
    for (int i = 0; i < numEntradas; i++) total += entradas[i].tamano;
    return total;
}

int Paquete::getNumEntradas() const { return numEntradas; }

void Paquete::actualizarChecksum(int indice) {
    if (indice >= 0 && indice < numEntradas) {
        unsigned int cs = 0;
        for (int i = 0; i < entradas[indice].tamano; i++) {
            cs = (cs << 5) + (unsigned char)entradas[indice].datos[i];
        }
        entradas[indice].checksum = cs;
    }
}

const char* Paquete::getNombre() const { return ""; }
void Paquete::setNombre(const char* nombre) {}
