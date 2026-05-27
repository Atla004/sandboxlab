#ifndef COMPRESOR_H
#define COMPRESOR_H

class Compresor {
public:
    static char* comprimirRLE(const char* datos, int tamOriginal, int& tamComprimido);
    static char* descomprimirRLE(const char* datos, int tamComprimido, int& tamOriginal);
    static float ratioCompresion(int tamOriginal, int tamComprimido);
};

#endif
