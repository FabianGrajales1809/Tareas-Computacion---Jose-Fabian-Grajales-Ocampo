import heapq
import json

cdef class NodoHuffman:
    cdef public object char
    cdef public int freq
    cdef public NodoHuffman left
    cdef public NodoHuffman right

    def __init__(self, caracter, int freq):
        self.char = caracter
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, NodoHuffman other):
        return self.freq < other.freq

cdef class DescompresorHuffman:
    cdef public list heap

    def __init__(self):
        self.heap = []

    # Reconstruir el árbol desde el header
    cpdef NodoHuffman _construir_arbol(self, dict frecuencias):
        cdef object caracter
        cdef int freq
        cdef NodoHuffman nodo, nodo1, nodo2, combinado

        for caracter, freq in frecuencias.items():
            nodo = NodoHuffman(caracter, freq)
            heapq.heappush(self.heap, nodo)

        while len(self.heap) > 1:
            nodo1 = heapq.heappop(self.heap)
            nodo2 = heapq.heappop(self.heap)
            combinado = NodoHuffman(None, nodo1.freq + nodo2.freq)
            combinado.left = nodo1
            combinado.right = nodo2
            heapq.heappush(self.heap, combinado)

        if self.heap:
            return self.heap[0]
        return None

    cpdef str _remover_padding(self, str padded_encoded_text):
        cdef str padded_info = padded_encoded_text[:8]
        cdef int extra_padding = int(padded_info, 2)
        cdef str texto_codificado = padded_encoded_text[8:]
        
        if extra_padding > 0:
            texto_codificado = texto_codificado[:-extra_padding]
        return texto_codificado

    cpdef void descomprimir(self, str ruta_comprimido, str ruta_salida):
        cdef bytes header_size_bytes, header_bytes, byte
        cdef int header_size, byte_val
        cdef str header_json, texto_bits, texto_codificado, bit, resultado
        cdef dict frecuencias
        cdef NodoHuffman raiz_arbol, nodo_actual
        cdef list bits = []
        cdef list texto_decodificado = []

        with open(ruta_comprimido, "rb") as f:
            # LEER HEADER
            header_size_bytes = f.read(4)
            header_size = int.from_bytes(header_size_bytes, byteorder="big")
            
            header_bytes = f.read(header_size)
            header_json = header_bytes.decode("utf-8")
            frecuencias = json.loads(header_json)
            
            # RECONSTRUIR ÁRBOL
            raiz_arbol = self._construir_arbol(frecuencias)
            
            # LEER DATOS COMPRIMIDOS
            byte = f.read(1)
            while byte:
                byte_val = ord(byte)
                bits.append("{0:08b}".format(byte_val))
                byte = f.read(1)

        texto_bits = "".join(bits)
        texto_codificado = self._remover_padding(texto_bits)
        nodo_actual = raiz_arbol

        for bit in texto_codificado:
            if bit == "0":
                nodo_actual = nodo_actual.left
            else:
                nodo_actual = nodo_actual.right

            if nodo_actual.char is not None:
                texto_decodificado.append(nodo_actual.char)
                nodo_actual = raiz_arbol

        resultado = "".join(texto_decodificado)
        with open(ruta_salida, "w", encoding="utf-8") as salida:
            salida.write(resultado)

        print(f"\n¡Éxito! Archivo descomprimido guardado como: {ruta_salida}")

if __name__ == "__main__":
    print("--- PROGRAMA DESCOMPRESOR DE HUFFMAN ---")
    archivo_entrada = input("Ingresa el archivo comprimido a abrir (ej. T7comprimido.mizip): ")
    archivo_salida = input("Ingresa el nombre para el archivo recuperado (ej. T7recuperado.txt): ")

    descompresor = DescompresorHuffman()
    descompresor.descomprimir(archivo_entrada, archivo_salida)
