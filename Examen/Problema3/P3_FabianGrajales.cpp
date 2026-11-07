#include <iostream>

using namespace std;


class Lista;

// Clase Nodo (Objeto que pertenece a Lista)
class Nodo {
private:
    int valor;
    Nodo* siguiente;
    Nodo* anterior;

public:
    // Constructor del Nodo
    Nodo(int v) : valor(v), siguiente(nullptr), anterior(nullptr) {}

    // Otorgamos acceso a la clase Lista y al operador <<
    friend class Lista;
    friend ostream& operator<<(ostream& os, const Lista& lista);
};


class Lista {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    // Constructor por defecto
    Lista() : cabeza(nullptr), cola(nullptr) {}

    // Constructor de copia 
    Lista(const Lista& otra) : cabeza(nullptr), cola(nullptr) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertarFinal(actual->valor);
            actual = actual->siguiente;
        }
    }

    // Destructor
    ~Lista() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* temp = actual->siguiente;
            delete actual; // Liberamos la memoria dinámica
            actual = temp;
        }
    }

    // Insertar al inicio
    void insertarInicio(int v) {
        Nodo* nuevo = new Nodo(v);
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
    }

    // Insertar al final
    void insertarFinal(int v) {
        Nodo* nuevo = new Nodo(v);
        if (!cola) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    // Insertar en una posición específica (índice 0 es la cabeza)
    void insertarPosicion(int v, int pos) {
        if (pos <= 0) {
            insertarInicio(v);
            return;
        }

        Nodo* actual = cabeza;
        int contador = 0;
        
        while (actual != nullptr && contador < pos - 1) {
            actual = actual->siguiente;
            contador++;
        }

        if (actual == nullptr || actual->siguiente == nullptr) {
            insertarFinal(v);
        } else {
            Nodo* nuevo = new Nodo(v);
            Nodo* temp_siguiente = actual->siguiente;

            nuevo->siguiente = temp_siguiente;
            nuevo->anterior = actual;
            
            actual->siguiente = nuevo;
            temp_siguiente->anterior = nuevo;
        }
    }

    // Borrar un nodo dado su valor (borra la primera coincidencia)
    void borrarNodo(int v) {
        Nodo* actual = cabeza;
        while (actual != nullptr && actual->valor != v) {
            actual = actual->siguiente;
        }

        if (!actual) return; // No se encontró

        if (actual == cabeza) {
            cabeza = actual->siguiente;
            if (cabeza) cabeza->anterior = nullptr;
            else cola = nullptr; // La lista quedó vacía
        } 
        else if (actual == cola) {
            cola = actual->anterior;
            if (cola) cola->siguiente = nullptr;
        } 
        else {
            actual->anterior->siguiente = actual->siguiente;
            actual->siguiente->anterior = actual->anterior;
        }
        delete actual;
    }

    // Ordenar la lista (Bubble sort intercambiando valores)
    void ordenar() {
        if (!cabeza || !cabeza->siguiente) return; // Vacía o con 1 elemento

        bool hubo_intercambio;
        do {
            hubo_intercambio = false;
            Nodo* actual = cabeza;
            
            while (actual->siguiente != nullptr) {
                if (actual->valor > actual->siguiente->valor) {
                    // Intercambiamos los valores, no los punteros
                    int temp = actual->valor;
                    actual->valor = actual->siguiente->valor;
                    actual->siguiente->valor = temp;
                    hubo_intercambio = true;
                }
                actual = actual->siguiente;
            }
        } while (hubo_intercambio);
    }

    // Sobrecarga del operador + para unir dos listas
    Lista operator+(const Lista& otra) const {
        // Hacemos una copia de la lista actual (gracias al constructor de copia)
        Lista nuevaLista(*this); 
        
        // Agregamos los elementos de la otra lista al final
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            nuevaLista.insertarFinal(actual->valor);
            actual = actual->siguiente;
        }
        
        return nuevaLista;
    }

    // Sobrecarga del operador << para imprimir
    friend ostream& operator<<(ostream& os, const Lista& lista) {
        Nodo* actual = lista.cabeza;
        os << "[ ";
        while (actual != nullptr) {
            os << actual->valor << " <-> ";
            actual = actual->siguiente;
        }
        os << "NULL ]";
        return os;
    }
};

int main() {
    cout << "--- PRUEBA DE LISTA DOBLEMENTE LIGADA ---" << endl;
    
    Lista L1;
    L1.insertarFinal(50);
    L1.insertarInicio(10);
    L1.insertarFinal(30);
    L1.insertarPosicion(20, 1); // Queda: 10, 20, 50, 30
    
    cout << "Lista 1 original: " << L1 << endl;
    
    L1.ordenar();
    cout << "Lista 1 ordenada: " << L1 << endl;
    
    L1.borrarNodo(20);
    cout << "Lista 1 tras borrar el 20: " << L1 << endl;

    Lista L2;
    L2.insertarFinal(100);
    L2.insertarFinal(200);
    cout << "\nLista 2: " << L2 << endl;

    // Prueba de la sobrecarga del operador +
    Lista L3 = L1 + L2;
    cout << "\nLista 3 (L1 + L2): " << L3 << endl;

    return 0;
}
