#include "red.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <filesystem>  // ✅ Para listar archivos fácilmente (C++17)

using namespace std;
namespace fs = std::filesystem;

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
    cout << "7. Guardar red\n";
    cout << "8. Mostrar tablas de enrutamiento\n";
    cout << "9. Salir\n";
    cout << "=========================================\n";
    cout << "Seleccione una opción: ";
}

/**
 * @brief Muestra los archivos red_*.txt disponibles en la carpeta "Datos".
 */
vector<string> listarRedesDisponibles(const string& carpeta) {
    vector<string> archivos;
    if (!fs::exists(carpeta)) fs::create_directory(carpeta);

    for (const auto& entry : fs::directory_iterator(carpeta)) {
        string nombre = entry.path().filename().string();

        // ✅ Compatible con C++17 (sin ends_with)
        if (nombre.rfind("red_", 0) == 0 &&
            nombre.size() >= 4 && nombre.substr(nombre.size() - 4) == ".txt") {
            archivos.push_back(nombre);
        }
    }

    if (archivos.empty()) {
        cout << "No hay redes guardadas disponibles.\n";
    } else {
        cout << "\nRedes disponibles:\n";
        for (size_t i = 0; i < archivos.size(); ++i) {
            cout << "  [" << i + 1 << "] " << archivos[i] << "\n";
        }
    }
    return archivos;
}

int main() {
    Red red;
    string carpeta = "../../Datos";
    string nombreArchivo;

    // ======================================================
    // Cargar red existente o crear una nueva
    // ======================================================
    cout << "¿Desea cargar una red existente o crear una nueva? (c/n): ";
    char opcion;
    cin >> opcion;

    if (opcion == 'c' || opcion == 'C') {
        vector<string> disponibles = listarRedesDisponibles(carpeta);
        if (!disponibles.empty()) {
            cout << "Seleccione el número de red a cargar: ";
            int eleccion;
            cin >> eleccion;
            if (eleccion > 0 && eleccion <= (int)disponibles.size()) {
                nombreArchivo = carpeta + "/" + disponibles[eleccion - 1];
                red.cargarDesdeArchivo(nombreArchivo);
            } else {
                cout << "Opción inválida. Finalizando.\n";
                return 0;
            }
        } else {
            cout << "No hay redes guardadas. Debe crear una nueva.\n";
            opcion = 'n';
        }
    }

    if (opcion == 'n' || opcion == 'N') {
        int cantidad;
        cout << "Ingrese la cantidad de enrutadores para la nueva red: ";
        cin >> cantidad;
        red = Red(cantidad);
        red.generarRedAleatoria();
        cout << "¿Desea guardar esta red? (s/n): ";
        char guardar;
        cin >> guardar;
        if (guardar == 's' || guardar == 'S') {
            nombreArchivo = carpeta + "/red_nueva.txt";
            red.guardarEnArchivo(nombreArchivo);
        }
    }

    // ======================================================
    // Menú principal
    // ======================================================
    int opcionMenu = 0;
    do {
        mostrarMenu();
        cin >> opcionMenu;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Intente nuevamente.\n";
            continue;
        }

        switch (opcionMenu) {
        case 1:
            red.mostrarRed();
            break;
        case 2: {
            int o, d;
            cout << "Ingrese enrutador origen: ";
            cin >> o;
            cout << "Ingrese enrutador destino: ";
            cin >> d;
            red.calcularRutaMasCorta(o, d);
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
            if (nombreArchivo.empty()) nombreArchivo = carpeta + "/red_guardada.txt";
            red.guardarEnArchivo(nombreArchivo);
            cout << "Red guardada en: " << nombreArchivo << endl;
            break;
        case 8:
            red.mostrarTablasDeEnrutamiento();
            break;
        case 9:
            cout << "\nSaliendo del programa...\n";
            break;
        default:
            cout << "Opción no válida.\n";
        }
    } while (opcionMenu != 9);

    cout << "Programa finalizado correctamente.\n";
    return 0;
}
