#include <iostream>
#include <limits>
#include <cstring>
#include "Paquete.h"
#include "Compresor.h"
#include "Integridad.h"
#include "GestorPaquetes.h"

static void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int leerInt(int min, int max) {
    int val;
    while (true) {
        std::cin >> val;
        limpiarBuffer();
        if (val >= min && val <= max) return val;
        std::cout << "Opcion invalida (" << min << "-" << max << "): ";
    }
}

static void esperarEnter() {
    std::cout << "\nPresiona Enter para volver...";
    limpiarBuffer();
    std::cin.get();
}

int main() {
    GestorPaquetes gestor(10);

    // Paquete A: documentos
    gestor.crearPaquete("documentos");
    Paquete* paqA = gestor.getPaquete(0);

    const char* docTexto = "Hola mundo, este es un archivo de prueba.";
    paqA->agregar("documento.txt", docTexto, strlen(docTexto) + 1);

    char datosBin[100];
    for (int i = 0; i < 100; i++) datosBin[i] = (char)i;
    paqA->agregar("datos.bin", datosBin, 100);
    paqA->actualizarChecksum(1);

    char patronImg[50];
    for (int i = 0; i < 50; i++) patronImg[i] = (i % 2 == 0) ? (char)0xAA : (char)0x55;
    paqA->agregar("imagen.img", patronImg, 50);
    paqA->actualizarChecksum(2);

    // Paquete B: respaldo
    gestor.crearPaquete("respaldo");
    Paquete* paqB = gestor.getPaquete(1);

    const char* notasTexto = "Esto es una nota de respaldo.";
    paqB->agregar("notas.txt", notasTexto, strlen(notasTexto) + 1);

    char datosBin2[100];
    for (int i = 0; i < 100; i++) datosBin2[i] = (char)i;
    paqB->agregar("datos.bin", datosBin2, 100);
    paqB->actualizarChecksum(1);

    int opcion;
    do {
        std::cout << "\n=== GESTOR DE PAQUETES (MEMORIA) ===" << std::endl;
        std::cout << "1. Ver todos los paquetes" << std::endl;
        std::cout << "2. Ver contenido de un paquete" << std::endl;
        std::cout << "3. Extraer y ver archivo" << std::endl;
        std::cout << "4. Eliminar archivo del paquete" << std::endl;
        std::cout << "5. Buscar archivo por nombre" << std::endl;
        std::cout << "6. Comprimir archivo (RLE)" << std::endl;
        std::cout << "7. Cifrar archivo (XOR)" << std::endl;
        std::cout << "8. Verificar integridad de archivo" << std::endl;
        std::cout << "9. Fusionar dos paquetes" << std::endl;
        std::cout << "10. Comparar archivos entre paquetes" << std::endl;
        std::cout << "11. Salir" << std::endl;
        std::cout << "> ";
        std::cin >> opcion;
        limpiarBuffer();

        if (opcion == 1) {
            gestor.listarTodo();
            esperarEnter();
        } else if (opcion == 2) {
            std::cout << "Indice del paquete (0-" << (gestor.getNumPaquetes() - 1) << "): ";
            int idx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(idx);
            if (p) p->listar();
            esperarEnter();
        } else if (opcion == 3) {
            std::cout << "Indice del paquete: ";
            int pIdx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(pIdx);
            if (!p) continue;
            std::cout << "Indice del archivo (0-" << (p->getNumEntradas() - 1) << "): ";
            int eIdx = leerInt(0, p->getNumEntradas() - 1);
            Entrada* e = p->extraer(eIdx);
            if (e) {
                std::cout << "\n--- " << e->nombre << " (" << e->tamano << " bytes) ---" << std::endl;
                if (e->tamano < 200) {
                    for (int i = 0; i < e->tamano; i++) {
                        std::cout << e->datos[i];
                    }
                    std::cout << std::endl;
                } else {
                    std::cout << "(archivo binario de " << e->tamano << " bytes)" << std::endl;
                }
            }
            esperarEnter();
        } else if (opcion == 4) {
            std::cout << "Indice del paquete: ";
            int pIdx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(pIdx);
            if (!p) continue;
            p->listar();
            std::cout << "Indice del archivo a eliminar: ";
            int eIdx = leerInt(0, p->getNumEntradas() - 1);
            p->eliminar(eIdx);
            std::cout << "Archivo eliminado." << std::endl;
            esperarEnter();
        } else if (opcion == 5) {
            std::cout << "Buscar: ";
            char patron[100];
            std::cin.getline(patron, 100);
            int numRes = 0;
            ResultadoBusqueda* res = gestor.buscarEnTodos(patron, numRes);
            std::cout << "\nResultados:" << std::endl;
            for (int i = 0; i < numRes; i++) {
                Paquete* p = gestor.getPaquete(res[i].paqueteIdx);
                Entrada* e = p->extraer(res[i].entradaIdx);
                std::cout << "  Paquete [" << res[i].paqueteIdx << "] " << e->nombre << " (" << e->tamano << " bytes)" << std::endl;
            }
            std::cout << "\nSe encontraron " << numRes << " coincidencias." << std::endl;
            delete[] res;
            esperarEnter();
        } else if (opcion == 6) {
            std::cout << "Indice del paquete: ";
            int pIdx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(pIdx);
            if (!p) continue;
            std::cout << "Indice del archivo: ";
            int eIdx = leerInt(0, p->getNumEntradas() - 1);
            Entrada* e = p->extraer(eIdx);
            if (e) {
                int tamComp = 0;
                char* comprimido = Compresor::comprimirRLE(e->datos, e->tamano, tamComp);
                float ratio = Compresor::ratioCompresion(e->tamano, tamComp);
                std::cout << "\nComprimiendo " << e->nombre << " con RLE..." << std::endl;
                std::cout << "Original: " << e->tamano << " bytes" << std::endl;
                std::cout << "Comprimido: " << tamComp << " bytes" << std::endl;
                std::cout << "Ratio: " << ratio << "%" << std::endl;
                p->agregar((std::string(e->nombre) + ".rle").c_str(), comprimido, tamComp);
                delete[] comprimido;
            }
            esperarEnter();
        } else if (opcion == 7) {
            std::cout << "Indice del paquete: ";
            int pIdx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(pIdx);
            if (!p) continue;
            std::cout << "Indice del archivo: ";
            int eIdx = leerInt(0, p->getNumEntradas() - 1);
            Entrada* e = p->extraer(eIdx);
            if (e) {
                int tamSalida = 0;
                char clave = 'K';
                char* cifrado = Integridad::cifrarXOR(e->datos, e->tamano, clave, tamSalida);
                char* descifrado = Integridad::descifrarXOR(cifrado, tamSalida, clave, tamSalida);
                std::cout << "\nArchivo cifrado y descifrado correctamente." << std::endl;
                std::cout << "Original: " << e->datos[0] << " ... | Descifrado: " << descifrado[0] << " ..." << std::endl;
                delete[] cifrado;
                delete[] descifrado;
            }
            esperarEnter();
        } else if (opcion == 8) {
            std::cout << "Indice del paquete: ";
            int pIdx = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* p = gestor.getPaquete(pIdx);
            if (!p) continue;
            std::cout << "Indice del archivo: ";
            int eIdx = leerInt(0, p->getNumEntradas() - 1);
            Entrada* e = p->extraer(eIdx);
            if (e) {
                unsigned int calculado = Integridad::calcularChecksum(e->datos, e->tamano);
                std::cout << "\n--- " << e->nombre << " ---" << std::endl;
                printf("Checksum almacenado:  0x%08X\n", e->checksum);
                printf("Checksum calculado:   0x%08X\n", calculado);
                if (e->checksum == calculado)
                    std::cout << "Estado: INTEGRO" << std::endl;
                else
                    std::cout << "Estado: CORROMPIDO" << std::endl;
            }
            esperarEnter();
        } else if (opcion == 9) {
            std::cout << "Paquete 1 (0-" << (gestor.getNumPaquetes() - 1) << "): ";
            int p1 = leerInt(0, gestor.getNumPaquetes() - 1);
            std::cout << "Paquete 2 (0-" << (gestor.getNumPaquetes() - 1) << "): ";
            int p2 = leerInt(0, gestor.getNumPaquetes() - 1);
            Paquete* fusion = gestor.fusionar(p1, p2);
            if (fusion) {
                std::cout << "\nFusionando paquetes..." << std::endl;
                std::cout << "Nuevo paquete con " << fusion->getNumEntradas() << " archivos (" << fusion->tamanoTotal() << " bytes)" << std::endl;
                delete fusion;
            }
            esperarEnter();
        } else if (opcion == 10) {
            std::cout << "\nBuscando archivos duplicados entre paquetes..." << std::endl;
            for (int i = 0; i < gestor.getNumPaquetes(); i++) {
                for (int j = i + 1; j < gestor.getNumPaquetes(); j++) {
                    Paquete* p1 = gestor.getPaquete(i);
                    Paquete* p2 = gestor.getPaquete(j);
                    for (int a = 0; a < p1->getNumEntradas(); a++) {
                        for (int b = 0; b < p2->getNumEntradas(); b++) {
                            Entrada* e1 = p1->extraer(a);
                            Entrada* e2 = p2->extraer(b);
                            if (Integridad::comparar(*e1, *e2)) {
                                printf("Encontrado: %s en paquete [%d] == %s en paquete [%d]\n",
                                       e1->nombre, i, e2->nombre, j);
                                printf("Checksum: 0x%08X | Tamano: %d bytes\n", e1->checksum, e1->tamano);
                            }
                        }
                    }
                }
            }
            esperarEnter();
        }
    } while (opcion != 11);

    std::cout << "Saliendo..." << std::endl;
    return 0;
}
