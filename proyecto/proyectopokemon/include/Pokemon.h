#ifndef POKEMON_H
#define POKEMON_H

#include "Tipos.h"

class Pokemon {
private:
    char* nombre;
    char* especie;
    char* tipo;
    int vida;
    int vidaMax;
    int ataque;
    int defensa;
    int velocidad;
    int nivel;

    void copiar(const char*& dest, const char* src);
    void liberarCadena(char*& cadena);

public:
    Pokemon();
    Pokemon(const char* nombre, const char* especie, const char* tipo,
            int vidaMax, int ataque, int defensa, int velocidad, int nivel);
    Pokemon(const Pokemon& otro);
    Pokemon& operator=(const Pokemon& otro);
    ~Pokemon();

    void recibirDano(int dano);
    void curar();
    bool estaVivo() const;
    void mostrar() const;

    const char* getNombre() const;
    const char* getEspecie() const;
    const char* getTipoStr() const;
    int getVida() const;
    int getVidaMax() const;
    int getAtaque() const;
    int getDefensa() const;
    int getVelocidad() const;
    int getNivel() const;
    TipoPokemon getTipo() const;
};

#endif
