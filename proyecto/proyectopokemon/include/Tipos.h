#ifndef TIPOS_H
#define TIPOS_H

enum TipoPokemon { Fuego, Agua, Planta, Electrico, Normal };

static float getMultiplicadorTipo(TipoPokemon ataque, TipoPokemon defensa) {
    float tabla[5][5] = {
        // Fuego  Agua  Planta  Elec   Normal  <- defensa
        {0.5f,   0.5f,  2.0f,   1.0f,  1.0f},  // Fuego <- ataque
        {2.0f,   0.5f,  0.5f,   1.0f,  1.0f},  // Agua
        {0.5f,   2.0f,  0.5f,   1.0f,  1.0f},  // Planta
        {1.0f,   2.0f,  0.5f,   0.5f,  1.0f},  // Electrico
        {1.0f,   1.0f,  1.0f,   1.0f,  1.0f},  // Normal
    };
    return tabla[ataque][defensa];
}

static const char* tipoAString(TipoPokemon tipo) {
    switch (tipo) {
        case Fuego: return "Fuego";
        case Agua: return "Agua";
        case Planta: return "Planta";
        case Electrico: return "Electrico";
        case Normal: return "Normal";
        default: return "Desconocido";
    }
}

#endif
