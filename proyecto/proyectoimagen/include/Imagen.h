#ifndef IMAGEN_H
#define IMAGEN_H

#include "Pixel.h"

class Imagen {
private:
    Pixel** pixeles;
    int ancho;
    int alto;

    void liberar();
    void copiar(const Imagen& otra);

public:
    Imagen();
    Imagen(int ancho, int alto, Pixel color = {0, 0, 0});
    Imagen(const Imagen& otra);
    Imagen& operator=(const Imagen& otra);
    ~Imagen();

    Pixel getPixel(int fila, int col) const;
    void setPixel(int fila, int col, Pixel color);
    void mostrarASCII() const;
    void rellenar(Pixel color);
    int getAncho() const;
    int getAlto() const;
};

#endif
