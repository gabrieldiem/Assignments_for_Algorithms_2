#include "abb.h"
#include <stdbool.h>
#include <stdlib.h>

const static int EXITO = 0, ERROR = -1;
const static char TENGO_IZQUIERDA = 'I', TENGO_DERECHA = 'D';
const static bool ESTA_VACIO = true, NO_ESTA_VACIO = false;
const static bool PARAR_RECORRIDO = true, SEGUIR_RECORRIENDO = false;

typedef struct auxiliares_almacenar_en_array{
	size_t* contador_interno;
	void** array;
	size_t tamanio_array;
} aux_save_in_array_t;

/* 
 * Pre-condiciones: nodo_raiz no debe ser nulo, nodo_a_borrar no debe ser nulo
 * Pos-condiciones: devuelve true si nodo_a_borrar es el mismo que nodo_raiz o false de otra manera
 */
bool es_raiz(nodo_abb_t* nodo_raiz, nodo_abb_t* nodo_a_borrar){
	return (nodo_raiz == nodo_a_borrar);
}

/* 
 * Pre-condiciones: nodo no debe ser nulo
 * Pos-condiciones: devuelve true si nodo tiene hijo izquierdo o false de otra manera
 */
bool hay_izquierda(nodo_abb_t* nodo){
	return (nodo->izquierda);
}

/* 
 * Pre-condiciones: nodo no debe ser nulo 
 * Pos-condiciones: devuelve true si nodo tiene hijo derecho o false de otra manera
 */
bool hay_derecha(nodo_abb_t* nodo){
	return (nodo->derecha);
}

/* 
 * Pre-condiciones: nodo no debe ser nulo
 * Pos-condiciones: devuelve true si el nodo es una hoja, es decir, si no tiene hijo izquierdo ni hijo derecho o false de otra manera
 */
bool es_hoja(nodo_abb_t* nodo){
	return ( !hay_izquierda(nodo) && !hay_derecha(nodo) );
}

/* 
 * Pre-condiciones: nodo no debe ser nulo
 * Pos-condiciones: devuelve true si nodo tiene un único hijo solamente a la izquierda o false de otra manera
 */
bool tiene_nodo_solo_izquierda(nodo_abb_t* nodo){
	return ( hay_izquierda(nodo) && !hay_derecha(nodo) );
}

/* 
 * Pre-condiciones: nodo no debe ser nulo
 * Pos-condiciones: devuelve true si nodo tiene un único hijo solamente a la derecha o false de otra manera
 */
bool tiene_nodo_solo_derecha(nodo_abb_t* nodo){
	return ( !hay_izquierda(nodo) && hay_derecha(nodo) );
}

/* 
 * Pre-condiciones:
 * Pos-condiciones: devuelve EXITO si cumple con que el árbol sea no nulo y que el comparador sea no nulo, o ERROR si uno de los dos es nulo 
 */
int abb_casos_erroneos_probar(abb_t* arbol){

	if(!arbol)
		return ERROR;

	if(!arbol->comparador)
		return ERROR;

	return EXITO;
}

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

	if(!comparador)
		return NULL;

	abb_t* abb = calloc(1, sizeof(abb_t));
	if(!abb)
		return NULL;

	abb->comparador = comparador;
	abb->destructor = destructor;

	return abb;
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Pos-condiciones: devuelve un nodo con el elemento recibido como parámetro en el campo elemento y el resto de los campos nulos o NULL si falla la
 	alocación de memoria, se encuentra alojado en la memoria dinámica
 */
nodo_abb_t* nodo_crear(void* elemento){

	nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));
	if(!nodo)
		return NULL;

	nodo->elemento = elemento;

	return nodo;
}

/* 
 * Pre-condiciones: abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, nodo_nuevo no puede ser nulo
 * Pos-condiciones: nodo_nuevo es insertado en la posición correspondiente manteniendo la estructura ABB válida
 */
void insertar_recursivo(abb_t* arbol, nodo_abb_t** nodo_actual, nodo_abb_t* nodo_nuevo){

	if( !(*nodo_actual) ){
		*nodo_actual = nodo_nuevo;
		return;
	}

	if(arbol->comparador((*nodo_actual)->elemento, nodo_nuevo->elemento) < 0)
		insertar_recursivo(arbol, &((*nodo_actual)->derecha), nodo_nuevo);
	else
		insertar_recursivo(arbol, &((*nodo_actual)->izquierda), nodo_nuevo);
}

/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */
int arbol_insertar(abb_t* arbol, void* elemento){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if(estado_pruebas == ERROR)
		return ERROR;

	nodo_abb_t* nodo_nuevo = nodo_crear(elemento);
	if(!nodo_nuevo)
		return ERROR;

	if(!arbol->nodo_raiz)
		arbol->nodo_raiz = nodo_nuevo;
	else
		insertar_recursivo(arbol, &(arbol->nodo_raiz), nodo_nuevo);

	return EXITO;
}

/* 
 * Pre-condiciones: el comparador no debe ser nulo, abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función
 * Pos-condiciones: "nodo" es el primer nodo que contiene el elemento "elemento" respetando el criterio del comparador o NULL si este no existe,
 	 nodo_padre es el nodo padre de "nodo" o NULL si no existe
 */
nodo_abb_t* nodo_buscar_y_retener_padre(nodo_abb_t* nodo, nodo_abb_t** nodo_padre, void* elemento, abb_comparador comparador){

	if(!nodo)
		return NULL;

	if( comparador(nodo->elemento, elemento) > 0 ){
		*nodo_padre = nodo;
		nodo = nodo_buscar_y_retener_padre(nodo->izquierda, nodo_padre, elemento, comparador);
	}
	else if( comparador(nodo->elemento, elemento) < 0 ){
		*nodo_padre = nodo;
		nodo = nodo_buscar_y_retener_padre(nodo->derecha, nodo_padre, elemento, comparador);	
	}

	return nodo;
}

/* 
 * Pre-condiciones: nodo no debe ser nulo, stdlib.h incluida, abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función
 * Pos-condiciones: al elemento de nodo se le es aplicado el destructor de existir, nodo es liberado del heap
 */
void borrar_hoja(nodo_abb_t** nodo, nodo_abb_t* nodo_padre, abb_liberar_elemento destructor){

	if(destructor)
		destructor((*nodo)->elemento);

	if(nodo_padre){
		if( nodo_padre->izquierda == *nodo)
			nodo_padre->izquierda = NULL;
		else
			nodo_padre->derecha = NULL;
	}
	
	free((*nodo));
}

/* 
 * Pre-condiciones: la raíz del árbol no debe ser nula, abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función
 * Pos-condiciones: al elemento de la raíz se le es aplicado el destructor de existir, la raíz es liberada del heap y el campo nodo_raiz del árbol pasa a ser
 	 NULL
 */
void borrar_raiz_hoja(abb_t* arbol){

	borrar_hoja(&(arbol->nodo_raiz), NULL, arbol->destructor);
	arbol->nodo_raiz = NULL;
}

/* 
 * Pre-condiciones:  abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, stdlib.h incluida, lado_hijo_existente debe
 	ser uno de los dos valores válidos ('I' ó 'D'), nodo no debe ser nulo y debe tener un sólo hijo
 * Pos-condiciones: al elemento de nodo se le es aplicado el destructor de existir, nodo es liberado del heap, el único nodo hijo toma el lugar del nodo que se
 	deseaba borrar con su elemento y sus hijos
 */
void borrar_con_1_hijo(nodo_abb_t* nodo, abb_liberar_elemento destructor, char lado_hijo_existente){

	nodo_abb_t* nodo_reemplazo = NULL;

	if(destructor)
		destructor(nodo->elemento);

	if(lado_hijo_existente == TENGO_IZQUIERDA)
		nodo_reemplazo = nodo->izquierda;
	else
		nodo_reemplazo = nodo->derecha;

	nodo->elemento = nodo_reemplazo->elemento;
	nodo->izquierda = nodo_reemplazo->izquierda;
	nodo->derecha = nodo_reemplazo->derecha;

	free(nodo_reemplazo);
}

/* 
 * Pre-condiciones: nodo no debe ser nulo y debe ser el hijo izquierdo de su padre, abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de
 	 llamar a esta función
 * Pos-condiciones: devuelve el nodo con la mayor clave de la rama menor al nodo padre de "nodo"
 */
nodo_abb_t* obtener_nodo_mayor_de_menores(nodo_abb_t* nodo, nodo_abb_t** nodo_mayor_de_menores_padre){

	if(!nodo->derecha)
		return nodo;

	*nodo_mayor_de_menores_padre = nodo;
	nodo = obtener_nodo_mayor_de_menores(nodo->derecha, nodo_mayor_de_menores_padre);

	return nodo;
}

/* 
 * Pre-condiciones:  abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, nodo no debe ser nulo y debe tener dos hijos,
 	stdlib.h incluida
 * Pos-condiciones: tomando el nodo con la mayor clave de las menores se elije el nodo cuya clave que tomará el lugar del nodo a borrar, al elemento del
 	nodo a borrar se le es aplicado el destructor de existir, se libera el nodo mayor de menores y se realizan las reasignaciones de punteros necesarias
 	para un borrado exitoso y mantener válida la estructura ABB
 */
void borrar_con_2_hijos(nodo_abb_t* nodo, abb_liberar_elemento destructor){

	nodo_abb_t* nodo_mayor_de_menores_padre = nodo;
	nodo_abb_t* nodo_mayor_de_menores = obtener_nodo_mayor_de_menores(nodo->izquierda, &nodo_mayor_de_menores_padre);
	
	if(destructor)
		destructor(nodo->elemento);

	nodo->elemento = nodo_mayor_de_menores->elemento;
	
	if(nodo_mayor_de_menores_padre != nodo)
		nodo_mayor_de_menores_padre->derecha = nodo_mayor_de_menores->izquierda;
	else
		nodo->izquierda = nodo_mayor_de_menores->izquierda;

	free(nodo_mayor_de_menores);
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) )
		return ERROR;

	nodo_abb_t* nodo_padre = NULL;
	nodo_abb_t* nodo_a_borrar = nodo_buscar_y_retener_padre(arbol->nodo_raiz, &nodo_padre, elemento, arbol->comparador);
	if(!nodo_a_borrar)
		return ERROR;

	if( es_hoja(nodo_a_borrar) ){

		if( es_raiz(arbol->nodo_raiz, nodo_a_borrar) )
			borrar_raiz_hoja(arbol);
		else
			borrar_hoja(&nodo_a_borrar, nodo_padre, arbol->destructor);
	}
	else if( tiene_nodo_solo_izquierda(nodo_a_borrar) ){
		borrar_con_1_hijo(nodo_a_borrar, arbol->destructor, TENGO_IZQUIERDA);
	}
	else if( tiene_nodo_solo_derecha(nodo_a_borrar) ){
		borrar_con_1_hijo(nodo_a_borrar, arbol->destructor, TENGO_DERECHA);
	}
	else{
		borrar_con_2_hijos(nodo_a_borrar, arbol->destructor);
	}

	return EXITO;
}

/* 
 * Pre-condiciones: abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, nodo_nuevo no puede ser nulo
 * Pos-condiciones: retorna el primer nodo que contiene el elemento "elemento" valíendose del comparador o NULL si no existe dicho nodo
 */
nodo_abb_t* nodo_buscar(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){

	if(!nodo)
		return NULL;

	if( comparador(nodo->elemento, elemento) > 0 ){
		nodo = nodo_buscar(nodo->izquierda, elemento, comparador);
	}
	else if( comparador(nodo->elemento, elemento) < 0 ){
		nodo = nodo_buscar(nodo->derecha, elemento, comparador);	
	}

	return nodo;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) )
		return NULL;

	nodo_abb_t* nodo_a_encontrar = nodo_buscar(arbol->nodo_raiz, elemento, arbol->comparador);
	if(!nodo_a_encontrar)
		return NULL;

	return (nodo_a_encontrar->elemento);
}

/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){

	if( (!arbol) )
		return NULL;

	if( !(arbol->nodo_raiz) )
		return NULL;

	return (arbol->nodo_raiz->elemento);
}

/*
 * Determina si el árbol está vacío. 
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){

	if( (!arbol) || (!arbol->nodo_raiz) )
		return ESTA_VACIO;

	return NO_ESTA_VACIO;
}

/* 
 * Pre-condiciones:  abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, función no debe ser nula, la raíz del árbol
 	no debe ser nula
 * Pos-condiciones: recorre el arbol con un criterio de tipo 'inorden' e invoca la funcion con cada elemento del mismo. El puntero 'extra' se pasa
  	como segundo parámetro a la función. Si la función devuelve true, se finaliza el recorrido aun si quedan elementos por recorrer. Si devuelve false
  	se sigue recorriendo mientras queden elementos. Al finalizar cant_actual contiene la cantidad de elementos que fueron recorridos
 */
void abb_con_cada_elemento_inorden(nodo_abb_t* nodo, bool* debo_seguir, size_t* cant_actual, bool (*funcion)(void*, void*), void* extra){

	if( (!nodo) || (*debo_seguir == PARAR_RECORRIDO) )
		return;

	if(nodo->izquierda)
		abb_con_cada_elemento_inorden(nodo->izquierda, debo_seguir, cant_actual, funcion, extra);

	if(*debo_seguir == SEGUIR_RECORRIENDO){
		*debo_seguir = funcion(nodo->elemento, extra);
		(*cant_actual)++;
	}

	if(nodo->derecha)
		abb_con_cada_elemento_inorden(nodo->derecha, debo_seguir, cant_actual, funcion, extra);
}

/* 
 * Pre-condiciones:  abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, función no debe ser nula, la raíz del árbol
 	no debe ser nula
 * Pos-condiciones: recorre el arbol con un criterio de tipo 'preorden' e invoca la funcion con cada elemento del mismo. El puntero 'extra' se pasa
  	como segundo parámetro a la función. Si la función devuelve true, se finaliza el recorrido aun si quedan elementos por recorrer. Si devuelve false
  	se sigue recorriendo mientras queden elementos. Al finalizar cant_actual contiene la cantidad de elementos que fueron recorridos
 */
void abb_con_cada_elemento_preorden(nodo_abb_t* nodo, bool* debo_seguir, size_t* cant_actual, bool (*funcion)(void*, void*), void* extra){

	if( (!nodo) || (*debo_seguir == PARAR_RECORRIDO) )
		return;

	if(*debo_seguir == SEGUIR_RECORRIENDO){
		*debo_seguir = funcion(nodo->elemento, extra);
		(*cant_actual)++;
	}

	if(nodo->izquierda)
		abb_con_cada_elemento_preorden(nodo->izquierda, debo_seguir, cant_actual, funcion, extra);

	if(nodo->derecha)
		abb_con_cada_elemento_preorden(nodo->derecha, debo_seguir, cant_actual, funcion, extra);
}

/* 
 * Pre-condiciones:  abb_casos_erroneos_probar tuvo que haber devuelto EXITO antes de llamar a esta función, función no debe ser nula, la raíz del árbol
 	no debe ser nula
 * Pos-condiciones: recorre el arbol con un criterio de tipo 'postorden' e invoca la funcion con cada elemento del mismo. El puntero 'extra' se pasa
  	como segundo parámetro a la función. Si la función devuelve true, se finaliza el recorrido aun si quedan elementos por recorrer. Si devuelve false
  	se sigue recorriendo mientras queden elementos. Al finalizar cant_actual contiene la cantidad de elementos que fueron recorridos
 */
void abb_con_cada_elemento_postorden(nodo_abb_t* nodo, bool* debo_seguir, size_t* cant_actual, bool (*funcion)(void*, void*), void* extra){

	if( (!nodo) || (*debo_seguir == PARAR_RECORRIDO) )
		return;

	if(nodo->izquierda)
		abb_con_cada_elemento_postorden(nodo->izquierda, debo_seguir, cant_actual, funcion, extra);

	if(nodo->derecha)
		abb_con_cada_elemento_postorden(nodo->derecha, debo_seguir, cant_actual, funcion, extra);
	
	if(*debo_seguir == SEGUIR_RECORRIENDO){
		*debo_seguir = funcion(nodo->elemento, extra);
		(*cant_actual)++;
	}
}

/* 
 * Pre-condiciones: contador_interno no debe ser nulo, array no debe ser nulo, tamanio_array debe representar el tope del array, el array debe poseer al menos
 	una posición disponible para cargar
 * Pos-condiciones: se cargar el elemento "elemento" en la primera posición disponible del array "array", se aumenta en uno el contador_interno
 */
void cargar_en_array(void* elemento, size_t* contador_interno, void** array, size_t tamanio_array){

	if( (*contador_interno) < tamanio_array ){
		array[*contador_interno] = elemento;
		(*contador_interno)++;
	}
}

/* 
 * Pre-condiciones: auxiliares_almacenar_en_array debió haber sido inicializado con inicializar_auxiliares
 * Pos-condiciones: carga el elemento recibido en el campo array de auxiliares_almacenar_en_array, aumenta en uno el campo contador_interno, devuelve
 	PARAR_RECORRIDO si se comprueba que se llenó el array o devuelve SEGUIR_RECORRIENDO de otra manera
 */
bool almacenar_en_array(void* elemento, void* auxiliares_almacenar_en_array){

	aux_save_in_array_t auxiliares_actual = *(aux_save_in_array_t*)auxiliares_almacenar_en_array;

	size_t* contador_interno = auxiliares_actual.contador_interno;
	void** array = auxiliares_actual.array;
	size_t tamanio_array = auxiliares_actual.tamanio_array;

	cargar_en_array(elemento, contador_interno, array, tamanio_array);

	if( (*contador_interno) == tamanio_array )
		return PARAR_RECORRIDO;
	else
		return SEGUIR_RECORRIENDO;
}

/* 
 * Pre-condiciones:  aux_almacenar_en_array no debe ser nulo, array no debe ser nulo, contador_interno no debe ser nulo, tamanio_array debió haber sido 
 	inicializado en cero
 * Pos-condiciones: aux_almacenar_en_array contiene sus respectivos campos asignados a los datos recibidos como parámetro
 */
void inicializar_auxiliares(aux_save_in_array_t* aux_almacenar_en_array, size_t* contador_interno, void** array, size_t tamanio_array){

	aux_almacenar_en_array->contador_interno = contador_interno;
	aux_almacenar_en_array->array = array;
	aux_almacenar_en_array->tamanio_array = tamanio_array;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) )
		return 0;

	size_t cant_actual = 0, contador_interno = 0;
	bool debo_seguir = SEGUIR_RECORRIENDO;
	bool (*almacenar_en_array_ptr)(void*, void*) = &almacenar_en_array;

	aux_save_in_array_t aux_almacenar_en_array;
	inicializar_auxiliares(&aux_almacenar_en_array, &contador_interno, array, tamanio_array);

	abb_con_cada_elemento_inorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, almacenar_en_array_ptr, (void*)&aux_almacenar_en_array);

	return cant_actual;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) )
		return 0;

	size_t cant_actual = 0, contador_interno = 0;
	bool debo_seguir = SEGUIR_RECORRIENDO;
	bool (*almacenar_en_array_ptr)(void*, void*) = &almacenar_en_array;

	aux_save_in_array_t aux_almacenar_en_array;
	inicializar_auxiliares(&aux_almacenar_en_array, &contador_interno, array, tamanio_array);

	abb_con_cada_elemento_preorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, almacenar_en_array_ptr, (void*)&aux_almacenar_en_array);

	return cant_actual;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) )
		return 0;

	size_t cant_actual = 0, contador_interno = 0;
	bool debo_seguir = SEGUIR_RECORRIENDO;
	bool (*almacenar_en_array_ptr)(void*, void*) = &almacenar_en_array;

	aux_save_in_array_t aux_almacenar_en_array;
	inicializar_auxiliares(&aux_almacenar_en_array, &contador_interno, array, tamanio_array);

	abb_con_cada_elemento_postorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, almacenar_en_array_ptr, (void*)&aux_almacenar_en_array);

	return cant_actual;
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Pos-condiciones: se le aplica el destructur, de existir, a cada elemento, recursivamente se recorre todo el árbol para liberar de la memoria dinámica
  	todos los nodos contenidos
 */
void destruir_nodos_recursivo(nodo_abb_t* nodo, abb_liberar_elemento destructor){

	if(!nodo)
		return;

	destruir_nodos_recursivo(nodo->izquierda, destructor);
	destruir_nodos_recursivo(nodo->derecha, destructor);

	if(destructor)
		destructor(nodo->elemento);

	free(nodo);
}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){

	if(!arbol)
		return;

	if(arbol->nodo_raiz)
		destruir_nodos_recursivo(arbol->nodo_raiz, arbol->destructor);

	free(arbol);
}

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){

	int estado_pruebas = abb_casos_erroneos_probar(arbol);
	if( (estado_pruebas == ERROR) || (!arbol->nodo_raiz) || (!funcion) )
		return 0;

	size_t cant_actual = 0;
	bool debo_seguir = SEGUIR_RECORRIENDO;

	if(recorrido == ABB_RECORRER_INORDEN){
		abb_con_cada_elemento_inorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, funcion, extra);
	}
	else if(recorrido == ABB_RECORRER_PREORDEN){
		abb_con_cada_elemento_preorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, funcion, extra);
	}
	else if(recorrido == ABB_RECORRER_POSTORDEN){
		abb_con_cada_elemento_postorden(arbol->nodo_raiz, &debo_seguir, &cant_actual, funcion, extra);
	}

	return cant_actual;
}