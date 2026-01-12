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

// --- FUNCIONES DEL SISTEMA DE ARCHIVOS ---

Nodo* buscarEnCarpeta(Nodo* carpeta, ListaCaracteres* nombre) {
    if (!carpeta) return nullptr;
    
    Nodo* aux = carpeta->primerHijo;
    while (aux != nullptr) {
        if (compararListas(aux->nombre, nombre)) return aux;
        aux = aux->siguienteHijo;
    }
    return nullptr;
}

void insertarHijo(Nodo* padre, Nodo* nuevo) {
    if (!padre) return;
    
    if (padre->primerHijo == nullptr) {
        padre->primerHijo = nuevo;
    } else {
        Nodo* aux = padre->primerHijo;
        while (aux->siguienteHijo != nullptr) {
            aux = aux->siguienteHijo;
        }
        aux->siguienteHijo = nuevo;
    }
    nuevo->padre = padre;
}

// Función para desconectar un nodo de su padre
void desconectarNodo(Nodo* nodo) {
    if (!nodo || !nodo->padre) return;
    
    Nodo* padre = nodo->padre;
    
    // Caso 1: Es el primer hijo
    if (padre->primerHijo == nodo) {
        padre->primerHijo = nodo->siguienteHijo;
    } 
    // Caso 2: Está en medio o al final
    else {
        Nodo* actual = padre->primerHijo;
        while (actual && actual->siguienteHijo != nodo) {
            actual = actual->siguienteHijo;
        }
        if (actual) {
            actual->siguienteHijo = nodo->siguienteHijo;
        }
    }
    nodo->siguienteHijo = nullptr;
}

// --- COMANDOS ---

void comandoLs(Nodo* actual) {
    if (!actual) return;
    
    Nodo* aux = actual->primerHijo;
    if (!aux) {
        cout << "(directorio vacio)\n";
        return;
    }
    
    while (aux != nullptr) {
        if (aux->esCarpeta) cout << "[DIR]  ";
        else cout << "[FILE] ";
        imprimirLista(aux->nombre);
        cout << "\n";
        aux = aux->siguienteHijo;
    }
}

void comandoMkdir(Nodo* actual, ListaCaracteres* nombre) {
    if (!actual || !nombre) {
        cout << "Error: Parametros invalidos.\n";
        if (nombre) liberarListaCaracteres(nombre);
        return;
    }
    
    if (buscarEnCarpeta(actual, nombre)) {
        cout << "Error: Ya existe una carpeta o archivo con ese nombre.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    Nodo* nuevo = new Nodo(nombre, true, actual);
    insertarHijo(actual, nuevo);
    cout << "Directorio '";
    imprimirLista(nombre);
    cout << "' creado exitosamente.\n";
    liberarListaCaracteres(nombre);
}

void comandoTouch(Nodo* actual, ListaCaracteres* nombre) {
    if (!actual || !nombre) {
        cout << "Error: Parametros invalidos.\n";
        if (nombre) liberarListaCaracteres(nombre);
        return;
    }
    
    if (buscarEnCarpeta(actual, nombre)) {
        cout << "Error: El archivo ya existe.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    Nodo* nuevo = new Nodo(nombre, false, actual);
    insertarHijo(actual, nuevo);
    cout << "Archivo '";
    imprimirLista(nombre);
    cout << "' creado exitosamente.\n";
    liberarListaCaracteres(nombre);
}

void comandoCd(Nodo* &actual, Nodo* raiz, ListaCaracteres* ruta) {
    if (!ruta) {
        cout << "Error: Ruta no especificada.\n";
        return;
    }
    
    // Casos especiales
    ListaCaracteres* raizStr = crearListaDesdeConstante("/");
    ListaCaracteres* padreStr = crearListaDesdeConstante("..");
    ListaCaracteres* actualStr = crearListaDesdeConstante(".");
    
    if (compararListas(ruta, raizStr)) {
        actual = raiz;
        cout << "Cambiado al directorio raiz.\n";
    }
    else if (compararListas(ruta, padreStr)) {
        if (actual->padre) {
            actual = actual->padre;
            cout << "Cambiado al directorio padre.\n";
        } else {
            cout << "Ya estas en el directorio raiz.\n";
        }
    }
    else if (compararListas(ruta, actualStr)) {
        cout << "Ya estas en este directorio.\n";
    }
    else {
        // Buscar en el directorio actual
        Nodo* destino = buscarEnCarpeta(actual, ruta);
        if (destino && destino->esCarpeta) {
            actual = destino;
            cout << "Cambiado al directorio '";
            imprimirLista(ruta);
            cout << "'.\n";
        } else {
            cout << "Error: Directorio no encontrado.\n";
        }
    }
    
    liberarListaCaracteres(raizStr);
    liberarListaCaracteres(padreStr);
    liberarListaCaracteres(actualStr);
    liberarListaCaracteres(ruta);
}