import heapq
import json

class NodoHuffman:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class DescompresorHuffman:
    def __init__(self):
        self.heap = []

    # Reconstruir el árbol desde el header
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

    def _remover_padding(self, padded_encoded_text):
        padded_info = padded_encoded_text[:8]
        extra_padding = int(padded_info, 2)
        texto_codificado = padded_encoded_text[8:]
        texto_codificado = texto_codificado[:-extra_padding if extra_padding > 0 else None]
        return texto_codificado

    def descomprimir(self, ruta_comprimido, ruta_salida):
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
            bits = []
            byte = f.read(1)
            while byte:
                byte_val = ord(byte)
                bits.append("{0:08b}".format(byte_val))
                byte = f.read(1)

        texto_bits = "".join(bits)
        texto_codificado = self._remover_padding(texto_bits)

        texto_actual = ""
        texto_decodificado = []
        nodo_actual = raiz_arbol

        for bit in texto_codificado:
            texto_actual += bit
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
    archivo_entrada = input("Ingresa el archivo comprimido a abrir (ej. T6comprimido.mizip): ")
    archivo_salida = input("Ingresa el nombre para el archivo recuperado (ej. T6recuperado.txt): ")
    
    descompresor = DescompresorHuffman()
    descompresor.descomprimir(archivo_entrada, archivo_salida)
