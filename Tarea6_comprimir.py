import heapq
import json
from collections import Counter

class NodoHuffman:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class CompresorHuffman:
    def __init__(self):
        self.heap = []
        self.codigos = {}
        self.inverso_codigos = {}

    def _calcular_frecuencias(self, texto):
        return Counter(texto)

    def _construir_arbol(self, frecuencias):
        for char, freq in frecuencias.items():
            nodo = NodoHuffman(char, freq)
            heapq.heappush(self.heap, nodo)

        while len(self.heap) > 1:
            nodo1 = heapq.heappop(self.heap)
            nodo2 = heapq.heappop(self.heap)
            combinado = NodoHuffman(None, nodo1.freq + nodo2.freq)
            combinado.left = nodo1
            combinado.right = nodo2
            heapq.heappush(self.heap, combinado)

        return self.heap[0] if self.heap else None

    def _generar_codigos_rec(self, nodo, codigo_actual):
        if nodo is None:
            return
        if nodo.char is not None:
            self.codigos[nodo.char] = codigo_actual
            self.inverso_codigos[codigo_actual] = nodo.char
            return
        self._generar_codigos_rec(nodo.left, codigo_actual + "0")
        self._generar_codigos_rec(nodo.right, codigo_actual + "1")

    def _pad_encoded_text(self, encoded_text):
        extra_padding = 8 - len(encoded_text) % 8
        encoded_text += "0" * extra_padding
        padded_info = "{0:08b}".format(extra_padding)
        return padded_info + encoded_text

    def _get_byte_array(self, padded_encoded_text):
        if len(padded_encoded_text) % 8 != 0:
            raise ValueError("El texto codificado no está alineado a bytes.")
        b = bytearray()
        for i in range(0, len(padded_encoded_text), 8):
            byte = padded_encoded_text[i : i + 8]
            b.append(int(byte, 2))
        return b

    def comprimir(self, ruta_archivo, ruta_salida):
        with open(ruta_archivo, "r", encoding="utf-8") as f:
            texto = f.read()

        frecuencias = self._calcular_frecuencias(texto)
        raiz = self._construir_arbol(frecuencias)
        self._generar_codigos_rec(raiz, "")

        texto_codificado = "".join([self.codigos[char] for char in texto])
        texto_relleno = self._pad_encoded_text(texto_codificado)
        bytes_salida = self._get_byte_array(texto_relleno)

        # HEADER
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
    archivo_entrada = input("Ingresa el nombre del archivo de texto a comprimir (ej. T6ejemplo.txt): ")
    archivo_salida = input("Ingresa el nombre para tu archivo comprimido (ej. T6comprimido.mizip): ")
    
    compresor = CompresorHuffman()
    compresor.comprimir(archivo_entrada, archivo_salida)
