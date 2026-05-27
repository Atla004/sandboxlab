#include "Pokemon.h"
#include <iostream>
#include <cstring>

void Pokemon::copiar(const char*& dest, const char* src) {
    if (src) {
        dest = new char[strlen(src) + 1];
        strcpy((char*)dest, src);
    } else {
        dest = nullptr;
    }
}

void Pokemon::liberarCadena(char*& cadena) {
    delete[] cadena;
    cadena = nullptr;
}

Pokemon::Pokemon() : nombre(nullptr), especie(nullptr), tipo(nullptr),
    vida(0), vidaMax(0), ataque(0), defensa(0), velocidad(0), nivel(0) {}

Pokemon::Pokemon(const char* nombre, const char* especie, const char* tipo,
                 int vidaMax, int ataque, int defensa, int velocidad, int nivel) {
    copiar(this->nombre, nombre);
    copiar(this->especie, especie);
    copiar(this->tipo, tipo);
    this->vidaMax = vidaMax;
    this->vida = vidaMax;
    this->ataque = ataque;
    this->defensa = defensa;
    this->velocidad = velocidad;
    this->nivel = nivel;
}

Pokemon::Pokemon(const Pokemon& otro) : nombre(nullptr), especie(nullptr), tipo(nullptr) {
    copiar(nombre, otro.nombre);
    copiar(especie, otro.especie);
    copiar(tipo, otro.tipo);
    vida = otro.vida;
    vidaMax = otro.vidaMax;
    ataque = otro.ataque;
    defensa = otro.defensa;
    velocidad = otro.velocidad;
    nivel = otro.nivel;
}

Pokemon& Pokemon::operator=(const Pokemon& otro) {
    if (this != &otro) {
        liberarCadena(nombre);
        liberarCadena(especie);
        liberarCadena(tipo);
        copiar(nombre, otro.nombre);
        copiar(especie, otro.especie);
        copiar(tipo, otro.tipo);
        vida = otro.vida;
        vidaMax = otro.vidaMax;
        ataque = otro.ataque;
        defensa = otro.defensa;
        velocidad = otro.velocidad;
        nivel = otro.nivel;
    }
    return *this;
}

Pokemon::~Pokemon() {
    liberarCadena(nombre);
    liberarCadena(especie);
    liberarCadena(tipo);
}

void Pokemon::recibirDano(int dano) {
    vida -= dano;
    if (vida < 0) vida = 0;
}

void Pokemon::curar() {
    vida = vidaMax;
}

bool Pokemon::estaVivo() const {
    return vida > 0;
}

void Pokemon::mostrar() const {
    std::cout << nombre << " (" << especie << ") - " << tipo
              << " - HP: " << vida << "/" << vidaMax
              << " - Nv." << nivel << std::endl;
}

const char* Pokemon::getNombre() const { return nombre; }
const char* Pokemon::getEspecie() const { return especie; }
const char* Pokemon::getTipoStr() const { return tipo; }
int Pokemon::getVida() const { return vida; }
int Pokemon::getVidaMax() const { return vidaMax; }
int Pokemon::getAtaque() const { return ataque; }
int Pokemon::getDefensa() const { return defensa; }
int Pokemon::getVelocidad() const { return velocidad; }
int Pokemon::getNivel() const { return nivel; }

TipoPokemon Pokemon::getTipo() const {
    if (strcmp(tipo, "Fuego") == 0) return Fuego;
    if (strcmp(tipo, "Agua") == 0) return Agua;
    if (strcmp(tipo, "Planta") == 0) return Planta;
    if (strcmp(tipo, "Electrico") == 0) return Electrico;
    return Normal;
}
