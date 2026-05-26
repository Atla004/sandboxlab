#include "persona.h"
#include <iostream>

Persona::Persona(const std::string& nombre, int edad) : nombre(nombre), edad(edad) {}

void Persona::mostrar() const {
    std::cout << "Nombre: " << nombre << ", Edad: " << edad << std::endl;
}

std::string Persona::getNombre() const {
    return nombre;
}

int Persona::getEdad() const {
    return edad;
}