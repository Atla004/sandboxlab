#include "util.h"
#include <iostream>

namespace utils {
    std::string saludar(const std::string& nombre) {
        return "Hola, " + nombre + "!";
    }

    int sumar(int a, int b) {
        return a + b;
    }
}