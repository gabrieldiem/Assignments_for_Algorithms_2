#include "heap.h"
#include <stdlib.h>

const static int EXITO = 0, ERROR = -1;
const static size_t POS_ELEMENTO_RAIZ = 0;

/* 
 * Pre-condiciones: comparador debe ser no nulo y debe devolver uno de los dos valores válidos cuando sea llamado (PRIMERO_MENOR ó SEGUNDO_MENOR)
 * Post-condiciones: devuelve un puntero a un heap minimal alojado en memoria dinámica, con el campo vector inicializado en NULL, el tope en cero y
 * 		el comparador siendo el comparador ingresado. De ocurrir un error se devuelve NULL
 */
heap_t* heap_crear(comparador_elementos comparador, destructor_elementos destructor) {
	if (!comparador)
		return NULL;

	heap_t* heap = calloc(1, sizeof(heap_t));
	if (!heap)
		return NULL;

	heap->comparador = comparador;
	heap->destructor = destructor;

	return heap;
}

/* 
 * Pre-condiciones: posicion debe ser distinto de cero
 * Post-condiciones: devuelve la posición del padre del elemento cuya posición se recibe como parámetro, si la posición recibida es cero se
 * 		 devuelve cero
 */
size_t posicion_padre(size_t posicion) {
	if (posicion == 0)
		return 0;

	return (posicion - 1) / 2;
}

/* 
 * Pre-condiciones:
 * Post-condiciones: se devuelve la posible posición del hijo derecho del elemento cuya posición se recibe como parámetro, puede que dicho elemento
 * 		no posea hijo derecho por lo cual la posición devuelta no tendría significado, se debe comprobar
 */
size_t posicion_hijo_derecho(size_t posicion) {
	return (2 * posicion) + 2;
}

/* 
 * Pre-condiciones:
 * Post-condiciones: se devuelve la posible posición del hijo izquierdo del elemento cuya posición se recibe como parámetro, puede que dicho elemento
 * 		no posea hijo izquierdo por lo cual la posición devuelta no tendría significado, se debe comprobar
 */
size_t posicion_hijo_izquierdo(size_t posicion) {
	return (2 * posicion) + 1;
}

/* 
 * Pre-condiciones: el vector debe ser no nulo, las posiciones deben ser válidas de manera que se puedan acceder como índices del vector
 * Post-condiciones: el elemento en la posicion_1 intercambia el lugar con el elemento en la posicion_2 del vector
 */
void intercambiar(void** vector, size_t posicion_1, size_t posicion_2) {
	if (!vector)
		return;

	void* elemento_aux = vector[posicion_1];
	vector[posicion_1] = vector[posicion_2];
	vector[posicion_2] = elemento_aux;
}

/* 
 * Pre-condiciones: el heap debe ser no nulo, pos_deseada debe ser una posicion válida, el comparador debe ser no nulo, el vector del heap debe ser
 * 		 no nulo
 * Post-condiciones: recursivamente hace "sift up" al elemento hasta que queda en la posición correcta para mantener la estructura heap minimal válida
 */
void sift_up(heap_t* heap, size_t pos_deseada) {
	if ((pos_deseada == 0) || !(heap->comparador) || !heap || (pos_deseada >= heap->tope))
		return;

	size_t pos_padre = posicion_padre(pos_deseada);

	if (heap->comparador(heap->vector_elementos[pos_deseada], heap->vector_elementos[pos_padre]) == PRIMERO_MENOR) {
		intercambiar(heap->vector_elementos, pos_deseada, pos_padre);
		sift_up(heap, pos_padre);
	}
}

/* 
 * Pre-condiciones: el heap debe ser no nulo, pos_deseada debe ser una posicion válida, el comparador debe ser no nulo, el vector del heap debe ser
 * 		 no nulo
 * Post-condiciones: recursivamente hace "sift down" al elemento hasta que queda en la posición correcta para mantener la estructura heap minimal válida
 */
void sift_down(heap_t* heap, size_t pos_deseada) {
	if (!heap || !(heap->comparador) || (pos_deseada == (heap->tope - 1)) || (pos_deseada >= heap->tope) || (heap->tope <= 1))
		return;

	size_t pos_hijo_derecho = posicion_hijo_derecho(pos_deseada);
	size_t pos_hijo_izquierdo = posicion_hijo_izquierdo(pos_deseada);
	size_t pos_hijo_menor = pos_hijo_izquierdo;

	if (pos_hijo_izquierdo >= heap->tope)
		return;

	if (pos_hijo_derecho < heap->tope) {
		if (heap->comparador(heap->vector_elementos[pos_hijo_izquierdo], heap->vector_elementos[pos_hijo_derecho]) == SEGUNDO_MENOR)
			pos_hijo_menor = pos_hijo_derecho;
	}

	if (heap->comparador(heap->vector_elementos[pos_deseada], heap->vector_elementos[pos_hijo_menor]) == SEGUNDO_MENOR) {
		intercambiar(heap->vector_elementos, pos_deseada, pos_hijo_menor);
		sift_down(heap, pos_hijo_menor);
	}
}

/* 
 * Pre-condiciones: heap debe ser no nulo, el comparador del heap debe ser no nulo
 * Post-condiciones: devuelve EXITO si pudo insertar un elemento correctamente manteniendo válida la estructura de dato heap minimal, redimensionando
 * 		 el vector acorde a la cantidad de elementos en el vector del heap en la memoria dinámica y aumenta en uno el tope, ó reservando memoria
 * 		 en caso de que se inserte el primer elemento del vector. De ocurrir un error se devuelve ERROR
 */
int heap_insertar(heap_t* heap, void* elemento) {
	if (!heap || !(heap->comparador))
		return ERROR;

	size_t nuevo_tope = (heap->tope) + 1;
	void** vector_aux;

	if (nuevo_tope == 1)
		vector_aux = calloc(1, sizeof(void**));
	else
		vector_aux = realloc(heap->vector_elementos, sizeof(void**) * nuevo_tope);

	if (!vector_aux)
		return ERROR;

	heap->tope = nuevo_tope;
	heap->vector_elementos = vector_aux;

	size_t pos_ultimo_elem = nuevo_tope - 1;
	(heap->vector_elementos)[pos_ultimo_elem] = elemento;
	sift_up(heap, pos_ultimo_elem);

	return EXITO;
}

/* 
 * Pre-condiciones: heap debe ser no nulo, el vector del heap debe ser no nulo, el tope actual debe representar la nueva cantidad deseada de elementos
 * Post-condiciones: devuelve EXITO si se puedo realocar el vector acorde a la nueva cantidad deseada representada en el tope, si el tope ingresa igual
 * 		a cero, se libera el vector y se iguala a NULL. De ocurrir un error se devuelve ERROR.
 */
int redimensionar_vector_heap(heap_t* heap) {
	if (!heap || !(heap->vector_elementos))
		return ERROR;

	if (heap->tope == 0) {
		free(heap->vector_elementos);
		heap->vector_elementos = NULL;
		return EXITO;
	}

	size_t cantidad = heap->tope;
	void** vector_aux = realloc(heap->vector_elementos, cantidad * sizeof(void**));
	if (!vector_aux)
		return ERROR;

	heap->vector_elementos = vector_aux;
	return EXITO;
}

/* 
 * Pre-condiciones: el redimensionamiento debió devolver ERROR para ingresar a la función, heap debe ser no nulo, el vector del heap debe ser no nulo,
 * 		el elemento elem_raiz_original debe representar al elemento que originalmente era la raíz del heap
 * Post-condiciones: el heap queda en el mismo estado que antes del intento de realizar la extracción
 */
void revertir_extraccion(heap_t* heap, void* elem_raiz_original) {
	size_t pos_ultimo_elem = heap->tope;
	(heap->tope)++;

	heap->vector_elementos[pos_ultimo_elem] = heap->vector_elementos[POS_ELEMENTO_RAIZ];
	heap->vector_elementos[POS_ELEMENTO_RAIZ] = elem_raiz_original;
}

/* 
 * Pre-condiciones: heap debe ser no nulo, el comparador del heap debe ser no nulo
 * Post-condiciones: devuelve el puntero al elemento ubicado en la raíz del heap minimal, es decir, el elemento con menor clave según lo determinado
 * 		por las comparaciones del comparador, elimina dicho elemento del heap recibido, reemplazándolo por el siguiente elemento correcto para mantener
 * 		válida la estructura heap, redimensiona el vector de elementos acorde a la cantidad de elementos restantes y disminuye en uno el tope (si no es
 * 		cero). De ocurrir un error o que el tope inicial sea cero (no hay elementos para eliminar) se devuelve NULL dejando todo como estaba en el
 * 		estado inicial
 */
void* heap_extraer_elemento_raiz(heap_t* heap) {
	if (!heap || (heap->tope == 0) || !(heap->comparador))
		return NULL;

	void* elemento_raiz = heap->vector_elementos[POS_ELEMENTO_RAIZ];
	(heap->tope)--;

	if (heap->tope > 0) {
		size_t pos_ultimo_elem = heap->tope;
		heap->vector_elementos[POS_ELEMENTO_RAIZ] = heap->vector_elementos[pos_ultimo_elem];
	}

	int resultado_redimensionar = redimensionar_vector_heap(heap);
	if (resultado_redimensionar == ERROR) {
		revertir_extraccion(heap, elemento_raiz);
		return NULL;
	}
	sift_down(heap, POS_ELEMENTO_RAIZ);

	return elemento_raiz;
}

/* 
 * Pre-condiciones: heap debe ser no nulo
 * Post-condiciones: devuelve un puntero al elemento ubicado en la raíz, pero no lo elimina de la estructura
 */
void* heap_elemento_raiz(heap_t* heap){
	if(!heap || (heap->tope == 0) || !(heap->vector_elementos))
		return NULL;
	
	return heap->vector_elementos[POS_ELEMENTO_RAIZ];
}

/* 
 * Pre-condiciones: 
 * Post-condiciones: devuelve la cantidad de elementos existentes en el heap, si el heap es nulo se devuelve cero
 */
size_t heap_cantidad(heap_t* heap){
	if(!heap)
		return 0;
	
	return (heap->tope);
}

/* 
 * Pre-condiciones: el heap no debe haber sido destruido previamente
 * Post-condiciones: el heap y su vector de elementos se encuentran destruidos y liberados, no se pueden volver a utilizar sin crear un nuevo heap
 */
void heap_destruir(heap_t* heap) {
	
	if (heap){
		for(size_t i = 0; i < heap->tope; i++){
			heap->destructor(heap->vector_elementos[i]);
		}
		free(heap->vector_elementos);
	}

	free(heap);
}