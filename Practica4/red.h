#ifndef RED_H
#define RED_H

#include <vector>
#include <string>
#include <utility>

class Router {
public:
    int id;
    int distancia;
    bool visitado;
    Router* previo;
    std::vector<std::pair<Router*, int>> vecinos;

    Router(int id);
    void nuevoVecino(Router* vecino, int costo);
    void reiniciar();
};

class Red {
private:
    std::vector<Router*> enrutadores;
    std::string rutaArchivo;

public:
    Red(int cantidad);
    ~Red();

    void generarRedAleatoria();
    void mostrarRed() const;
    void guardarEnArchivo() const;
    void calcularRutaMasCorta(int origen, int destino);
};

#endif
