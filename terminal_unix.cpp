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

void comandoMv(Nodo* actual, ListaCaracteres* origenStr, ListaCaracteres* destinoStr) {
    if (!origenStr || !destinoStr) {
        cout << "Error: Parametros invalidos.\n";
        if (origenStr) liberarListaCaracteres(origenStr);
        if (destinoStr) liberarListaCaracteres(destinoStr);
        return;
    }
    
    // Buscar el nodo origen en el directorio actual
    Nodo* origen = buscarEnCarpeta(actual, origenStr);
    if (!origen) {
        cout << "Error: Elemento origen no encontrado.\n";
        liberarListaCaracteres(origenStr);
        liberarListaCaracteres(destinoStr);
        return;
    }
    
    // Buscar si el destino existe como carpeta
    Nodo* destinoCarpeta = buscarEnCarpeta(actual, destinoStr);
    
    if (destinoCarpeta && destinoCarpeta->esCarpeta) {
        // Mover a otra carpeta
        if (buscarEnCarpeta(destinoCarpeta, origen->nombre)) {
            cout << "Error: Ya existe un elemento con ese nombre en el directorio destino.\n";
        } else {
            desconectarNodo(origen);
            insertarHijo(destinoCarpeta, origen);
            cout << "Elemento movido exitosamente.\n";
        }
    } else {
        // Renombrar
        if (buscarEnCarpeta(actual, destinoStr)) {
            cout << "Error: Ya existe un elemento con ese nombre.\n";
        } else {
            liberarListaCaracteres(origen->nombre);
            origen->nombre = copiarListaCaracteres(destinoStr);
            cout << "Elemento renombrado exitosamente.\n";
        }
    }
    
    liberarListaCaracteres(origenStr);
    liberarListaCaracteres(destinoStr);
}

// EDITOR DE TEXTO CORREGIDO
void comandoEdit(Nodo* actual, ListaCaracteres* nombre) {
    if (!actual || !nombre) {
        cout << "Error: Parametros invalidos.\n";
        if (nombre) liberarListaCaracteres(nombre);
        return;
    }
    
    Nodo* archivo = buscarEnCarpeta(actual, nombre);
    if (!archivo) {
        cout << "Error: Archivo no encontrado.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    if (archivo->esCarpeta) {
        cout << "Error: No se puede editar una carpeta.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    cout << "Editando archivo '";
    imprimirLista(archivo->nombre);
    cout << "' (presione Enter en una linea vacia para terminar):\n";
    
    // IMPORTANTE: Limpiar el buffer antes de empezar a leer
    limpiarBuffer();
    
    // Liberar contenido anterior si existe
    if (archivo->contenido) {
        liberarListaCaracteres(archivo->contenido);
        archivo->contenido = nullptr;
    }
    
    ListaCaracteres* contenidoTotal = new ListaCaracteres;
    bool primeraLinea = true;
    
    while (true) {
        cout << "> ";
        ListaCaracteres* linea = leerLinea();
        
        // Si línea es nullptr o vacía, terminar
        if (!linea || listaVacia(linea)) {
            if (linea) liberarListaCaracteres(linea);
            break;
        }
        
        // Agregar salto de línea si no es la primera línea
        if (!primeraLinea) {
            StringDinamico* saltoLinea = new StringDinamico;
            saltoLinea->caracter = '\n';
            saltoLinea->siguiente = nullptr;
            
            if (!contenidoTotal->primero) {
                contenidoTotal->primero = saltoLinea;
                contenidoTotal->ultimo = saltoLinea;
            } else {
                contenidoTotal->ultimo->siguiente = saltoLinea;
                contenidoTotal->ultimo = saltoLinea;
            }
        }
        
        // Agregar contenido de la línea
        StringDinamico* charActual = linea->primero;
        while (charActual != nullptr) {
            StringDinamico* nuevo = new StringDinamico;
            nuevo->caracter = charActual->caracter;
            nuevo->siguiente = nullptr;
            
            if (!contenidoTotal->primero) {
                contenidoTotal->primero = nuevo;
                contenidoTotal->ultimo = nuevo;
            } else {
                contenidoTotal->ultimo->siguiente = nuevo;
                contenidoTotal->ultimo = nuevo;
            }
            
            charActual = charActual->siguiente;
        }
        
        primeraLinea = false;
        liberarListaCaracteres(linea);
    }
    
    archivo->contenido = contenidoTotal;
    cout << "Contenido guardado exitosamente.\n";
    liberarListaCaracteres(nombre);
}

void comandoCat(Nodo* actual, ListaCaracteres* nombre) {
    if (!actual || !nombre) {
        cout << "Error: Parametros invalidos.\n";
        if (nombre) liberarListaCaracteres(nombre);
        return;
    }
    
    Nodo* archivo = buscarEnCarpeta(actual, nombre);
    if (!archivo) {
        cout << "Error: Archivo no encontrado.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    if (archivo->esCarpeta) {
        cout << "Error: No se puede mostrar contenido de una carpeta.\n";
        liberarListaCaracteres(nombre);
        return;
    }
    
    if (!archivo->contenido || !archivo->contenido->primero) {
        cout << "(archivo vacio)\n";
    } else {
        imprimirLista(archivo->contenido);
        cout << "\n";
    }
    liberarListaCaracteres(nombre);
}

void imprimirRuta(Nodo* actual) {
    if (!actual) return;
    
    if (actual->padre == nullptr) {
        cout << "/";
        return;
    }
    imprimirRuta(actual->padre);
    if (actual->padre->padre != nullptr) cout << "/";
    imprimirLista(actual->nombre);
}