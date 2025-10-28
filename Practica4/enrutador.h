#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <vector>
#include <utility>

/**
 * @brief Clase que representa un enrutador dentro de la red.
 */
class Router {
public:
    int id;                                       ///< Identificador del enrutador.
    int distancia;                                ///< Distancia mínima calculada (para Dijkstra).
    bool visitado;                                ///< Marca si ya fue visitado.
    Router* previo;                               ///< Enrutador anterior en la ruta más corta.
    std::vector<std::pair<Router*, int>> vecinos; ///< Lista de vecinos y costos de conexión.

    /**
     * @brief Constructor del enrutador.
     * @param id Identificador único del enrutador.
     */
    Router(int id);

    /**
     * @brief Agrega un nuevo vecino con su costo asociado.
     * @param vecino Puntero al enrutador vecino.
     * @param costo Costo del enlace.
     */
    void nuevoVecino(Router* vecino, int costo);

    /**
     * @brief Reinicia las variables usadas en los cálculos de rutas.
     */
    void reiniciar();
};

#endif // ENRUTADOR_H
