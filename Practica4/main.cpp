#include "red.h"
#include <iostream>
using namespace std;

/**
 * @brief Programa principal que simula la creación y análisis de una red de enrutadores.
 */
int main() {
    cout << "=========================================\n";
    cout << "|| SIMULADOR DE REDES DE ENRUTADORES   ||\n";
    cout << "=========================================\n\n";

    int cantidad;
    cout << "Ingrese la cantidad de enrutadores a generar: ";
    cin >> cantidad;

    Red red(cantidad);
    red.generarRedAleatoria();
    red.mostrarRed();
    red.guardarEnArchivo();

    int origen, destino;
    cout << "\nIngrese el enrutador origen (número): R";
    cin >> origen;
    cout << "Ingrese el enrutador destino (número): R";
    cin >> destino;

    red.calcularRutaMasCorta(origen, destino);

    cout << "\nProceso completado exitosamente.\n";
    return 0;
}
