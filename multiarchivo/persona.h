#ifndef PERSONA_H
#define PERSONA_H

#include <string>

class Persona {
private:
    std::string nombre;
    int edad;

public:
    Persona(const std::string& nombre, int edad);
    void mostrar() const;
    std::string getNombre() const;
    int getEdad() const;
};

#endif