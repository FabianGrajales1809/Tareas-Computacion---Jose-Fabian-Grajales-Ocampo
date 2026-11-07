class Nodo:
    def __init__(self, valor, n):
        self.valor = valor
        # Arreglo de tamaño exacto 'n' inicializado en None
        self.hijos = [None] * n

class ArbolNArio:
    def __init__(self, n):
        if n <= 2:
            raise ValueError("El valor de n debe ser mayor a 2.")
        self.n = n
        self.raiz = None

    def insertar(self, valor):
        if self.raiz is None:
            self.raiz = Nodo(valor, self.n)
        else:
            self._insertar_recursivo(self.raiz, valor)

    def _insertar_recursivo(self, actual, valor):
        # Intentar insertar en los espacios libres del nodo actual
        for i in range(self.n):
            if actual.hijos[i] is None:
                actual.hijos[i] = Nodo(valor, self.n)
                return True
                
        # Si están llenos, bajar recursivamente por los hijos
        for i in range(self.n):
            if self._insertar_recursivo(actual.hijos[i], valor):
                return True
                
        return False

    def imprimir(self):
        print("Contenido del árbol:")
        self._imprimir_recursivo(self.raiz, 0)

    def _imprimir_recursivo(self, actual, nivel):
        if actual is None:
            return
            
        # Sangría visual
        print("   " * nivel + f"-> {actual.valor}")
        
        # Llamada recursiva a los hijos
        for hijo in actual.hijos:
            if hijo is not None:
                self._imprimir_recursivo(hijo, nivel + 1)

def main():
    n = 3 # Parámetro: cada nodo tiene exactamente 3 hijos (n > 2)
    print(f"--- ÁRBOL N-ARIO (n = {n}) en Python ---")
    
    arbol = ArbolNArio(n)
    
    # Insertar nodos
    datos_a_insertar = [10, 20, 30, 40, 50, 60, 70, 80]
    for dato in datos_a_insertar:
        arbol.insertar(dato)
        
    # Imprimir árbol usando recursividad
    arbol.imprimir()

if __name__ == "__main__":
    main()
