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

// --- UTILIDADES DE CADENA DINÁMICA ---

ListaCaracteres* crearListaDesdeConstante(const char* texto) {
    ListaCaracteres* lista = new ListaCaracteres;
    if (!texto) return lista;
    
    int i = 0;
    while (texto[i] != '\0') {
        StringDinamico* nuevo = new StringDinamico;
        nuevo->caracter = texto[i];
        nuevo->siguiente = nullptr;
        
        if (!lista->primero) {
            lista->primero = nuevo;
            lista->ultimo = nuevo;
        } else {
            lista->ultimo->siguiente = nuevo;
            lista->ultimo = nuevo;
        }
        i++;
    }
    return lista;
}

ListaCaracteres* copiarListaCaracteres(ListaCaracteres* original) {
    if (!original) return nullptr;
    
    ListaCaracteres* copia = new ListaCaracteres;
    StringDinamico* actual = original->primero;
    
    while (actual != nullptr) {
        StringDinamico* nuevo = new StringDinamico;
        nuevo->caracter = actual->caracter;
        nuevo->siguiente = nullptr;
        
        if (!copia->primero) {
            copia->primero = nuevo;
            copia->ultimo = nuevo;
        } else {
            copia->ultimo->siguiente = nuevo;
            copia->ultimo = nuevo;
        }
        actual = actual->siguiente;
    }
    return copia;
}

bool compararListas(ListaCaracteres* l1, ListaCaracteres* l2) {
    if (!l1 && !l2) return true;
    if (!l1 || !l2) return false;
    
    StringDinamico* a1 = l1->primero;
    StringDinamico* a2 = l2->primero;
    
    while (a1 != nullptr && a2 != nullptr) {
        if (a1->caracter != a2->caracter) return false;
        a1 = a1->siguiente;
        a2 = a2->siguiente;
    }
    return a1 == nullptr && a2 == nullptr;
}

void imprimirLista(ListaCaracteres* lista) {
    if (!lista || !lista->primero) return;
    
    StringDinamico* actual = lista->primero;
    while (actual != nullptr) {
        cout << actual->caracter;
        actual = actual->siguiente;
    }
}

void liberarListaCaracteres(ListaCaracteres* lista) {
    if (!lista) return;
    
    StringDinamico* actual = lista->primero;
    while (actual != nullptr) {
        StringDinamico* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    delete lista;
}

// Función para leer una palabra (token)
ListaCaracteres* leerPalabra() {
    ListaCaracteres* lista = new ListaCaracteres;
    char c;
    
    // Leer primer carácter
    if (!cin.get(c)) {
        delete lista;
        return nullptr;
    }
    
    // Si es espacio o salto de línea, saltar hasta encontrar un carácter válido
    if (c == ' ' || c == '\t' || c == '\n') {
        while (c == ' ' || c == '\t' || c == '\n') {
            if (!cin.get(c)) {
                delete lista;
                return nullptr;
            }
        }
    }
    
    // Primer carácter válido
    StringDinamico* nuevo = new StringDinamico;
    nuevo->caracter = c;
    nuevo->siguiente = nullptr;
    lista->primero = nuevo;
    lista->ultimo = nuevo;
    
    // Leer resto de la palabra
    while (cin.get(c) && c != ' ' && c != '\t' && c != '\n') {
        nuevo = new StringDinamico;
        nuevo->caracter = c;
        nuevo->siguiente = nullptr;
        lista->ultimo->siguiente = nuevo;
        lista->ultimo = nuevo;
    }
    
    // Si leímos un espacio o salto de línea, devolverlo
    if (c == ' ' || c == '\t' || c == '\n') {
        cin.putback(c);
    }
    
    return lista;
}

// NUEVA FUNCIÓN MEJORADA PARA LEER LÍNEA
ListaCaracteres* leerLinea() {
    ListaCaracteres* lista = new ListaCaracteres;
    char c;
    
    // Leer primer carácter
    if (!cin.get(c)) {
        delete lista;
        return nullptr;
    }
    
    // Si el primer carácter es salto de línea, es línea vacía
    if (c == '\n') {
        delete lista;
        return nullptr;
    }
    
    // Primer carácter de la línea
    StringDinamico* nuevo = new StringDinamico;
    nuevo->caracter = c;
    nuevo->siguiente = nullptr;
    lista->primero = nuevo;
    lista->ultimo = nuevo;
    
    // Leer resto de la línea
    while (cin.get(c) && c != '\n') {
        nuevo = new StringDinamico;
        nuevo->caracter = c;
        nuevo->siguiente = nullptr;
        lista->ultimo->siguiente = nuevo;
        lista->ultimo = nuevo;
    }
    
    return lista;
}

// Limpiar el buffer de entrada
void limpiarBuffer() {
    char c;
    while (cin.get(c) && c != '\n') {
        // Consumir caracteres
    }
}

bool listaVacia(ListaCaracteres* lista) {
    return !lista || !lista->primero;
}