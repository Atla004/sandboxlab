#include "Imagen.h"
#include <iostream>

Imagen::Imagen() : ancho(1), alto(1) {
    pixeles = new Pixel*[1];
    pixeles[0] = new Pixel[1];
    pixeles[0][0] = {0, 0, 0};
}

Imagen::Imagen(int ancho, int alto, Pixel color) : ancho(ancho), alto(alto) {
    pixeles = new Pixel*[alto];
    for (int i = 0; i < alto; i++) {
        pixeles[i] = new Pixel[ancho];
        for (int j = 0; j < ancho; j++) {
            pixeles[i][j] = color;
        }
    }
}

void Imagen::liberar() {
    for (int i = 0; i < alto; i++) {
        delete[] pixeles[i];
    }
    delete[] pixeles;
}

void Imagen::copiar(const Imagen& otra) {
    ancho = otra.ancho;
    alto = otra.alto;
    pixeles = new Pixel*[alto];
    for (int i = 0; i < alto; i++) {
        pixeles[i] = new Pixel[ancho];
        for (int j = 0; j < ancho; j++) {
            pixeles[i][j] = otra.pixeles[i][j];
        }
    }
}

Imagen::Imagen(const Imagen& otra) {
    copiar(otra);
}

Imagen& Imagen::operator=(const Imagen& otra) {
    if (this != &otra) {
        liberar();
        copiar(otra);
    }
    return *this;
}

Imagen::~Imagen() {
    liberar();
}

Pixel Imagen::getPixel(int fila, int col) const {
    if (fila >= 0 && fila < alto && col >= 0 && col < ancho)
        return pixeles[fila][col];
    return {0, 0, 0};
}

void Imagen::setPixel(int fila, int col, Pixel color) {
    if (fila >= 0 && fila < alto && col >= 0 && col < ancho)
        pixeles[fila][col] = color;
}

void Imagen::mostrarASCII() const {
    const char chars[] = "@%#*+=-:. ";
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            int brillo = (pixeles[i][j].r + pixeles[i][j].g + pixeles[i][j].b) / 3;
            int idx = brillo * 9 / 255;
            if (idx > 9) idx = 9;
            std::cout << chars[idx];
        }
        std::cout << std::endl;
    }
}

void Imagen::rellenar(Pixel color) {
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            pixeles[i][j] = color;
}

int Imagen::getAncho() const { return ancho; }
int Imagen::getAlto() const { return alto; }
