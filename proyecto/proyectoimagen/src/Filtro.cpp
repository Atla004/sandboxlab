#include "Filtro.h"
#include <cmath>

static unsigned char saturar(int valor) {
    if (valor < 0) return 0;
    if (valor > 255) return 255;
    return (unsigned char)valor;
}

void Filtro::invertir(Imagen& img) {
    for (int i = 0; i < img.getAlto(); i++) {
        for (int j = 0; j < img.getAncho(); j++) {
            Pixel p = img.getPixel(i, j);
            p.r = 255 - p.r;
            p.g = 255 - p.g;
            p.b = 255 - p.b;
            img.setPixel(i, j, p);
        }
    }
}

void Filtro::brillo(Imagen& img, int ajuste) {
    for (int i = 0; i < img.getAlto(); i++) {
        for (int j = 0; j < img.getAncho(); j++) {
            Pixel p = img.getPixel(i, j);
            p.r = saturar((int)p.r + ajuste);
            p.g = saturar((int)p.g + ajuste);
            p.b = saturar((int)p.b + ajuste);
            img.setPixel(i, j, p);
        }
    }
}

void Filtro::umbral(Imagen& img, int limite) {
    for (int i = 0; i < img.getAlto(); i++) {
        for (int j = 0; j < img.getAncho(); j++) {
            Pixel p = img.getPixel(i, j);
            int brillo = (p.r + p.g + p.b) / 3;
            if (brillo < limite)
                img.setPixel(i, j, {0, 0, 0});
            else
                img.setPixel(i, j, {255, 255, 255});
        }
    }
}

void Filtro::mezclar(Imagen& img1, const Imagen& img2, float peso) {
    int h = (img1.getAlto() < img2.getAlto()) ? img1.getAlto() : img2.getAlto();
    int w = (img1.getAncho() < img2.getAncho()) ? img1.getAncho() : img2.getAncho();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel p1 = img1.getPixel(i, j);
            Pixel p2 = img2.getPixel(i, j);
            p1.r = saturar((int)(p1.r * (1 - peso) + p2.r * peso));
            p1.g = saturar((int)(p1.g * (1 - peso) + p2.g * peso));
            p1.b = saturar((int)(p1.b * (1 - peso) + p2.b * peso));
            img1.setPixel(i, j, p1);
        }
    }
}
