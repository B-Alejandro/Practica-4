#include "red.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <random>
#include <algorithm>
#include <filesystem>
#include <climits>

using namespace std;
namespace fs = std::filesystem;  ///< Alias para simplificar el uso del espacio de nombres filesystem.

// =======================================================
//              Implementación de la clase Router
// =======================================================

/**
 * @brief Constructor de la clase Router.
 *
 * Inicializa los valores internos del enrutador:
 * - `id`: Identificador único.
 * - `distancia`: Valor máximo (infinito) hasta ser calculado.
 * - `visitado`: Bandera para el algoritmo de Dijkstra.
 * - `previo`: Puntero al enrutador previo en la ruta más corta.
 *
 * @param id Identificador del enrutador dentro de la red.
 */
Router::Router(int id)
    : id(id), distancia(INT_MAX), visitado(false), previo(nullptr) {}

/**
 * @brief Agrega un nuevo vecino al enrutador actual.
 *
 * Crea una relación bidireccional de conectividad entre enrutadores,
 * almacenando el puntero al vecino y el costo de su enlace.
 *
 * @param vecino Puntero al enrutador vecino.
 * @param costo Costo o peso del enlace entre ambos enrutadores.
 */
void Router::nuevoVecino(Router* vecino, int costo) {
    vecinos.emplace_back(vecino, costo); ///< Inserta el par (vecino, costo) en el vector de vecinos.
}

/**
 * @brief Reinicia los atributos del enrutador.
 *
 * Deja los valores listos para una nueva ejecución del algoritmo de Dijkstra,
 * estableciendo:
 * - Distancia infinita.
 * - Bandera `visitado` en `false`.
 * - Puntero `previo` en `nullptr`.
 */
void Router::reiniciar() {
    distancia = INT_MAX;
    visitado = false;
    previo = nullptr;
}

// =======================================================
//              Implementación de la clase Red
// =======================================================

/**
 * @brief Constructor de la clase Red.
 *
 * Crea un conjunto de enrutadores numerados del 0 al `cantidad-1` y
 * define la ruta base donde se guardará la topología generada.
 *
 * @param cantidad Número total de enrutadores que tendrá la red.
 */
Red::Red(int cantidad)
    : rutaArchivo("../../Datos/topologia.txt") ///< Ruta predeterminada del archivo de salida.
{
    // Crea dinámicamente cada enrutador y lo agrega al vector principal.
    for (int i = 0; i < cantidad; ++i)
        enrutadores.push_back(new Router(i));
}

/**
 * @brief Destructor de la clase Red.
 *
 * Libera la memoria de los enrutadores creados dinámicamente.
 */
Red::~Red() {
    for (auto r : enrutadores)
        delete r;
}

/**
 * @brief Genera una topología de red aleatoria.
 *
 * Cada par de enrutadores tiene una probabilidad del 40% de conectarse.
 * Si la conexión ocurre, se asigna un costo aleatorio entre 1 y 20.
 *
 * @note Este método simula un entorno de red con conexiones parciales
 *       y se utiliza para probar el algoritmo de Dijkstra en topologías no triviales.
 */
void Red::generarRedAleatoria() {
    random_device rd;               ///< Fuente de entropía del sistema.
    mt19937 gen(rd());              ///< Generador Mersenne Twister.
    uniform_int_distribution<> distCosto(1, 20); ///< Distribución de costos (1–20).
    uniform_real_distribution<> distProb(0.0, 1.0); ///< Distribución de probabilidad (0–1).

    int n = enrutadores.size();

    // Itera sobre todos los pares posibles de enrutadores.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Si la probabilidad es menor al 40%, se crea un enlace.
            if (distProb(gen) < 0.4) {
                int costo = distCosto(gen);
                enrutadores[i]->nuevoVecino(enrutadores[j], costo); ///< Conexión de i → j.
                enrutadores[j]->nuevoVecino(enrutadores[i], costo); ///< Conexión de j → i.
            }
        }
    }

    cout << "Red aleatoria generada con " << n << " enrutadores.\n";
}

/**
 * @brief Muestra por consola la matriz de adyacencia con los costos de enlace.
 *
 * Cada celda indica el costo de la conexión entre dos enrutadores.
 * Si no existe conexión directa, se muestra un guion (`-`).
 */
void Red::mostrarRed() const {
    int n = enrutadores.size();
    vector<vector<int>> matriz(n, vector<int>(n, -1)); ///< Inicializa matriz con -1 (sin conexión).

    // Construcción de la matriz de adyacencia.
    for (int i = 0; i < n; ++i) {
        matriz[i][i] = 0; ///< Costo cero entre un enrutador y sí mismo.
        for (auto& v : enrutadores[i]->vecinos)
            matriz[i][v.first->id] = v.second; ///< Inserta el costo en la posición correspondiente.
    }

    // Imprime encabezado con los nombres de los enrutadores.
    cout << "\n========= MATRIZ DE COSTOS =========\n";
    cout << setw(5) << " ";
    for (int j = 0; j < n; ++j)
        cout << setw(4) << "R" + to_string(j);
    cout << endl;

    // Imprime filas con los costos de conexión.
    for (int i = 0; i < n; ++i) {
        cout << setw(4) << "R" + to_string(i);
        for (int j = 0; j < n; ++j) {
            if (matriz[i][j] == -1)
                cout << setw(4) << "-";
            else
                cout << setw(4) << matriz[i][j];
        }
        cout << endl;
    }
}

/**
 * @brief Guarda la topología de la red en un archivo de texto.
 *
 * Crea el directorio `../../Datos/` si no existe y guarda la matriz de adyacencia
 * en el archivo `topologia.txt`.
 *
 * Cada fila representa los costos de un enrutador hacia los demás.
 */
void Red::guardarEnArchivo() const {
    fs::create_directories("../../Datos"); ///< Crea el directorio si no existe.
    ofstream archivo(rutaArchivo);         ///< Crea o sobrescribe el archivo.

    // Verificación de apertura del archivo.
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo crear el archivo en " << rutaArchivo << endl;
        return;
    }

    int n = enrutadores.size();
    vector<vector<int>> matriz(n, vector<int>(n, -1));

    // Construye la matriz antes de guardarla.
    for (int i = 0; i < n; ++i) {
        matriz[i][i] = 0;
        for (auto& v : enrutadores[i]->vecinos)
            matriz[i][v.first->id] = v.second;
    }

    // Escribe la matriz en el archivo.
    for (const auto& fila : matriz) {
        for (int valor : fila)
            archivo << valor << " ";
        archivo << "\n";
    }

    archivo.close();
    cout << "Red guardada en: " << rutaArchivo << endl;
}

/**
 * @brief Calcula la ruta más corta entre dos enrutadores usando Dijkstra.
 *
 * Aplica el algoritmo de Dijkstra para determinar la distancia mínima
 * entre un enrutador origen y otro destino dentro de la red.
 *
 * @param origen Índice del enrutador origen.
 * @param destino Índice del enrutador destino.
 */
void Red::calcularRutaMasCorta(int origen, int destino) {
    // Verifica que los índices sean válidos.
    if (origen < 0 || destino < 0 ||
        origen >= (int)enrutadores.size() || destino >= (int)enrutadores.size()) {
        cerr << "Error: Índices de enrutadores fuera de rango.\n";
        return;
    }

    // Reinicia los estados de todos los enrutadores.
    for (auto& r : enrutadores)
        r->reiniciar();

    Router* fuente = enrutadores[origen];
    fuente->distancia = 0; ///< Distancia inicial del origen es 0.

    // Cola de prioridad para seleccionar el enrutador con menor distancia.
    priority_queue<pair<int, Router*>, vector<pair<int, Router*>>, greater<>> pq;
    pq.push({0, fuente});

    // Bucle principal de Dijkstra.
    while (!pq.empty()) {
        Router* actual = pq.top().second;
        pq.pop();

        // Si ya fue visitado, se omite.
        if (actual->visitado)
            continue;

        actual->visitado = true;

        // Explora cada vecino del enrutador actual.
        for (auto& v : actual->vecinos) {
            Router* vecino = v.first;
            int costo = v.second;

            int nuevaDist = actual->distancia + costo;

            // Si se encuentra una ruta más corta, se actualiza.
            if (nuevaDist < vecino->distancia) {
                vecino->distancia = nuevaDist;
                vecino->previo = actual;
                pq.push({nuevaDist, vecino});
            }
        }
    }

    // Si el destino no fue alcanzado, no hay ruta.
    if (enrutadores[destino]->distancia == INT_MAX) {
        cout << "\nNo existe una ruta entre R" << origen << " y R" << destino << ".\n";
        return;
    }

    // Muestra resultados.
    cout << "\n==================================\n";
    cout << "RUTA MÁS CORTA ENTRE R" << origen << " Y R" << destino << endl;
    cout << "==================================\n";
    cout << "Costo total: " << enrutadores[destino]->distancia << endl;
    cout << "Camino: ";

    // Reconstruye la ruta inversa desde el destino.
    vector<int> camino;
    for (Router* r = enrutadores[destino]; r != nullptr; r = r->previo)
        camino.push_back(r->id);
    reverse(camino.begin(), camino.end()); ///< Invierte el vector para mostrar origen → destino.

    // Imprime la secuencia de enrutadores.
    for (size_t i = 0; i < camino.size(); ++i) {
        cout << "R" << camino[i];
        if (i < camino.size() - 1)
            cout << " -> ";
    }
    cout << "\n";
}
