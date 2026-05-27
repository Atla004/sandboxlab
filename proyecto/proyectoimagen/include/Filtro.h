#ifndef FILTRO_H
#define FILTRO_H

#include "Imagen.h"

class Filtro {
public:
    static void invertir(Imagen& img);
    static void brillo(Imagen& img, int ajuste);
    static void umbral(Imagen& img, int limite);
    static void mezclar(Imagen& img1, const Imagen& img2, float peso);
};

#endif
