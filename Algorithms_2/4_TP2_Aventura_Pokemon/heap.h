#include <stddef.h>

const static int PRIMERO_MENOR = 1, SEGUNDO_MENOR = 2;

/* 
 * El comparador debe ser implementado por el usuario del TDA, asegurándose que cuando el primer elemento recibido por parámetro es menor que el segundo
 * se devuelva PRIMERO_MENOR, en caso contrario se devuelve SEGUNDO_MENOR.
 */
typedef int (*comparador_elementos)(void*, void*);

/*
 * El destructor es un parámetro "opcional" para la creación y funcionamiento interno del heap, de recibirse NULL en lugar de un destructor el heap
 * no hará nada sobre el elemento que se elimina (es el caso donde se ingresan punteros a cosas que están en el stack). Su función es destruir los
 * elementos almacenados en el heap al momento de liberar toda la estructura
 */
typedef void (*destructor_elementos)(void*);

typedef struct heap {
	void** vector_elementos;
	size_t tope;
	comparador_elementos comparador;
	destructor_elementos destructor;
} heap_t;

/* 
 * Pre-condiciones: comparador debe ser no nulo y debe devolver uno de los dos valores válidos cuando sea llamado (PRIMERO_MENOR ó SEGUNDO_MENOR)
 * Post-condiciones: devuelve un puntero a un heap minimal alojado en memoria dinámica, con el campo vector inicializado en NULL, el tope en cero y
 * 		el comparador siendo el comparador ingresado. De ocurrir un error se devuelve NULL
 */
heap_t* heap_crear(comparador_elementos comparador, destructor_elementos destructor);

/* 
 * Pre-condiciones: heap debe ser no nulo, el comparador del heap debe ser no nulo
 * Post-condiciones: devuelve 0 (cero) si pudo insertar un elemento correctamente manteniendo válida la estructura de dato heap minimal, redimensionando
 * 		 el vector acorde a la cantidad de elementos en el vector del heap en la memoria dinámica y aumenta en uno el tope. De ocurrir un error se
 * 		 devuelve -1 (uno negativo)
 */
int heap_insertar(heap_t* heap, void* elemento);

/* 
 * Pre-condiciones: heap debe ser no nulo, el comparador del heap debe ser no nulo
 * Post-condiciones: devuelve el puntero al elemento ubicado en la raíz del heap minimal, es decir, el elemento con menor clave según lo determinado
 * 		por las comparaciones del comparador, elimina dicho elemento del heap recibido, reemplazándolo por el siguiente elemento correcto para mantener
 * 		válida la estructura heap, redimensiona el vector de elementos acorde a la cantidad de elementos restantes y disminuye en uno el tope (si no es
 * 		cero). De ocurrir un error o que el tope inicial sea cero (no hay elementos para eliminar) se devuelve NULL dejando todo como estaba en el
 * 		estado inicial
 */
void* heap_extraer_elemento_raiz(heap_t* heap);

/* 
 * Pre-condiciones: heap debe ser no nulo
 * Post-condiciones: devuelve un puntero al elemento ubicado en la raíz, pero no lo elimina de la estructura, devuelve NULL si el heap es nulo o si no
 * 		posee elementos almacenados
 */
void* heap_elemento_raiz(heap_t* heap);

/* 
 * Post-condiciones: devuelve la cantidad de elementos existentes en el heap, si el heap es nulo se devuelve cero
 */
size_t heap_cantidad(heap_t* heap);

/* 
 * Pre-condiciones: el heap no debe haber sido destruido previamente
 * Post-condiciones: el heap y su vector de elementos se encuentran destruidos y liberados, no se pueden volver a utilizar sin crear un nuevo heap
 */
void heap_destruir(heap_t* heap);