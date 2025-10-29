#include "enrutador.h"
#include <climits>
#include <algorithm> // remove_if
#include <iomanip>   // setw

// =======================================================
//              Implementación de la clase Router
// =======================================================

Router::Router(int id)
    : id(id), distancia(INT_MAX), visitado(false), previo(nullptr) {}

// -------------------------------------------------------
// Agregar un vecino (si no existe ya)
// -------------------------------------------------------
void Router::nuevoVecino(Router* vecino, int costo) {
    if (!existeVecino(vecino))
        vecinos.emplace_back(vecino, costo);
}

// -------------------------------------------------------
// Eliminar un vecino específico
// -------------------------------------------------------
void Router::eliminarVecino(Router* vecino) {
    vecinos.erase(
        std::remove_if(vecinos.begin(), vecinos.end(),
                       [vecino](auto& p) { return p.first == vecino; }),
        vecinos.end());
}

// -------------------------------------------------------
// Verificar si un vecino ya está en la lista
// -------------------------------------------------------
bool Router::existeVecino(Router* vecino) const {
    for (auto& v : vecinos)
        if (v.first == vecino)
            return true;
    return false;
}

// -------------------------------------------------------
// Reiniciar campos de Dijkstra
// -------------------------------------------------------
void Router::reiniciar() {
    distancia = INT_MAX;
    visitado = false;
    previo = nullptr;
}

// -------------------------------------------------------
// Mostrar los vecinos del enrutador (para depuración)
// -------------------------------------------------------
void Router::mostrarVecinos() const {
    std::cout << "R" << id << " -> ";
    if (vecinos.empty()) {
        std::cout << "(sin vecinos)\n";
        return;
    }
    for (auto& [v, c] : vecinos)
        std::cout << "R" << v->id << "(" << c << ") ";
    std::cout << "\n";
}
