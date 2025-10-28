#include "enrutador.h"
#include <climits>

// =======================================================
//              Implementación de la clase Router
// =======================================================

Router::Router(int id)
    : id(id), distancia(INT_MAX), visitado(false), previo(nullptr) {}

void Router::nuevoVecino(Router* vecino, int costo) {
    vecinos.emplace_back(vecino, costo);
}

void Router::reiniciar() {
    distancia = INT_MAX;
    visitado = false;
    previo = nullptr;
}
