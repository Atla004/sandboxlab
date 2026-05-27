#include "Compresor.h"

char* Compresor::comprimirRLE(const char* datos, int tamOriginal, int& tamComprimido) {
    char* salida = new char[tamOriginal * 2];
    int idx = 0;
    int i = 0;
    while (i < tamOriginal) {
        char byte = datos[i];
        int count = 1;
        while (i + count < tamOriginal && datos[i + count] == byte && count < 255) {
            count++;
        }
        if (count >= 3) {
            salida[idx++] = byte;
            salida[idx++] = (char)0xFF;
            salida[idx++] = (char)count;
        } else {
            for (int j = 0; j < count; j++) {
                salida[idx++] = datos[i + j];
            }
        }
        i += count;
    }
    tamComprimido = idx;
    return salida;
}

char* Compresor::descomprimirRLE(const char* datos, int tamComprimido, int& tamOriginal) {
    char* salida = new char[tamComprimido * 2];
    int idx = 0;
    int i = 0;
    while (i < tamComprimido) {
        char byte = datos[i];
        if (i + 2 < tamComprimido && (unsigned char)datos[i + 1] == 0xFF) {
            int count = (unsigned char)datos[i + 2];
            for (int j = 0; j < count; j++) {
                salida[idx++] = byte;
            }
            i += 3;
        } else {
            salida[idx++] = byte;
            i++;
        }
    }
    tamOriginal = idx;
    return salida;
}

float Compresor::ratioCompresion(int tamOriginal, int tamComprimido) {
    if (tamOriginal == 0) return 0;
    return (float)tamComprimido / tamOriginal * 100;
}
