#ifndef RED_H
#define RED_H

#include "enrutador.h"
#include <vector>
#include <string>
#include <utility>
#include <iostream>

class Red {
private:
    std::vector<Router*> enrutadores;
    std::string rutaArchivo;

public:
    // ===========================
    // Constructores y destructor
    // ===========================
    Red();                      // Constructor vacío
    Red(int cantidad);           // Constructor con número de routers
    ~Red();                      // Destructor

    // ===========================
    // Funciones principales
    // ===========================
    void generarRedAleatoria();
    void mostrarRed() const;

    void guardarEnArchivo(const std::string& nombreArchivo) const;
    void cargarDesdeArchivo(const std::string& nombreArchivo);

    void calcularRutaMasCorta(int origen, int destino);

    // ===========================
    // Gestión de enrutadores
    // ===========================
    void agregarEnrutador();
    void eliminarEnrutador(int id);

    // ===========================
    // Gestión de enlaces
    // ===========================
    void agregarEnlace();
    void eliminarEnlace();
};

#endif // RED_H
