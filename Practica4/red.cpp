#include "red.h"
#include "enrutador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>      // setw()
#include <queue>        // priority_queue
#include <algorithm>    // reverse(), remove_if()
#include <utility>      // pair
#include <functional>   // greater<>
#include <cstdlib>      // rand(), srand()
#include <ctime>        // time()
#include <climits>      // INT_MAX
using namespace std;

// =======================================================
//              Constructores y destructor
// =======================================================

Red::Red() {}  // Constructor vacío

Red::Red(int cantidad) {
    for (int i = 1; i <= cantidad; ++i)
        enrutadores.push_back(new Router(i));
}

Red::~Red() {
    for (auto* r : enrutadores)
        delete r;
    enrutadores.clear();
}

// =======================================================
//              Generación aleatoria
// =======================================================
void Red::generarRedAleatoria() {
    srand(time(nullptr));
    int n = enrutadores.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int prob = rand() % 100;
            if (prob < 40) { // 40% de probabilidad de conexión
                int costo = (rand() % 20) + 1;
                enrutadores[i]->nuevoVecino(enrutadores[j], costo);
                enrutadores[j]->nuevoVecino(enrutadores[i], costo);
            }
        }
    }

    cout << "✅ Red aleatoria generada correctamente.\n";
}

// =======================================================
//              Mostrar la red (matriz de costos)
// =======================================================
void Red::mostrarRed() const {
    int n = enrutadores.size();
    if (n == 0) {
        cout << "⚠️ No hay enrutadores en la red.\n";
        return;
    }

    vector<vector<int>> matriz(n, vector<int>(n, -1));

    for (int i = 0; i < n; ++i) {
        matriz[i][i] = 0;
        for (auto& v : enrutadores[i]->vecinos)
            matriz[i][v.first->id - 1] = v.second;
    }

    cout << "\n========= MATRIZ DE COSTOS =========\n";
    cout << setw(5) << " ";
    for (int j = 0; j < n; ++j)
        cout << setw(4) << "R" + to_string(j + 1);
    cout << endl;

    for (int i = 0; i < n; ++i) {
        cout << setw(4) << "R" + to_string(i + 1);
        for (int j = 0; j < n; ++j) {
            if (matriz[i][j] == -1)
                cout << setw(4) << "-";
            else
                cout << setw(4) << matriz[i][j];
        }
        cout << endl;
    }
}

// =======================================================
//              Guardar y cargar desde archivo
// =======================================================
void Red::guardarEnArchivo(const string& nombreArchivo) const {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "❌ No se pudo guardar el archivo.\n";
        return;
    }

    for (auto* r : enrutadores) {
        for (auto& [vecino, costo] : r->vecinos) {
            if (r->id < vecino->id) // evitar duplicados
                archivo << "R" << r->id << " R" << vecino->id << " " << costo << "\n";
        }
    }

    archivo.close();
    cout << "💾 Red guardada correctamente en " << nombreArchivo << "\n";
}

void Red::cargarDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "❌ No se pudo abrir el archivo: " << nombreArchivo << "\n";
        return;
    }

    enrutadores.clear();

    string origen, destino;
    int costo;

    // Crear routers a medida que se encuentran
    auto obtenerRouter = [&](int id) -> Router* {
        for (auto* r : enrutadores)
            if (r->id == id) return r;
        Router* nuevo = new Router(id);
        enrutadores.push_back(nuevo);
        return nuevo;
    };

    while (archivo >> origen >> destino >> costo) {
        if (origen[0] == '#') { // Ignorar comentarios
            archivo.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        int id1 = stoi(origen.substr(1));
        int id2 = stoi(destino.substr(1));

        Router* r1 = obtenerRouter(id1);
        Router* r2 = obtenerRouter(id2);

        r1->nuevoVecino(r2, costo);
        r2->nuevoVecino(r1, costo); // bidireccional
    }

    archivo.close();
    cout << "✅ Red cargada correctamente desde " << nombreArchivo << "\n";
}

// =======================================================
//              Calcular ruta más corta (Dijkstra)
// =======================================================
void Red::calcularRutaMasCorta(int origen, int destino) {
    if (enrutadores.empty()) {
        cout << "⚠️ La red está vacía.\n";
        return;
    }

    if (origen <= 0 || destino <= 0 ||
        origen > (int)enrutadores.size() || destino > (int)enrutadores.size()) {
        cout << "⚠️ IDs fuera de rango.\n";
        return;
    }

    for (auto& r : enrutadores)
        r->reiniciar();

    Router* fuente = enrutadores[origen - 1];
    fuente->distancia = 0;

    priority_queue<pair<int, Router*>, vector<pair<int, Router*>>, greater<>> pq;
    pq.push({0, fuente});

    while (!pq.empty()) {
        Router* actual = pq.top().second;
        pq.pop();

        if (actual->visitado) continue;
        actual->visitado = true;

        for (auto& [vecino, costo] : actual->vecinos) {
            int nuevaDist = actual->distancia + costo;
            if (nuevaDist < vecino->distancia) {
                vecino->distancia = nuevaDist;
                vecino->previo = actual;
                pq.push({nuevaDist, vecino});
            }
        }
    }

    Router* destinoPtr = enrutadores[destino - 1];
    if (destinoPtr->distancia == INT_MAX) {
        cout << "❌ No existe una ruta entre R" << origen << " y R" << destino << ".\n";
        return;
    }

    vector<int> camino;
    for (Router* r = destinoPtr; r != nullptr; r = r->previo)
        camino.push_back(r->id);
    reverse(camino.begin(), camino.end());

    cout << "\nRuta más corta entre R" << origen << " y R" << destino << ":\n";
    for (size_t i = 0; i < camino.size(); ++i) {
        cout << "R" << camino[i];
        if (i < camino.size() - 1) cout << " -> ";
    }
    cout << "\nCosto total: " << destinoPtr->distancia << "\n";
}

// =======================================================
//              Gestión de enrutadores
// =======================================================
void Red::agregarEnrutador() {
    int nuevoID = enrutadores.size() + 1;
    Router* nuevo = new Router(nuevoID);

    cout << "\nAgregando nuevo enrutador R" << nuevoID << "...\n";
    if (nuevoID > 1) {
        cout << "¿Desea conectar este enrutador con otros? (s/n): ";
        char op;
        cin >> op;
        if (op == 's' || op == 'S') {
            while (true) {
                int destino, costo;
                cout << "Ingrese ID destino (-1 para terminar): ";
                cin >> destino;
                if (destino == -1) break;

                if (destino <= 0 || destino > (int)enrutadores.size()) {
                    cout << "⚠️ ID inválido.\n";
                    continue;
                }

                cout << "Ingrese costo del enlace: ";
                cin >> costo;
                if (costo <= 0) {
                    cout << "⚠️ Costo inválido.\n";
                    continue;
                }

                nuevo->nuevoVecino(enrutadores[destino - 1], costo);
                enrutadores[destino - 1]->nuevoVecino(nuevo, costo);
                cout << "Conexión creada entre R" << nuevoID << " y R" << destino << ".\n";
            }
        }
    }

    enrutadores.push_back(nuevo);
    cout << "✅ Enrutador R" << nuevoID << " agregado correctamente.\n";
}

void Red::eliminarEnrutador(int id) {
    if (id <= 0 || id > (int)enrutadores.size()) {
        cout << "⚠️ ID inválido.\n";
        return;
    }

    Router* objetivo = enrutadores[id - 1];
    for (Router* r : enrutadores) {
        if (r != objetivo) {
            r->vecinos.erase(
                remove_if(r->vecinos.begin(), r->vecinos.end(),
                          [objetivo](auto& p){ return p.first == objetivo; }),
                r->vecinos.end());
        }
    }

    delete objetivo;
    enrutadores.erase(enrutadores.begin() + id - 1);

    for (int i = 0; i < (int)enrutadores.size(); ++i)
        enrutadores[i]->id = i + 1;

    cout << "✅ Enrutador eliminado correctamente.\n";
}

// =======================================================
//              Gestión de enlaces
// =======================================================
void Red::agregarEnlace() {
    if (enrutadores.size() < 2) {
        cout << "⚠️ No hay suficientes enrutadores.\n";
        return;
    }

    int a, b, costo;
    cout << "Ingrese ID del primer enrutador: ";
    cin >> a;
    cout << "Ingrese ID del segundo enrutador: ";
    cin >> b;
    cout << "Ingrese costo del enlace: ";
    cin >> costo;

    if (a == b || a <= 0 || b <= 0 ||
        a > (int)enrutadores.size() || b > (int)enrutadores.size() || costo <= 0) {
        cout << "⚠️ Datos inválidos.\n";
        return;
    }

    enrutadores[a - 1]->nuevoVecino(enrutadores[b - 1], costo);
    enrutadores[b - 1]->nuevoVecino(enrutadores[a - 1], costo);
    cout << "✅ Enlace creado entre R" << a << " y R" << b << " con costo " << costo << ".\n";
}

void Red::eliminarEnlace() {
    if (enrutadores.size() < 2) {
        cout << "⚠️ No hay suficientes enrutadores.\n";
        return;
    }

    int a, b;
    cout << "Ingrese ID del primer enrutador: ";
    cin >> a;
    cout << "Ingrese ID del segundo enrutador: ";
    cin >> b;

    if (a == b || a <= 0 || b <= 0 ||
        a > (int)enrutadores.size() || b > (int)enrutadores.size()) {
        cout << "⚠️ IDs inválidos.\n";
        return;
    }

    Router* rA = enrutadores[a - 1];
    Router* rB = enrutadores[b - 1];

    auto remover = [&](Router* origen, Router* destino) {
        origen->vecinos.erase(
            remove_if(origen->vecinos.begin(), origen->vecinos.end(),
                      [destino](auto& p){ return p.first == destino; }),
            origen->vecinos.end());
    };

    remover(rA, rB);
    remover(rB, rA);

    cout << "✅ Enlace eliminado entre R" << a << " y R" << b << ".\n";
}
