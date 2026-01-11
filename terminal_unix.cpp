#include <iostream>

using namespace std;

// --- ESTRUCTURAS DINÁMICAS ---

struct StringDinamico {
    char caracter;
    StringDinamico* siguiente;
};

struct ListaCaracteres {
    StringDinamico* primero;
    StringDinamico* ultimo;
    
    ListaCaracteres() : primero(nullptr), ultimo(nullptr) {}
};

// Declaración anticipada
ListaCaracteres* copiarListaCaracteres(ListaCaracteres* original);

struct Nodo {
    ListaCaracteres* nombre;
    bool esCarpeta;
    ListaCaracteres* contenido;
    
    Nodo* padre;
    Nodo* primerHijo;
    Nodo* siguienteHijo;

    Nodo(ListaCaracteres* n, bool carpeta, Nodo* p) {
        nombre = copiarListaCaracteres(n);
        esCarpeta = carpeta;
        padre = p;
        primerHijo = nullptr;
        siguienteHijo = nullptr;
        contenido = nullptr;
    }
};