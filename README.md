# Práctica 4 - Simulador de Red de Enrutadores

Este proyecto corresponde a la **Práctica 4** del curso de Ingeniería de Telecomunicaciones, cuyo objetivo es **simular el funcionamiento básico de una red de enrutadores**.  
El programa permite agregar, eliminar y conectar enrutadores, así como calcular rutas más cortas entre ellos mediante un menú interactivo en consola.

---

## Descripción del proyecto

El simulador implementa una estructura de red en la que cada enrutador se representa como un nodo, conectado mediante enlaces con costos asociados.

El usuario puede realizar diferentes operaciones sobre la red, tales como:

- Mostrar la red actual y sus conexiones  
- Calcular la ruta más corta entre dos enrutadores  
- Agregar o eliminar enrutadores  
- Agregar o eliminar enlaces  
- Guardar y cargar la red desde un archivo externo

---

## Estructura del repositorio
```
Practica-4/
│
├── Datos/
│ └── red.txt Archivo de configuración inicial de la red
| |-topologia.txt
│
├── Practica4/
│ ├── red.h Declaración de la clase Red
│ ├── red.cpp Implementación de la clase Red
│ ├── main.cpp Programa principal con el menú de usuario
│ └── (otros archivos necesarios)
│
├── README.md Documentación del proyecto
```

---

## Funcionalidades principales

1. **Mostrar red actual**  
   Muestra todos los enrutadores y los costos de los enlaces entre ellos.

2. **Calcular ruta más corta**  
   Calcula la ruta óptima entre dos enrutadores usando el algoritmo de Dijkstra.

3. **Agregar enrutador**  
   Permite añadir un nuevo nodo a la red.

4. **Eliminar enrutador**  
   Elimina un enrutador existente y todos sus enlaces.

5. **Agregar enlace**  
   Conecta dos enrutadores con un costo definido por el usuario.

6. **Eliminar enlace**  
   Elimina una conexión existente entre dos enrutadores.

7. **Guardar red en archivo**  
   Guarda la topología actual en el archivo `Datos/red.txt`.

8. **Cargar red desde archivo**  
   Carga una topología previamente guardada desde el archivo `Datos/red.txt`.


## Instrucciones de uso

### Clonación del repositorio

Para clonar este proyecto, use el siguiente comando en su terminal:
---
```bash
git clone https://github.com/B-Alejandro/Practica-4.git
```
---

## Autor

**Alejandro Bedoya**  
Estudiante de Ingeniería de Telecomunicaciones  
Universidad de Antioquia
