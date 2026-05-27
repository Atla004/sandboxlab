#include "Entrenador.h"
#include <iostream>
#include <cstring>

Entrenador::Entrenador() : nombre(nullptr), equipo(nullptr), numPokemon(0), capacidad(0) {}

Entrenador::Entrenador(const char* nombre) : equipo(nullptr), numPokemon(0), capacidad(0) {
    if (nombre) {
        this->nombre = new char[strlen(nombre) + 1];
        strcpy(this->nombre, nombre);
    } else {
        this->nombre = nullptr;
    }
}

void Entrenador::copiar(const Entrenador& otro) {
    if (otro.nombre) {
        nombre = new char[strlen(otro.nombre) + 1];
        strcpy(nombre, otro.nombre);
    } else {
        nombre = nullptr;
    }
    capacidad = otro.capacidad;
    numPokemon = otro.numPokemon;
    if (capacidad > 0) {
        equipo = new Pokemon*[capacidad];
        for (int i = 0; i < numPokemon; i++) {
            equipo[i] = new Pokemon(*(otro.equipo[i]));
        }
    } else {
        equipo = nullptr;
    }
}

void Entrenador::liberar() {
    for (int i = 0; i < numPokemon; i++) {
        delete equipo[i];
    }
    delete[] equipo;
    delete[] nombre;
    equipo = nullptr;
    nombre = nullptr;
    numPokemon = 0;
    capacidad = 0;
}

void Entrenador::redimensionar() {
    int nuevaCap = (capacidad == 0) ? 4 : capacidad * 2;
    Pokemon** nuevo = new Pokemon*[nuevaCap];
    for (int i = 0; i < numPokemon; i++) {
        nuevo[i] = equipo[i];
    }
    delete[] equipo;
    equipo = nuevo;
    capacidad = nuevaCap;
}

Entrenador::Entrenador(const Entrenador& otro) : equipo(nullptr), nombre(nullptr) {
    copiar(otro);
}

Entrenador& Entrenador::operator=(const Entrenador& otro) {
    if (this != &otro) {
        liberar();
        copiar(otro);
    }
    return *this;
}

Entrenador::~Entrenador() {
    liberar();
}

void Entrenador::agregarPokemon(const Pokemon& p) {
    if (numPokemon >= capacidad) redimensionar();
    equipo[numPokemon] = new Pokemon(p);
    numPokemon++;
}

void Entrenador::eliminarPokemon(int indice) {
    if (indice < 0 || indice >= numPokemon) return;
    delete equipo[indice];
    for (int i = indice; i < numPokemon - 1; i++) {
        equipo[i] = equipo[i + 1];
    }
    numPokemon--;
}

Pokemon* Entrenador::getPokemon(int indice) const {
    if (indice >= 0 && indice < numPokemon) return equipo[indice];
    return nullptr;
}

void Entrenador::mostrar() const {
    std::cout << "--- EQUIPO DE " << nombre << " ---" << std::endl;
    for (int i = 0; i < numPokemon; i++) {
        std::cout << "Pokemon " << (i + 1) << ": ";
        equipo[i]->mostrar();
    }
}

const char* Entrenador::getNombre() const { return nombre; }
int Entrenador::getNumPokemon() const { return numPokemon; }
