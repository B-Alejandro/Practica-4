#ifndef RED_H
#define RED_H

#include "enrutador.h"
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <queue>
#include <climits>
#include <iomanip>

class Red {
private:
    std::vector<Router*> enrutadores; // Lista de enrutadores de la red
    std::string rutaArchivo;          // Ruta del archivo de guardado (opcional)

public:
    // ===========================
    // Constructores y destructor
    // ===========================
    Red();                       // Constructor vacío
    Red(int cantidad);            // Constructor con número de routers
    ~Red();                       // Destructor (libera memoria)

    // ===========================
    // Funciones principales
    // ===========================
    void generarRedAleatoria();   // Genera red con enlaces aleatorios
    void mostrarRed() const;      // Muestra todos los enrutadores y sus enlaces

    void guardarEnArchivo(const std::string& nombreArchivo) const; // Guarda la red en archivo
    void cargarDesdeArchivo(const std::string& nombreArchivo);     // Carga red desde archivo

    void calcularRutaMasCorta(int origen, int destino);            // Aplica Dijkstra entre dos routers
    void mostrarTablasDeEnrutamiento();                            // 🆕 Muestra tabla de enrutamiento de cada router

    // ===========================
    // Gestión de enrutadores
    // ===========================
    void agregarEnrutador();      // Agrega un nuevo router
    void eliminarEnrutador(int id); // Elimina un router y sus enlaces

    // ===========================
    // Gestión de enlaces
    // ===========================
    void agregarEnlace();         // Agrega enlace entre dos routers
    void eliminarEnlace();        // Elimina enlace entre dos routers
};

#endif // RED_H
