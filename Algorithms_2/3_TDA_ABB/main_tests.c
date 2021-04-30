#include <stdbool.h>
#include "abb.h"
#include "test_suite_gd.h"

const bool NO_FUE_DESTRUIDO = false, SI_FUE_DESTRUIDO = true;
const bool HUBO_UN_ERROR = false;
const bool ESTA_VACIO = true, NO_ESTA_VACIO = false;
const int EXITO = 0, ERROR = -1;
const int SON_IGUALES = 0, PRIMERO_MAYOR = 1, SEGUNDO_MAYOR = -1;
const int NUM_SERIE_INVALIDO = -1;
const char CONTENIDO_INVALIDO = '-', CONTENIDO_CAMBIADO = '~';
const int INORDEN = 0, PREORDEN = 1, POSTORDEN = 2;
const bool PARAR_RECORRIDO = true, SEGUIR_RECORRIENDO = false;

/*
 * De tipo "paquete_t" serán los elementos utilizados para realizar los tests
 */
typedef struct paquete{
	int num_serie;
	char contenido;
	bool fue_destruido;
} paquete_t;

typedef struct auxiliares_abb_con_cada_elemento{
	size_t* contador_interno;
	void** array;
	size_t tamanio_array;
} aux_abb_con_elemento_t;

/*
 * Compara num_serie de paquetes devolviendo PRIMERO_MAYOR si el primer paquete posee mayor num_serie mayor o SEGUNDO_MAYOR si el segundo es el de mayor
 * num_serie, o iguales si son iguales
 */
int comparador_paquetes(void* primer_paquete, void* segundo_paquete){

	paquete_t paquete_1 = *(paquete_t*)primer_paquete;
	paquete_t paquete_2 = *(paquete_t*)segundo_paquete;

	if( paquete_1.num_serie > paquete_2.num_serie )
		return PRIMERO_MAYOR;
	else if( paquete_2.num_serie > paquete_1.num_serie )
		return SEGUNDO_MAYOR;
	else
		return SON_IGUALES;
}

/*
 * La destrucción de un paquete consiste en cambiar el valor del campo booleano "fue_destruido" a SI_FUE_DESTRUIDO
 */
void destructor_paquetes(void* paquete){
	if(paquete){
		paquete_t* paquete_a_destruir = (paquete_t*)paquete;
		paquete_a_destruir->fue_destruido = SI_FUE_DESTRUIDO;
	}
}

/*
 * Al haber sido validado el comportamiento de la función arbol_insertar, esta función insertar_elementos se basa en ella para insertar una cantidad
 * "cantidad" de los paquetes recibidos como vector. Si ocurre un error durante la inserción es notificado por pantalla
 */
int insertar_elementos(abb_t* arbol, int cantidad, paquete_t* paquetes, int tope){

	int i = 0;
	int resultado_insercion = EXITO;

	while( (i < tope) && (i < cantidad) && (resultado_insercion == EXITO) ){
		resultado_insercion = arbol_insertar(arbol, paquetes+i);
		i++;
	}

	if(resultado_insercion == ERROR){
		arbol_destruir(arbol);
		tsgd_afirmar( HUBO_UN_ERROR , "FINALIZACIÓN REPENTINA DE EJECUCIÓN POR UN ERROR." );
		return ERROR;
	}

	return EXITO;
}

/*
 * Inicializa un vector de paquetes con "paquetes inexistentes", estos contienen un num_serie especial de valor NUM_SERIE_INVALIDO y un contenido especial
 * de valor CONTENIDO_INVALIDO, así son distinguibles en las pruebas de los otros paquetes.
 * Además asigna las direcciones de memoria de estos paquetes inexistentes a un vector de punteros de paquetes
 */
void inicializar_paquetes_inexistentes(paquete_t* punteros_paquetes[], paquete_t paquetes_inexistentes[], size_t tope){

	for(int i = 0; i < tope; i++){
		paquetes_inexistentes[i].num_serie = NUM_SERIE_INVALIDO;
		paquetes_inexistentes[i].contenido = CONTENIDO_INVALIDO;
	}

	for(int i = 0; i < tope; i++){
		punteros_paquetes[i] = &paquetes_inexistentes[i];
	}
}

/*
 * Dependiendo del tipo de recorrido recibido como parámetro, valida que el num_serie sea el esperado comparando con el vector correspondiente esperado.
 */
bool es_valido_num_serie(int num_serie_actual, size_t pos_actual, int recorrido){

	int num_series_esperados_inorden[16] =   {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int num_series_esperados_preorden[16] =  {8,4,2,1,3,6,5,7,12,10,9,11,14,13,15};
	int num_series_esperados_postorden[16] = {1,3,2,5,7,6,4,9,11,10,13,15,14,12,8};
	bool es_valido = false;

	if(pos_actual > 15)
		return false;

	if( (recorrido == INORDEN) && (num_serie_actual == num_series_esperados_inorden[pos_actual]) ){
		es_valido = true;
	}
	else if( (recorrido == PREORDEN) && (num_serie_actual == num_series_esperados_preorden[pos_actual]) ) {
		es_valido = true;
	}
	else if( (recorrido == POSTORDEN) && (num_serie_actual == num_series_esperados_postorden[pos_actual]) ){
		es_valido = true;
	}

	return es_valido;
}

/*
 * Dependiendo del tipo de recorrido recibido como parámetro, valida que el contenido sea el esperado comparando con el vector correspondiente esperado.
 */
bool es_valido_contenido(char contenido_actual, size_t pos_actual, int recorrido){
	
	char contenidos_esperados_inorden[15] =   {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'};
	char contenidos_esperados_preorden[15] =  {'H','D','B','A','C','F','E','G','L','J','I','K','N','M','O'};	
	char contenidos_esperados_postorden[15] = {'A','C','B','E','G','F','D','I','K','J','M','O','N','L','H'};
	bool es_valido = false;

	if(pos_actual > 15)
		return false;

	if( (recorrido == INORDEN) && (contenido_actual == contenidos_esperados_inorden[pos_actual]) ){
		es_valido = true;
	}
	else if( (recorrido == PREORDEN) && (contenido_actual == contenidos_esperados_preorden[pos_actual]) ){
		es_valido = true;
	}
	else if( (recorrido == POSTORDEN) && (contenido_actual == contenidos_esperados_postorden[pos_actual]) ){
		es_valido = true;
	}

	return es_valido;
}

/*
 * Devuelve true si el contenido_actual fue cambiado al CONTENIDO_CAMBIADO, los campos "relleno_basura" son para cumplir la definición de un puntero a función,
 * pero no cumplen ninguna función útil
 */
bool es_valido_contenido_modificado(char contenido_actual, size_t relleno_basura_1, int relleno_basura_2){
	return (contenido_actual == CONTENIDO_CAMBIADO);
}

/*
 * Dependiendo de la cantidad "cant_esperada_validos", contará una cierta cantidad de paquetes válidos que contiene el vector punteros_paquetes, devolverá
 * true si se encuentra exactamente la misma cantidad de paquetes válidos como la cantidad esperada o false de otra manera, recibe el recorrido para
 * utilizar los correspondientes vectores que contienen los resultados esperados para cada recorrido. Un paquete es válido cuando posee el num_serie y el
 * contenido esperado
 */
bool es_cantidad_validos_correcta(paquete_t** punteros_paquetes, size_t tope, size_t cant_esperada_validos, int recorrido, bool (*es_valido_contenido)(char, size_t, int) ){

	size_t contador_interno_validos = 0;
	size_t i = 0;
	bool es_finalmente_valido = true;
	bool debo_seguir_iterando = true;
	bool es_valido_paquete = false;

	while( (i < tope) && (debo_seguir_iterando == true) && (es_finalmente_valido == true) ){
		
		paquete_t* paquete_actual = (paquete_t*)punteros_paquetes[i];
		if(paquete_actual->num_serie == NUM_SERIE_INVALIDO)
			debo_seguir_iterando = false;

		if( contador_interno_validos > cant_esperada_validos )
			debo_seguir_iterando = false;

		if(debo_seguir_iterando && es_finalmente_valido)
			es_valido_paquete = es_valido_num_serie(paquete_actual->num_serie, i, recorrido) && es_valido_contenido(paquete_actual->contenido, i, recorrido);

		if( es_valido_paquete && es_finalmente_valido && debo_seguir_iterando )
			contador_interno_validos++;

		i++;
	}

	if( contador_interno_validos != cant_esperada_validos )
		es_finalmente_valido = false;

	return es_finalmente_valido;
}

/*
 * Carga el elemento "elemento" en el array recibido, aumenta en uno el contador_interno
 */
void cargar_en_array_para_tests(void* elemento, size_t* contador_interno, void** array, size_t tamanio_array){

	if( (*contador_interno) < tamanio_array ){
		array[*contador_interno] = elemento;
		(*contador_interno)++;
	}
}

/*
 * Carga el elemento recibido en el campo array, aumenta en uno el campo contador_interno, devuelve PARAR_RECORRIDO si
 * se comprueba que se llenó el array o devuelve SEGUIR_RECORRIENDO de otra manera
 */
bool almacenar_en_array_para_tests(void* elemento, void* auxiliares_abb){

	aux_abb_con_elemento_t auxiliares_actual = *(aux_abb_con_elemento_t*)auxiliares_abb;

	size_t* contador_interno = auxiliares_actual.contador_interno;
	void** array = auxiliares_actual.array;
	size_t tamanio_array = auxiliares_actual.tamanio_array;

	cargar_en_array_para_tests(elemento, contador_interno, array, tamanio_array);

	if( (*contador_interno) == tamanio_array )
		return PARAR_RECORRIDO;
	else
		return SEGUIR_RECORRIENDO;
}

/*
 * Inicializa auxiliares_abb con los elementos recibidos
 */
void inicializar_auxiliares_para_tests(aux_abb_con_elemento_t* auxiliares_abb, size_t* contador_interno, void** array, size_t tamanio_array){

	auxiliares_abb->contador_interno = contador_interno;
	auxiliares_abb->array = array;
	auxiliares_abb->tamanio_array = tamanio_array;
}

/*
 * Modifica el elemento asignandole a su contenido CONTENIDO_CAMBIADO
 */
void modificar_contenido(void* elemento){

	paquete_t* paquete_a_modificar = (paquete_t*)elemento;
	paquete_a_modificar->contenido = CONTENIDO_CAMBIADO;
}

/*
 * Modifica el elemento asignandole a su contenido CONTENIDO_CAMBIADO. Carga el elemento recibido en el campo array, aumenta en uno el campo contador_interno,
 * devuelve PARAR_RECORRIDO si se comprueba que se llenó el array o devuelve SEGUIR_RECORRIENDO de otra manera.
 */
bool modificar_contenido_y_cargar_array(void* elemento, void* aux_abb_con_elemento){

	aux_abb_con_elemento_t* auxiliares = (aux_abb_con_elemento_t*)aux_abb_con_elemento;

	modificar_contenido(elemento);

	return (almacenar_en_array_para_tests(elemento, (void*)auxiliares) == PARAR_RECORRIDO);
}




void tests_creacion(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes){

	abb_t* arbol = arbol_crear(comparador_paquetes, NULL);

	tsgd_afirmar( (arbol != NULL) && (arbol->comparador == comparador_paquetes) && (arbol->destructor == NULL), "Puedo crear un árbol con un comparador no nulo y un destructor nulo." );
	tsgd_afirmar( arbol->nodo_raiz == NULL, "El árbol tiene la raíz nula." );

	arbol_destruir(arbol);
	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);

	tsgd_afirmar( (arbol != NULL) && (arbol->comparador == comparador_paquetes) && (arbol->destructor == destructor_paquetes), "Puedo crear un árbol con un comparador no nulo y un destructor no nulo.");
	tsgd_afirmar( arbol->nodo_raiz == NULL, "El árbol tiene la raíz nula." );

	arbol_destruir(arbol);
	arbol = arbol_crear(NULL, destructor_paquetes);

	tsgd_afirmar( arbol == NULL , "No puedo crear un árbol con un comparador nulo." );

	arbol_destruir(arbol);
}

void tests_insercion(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	abb_t* arbol = NULL;
	tsgd_afirmar( arbol_insertar(arbol, paquetes) == ERROR, "No puedo insertar un elemento en un árbol nulo." );	

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);

	tsgd_afirmar( (arbol_insertar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz->elemento == paquetes) , "Puedo insertar el elemento 1 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz->izquierda->elemento == paquetes) , "Puedo insertar el elemento 1 de nuevo correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz->izquierda->izquierda->elemento == paquetes) , "Puedo insertar por tercera vez el elemento 1 de nuevo correctamente y está en la ubicación correcta." );

	arbol_destruir(arbol);
	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);

	tsgd_afirmar( (arbol_insertar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz->elemento == paquetes) , "Puedo insertar el elemento 1 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+1) == EXITO) && (arbol->nodo_raiz->izquierda->elemento == paquetes+1) , "Puedo insertar el elemento 2 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+2) == EXITO) && (arbol->nodo_raiz->izquierda->izquierda->elemento == paquetes+2) , "Puedo insertar el elemento 3 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+3) == EXITO) && (arbol->nodo_raiz->izquierda->izquierda->izquierda->elemento == paquetes+3) , "Puedo insertar el elemento 4 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+4) == EXITO) && (arbol->nodo_raiz->izquierda->izquierda->derecha->elemento == paquetes+4) , "Puedo insertar el elemento 5 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+5) == EXITO) && (arbol->nodo_raiz->izquierda->derecha->elemento == paquetes+5) , "Puedo insertar el elemento 6 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+6) == EXITO) && (arbol->nodo_raiz->izquierda->derecha->izquierda->elemento == paquetes+6) , "Puedo insertar el elemento 7 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+7) == EXITO) && (arbol->nodo_raiz->izquierda->derecha->derecha->elemento == paquetes+7) , "Puedo insertar el elemento 8 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+8) == EXITO) && (arbol->nodo_raiz->derecha->elemento == paquetes+8) , "Puedo insertar el elemento 9 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+9) == EXITO) && (arbol->nodo_raiz->derecha->izquierda->elemento == paquetes+9) , "Puedo insertar el elemento 10 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+10) == EXITO) && (arbol->nodo_raiz->derecha->izquierda->izquierda->elemento == paquetes+10) , "Puedo insertar el elemento 11 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+11) == EXITO) && (arbol->nodo_raiz->derecha->izquierda->derecha->elemento == paquetes+11) , "Puedo insertar el elemento 12 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+12) == EXITO) && (arbol->nodo_raiz->derecha->derecha->elemento == paquetes+12) , "Puedo insertar el elemento 13 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+13) == EXITO) && (arbol->nodo_raiz->derecha->derecha->izquierda->elemento == paquetes+13) , "Puedo insertar el elemento 14 correctamente y está en la ubicación correcta." );
	tsgd_afirmar( (arbol_insertar(arbol, paquetes+14) == EXITO) && (arbol->nodo_raiz->derecha->derecha->derecha->elemento == paquetes+14) , "Puedo insertar el elemento 15 correctamente y está en la ubicación correcta." );

	arbol_destruir(arbol);
}

void tests_busqueda(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	abb_t* arbol = NULL;
	tsgd_afirmar( arbol_buscar(arbol, paquetes) == NULL , "No puedo buscar de un árbol nulo." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( arbol_buscar(arbol, paquetes) == NULL , "No puedo buscar de un árbol no nulo que tiene una raíz nula." );

	int resultado = insertar_elementos(arbol, 14, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( arbol_buscar(arbol, paquetes+14) == NULL , "No encuentro un elemento que no existe en el árbol." );
	tsgd_afirmar( arbol_buscar(arbol, paquetes) == paquetes , "Encuentro un elemento que existe." );
	tsgd_afirmar( arbol_buscar(arbol, paquetes+7) == paquetes+7 , "Encuentro otro elemento que existe." );
	tsgd_afirmar( arbol_buscar(arbol, paquetes+8) == paquetes+8 , "Encuentro un tercer elemento que existe." );
	tsgd_afirmar( arbol_buscar(arbol, paquetes+5) == paquetes+5 , "Encuentro un cuarto elemento que existe." );

	arbol_destruir(arbol);
}

void tests_borrado(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	abb_t* arbol = NULL;
	tsgd_afirmar( (arbol_borrar(arbol, paquetes) == ERROR) && (arbol == NULL) , "No puedo borrar algo de un árbol nulo." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( (arbol_borrar(arbol, paquetes) == ERROR) && (arbol->nodo_raiz == NULL) , "No puedo borrar algo de un árbol con raíz nula." );

	int resultado = insertar_elementos(arbol, 2, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( (arbol_borrar(arbol, paquetes+1) == EXITO) && (arbol->nodo_raiz->izquierda == NULL) && (arbol->nodo_raiz->derecha == NULL) , "Puedo borrar un elemento del árbol correctamente." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes+1) == NULL) && (paquetes[1].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	tsgd_afirmar( (arbol_borrar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz == NULL) , "Puedo borrar el último elemento de un árbol y la raíz hoja queda nula." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes) == NULL) && (paquetes[0].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	tsgd_afirmar( (arbol_borrar(arbol, paquetes) == ERROR) && (arbol->nodo_raiz == NULL) , "No puedo borrar de un árbol que quedó vacío." );

	resultado = insertar_elementos(arbol, 10, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( (arbol_borrar(arbol, paquetes) == EXITO) && (arbol->nodo_raiz->elemento == paquetes+7) && (arbol->nodo_raiz->izquierda->derecha->derecha == NULL) , "Puedo borrar la raíz con 2 hijos correctamente. El reemplazo es correcto." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes) == NULL) && (paquetes[0].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	tsgd_afirmar( (arbol_borrar(arbol, paquetes+5) == EXITO) && (arbol->nodo_raiz->izquierda->derecha->elemento == paquetes+6) && (arbol->nodo_raiz->izquierda->derecha->izquierda == NULL) , "Puedo borrar un nodo con 1 hijo correctamente. El reemplazo es correcto." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes+5) == NULL) && (paquetes[5].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	tsgd_afirmar( (arbol_borrar(arbol, paquetes+4) == EXITO) && (arbol->nodo_raiz->izquierda->izquierda->derecha == NULL) , "Puedo borrar un nodo hoja correctamente." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes+4) == NULL) && (paquetes[4].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	tsgd_afirmar( (arbol_borrar(arbol, paquetes+1) == EXITO) && (arbol->nodo_raiz->izquierda->elemento == paquetes+2) && (arbol->nodo_raiz->izquierda->izquierda->elemento == paquetes+3) , "Puedo borrar un nodo con 2 hijos correctamente. El reemplazo es correcto." );
	tsgd_afirmar( (arbol_buscar(arbol, paquetes+1) == NULL) && (paquetes[1].fue_destruido == SI_FUE_DESTRUIDO) , "El elemento borrado no está en el árbol y se le fue aplicado el destructor." );

	arbol_destruir(arbol);
}

void tests_raiz(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	abb_t* arbol = NULL;
	tsgd_afirmar( arbol_raiz(arbol) == NULL , "Obtengo NULL porque el árbol no existe." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( arbol_raiz(arbol) == NULL , "La raíz de un árbol vacío es nula." );

	if(arbol_insertar(arbol, paquetes) == ERROR)
		return;
	tsgd_afirmar( arbol_raiz(arbol) == paquetes , "La raíz es correcta para un caso." );
	if( arbol_borrar(arbol, paquetes) == ERROR )
		return;

	if(arbol_insertar(arbol, paquetes+1) == ERROR)
		return;
	tsgd_afirmar( arbol_raiz(arbol) == paquetes+1 , "La raíz es correcta para otro caso." );
	if( arbol_borrar(arbol, paquetes+1) == ERROR )
		return;	

	if(arbol_insertar(arbol, paquetes+2) == ERROR)
		return;
	tsgd_afirmar( arbol_raiz(arbol) == paquetes+2 , "La raíz es correcta para un tercer caso." );
	if( arbol_borrar(arbol, paquetes+2) == ERROR )
		return;

	arbol_destruir(arbol);
}

void tests_vacio(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	abb_t* arbol = NULL;
	tsgd_afirmar( arbol_vacio(arbol) == ESTA_VACIO , "Un árbol nulo está vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( arbol_vacio(arbol) == ESTA_VACIO , "Un árbol no nulo con raíz nula está vacío." );

	int resultado = insertar_elementos(arbol, 1, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( arbol_vacio(arbol) == NO_ESTA_VACIO , "Un árbol un elemento no está vacío." );

	arbol_destruir(arbol);
	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);

	resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( arbol_vacio(arbol) == NO_ESTA_VACIO , "Un árbol con elementos no está vacío." );

	arbol_destruir(arbol);
}

void tests_recorrido_inorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido;

	abb_t* arbol = NULL;

	tsgd_afirmar( (arbol_recorrido_inorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, INORDEN, verificador_contenido)) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( (arbol_recorrido_inorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, INORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol no nulo con raíz nula y el array permanece vacío." );

	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

 	tsgd_afirmar( (arbol_recorrido_inorden(arbol, (void**)punteros_paquetes, tope) == 15) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 15, INORDEN, verificador_contenido) == true) , "Puedo iterar sobre 15 elementos, con 15 elementos totales y guardar 'in-orden' todos en el array de tope 15." );

 	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
 	tsgd_afirmar( (arbol_recorrido_inorden(arbol, (void**)punteros_paquetes, 10) == 10) && (es_cantidad_validos_correcta(punteros_paquetes, 10, 10, INORDEN, verificador_contenido) == true) , "Puedo iterar sobre 10 elementos, con 15 elementos totales y guardar 'in-orden' los 10 en el array de tope 10." );

	arbol_destruir(arbol);
}

void tests_recorrido_preorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido;

	abb_t* arbol = NULL;

	tsgd_afirmar( (arbol_recorrido_preorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, PREORDEN, verificador_contenido)) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( (arbol_recorrido_preorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, PREORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol no nulo con raíz nula y el array permanece vacío." );

	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

 	tsgd_afirmar( (arbol_recorrido_preorden(arbol, (void**)punteros_paquetes, tope) == 15) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 15, PREORDEN, verificador_contenido) == true) , "Puedo iterar sobre 15 elementos, con 15 elementos totales y guardar 'pre-orden' todos en el array de tope 15." );

 	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
 	tsgd_afirmar( (arbol_recorrido_preorden(arbol, (void**)punteros_paquetes, 10) == 10) && (es_cantidad_validos_correcta(punteros_paquetes, 10, 10, PREORDEN, verificador_contenido) == true) , "Puedo iterar sobre 10 elementos, con 15 elementos totales y guardar 'pre-orden' los 10 en el array de tope 10." );

	arbol_destruir(arbol);
}

void tests_recorrido_postorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido;

	abb_t* arbol = NULL;

	tsgd_afirmar( (arbol_recorrido_postorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, POSTORDEN, verificador_contenido)) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	tsgd_afirmar( (arbol_recorrido_postorden(arbol, (void**)punteros_paquetes, tope) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, POSTORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol no nulo con raíz nula y el array permanece vacío." );

	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

 	tsgd_afirmar( (arbol_recorrido_postorden(arbol, (void**)punteros_paquetes, tope) == 15) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 15, POSTORDEN, verificador_contenido) == true) , "Puedo iterar sobre 15 elementos, con 15 elementos totales y guardar 'post-orden' todos en el array de tope 15." );

 	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
 	tsgd_afirmar( (arbol_recorrido_postorden(arbol, (void**)punteros_paquetes, 10) == 10) && (es_cantidad_validos_correcta(punteros_paquetes, 10, 10, POSTORDEN, verificador_contenido) == true) , "Puedo iterar sobre 10 elementos, con 15 elementos totales y guardar 'post-orden' los 10 en el array de tope 10." );

	arbol_destruir(arbol);
}

void tests_abb_con_cada_elemento_inorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	size_t contador_interno = 0;
	bool (*funcion_ptr)(void*, void*) = &modificar_contenido_y_cargar_array;

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido_modificado;

	aux_abb_con_elemento_t auxiliares_abb;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, tope);

	abb_t* arbol = NULL;

	tsgd_afirmar( (abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion_ptr, &auxiliares_abb) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, INORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, &auxiliares_abb) == 0 , "No itero sobre ningún elemento con un árbol no nulo y una función recibida como parámetro nula." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 0, INORDEN, verificador_contenido) == true , "No se guarda ningún elemento en el array." );

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion_ptr, &auxiliares_abb) == 15 , "Puedo iterar sobre 15 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 15, INORDEN, verificador_contenido) == true , "Se guardan 'in-orden' todos los elementos en el array de tope 15. Todos elementos se modifican correctamente." );

	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	contador_interno = 0;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, 10);

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, funcion_ptr, &auxiliares_abb) == 10 , "Puedo iterar sobre 10 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 10, INORDEN, verificador_contenido) == true , "Se guardan 'in-orden' los 10 elementos en el array de tope 15. Los 10 elementos se modifican correctamente." );

	arbol_destruir(arbol);
}

void tests_abb_con_cada_elemento_preorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	size_t contador_interno = 0;
	bool (*funcion_ptr)(void*, void*) = &modificar_contenido_y_cargar_array;

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido_modificado;

	aux_abb_con_elemento_t auxiliares_abb;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, tope);

	abb_t* arbol = NULL;

	tsgd_afirmar( (abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, funcion_ptr, &auxiliares_abb) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, PREORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, NULL, &auxiliares_abb) == 0 , "No itero sobre ningún elemento con un árbol no nulo y una función recibida como parámetro nula." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 0, PREORDEN, verificador_contenido) == true , "No se guarda ningún elemento en el array." );

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, funcion_ptr, &auxiliares_abb) == 15 , "Puedo iterar sobre 15 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 15, PREORDEN, verificador_contenido) == true , "Se guardan 'pre-orden' todos los elementos en el array de tope 15. Todos elementos se modifican correctamente." );

	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	contador_interno = 0;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, 10);

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, funcion_ptr, &auxiliares_abb) == 10 , "Puedo iterar sobre 10 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 10, PREORDEN, verificador_contenido) == true , "Se guardan 'pre-orden' los 10 elementos en el array de tope 15. Los 10 elementos se modifican correctamente." );

	arbol_destruir(arbol);
}

void tests_abb_con_cada_elemento_postorden(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	size_t contador_interno = 0;
	bool (*funcion_ptr)(void*, void*) = &modificar_contenido_y_cargar_array;

	paquete_t paquetes_inexistentes[15];
	paquete_t* punteros_paquetes[15];
	size_t tope = 15;
	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	bool (*verificador_contenido)(char, size_t, int) = &es_valido_contenido_modificado;

	aux_abb_con_elemento_t auxiliares_abb;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, tope);

	abb_t* arbol = NULL;

	tsgd_afirmar( (abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, funcion_ptr, &auxiliares_abb) == 0) && (es_cantidad_validos_correcta(punteros_paquetes, tope, 0, POSTORDEN, verificador_contenido) == true) , "No itero sobre ningún elemento con un árbol nulo y el array permanece vacío." );

	arbol = arbol_crear(comparador_paquetes, destructor_paquetes);
	int resultado = insertar_elementos(arbol, 15, paquetes, 15);
	if(resultado == ERROR)
		return;

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, NULL, &auxiliares_abb) == 0 , "No itero sobre ningún elemento con un árbol no nulo y una función recibida como parámetro nula." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 0, POSTORDEN, verificador_contenido) == true , "No se guarda ningún elemento en el array." );

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, funcion_ptr, &auxiliares_abb) == 15 , "Puedo iterar sobre 15 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 15, POSTORDEN, verificador_contenido) == true , "Se guardan 'post-orden' todos los elementos en el array de tope 15. Todos elementos se modifican correctamente." );

	inicializar_paquetes_inexistentes(punteros_paquetes, paquetes_inexistentes, tope);
	contador_interno = 0;
	inicializar_auxiliares_para_tests(&auxiliares_abb, &contador_interno, (void**)punteros_paquetes, 10);

	tsgd_afirmar( abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, funcion_ptr, &auxiliares_abb) == 10 , "Puedo iterar sobre 10 elementos, con 15 elementos totales." );
	tsgd_afirmar( es_cantidad_validos_correcta(punteros_paquetes, tope, 10, POSTORDEN, verificador_contenido) == true , "Se guardan 'post-orden' los 10 elementos en el array de tope 15. Los 10 elementos se modifican correctamente." );

	arbol_destruir(arbol);
}

void tests_abb_con_cada_elemento(abb_comparador comparador_paquetes, abb_liberar_elemento destructor_paquetes, paquete_t* paquetes){

	tsgd_nuevo_subgrupo("TESTS RECORRIENDO IN-ORDEN");
	tests_abb_con_cada_elemento_inorden(comparador_paquetes, destructor_paquetes, paquetes);

	tsgd_espaciar_subgrupo();

	tsgd_nuevo_subgrupo("TESTS RECORRIENDO PRE-ORDEN");
	tests_abb_con_cada_elemento_preorden(comparador_paquetes, destructor_paquetes, paquetes);

	tsgd_espaciar_subgrupo();

	tsgd_nuevo_subgrupo("TESTS RECORRIENDO POST-ORDEN");
	tests_abb_con_cada_elemento_postorden(comparador_paquetes, destructor_paquetes, paquetes);
}

/* 
 * Cada número (num_serie) tiene una letra (contenido) asociada como se muestra en la primera parte. La sucesión de valores clave elegidos es para tener
 * un árbol completo.
 *
 *	Número: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
 *	Letra:  A B C D E F G H I J  K  L  M  N  O
 *
 *	Posiciones:  0,1,2,3,4,5,6,7, 8, 9,10,11,12,13,14
 *	Valores:    {8,4,2,1,3,6,5,7,12,10, 9,11,14,13,15}
*/
void cargar_array_paquetes(paquete_t paquetes[], int tope){

	int num_serie_arbol_completo[15] = {8,4,2,1,3,6,5,7,12,10,9,11,14,13,15};
	char contenido_arbol_completo[15]  = {'H','D','B','A','C','F','E','G','L','J','I','K','N','M','O'};

	for(int i = 0; i < tope; i++){
		paquetes[i].num_serie = num_serie_arbol_completo[i];
		paquetes[i].contenido = contenido_arbol_completo[i];
		paquetes[i].fue_destruido = NO_FUE_DESTRUIDO;
	}
}

int main(){

	int (*comparador_paquetes_ptr)(void*, void*) = &comparador_paquetes;
	void (*destructor_paquetes_ptr)(void*) = &destructor_paquetes;

	paquete_t paquetes[15];
	cargar_array_paquetes(paquetes, 15);

	tsgd_nuevo_titulo("CORRIENDO PRUEBAS A TDA ABB");

	tsgd_nuevo_grupo("TESTS A ARBOL_CREAR Y ARBOL_DESTRUIR");
	tests_creacion(comparador_paquetes_ptr, destructor_paquetes_ptr);

	tsgd_nuevo_grupo("TESTS A ARBOL_INSERTAR");
	tests_insercion(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_BUSCAR");
	tests_busqueda(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_BORRAR");
	tests_borrado(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_RAIZ");
	tests_raiz(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_VACIO");
	tests_vacio(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_RECORRIDO_INORDEN");
	tests_recorrido_inorden(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_RECORRIDO_PREORDEN");
	tests_recorrido_preorden(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ARBOL_RECORRIDO_POSTORDEN");
	tests_recorrido_postorden(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_nuevo_grupo("TESTS A ABB_CON_CADA_ELEMENTO");
	tests_abb_con_cada_elemento(comparador_paquetes_ptr, destructor_paquetes_ptr, paquetes);

	tsgd_mostrar_reporte();

	return 0;
}