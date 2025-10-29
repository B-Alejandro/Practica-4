#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <vector>
#include <utility>  // pair
#include <iostream>
#include <string>

class Router {
public:
    int id;                             // Identificador del enrutador
    int distancia;                      // Distancia usada en Dijkstra
    bool visitado;                      // Bandera de visita
    Router* previo;                     // Predecesor en el camino más corto
    std::vector<std::pair<Router*, int>> vecinos; // Lista de vecinos (Router*, costo)

    // ==========================
    // Constructores y básicos
    // ==========================
    Router(int id);
    void nuevoVecino(Router* vecino, int costo);
    void eliminarVecino(Router* vecino);
    void reiniciar();

    // ==========================
    // Utilitarios
    // ==========================
    bool existeVecino(Router* vecino) const;
    void mostrarVecinos() const;
};

#endif
