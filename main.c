//Realizado por: Juan David Torres Albarracín - 202317608
//Camilo Puerto Peña - 202211362
#include <stdio.h>
#include <stdlib.h>

void procesarImagen(unsigned char *imagenEntrada, unsigned char *imagenSalida, int n, int k);
void insertarComponente(unsigned char *imagenSalida, unsigned char componente, int k, int numComponente);

int main() {
    int n, k;

    // Solicitar tamaño de la imagen y valor de k
    printf("Ingrese el tamaño de la imagen en pixeles: ");
    scanf("%d", &n);
    printf("Ingrese el valor de k (profundidad de color deseada para cada componente en la salida): ");
    scanf("%d", &k);

    // Calcular el tamaño necesario para los vectores de imagen de salida
    int sizeOut = n * 3 * k / 8;
    if (n * 3 * k % 8 != 0) {
        sizeOut++; // Ajustar el tamaño para acomodar los bits adicionales
    }

    // Crear vectores dinámicos para almacenar la imagen de entrada y salida
    unsigned char *imagenEntrada = (unsigned char *)calloc(k*3 ,sizeof(unsigned char));
    unsigned char *imagenSalida = (unsigned char *)calloc(sizeOut, sizeof(unsigned char));

    // Leer la imagen de entrada
    printf("Ingrese los valores hexadecimales de cada componente (con espacios entre cada par de dígitos. Ej: A0 85 7B):\n");
    for (int i = 0; i < n * 3; i++) {
        scanf("%2hhX", &imagenEntrada[i]);
        getchar();
    }

    // Procesar la imagen
    procesarImagen(imagenEntrada, imagenSalida, n, k);

    // Imprimir la imagen de salida en hexadecimal
    printf("Imagen de salida en hexadecimal:\n");
    for (int i = 0; i < sizeOut; i++) {
        printf("%02X ", imagenSalida[i]);
    }
    printf("\n");

    return 0;
}

void procesarImagen(unsigned char *imagenEntrada, unsigned char *imagenSalida, int n, int k) {
    for (int i = 0; i < n * 3; i++) {
        insertarComponente(imagenSalida, imagenEntrada[i], k, i);
    }
}

void insertarComponente(unsigned char *imagenSalida, unsigned char componenteColor, int k, int numComponente) {
    __asm {
        mov ebx, k              // Cargar k en EBX
        mov eax, numComponente  // Cargar numComponente en ECX
        mov esi, 8
        // Calcular indiceByteOut = (numComponente * k) / 8
        imul eax, ebx             // ECX = numComponente * k
		cdq
		idiv esi // EAX = indiceByteOut; EDX = bitsCorrimiento
        
		mov bl, componenteColor
		mov ecx, 8
		mov esi, k
		sub ecx, esi	//8-k
		//mov cl,ch
		shr bl,cl //en bl se guarda char value = (componenteColor >> (8 - k));
        
        // Comprobar si (bitsCorrimiento + k) <= 8
        add esi, edx               // ESI = k+ bitsCorrimiento
        cmp esi, 8                 // Comparar con 8
        jbe no_overflow            // Saltar si no hay desbordamiento
        
        // Caso de desbordamiento
        mov ecx, k
		mov esi, 8
		sub esi, edx //8-bitsCorrimiento
		sub ecx,esi //k-(8-bitsCorrimiento)
		mov dl, bl //mueve value a dl
		//mov cl,ch
		shr dl, cl //value >> (k-(8-bitsCorrimiento))
		add eax, imagenSalida //Va a la posición a modificar
		or [eax], dl
		
		add eax, 1 //avanza a la siguiente posición del vector
		mov ecx, 8
		mov edi, k
		sub ecx,edi //8-k
		add ecx, esi //(8 - k + (8-bitsCorrimiento))
		//mov cl,ch
		shl bl,cl //value << (8 - k + (8-bitsCorrimiento))
		or [eax],bl
        jmp end_                   // Saltar al final
        
    no_overflow:
        // Caso sin desbordamiento
        // Almacenar el valor en imagenSalida[indiceByteOut]
        mov ecx, 8
		mov edi, k
		sub ecx, edi //8-k
		sub ecx, edx //8-k-bitsCorrimiento
		//mov cl,ch
		shl bl, cl //value << (8 - k - bitsCorrimiento)
		add eax, imagenSalida //Va a la posición a modificar
		or [eax],bl
        
    end_:
    }
}











