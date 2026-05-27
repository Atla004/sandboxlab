#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include "Pokemon.h"

class Entrenador {
private:
    char* nombre;
    Pokemon** equipo;
    int numPokemon;
    int capacidad;

    void copiar(const Entrenador& otro);
    void liberar();
    void redimensionar();

public:
    Entrenador();
    Entrenador(const char* nombre);
    Entrenador(const Entrenador& otro);
    Entrenador& operator=(const Entrenador& otro);
    ~Entrenador();

    void agregarPokemon(const Pokemon& p);
    void eliminarPokemon(int indice);
    Pokemon* getPokemon(int indice) const;
    void mostrar() const;
    const char* getNombre() const;
    int getNumPokemon() const;
    Pokemon* getPokemonActivo() const;
    void setPokemonActivo(int indice);
    int getPokemonActivoIndice() const;
};

#endif
