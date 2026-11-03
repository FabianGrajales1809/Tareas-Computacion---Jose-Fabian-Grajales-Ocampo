import heapq
import json
from collections import Counter

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

cdef class CompresorHuffman:
    cdef public list heap
    cdef public dict codigos
    cdef public dict inverso_codigos

    def __init__(self):
        self.heap = []
        self.codigos = {}
        self.inverso_codigos = {}

    cpdef dict _calcular_frecuencias(self, str texto):
        return dict(Counter(texto))

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

    cpdef void _generar_codigos_rec(self, NodoHuffman nodo, str codigo_actual):
        if nodo is None:
            return
        if nodo.char is not None:
            self.codigos[nodo.char] = codigo_actual
            self.inverso_codigos[codigo_actual] = nodo.char
            return
        self._generar_codigos_rec(nodo.left, codigo_actual + "0")
        self._generar_codigos_rec(nodo.right, codigo_actual + "1")

    cpdef str _pad_encoded_text(self, str encoded_text):
        cdef int extra_padding = 8 - len(encoded_text) % 8
        encoded_text += "0" * extra_padding
        cdef str padded_info = "{0:08b}".format(extra_padding)
        return padded_info + encoded_text

    cpdef bytearray _get_byte_array(self, str padded_encoded_text):
        if len(padded_encoded_text) % 8 != 0:
            raise ValueError("El texto codificado no está alineado a bytes.")
        cdef bytearray b = bytearray()
        cdef int i
        cdef str byte_str
        for i in range(0, len(padded_encoded_text), 8):
            byte_str = padded_encoded_text[i : i + 8]
            b.append(int(byte_str, 2))
        return b

    cpdef void comprimir(self, str ruta_archivo, str ruta_salida):
        cdef str texto, texto_codificado, texto_relleno, caracter
        cdef dict frecuencias
        cdef NodoHuffman raiz
        cdef bytearray bytes_salida
        cdef bytes header_bytes, header_size
        cdef str header_json

        with open(ruta_archivo, "r", encoding="utf-8") as f:
            texto = f.read()

        frecuencias = self._calcular_frecuencias(texto)
        raiz = self._construir_arbol(frecuencias)
        self._generar_codigos_rec(raiz, "")

        texto_codificado = "".join([self.codigos[caracter] for caracter in texto])
        texto_relleno = self._pad_encoded_text(texto_codificado)
        bytes_salida = self._get_byte_array(texto_relleno)

        header_json = json.dumps(frecuencias)
        header_bytes = header_json.encode("utf-8")
        header_size = len(header_bytes).to_bytes(4, byteorder="big")

        with open(ruta_salida, "wb") as salida:
            salida.write(header_size)
            salida.write(header_bytes)
            salida.write(bytes_salida)

        print(f"\n¡Éxito! Archivo comprimido guardado como: {ruta_salida}")

if __name__ == "__main__":
    print("--- PROGRAMA COMPRESOR DE HUFFMAN ---")
    archivo_entrada = input("Ingresa el nombre del archivo de texto a comprimir (ej. T7ejemplo.txt): ")
    archivo_salida = input("Ingresa el nombre para tu archivo comprimido (ej. T7comprimido.mizip): ")
    
    compresor = CompresorHuffman()
    compresor.comprimir(archivo_entrada, archivo_salida)
