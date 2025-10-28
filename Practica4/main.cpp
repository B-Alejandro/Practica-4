#include "red.h"
#include <iostream>
#include <limits>  // Para limpiar el buffer del cin
#include <fstream> // Para verificar si el archivo existe

using namespace std;

/**
 * @brief Muestra el menú principal del simulador.
 */
void mostrarMenu() {
    cout << "\n=========================================\n";
    cout << "||    SIMULADOR DE REDES DE ENRUTADORES ||\n";
    cout << "=========================================\n";
    cout << "1. Mostrar red actual\n";
    cout << "2. Calcular ruta más corta\n";
    cout << "3. Agregar enrutador\n";
    cout << "4. Eliminar enrutador\n";
    cout << "5. Agregar enlace\n";
    cout << "6. Eliminar enlace\n";
    cout << "7. Guardar red en archivo\n";
    cout << "8. Salir\n";
    cout << "=========================================\n";
    cout << "Seleccione una opción: ";
}

/**
 * @brief Programa principal que permite gestionar una red mediante un menú.
 */
int main() {
    Red red;  // Se crea una red vacía inicialmente

    string nombreArchivo = "../../Datos/red.txt";

    // ======================================================
    // Cargar red inicial desde archivo (si existe)
    // ======================================================
    ifstream archivoEntrada(nombreArchivo);
    if (archivoEntrada.good()) {
        cout << "🔄 Cargando red desde archivo \"" << nombreArchivo << "\"...\n";
        red.cargarDesdeArchivo(nombreArchivo);
        cout << "✅ Red cargada correctamente.\n";
    } else {
        cout << "⚠️  No se encontró el archivo \"" << nombreArchivo << "\".\n";
        cout << "¿Desea generar una nueva red aleatoria? (s/n): ";
        char opcion;
        cin >> opcion;

        if (opcion == 's' || opcion == 'S') {
            int cantidad;
            cout << "Ingrese la cantidad de enrutadores a generar: ";
            cin >> cantidad;
            red = Red(cantidad);
            red.generarRedAleatoria();
            cout << "✅ Red generada exitosamente.\n";
        } else {
            cout << "No se cargó ninguna red. Finalizando...\n";
            return 0;
        }
    }

    // ======================================================
    // Menú interactivo
    // ======================================================
    int opcion = 0;
    do {
        mostrarMenu();
        cin >> opcion;

        // Limpieza del buffer en caso de error de entrada
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Intente nuevamente.\n";
            continue;
        }

        switch (opcion) {
        case 1:
            red.mostrarRed();
            break;

        case 2: {
            int origen, destino;
            cout << "Ingrese el enrutador origen (solo número, sin 'R'): ";
            cin >> origen;
            cout << "Ingrese el enrutador destino (solo número, sin 'R'): ";
            cin >> destino;
            red.calcularRutaMasCorta(origen, destino);
            break;
        }

        case 3:
            red.agregarEnrutador();
            break;

        case 4: {
            int id;
            cout << "Ingrese el ID del enrutador a eliminar: ";
            cin >> id;
            red.eliminarEnrutador(id);
            break;
        }

        case 5:
            red.agregarEnlace();
            break;

        case 6:
            red.eliminarEnlace();
            break;

        case 7:
            red.guardarEnArchivo(nombreArchivo);
            cout << "💾 Red guardada en \"" << nombreArchivo << "\" correctamente.\n";
            break;

        case 8:
            cout << "\nSaliendo del programa...\n";
            break;

        default:
            cout << "Opción no válida. Intente nuevamente.\n";
            break;
        }

    } while (opcion != 8);

    cout << "✅ Programa finalizado correctamente.\n";
    return 0;
}
