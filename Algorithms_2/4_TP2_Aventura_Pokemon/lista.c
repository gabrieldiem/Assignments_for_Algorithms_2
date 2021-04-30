#include "lista.h"

#include <stdlib.h>

const static int EXITO = 0, ERROR = -1;
const static bool ESTA_VACIA = true, NO_ESTA_VACIA = false;
const static bool SE_PUDO_AVANZAR = true, NO_SE_PUDO_AVANZAR = false;
const static bool PARAR_POR_ERROR = false;
const static bool SIGO_ITERANDO = true, PARO_ITERACION = false;
const static size_t ERROR_ITERADOR = 0;

/* 
 * Pre-condiciones: biblioteca estándar stdbool incluida, la lista recibida debe ser no nula
 * Post-condiciones: devuelve true si el inicio y el fin de la lista son nulos, o false de otra manera
 */
bool son_inicio_y_fin_nulos(lista_t* lista) {
	return ((lista->nodo_inicio == NULL) && (lista->nodo_fin == NULL));
}

/* 
 * Pre-condiciones: biblioteca estándar stdbool incluida, la lista recibida debe ser no nula
 * Post-condiciones: devuelve true si el inicio o el fin es nulo, o false de otra manera
 */
bool es_inicio_o_fin_nulo(lista_t* lista) {
	return ((lista->nodo_inicio == NULL) || (lista->nodo_fin == NULL));
}

/* 
 * Pre-condiciones: biblioteca estándar stdbool incluida, la lista recibida debe ser no nula
 * Post-condiciones: devuelve true si inicio y fin son iguales, o false de otra manera
 */
bool son_inicio_y_fin_iguales(lista_t* lista) {
	return (lista->nodo_inicio == lista->nodo_fin);
}

/* 
 * Pre-condiciones: biblioteca estándar stdbool incluida, la lista recibida debe ser no nula
 * Post-condiciones: devuelve true si inicio y fin son iguales y a la vez ambos son no nulos, o false de otra manera
 */
bool son_inicio_y_fin_iguales_no_nulos(lista_t* lista) {
	return (son_inicio_y_fin_iguales(lista) && !son_inicio_y_fin_nulos(lista));
}

/*
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t* lista_crear(lista_destructor_elementos destructor) {
	lista_t* lista = calloc(1, sizeof(lista_t));
	if (!lista)
		return NULL;

	lista->destructor = destructor;

	return lista;
}

/* 
 * Pre-condiciones: biblioteca estándar stdlib incluida
 * Post-condiciones: crea un nodo reservando la memoria necesaria. Devuelve un puntero al nodo creado o NULL en caso de error.
 */
nodo_t* nodo_crear() {
	nodo_t* nodo = calloc(1, sizeof(nodo_t));
	if (!nodo)
		return NULL;

	return nodo;
}

/* 
 * Pre-condiciones: la lista debe ingresar como nula o como un puntero que sea accesible, biblioteca estándar stdbool incluida
 * Post-condiciones: devuelve ERROR si se detecta uno de los casos donde una lista puede estar rota por dentro (en la estructura) o si la lista es nula.
 *	Devuelve true si todo es correcto
 */
int lista_casos_erroneos_probar(lista_t* lista) {
	if (!lista)
		return ERROR;

	if ((lista->cantidad == 0) && !son_inicio_y_fin_nulos(lista))
		return ERROR;

	if ((lista->cantidad > 0) && es_inicio_o_fin_nulo(lista))
		return ERROR;

	if ((lista->cantidad == 1) && !son_inicio_y_fin_iguales(lista))
		return ERROR;

	if ((lista->cantidad > 1) && son_inicio_y_fin_iguales(lista))
		return ERROR;

	return EXITO;
}

/* 
 * Pre-condiciones: biblioteca estándar stdlib incluida, lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función
 * Post-condiciones: crea e inserta un primer nodo reservando la memoria necesaria y realizando las asignaciones de punteros correspondientes para mantener
 *	 válida la lista
 */
int crear_primer_nodo(lista_t* lista) {
	lista->nodo_inicio = nodo_crear();
	if (!lista->nodo_inicio)
		return ERROR;

	lista->nodo_fin = lista->nodo_inicio;
	return EXITO;
}

/* 
 * Pre-condiciones: biblioteca estándar stdlib incluida, lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función
 * Post-condiciones: crea e inserta un nodo que no es el primero, reservando la memoria necesaria y realizando las asignaciones de punteros correspondientes
 * 		para mantener válida la lista
 */
int crear_nodo_siguiente(lista_t* lista, nodo_t** nodo_nuevo) {
	*nodo_nuevo = nodo_crear();
	if (!nodo_nuevo)
		return ERROR;

	lista->nodo_fin->siguiente = *nodo_nuevo;
	lista->nodo_fin = *nodo_nuevo;

	return EXITO;
}

/*
 * Inserta un elemento al final de la lista.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int lista_insertar(lista_t* lista, void* elemento) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_nuevo;
	int estado_creacion = EXITO;

	if (!(lista->nodo_inicio))
		estado_creacion = crear_primer_nodo(lista);
	else if (lista->nodo_fin)
		estado_creacion = crear_nodo_siguiente(lista, &nodo_nuevo);

	if (estado_creacion == ERROR)
		return ERROR;

	lista->nodo_fin->elemento = elemento;
	(lista->cantidad)++;

	return EXITO;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función, nodo_nuevo debe ser un nodo válido y accesible
 * Post-condiciones: realiza las asignaciones de punteros correspondientes para mantener que el nodo recibido se convierta en el nuevo primer nodo
 *	manteniendo válida la lista, aumenta la cantidad en 1
 */
void insertar_nuevo_primer_nodo(lista_t* lista, nodo_t* nodo_nuevo, void* elemento) {
	nodo_nuevo->elemento = elemento;
	nodo_nuevo->siguiente = lista->nodo_inicio;
	lista->nodo_inicio = nodo_nuevo;
	(lista->cantidad)++;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función, nodo_nuevo debe ser un nodo válido y accesible
 *	posicion no puede ser 0
 * Post-condiciones: realiza las asignaciones de punteros correspondientes para mantener que el nodo recibido se convierta en el nuevo nodo que ocupa
 *	la posición recibida manteniendo válida la lista, aumenta la cantidad en 1
 */
void insertar_nodo_en_medio(lista_t* lista, nodo_t* nodo_nuevo, void* elemento, size_t posicion) {
	nodo_t* nodo_previo = lista->nodo_inicio;
	for (int i = 0; i < (posicion - 1); i++) {
		nodo_previo = nodo_previo->siguiente;
	}

	nodo_nuevo->elemento = elemento;
	nodo_nuevo->siguiente = nodo_previo->siguiente;
	nodo_previo->siguiente = nodo_nuevo;
	(lista->cantidad)++;
}

/*
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	int estado_insercion = EXITO;

	if (posicion >= lista->cantidad) {
		estado_insercion = lista_insertar(lista, elemento);
		return estado_insercion;
	}

	nodo_t* nodo_nuevo = nodo_crear();
	if (!nodo_nuevo)
		return ERROR;

	if (posicion == 0)
		insertar_nuevo_primer_nodo(lista, nodo_nuevo, elemento);
	else if (posicion < lista->cantidad)
		insertar_nodo_en_medio(lista, nodo_nuevo, elemento, posicion);

	return estado_insercion;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función
 * Post-condiciones: el nodo_a_borrar contiene la dirección del nodo_inicio para ser borrado, inicio y fin son apuntados a NULL, manteniendo válida la lista
 */
void borrar_primer_unico_nodo(lista_t* lista, nodo_t** nodo_a_borrar) {
	*nodo_a_borrar = lista->nodo_inicio;
	lista->nodo_inicio = NULL;
	lista->nodo_fin = NULL;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función
 * Post-condiciones: el nodo_a_borrar contiene la dirección del nodo_fin, se realizan las asignaciones de punteros correspondientes para mantener válida
 *	 la lista, sin dejar enlaces simples rotos ni perdidos
 */
void borrar_ultimo_nodo(lista_t* lista, nodo_t** nodo_a_borrar) {
	*nodo_a_borrar = lista->nodo_fin;

	lista->nodo_fin = lista->nodo_inicio;
	for (int i = 0; i < ((lista->cantidad) - 2); i++) {
		lista->nodo_fin = lista->nodo_fin->siguiente;
	}
	lista->nodo_fin->siguiente = NULL;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función
 * Post-condiciones: el nodo_a_borrar contiene la dirección del nodo_inicio para ser borrado, el nodo_inicio pasa a ser lo que antes era el segundo elemento
 *	de la lista, manteniendo válida la estructura de la lista, sin dejar enlaces simples rotos ni perdidos
 */
void borrar_primer_no_unico_nodo(lista_t* lista, nodo_t** nodo_a_borrar) {
	*nodo_a_borrar = lista->nodo_inicio;
	lista->nodo_inicio = lista->nodo_inicio->siguiente;
}

/* 
 * Pre-condiciones: lista_casos_erroneos_probar tuvo que devolver EXITO antes de ser llamada esta función, posición no puede ser 0 ni representar el último
 *	nodo de la lista
 * Post-condiciones: el nodo_a_borrar contiene la dirección del nodo que pertenece a la posición recibida, se realizan las asignaciones de punteros
 *	 correspondientes para mantener válida la lista, sin dejar enlaces simples rotos ni perdidos
 */
void borrar_nodo_en_medio(lista_t* lista, nodo_t** nodo_a_borrar, size_t posicion) {
	nodo_t* nodo_previo = lista->nodo_inicio;
	*nodo_a_borrar = lista->nodo_inicio;

	for (int i = 0; i < posicion; i++) {
		nodo_previo = *nodo_a_borrar;
		*nodo_a_borrar = (*nodo_a_borrar)->siguiente;
	}

	nodo_previo->siguiente = (*nodo_a_borrar)->siguiente;
}

/*
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_a_borrar = NULL;
	size_t cantidad = lista->cantidad;

	if (cantidad == 1) {
		borrar_primer_unico_nodo(lista, &nodo_a_borrar);
	} else if (cantidad > 1) {
		borrar_ultimo_nodo(lista, &nodo_a_borrar);
	} else
		return ERROR;

	nodo_a_borrar->siguiente = NULL;

	if (cantidad > 0)
		(lista->cantidad)--;

	if (nodo_a_borrar && lista->destructor)
		lista->destructor(nodo_a_borrar->elemento);

	free(nodo_a_borrar);
	return EXITO;
}

/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento.  
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.  
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar_de_posicion(lista_t* lista, size_t posicion) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_a_borrar = NULL;
	size_t cantidad = lista->cantidad;

	if (cantidad == 0)
		return ERROR;

	else if ((posicion == 0) || (cantidad == 1)) {
		if (son_inicio_y_fin_iguales(lista))
			borrar_primer_unico_nodo(lista, &nodo_a_borrar);
		else
			borrar_primer_no_unico_nodo(lista, &nodo_a_borrar);

	} else if (posicion >= (cantidad - 1))
		borrar_ultimo_nodo(lista, &nodo_a_borrar);

	else
		borrar_nodo_en_medio(lista, &nodo_a_borrar, posicion);

	if (nodo_a_borrar && lista->destructor)
		lista->destructor(nodo_a_borrar->elemento);

	(lista->cantidad)--;
	free(nodo_a_borrar);

	return EXITO;
}

/*
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return NULL;

	if (posicion >= lista->cantidad)
		return NULL;

	nodo_t* nodo_buscado = lista->nodo_inicio;
	if (!nodo_buscado)
		return NULL;

	for (int i = 0; i < posicion; i++) {
		nodo_buscado = nodo_buscado->siguiente;
	}

	return (nodo_buscado->elemento);
}

/* 
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía.
 */
void* lista_ultimo(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return NULL;

	if (lista->nodo_fin)
		return (lista->nodo_fin->elemento);
	else
		return NULL;
}

/* 
 * Devuelve true si la lista está vacía o false en caso contrario.
 */
bool lista_vacia(lista_t* lista) {
	if (!lista)
		return ESTA_VACIA;

	if ((!lista->nodo_inicio) && (!lista->nodo_fin) && (lista->cantidad == 0))
		return ESTA_VACIA;

	return NO_ESTA_VACIA;
}

/*
 * Devuelve la cantidad de elementos almacenados en la lista.
 */
size_t lista_elementos(lista_t* lista) {
	if (!lista)
		return 0;

	return lista->cantidad;
}

/* 
 * Apila un elemento.
 * Devuelve 0 si pudo o -1 en caso contrario.
 */
int lista_apilar(lista_t* lista, void* elemento) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_nuevo = nodo_crear();
	if (!nodo_nuevo)
		return ERROR;

	nodo_nuevo->elemento = elemento;

	if (lista->cantidad == 0)
		lista->nodo_inicio = nodo_nuevo;
	else
		lista->nodo_fin->siguiente = nodo_nuevo;

	lista->nodo_fin = nodo_nuevo;
	(lista->cantidad)++;

	return EXITO;
}

/* 
 * Desapila un elemento.
 * Devuelve 0 si pudo desapilar o -1 si no pudo.
 */
int lista_desapilar(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_a_borrar = NULL;
	size_t cantidad = lista->cantidad;

	if (cantidad == 0)
		return ERROR;
	else if (cantidad == 1)
		borrar_primer_unico_nodo(lista, &nodo_a_borrar);
	else if (cantidad > 1)
		borrar_ultimo_nodo(lista, &nodo_a_borrar);

	if (cantidad > 0)
		(lista->cantidad)--;

	if (nodo_a_borrar && lista->destructor)
		lista->destructor(nodo_a_borrar->elemento);

	free(nodo_a_borrar);
	return EXITO;
}

/*
 * Devuelve el elemento en el tope de la pila o NULL
 * en caso de estar vacía.
 */
void* lista_tope(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return NULL;

	if (lista->cantidad == 0)
		return NULL;
	else
		return (lista->nodo_fin->elemento);
}

/*
 * Copia los elementos de lista_fuente en sentido invertido a lista_destino. Para un determinado elemento X que tendrá la posición inversa en
 * lista_destino, se tendrá que el puntero a dicho elemento es el mismo, es lo único que comparten ambas listas. Los nodos de cada lista son
 * independientes de la otra, alojando nueva memoria dinámica para crear los nodos para  lista_destino.
 * Ambas listas deben estar creadas por lista_crear. Devuelve 0 si pudo operar o -1 si no pudo.
 */
int lista_hacer_copia_invertida(lista_t* destino, lista_t* fuente) {
	int estado_pruebas = lista_casos_erroneos_probar(destino);
	if (estado_pruebas == ERROR)
		return ERROR;

	estado_pruebas = lista_casos_erroneos_probar(fuente);
	if (estado_pruebas == ERROR)
		return ERROR;

	if (destino->cantidad != 0)
		return ERROR;

	if (fuente->cantidad == 0)
		return EXITO;

	int resultado = EXITO;
	size_t pos_destino = 0;
	int pos_fuente = (int)(fuente->cantidad - 1);
	void* elemento_actual = lista_elemento_en_posicion(fuente, (size_t)pos_fuente);

	while ((resultado == EXITO) && (pos_fuente >= 0)) {
		resultado = lista_insertar_en_posicion(destino, elemento_actual, pos_destino);
		pos_fuente--;
		if (resultado == EXITO) {
			pos_destino++;
			if (pos_fuente >= 0)
				elemento_actual = lista_elemento_en_posicion(fuente, (size_t)pos_fuente);
		}
	}

	return resultado;
}

/* 
 * Encola un elemento.
 * Devuelve 0 si pudo encolar o -1 si no pudo.
 */
int lista_encolar(lista_t* lista, void* elemento) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_nuevo;
	int estado_creacion = EXITO;

	if (!(lista->nodo_inicio))
		estado_creacion = crear_primer_nodo(lista);
	else if (lista->nodo_fin)
		estado_creacion = crear_nodo_siguiente(lista, &nodo_nuevo);

	if (estado_creacion == ERROR)
		return ERROR;

	lista->nodo_fin->elemento = elemento;
	(lista->cantidad)++;

	return EXITO;
}

/* 
 * Desencola un elemento.
 * Devuelve 0 si pudo desencolar o -1 si no pudo.
 */
int lista_desencolar(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR;

	nodo_t* nodo_a_borrar = NULL;

	if (lista->cantidad == 0)
		return ERROR;
	else if (lista->cantidad == 1)
		borrar_primer_unico_nodo(lista, &nodo_a_borrar);
	else if (lista->cantidad > 1)
		borrar_primer_no_unico_nodo(lista, &nodo_a_borrar);

	if (lista->cantidad > 0)
		(lista->cantidad)--;

	if (nodo_a_borrar && lista->destructor)
		lista->destructor(nodo_a_borrar->elemento);

	free(nodo_a_borrar);

	return EXITO;
}

/*
 * Devuelve el primer elemento de la cola o NULL en caso de estar
 * vacía.
 */
void* lista_primero(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return NULL;

	if (lista->cantidad == 0)
		return NULL;
	else
		return (lista->nodo_inicio->elemento);
}

/*
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t* lista) {
	if (!lista)
		return;

	nodo_t* nodo_a_borrar = lista->nodo_inicio;
	nodo_t* nodo_siguiente = NULL;
	if (nodo_a_borrar)
		nodo_siguiente = lista->nodo_inicio->siguiente;

	for (int i = 0; i < lista->cantidad; i++) {
		if (nodo_a_borrar && lista->destructor)
			lista->destructor(nodo_a_borrar->elemento);

		free(nodo_a_borrar);
		nodo_a_borrar = nodo_siguiente;
		if (nodo_siguiente)
			nodo_siguiente = nodo_siguiente->siguiente;
	}

	free(lista);
}

/*
 * Crea un iterador para una lista. El iterador creado es válido desde
 * el momento de su creación hasta que no haya mas elementos por
 * recorrer o se modifique la lista iterada (agregando o quitando
 * elementos de la lista). 
 *
 * Al momento de la creación, el iterador queda listo para devolver el
 * primer elemento utilizando lista_iterador_elemento_actual.
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t* lista_iterador_crear(lista_t* lista) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return NULL;

	lista_iterador_t* iterador = calloc(1, sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

/*
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
	if ((!iterador) || (!iterador->lista))
		return PARAR_POR_ERROR;

	int estado_pruebas = lista_casos_erroneos_probar(iterador->lista);
	if (estado_pruebas == ERROR)
		return PARAR_POR_ERROR;

	return (iterador->corriente != NULL);
}

/*
 * Avanza el iterador al siguiente elemento.
 * Devuelve true si pudo avanzar el iterador o false en caso de
 * que no queden elementos o en caso de error.
 *
 * Una vez llegado al último elemento, si se invoca a
 * lista_iterador_elemento_actual, el resultado siempre será NULL.
 */
bool lista_iterador_avanzar(lista_iterador_t* iterador) {
	if ((!iterador) || (!iterador->lista))
		return NO_SE_PUDO_AVANZAR;

	int estado_pruebas = lista_casos_erroneos_probar(iterador->lista);
	if (estado_pruebas == ERROR)
		return NO_SE_PUDO_AVANZAR;

	if ((iterador->corriente) && (iterador->corriente->siguiente)) {
		iterador->corriente = iterador->corriente->siguiente;
		return SE_PUDO_AVANZAR;

	} else if ((iterador->corriente) && (!iterador->corriente->siguiente)) {
		iterador->corriente = iterador->corriente->siguiente;
		return NO_SE_PUDO_AVANZAR;
	} else
		return NO_SE_PUDO_AVANZAR;
}

/*
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
	if ((!iterador) || (!iterador->lista))
		return NULL;

	int estado_pruebas = lista_casos_erroneos_probar(iterador->lista);
	if (estado_pruebas == ERROR)
		return NULL;

	if (iterador->corriente)
		return (iterador->corriente->elemento);
	else
		return NULL;
}

/*
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t* iterador) {
	free(iterador);
}

/*
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma. Dicha función puede devolver true si se deben seguir recorriendo
 * elementos o false si se debe dejar de iterar elementos.
 *
 * La función retorna la cantidad de elementos iterados o 0 en caso de error.
 */
size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void* contexto) {
	int estado_pruebas = lista_casos_erroneos_probar(lista);
	if (estado_pruebas == ERROR)
		return ERROR_ITERADOR;

	if (!funcion)
		return ERROR_ITERADOR;

	size_t i = 0;
	bool debo_seguir_iterando = SIGO_ITERANDO;
	nodo_t* nodo_actual = lista->nodo_inicio;

	while ((i < lista->cantidad) && (debo_seguir_iterando) && (nodo_actual)) {
		if (!funcion(nodo_actual->elemento, contexto))
			debo_seguir_iterando = PARO_ITERACION;

		nodo_actual = nodo_actual->siguiente;
		i++;
	}

	return i;
}