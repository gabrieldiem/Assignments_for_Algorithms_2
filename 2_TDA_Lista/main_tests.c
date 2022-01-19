#include <stdlib.h>
#include <time.h>
#include "lista.h"
#include "test_suite_gd.h"

const int EXITO = 0, ERROR = -1;
const bool ESTA_VACIA = true, NO_ESTA_VACIA = false;
const size_t ERROR_ITERADOR = 0;

/*
 * Inicializa una semilla para que la función rand() funcione correctamente
 */
void generar_semilla_aleatoria(){
	srand ((unsigned)time(NULL));
}

/*
 * Carga una cantidad de letras "tope" dentro del arreglo "letras" de manera aleatoria, incluye letras mayúsculas desde la A a la Z
 */
void cargar_letras(char letras[], size_t tope){

	for(size_t i = 0; i < tope; i++){
		letras[i] = (char)(rand()%26 + 65);
	}
}

/*
 * Inserta una cantidad de letras cant en la lista con lista_insertar y verifica su validez con lista_elemento_en_posicion, devuelve true si todas las
 * operaciones son válidas o false en caso contrario
 */
bool fue_exitosa_insercion_en_masa(lista_t* lista, size_t cant, size_t desface){

	char letras[cant];
	cargar_letras(letras, cant);
	bool puedo_insertar_cantidad = true;
	size_t i = 0; 

	while( (i < cant) && (puedo_insertar_cantidad == true) ){

		if( (lista_insertar(lista, (letras+i)) == ERROR) || ( lista_elemento_en_posicion(lista, i+desface) != (letras+i)) )
			puedo_insertar_cantidad = false;
		i++;
	}

	return puedo_insertar_cantidad;
}

/*
 * Inserta una cantidad de letras cant en la lista con lista_insertar_en_posicion y verifica su validez con lista_elemento_en_posicion, devuelve true si todas
 * las operaciones son válidas o false en caso contrario
 */
bool fue_exitosa_insercion_en_posicion_en_masa(lista_t* lista, size_t cant, size_t posicion){
	
	char letras[cant];
	cargar_letras(letras, cant);
	bool puedo_insertar_cantidad = true;
	size_t i = 0; 

	while( (i < cant) && (puedo_insertar_cantidad == true) ){

		if( (lista_insertar_en_posicion(lista, (letras+i), posicion) == ERROR) || ( lista_elemento_en_posicion(lista, posicion) != (letras+i)) )
			puedo_insertar_cantidad = false;
		i++;
	}

	return puedo_insertar_cantidad;
}

/*
 * Borra una cantidad de elementos cant de la lista con lista_borrar y verfica la validez del borrado con lista_elementos, devuelve true si todas
 * las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_borrado_en_masa(lista_t* lista, size_t cant){

	bool puedo_borrar_cantidad = true;
	size_t cant_original = lista_elementos(lista);
	size_t i = 0;

	while( (i < cant) && (puedo_borrar_cantidad == true) ){

		if( (lista_borrar(lista) == ERROR) || (cant_original != (lista_elementos(lista)+(i+1))) )
			puedo_borrar_cantidad = false;
		i++;
	}
	return puedo_borrar_cantidad;
}

/*
 * Borra una cantidad de elementos cant de la lista con lista_borrar_de_posicion y verfica la validez del borrado con lista_elementos, devuelve true si todas
 * las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_borrado_en_posicion_en_masa(lista_t* lista, size_t cant, size_t posicion){

	bool puedo_borrar_cantidad = true;
	size_t cant_original = lista_elementos(lista);
	size_t i = 0;

	while( (i < cant) && (puedo_borrar_cantidad == true) ){

		if( (lista_borrar_de_posicion(lista, 10000) == ERROR) || (cant_original != (lista_elementos(lista)+(i+1))) )
			puedo_borrar_cantidad = false;
		i++;
	}
	return puedo_borrar_cantidad;
}

/*
 * Carga una cantidad de letras cant dentro del arreglo "letras" de manera aleatoria, incluye letras mayúsculas desde la A a la Z y luego las inserta
 * en la lista con lista_insertar
 */
void cargar_cantidad_letras(lista_t* lista, size_t cant, char letras[]){

	cargar_letras(letras, cant);
	size_t i = 0; 

	while(i < cant){
		lista_insertar(lista, letras+i);
		i++;
	}
}

/*
 * Apila una cantidad de letras cant en la pila con lista_apilar y verifica su validez utilizando lista_tope, devuelve true si todas
 * las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_apilamiento_en_masa(lista_t* pila, size_t cant){

	char letras[cant];
	cargar_letras(letras, cant);
	size_t i = 0;
	bool puedo_apilar_cantidad = true;

	while( (i < cant) && (puedo_apilar_cantidad == true) ){

		if( (lista_apilar(pila, letras+i) == ERROR) || ((char*)lista_tope(pila) != letras+i) )
			puedo_apilar_cantidad = false;
		i++;
	}
	return puedo_apilar_cantidad;
}

/*
 * Apila una cantidad de letras cant en la pila con lista_apilar
 */
void apilar_cant(lista_t* pila, char elementos[], size_t cant){

	cargar_letras(elementos, cant);
	for(int i = 0; i < cant; i++){
		lista_apilar(pila, elementos+i);
	}
}

/*
 * Apila para desapilar una cantidad de elementos cant de la pila con lista_desapilar y verfica la validez del borrado utilizando lista_tope, devuelve true si 
 * todas las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_desapilamiento_en_masa(lista_t* pila, char elementos[], size_t cant){

	size_t i = 0;
	bool puedo_desapilar_cantidad = true;
	apilar_cant(pila, elementos, cant);

	while( (i < cant) && (puedo_desapilar_cantidad == true) ){

		int desface = (int)(cant - (i+2));

		char* puntero_comparación = elementos+desface;
		if(desface < 0)
			puntero_comparación = NULL;

		if( (lista_desapilar(pila) == ERROR) || (lista_tope(pila) != puntero_comparación) )
			puedo_desapilar_cantidad = false;

		i++;
	}
	return puedo_desapilar_cantidad;
}

/*
 * Encola una cantidad de letras cant en la cola con lista_encolar y verifica su validez utilizando lista_primero, devuelve true si todas
 * las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_encolado_en_masa(lista_t* cola, char elementos[], size_t cant){

	size_t i = 0;
	bool puedo_encolar_cantidad = true;
	cargar_letras(elementos, cant);

	while( (i < cant) && (puedo_encolar_cantidad == true) ){

		if( (lista_encolar(cola, elementos+i) == ERROR) || (lista_primero(cola) != elementos) )
			puedo_encolar_cantidad = false;
		i++;
	}

	return puedo_encolar_cantidad;
}

/*
 * Desencola una cantidad de elementos cant de la cola con lista_desencolar y verfica la validez del borrado utilizando lista_primero, devuelve true si 
 * todas las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_desencolado_en_masa(lista_t* cola, char elementos[], size_t cant){

	size_t i = 0;
	bool puedo_desencolar_cantidad = true;

	while( (i < cant) && (puedo_desencolar_cantidad == true) ){

		char* puntero_comparación = elementos+(i+1);
		if( (i+1) >= cant)
			puntero_comparación = NULL;

		if( (lista_desencolar(cola) == ERROR) || (lista_primero(cola) != puntero_comparación) )
			puedo_desencolar_cantidad = false;
		i++;
	}

	return puedo_desencolar_cantidad;
}

/*
 * Recorre la totalidad de una lista utilizando lista_iterador_tiene_siguiente como una de las condiciones de corte del ciclado, compara que el elemento
 * corriente obtenido por lista_iterador_elemento_actual sea la misma de esa posición con lista_elemento_en_posicion, agregando otra condición de corte
 * para el ciclado y avanzando el iterador con lista_iterador_avanzar devuelve true si todas las operaciones son válidas o false en caso contrario
 */
bool fue_exitoso_recorrido_iterador_externo(lista_t* lista, lista_iterador_t* iterador){

	size_t i = 0;
	bool puedo_iterar_cantidad = true;
	bool pude_avanzar_iterador = true;

	while( lista_iterador_tiene_siguiente(iterador) && pude_avanzar_iterador && puedo_iterar_cantidad ){

		if(lista_iterador_elemento_actual(iterador) != lista_elemento_en_posicion(lista, i))
			puedo_iterar_cantidad = false;
		
		if(lista_iterador_avanzar(iterador) == false)
			pude_avanzar_iterador = false;

		i++;
	}

	return puedo_iterar_cantidad;
}

/*
 * Si la letra recibida es una A (mayúscula solamente) aumenta en 1 el contador recibido, si la letra recibida es una X (mayúscula solamente) la función
 * devuelve false, si dicha letra es una A o cualquier otra letra distinta de X devolverá true
 */
bool funcion_contadora_letra_A(void* letra, void* cant_de_letras_A){

	if( *(char*)letra == 'A' )
		(*(int*)cant_de_letras_A)++;
	else if( *(char*)letra == 'X' )
		return false;

	return true;
}

/*
 * Inserta una cantidad cant de letras A en la lista, colocando una letra X en la posición "(cant/2)-1", luego itera con un ciclo for contando la cantidad
 * correcta y real de letras A que hay. Itera la lista con la función de iterador interno lista_con_cada_elemento ejecutando como función recibida como
 * parámetro a funcion_contadora_letra_A. Compara la cantidad iterada por el iterador con la cantidad esperada, además compara si la cantidad de letras A
 * contadas es la cantidad real y verdadera. De ser válidas estas dos condiciones devuelve true, o false si no es así
 */
bool fue_exitosa_iteracion_interna_en_masa(lista_t* lista, size_t cant){

	bool (*ptr_funcion)(void*, void*) = &funcion_contadora_letra_A;
	int cant_de_letras_A_iterador = 0, cant_de_letras_A_real = 0;
	size_t i = 0;
	bool sigo_iterando = true;
	char elementos[cant];

	for(size_t j = 0; j < cant; j++){
		if( j == (cant/2)-1 )
			elementos[j] = 'X';
		else
			elementos[j] = 'A';
	}
	
	for(size_t j = 0; j < cant; j++){
		lista_insertar(lista, elementos+j);
	}

	while( (i < cant) && (sigo_iterando == true) ){
		sigo_iterando = ptr_funcion(lista_elemento_en_posicion(lista, i), (void*)(&cant_de_letras_A_real));
		i++;
	}

	if( (lista_con_cada_elemento(lista, ptr_funcion, &cant_de_letras_A_iterador) == (cant/2) ) && (cant_de_letras_A_iterador == cant_de_letras_A_real) )
		return true;
	else
		return false;
}



void tests_lista_crear_y_lista_elementos(){

	lista_t* lista = lista_crear();
	tsgd_afirmar( lista != NULL , "Se crea una lista.");
	tsgd_afirmar( lista_elementos(lista) == 0, "Tiene cero elementos iniciales.");
	lista_destruir(lista);
}

void tests_lista_insertar_y_lista_elemento_posicion_y_lista_elementos(){

	char elementos[11] = "abcdefghi";
	lista_t* lista = NULL;

	tsgd_afirmar( (lista_insertar(lista, elementos) == ERROR) && ( lista_elemento_en_posicion(lista, 0) == NULL) , "No puedo insertar en lista nula.");

	lista = lista_crear();

	tsgd_afirmar( (lista_insertar(lista, elementos) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 0) == elementos) , "Puedo insertar un primer elemento en la lista vacía.");
	tsgd_afirmar( lista_elementos(lista) == 1 , "Tengo un elemento." );
	tsgd_afirmar( (lista_insertar(lista, elementos+1) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 1) == elementos+1) , "Puedo insertar un segundo elemento.");
	tsgd_afirmar( lista_elementos(lista) == 2 , "Tengo dos elementos." );
	tsgd_afirmar( (lista_insertar(lista, elementos+2) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 2) == elementos+2) , "Puedo insertar un tercer elemento.");
	tsgd_afirmar( lista_elementos(lista) == 3 , "Tengo tres elementos." );
	tsgd_afirmar( fue_exitosa_insercion_en_masa(lista, 1000, 3) , "Puedo insertar 1.000 elementos.");
	tsgd_afirmar( lista_elementos(lista) == 1003 , "Tengo 1.003 elementos." );
	lista_destruir(lista);
}

void tests_lista_insertar_posicion(){

	char elementos[11] = "abcdefghi";
	lista_t* lista = NULL;

	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos, 0) == ERROR) && ( lista_elemento_en_posicion(lista, 0) == NULL) , "No puedo insertar en lista nula en posicion 0.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos, 10) == ERROR) && ( lista_elemento_en_posicion(lista, 10) == NULL) , "No puedo insertar en lista nula en una posicion inexistente mayor a cero.");

	lista = lista_crear();

	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos, 0) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 0) == elementos) , "Puedo insertar un elemento en posicion 0 con lista vacía.");
	tsgd_afirmar( lista_elementos(lista) == 1 , "Tengo 1 elemento." );

	lista_destruir(lista);
	lista = lista_crear();

	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos, 5) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 0) == elementos) , "Puedo insertar un elemento en posicion inexistente en otra lista vacía.");
	tsgd_afirmar( lista_elementos(lista) == 1 , "Tengo 1 elemento.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos, 0) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 0) == elementos) , "Puedo insertar otro elemento en posicion 0.");
	tsgd_afirmar( lista_elementos(lista) == 2 , "Tengo 2 elementos.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos+2, 1) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 1) == elementos+2) , "Puedo insertar un elemento en posicion 1.");
	tsgd_afirmar( lista_elementos(lista) == 3 , "Tengo 3 elementos.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos+3, 1) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 1) == elementos+3) , "Puedo insertar otro elemento en posicion 1.");
	tsgd_afirmar( lista_elementos(lista) == 4 , "Tengo 4 elementos.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos+4, 10) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 4) == elementos+4) , "Puedo insertar un elemento en posicion inexistente (mayor a la cantidad).");
	tsgd_afirmar( lista_elementos(lista) == 5 , "Tengo 5 elementos.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos+5, 5) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 5) == elementos+5) , "Puedo insertar un elemento en posicion inexistente (igual a la cantidad).");
	tsgd_afirmar( lista_elementos(lista) == 6 , "Tengo 6 elementos.");
	tsgd_afirmar( (lista_insertar_en_posicion(lista, elementos+6, 3) == EXITO) && ( (char*)lista_elemento_en_posicion(lista, 3) == elementos+6) , "Puedo insertar un elemento en el medio.");
	tsgd_afirmar( lista_elementos(lista) == 7 , "Tengo 7 elementos.");
	tsgd_afirmar( fue_exitosa_insercion_en_posicion_en_masa(lista, 1000, 4) , "Puedo insertar 1.000 elementos en el medio." );
	tsgd_afirmar( lista_elementos(lista) == 1007 , "Tengo 1.007 elementos." );

	lista_destruir(lista);
}

void tests_lista_borrar(){

	lista_t* lista = NULL;
	char elementos[1000];

	tsgd_afirmar( (lista_borrar(lista) == ERROR) && (lista_elemento_en_posicion(lista, 0) == NULL) && (lista_elementos(lista) == 0) , "No puedo borrar algo de una lista nula.");

	lista = lista_crear();

	tsgd_afirmar( (lista_borrar(lista) == ERROR) && (lista_elemento_en_posicion(lista, 0) == NULL) && (lista_elementos(lista) == 0) , "No puedo borrar algo de una lista vacía.");

	cargar_cantidad_letras(lista, 3, elementos);

	tsgd_afirmar( (lista_borrar(lista) == EXITO) && (lista_elementos(lista) == 2) , "Puedo borrar un elemento.");
	tsgd_afirmar( (lista_borrar(lista) == EXITO) && (lista_elementos(lista) == 1) , "Puedo borrar otro elemento.");
	tsgd_afirmar( (lista_borrar(lista) == EXITO) && (lista_elementos(lista) == 0) , "Puedo borrar el último elemento. La lista queda vacía.");
	tsgd_afirmar( (lista_borrar(lista) == ERROR) && (lista_elementos(lista) == 0) , "No puedo borrar algo de una lista que quedó vacía.");

	cargar_cantidad_letras(lista, 1000, elementos);

	tsgd_afirmar( fue_exitoso_borrado_en_masa(lista, 1000) && (lista_elementos(lista) == 0) , "Puedo borrar todos los 1.000 elementos. La lista queda vacía." );

	lista_destruir(lista);
}

void tests_lista_borrar_en_posicion(){

	lista_t* lista = NULL;
	char elementos[1000];

	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == ERROR) && (lista_elemento_en_posicion(lista, 0) == NULL) && (lista_elementos(lista) == 0) , "No puedo borrar algo de la posición 0 de una lista nula.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 5) == ERROR) && (lista_elemento_en_posicion(lista, 0) == NULL) && (lista_elementos(lista) == 0) , "No puedo borrar algo de una posición inexistente mayor a cero de una lista nula.");
	
	lista = lista_crear();

	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == ERROR) && (lista_elementos(lista) == 0) , "No puedo borrar algo de la posición 0 de una lista vacía.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 5) == ERROR) && (lista_elementos(lista) == 0) , "No puedo borrar algo de una posición inexistente mayor a cero de una lista vacía.");

	cargar_cantidad_letras(lista, 10, elementos);

	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == EXITO) && (lista_elemento_en_posicion(lista, 0) == elementos+1) , "Puedo borrar un elemento de la posición 0.");
	tsgd_afirmar( lista_elementos(lista) == 9, "Tengo 9 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == EXITO) && (lista_elemento_en_posicion(lista, 0) == elementos+2) , "Puedo borrar otro elemento de la posición 0.");
	tsgd_afirmar( lista_elementos(lista) == 8, "Tengo 8 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 1) == EXITO) && (lista_elemento_en_posicion(lista, 1) == elementos+4) , "Puedo borrar un elemento de la posición 1.");
	tsgd_afirmar( lista_elementos(lista) == 7, "Tengo 7 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 1) == EXITO) && (lista_elemento_en_posicion(lista, 1) == elementos+5) , "Puedo borrar otro elemento de la posición 1.");
	tsgd_afirmar( lista_elementos(lista) == 6, "Tengo 6 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 2) == EXITO) && (lista_elemento_en_posicion(lista, 2) == elementos+7) , "Puedo borrar un elemento del medio.");
	tsgd_afirmar( lista_elementos(lista) == 5, "Tengo 5 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 2) == EXITO) && (lista_elemento_en_posicion(lista, 2) == elementos+8) , "Puedo borrar otro elemento del medio.");
	tsgd_afirmar( lista_elementos(lista) == 4, "Tengo 4 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 3) == EXITO) && (lista_elemento_en_posicion(lista, 2) == elementos+8) , "Puedo borrar un elemento del final.");
	tsgd_afirmar( lista_elementos(lista) == 3, "Tengo 3 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 20) == EXITO) && (lista_elemento_en_posicion(lista, 1) == elementos+5) , "Puedo borrar un elemento de una posicion que no existe (mayor a la cantidad).");
	tsgd_afirmar( lista_elementos(lista) == 2, "Tengo 2 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 15) == EXITO) && (lista_elemento_en_posicion(lista, 0) == elementos+2) , "Puedo borrar otro elemento de una posicion que no existe (mayor a la cantidad).");
	tsgd_afirmar( lista_elementos(lista) == 1, "Tengo 1 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == EXITO) && (lista_elemento_en_posicion(lista, 0) == NULL) , "Puedo borrar el último elemento presente en la lista.");
	tsgd_afirmar( lista_elementos(lista) == 0, "Tengo 0 elementos.");
	tsgd_afirmar( (lista_borrar_de_posicion(lista, 0) == ERROR) && (lista_elemento_en_posicion(lista, 0) == NULL) , "No puedo borrar elementos de una lista que quedó vacía.");

	cargar_cantidad_letras(lista, 1000, elementos);

	tsgd_afirmar( fue_exitoso_borrado_en_posicion_en_masa(lista, 1000, 2) && (lista_elementos(lista) == 0), "Puedo borrar todos los 1.000 desde una posición. La lista queda vacía.");

	lista_destruir(lista);
}

void tests_lista_ultimo(){

	lista_t* lista = NULL;
	char elementos[4];

	tsgd_afirmar( lista_ultimo(lista) == NULL , "Obtengo NULL porque la lista es nula.");

	lista = lista_crear();

	tsgd_afirmar( lista_ultimo(lista) == NULL , "Obtengo NULL porque la lista está vacía.");

	cargar_cantidad_letras(lista, 3, elementos);

	tsgd_afirmar( lista_ultimo(lista) == elementos+2 , "Obtengo el último elemento esperado.");
	lista_borrar(lista);
	tsgd_afirmar( lista_ultimo(lista) == elementos+1 , "Obtengo otro último elemento esperado.");
	lista_borrar(lista);
	tsgd_afirmar( lista_ultimo(lista) == elementos , "Obtengo un último elemento esperado.");
	lista_borrar(lista);
	tsgd_afirmar( lista_ultimo(lista) == NULL , "Obtengo NULL porque la lista quedó vacía.");

	lista_destruir(lista);
}

void tests_lista_vacia(){

	lista_t* lista = NULL;
	char elemento = 'x';

	tsgd_afirmar( lista_vacia(lista) == ESTA_VACIA , "Obtengo que la lista está vacía porque es nula.");

	lista = lista_crear();
	tsgd_afirmar( lista_vacia(lista) == ESTA_VACIA , "Obtengo que la lista está vacía porque está recién creada vacía.");

	lista_insertar(lista, &elemento);
	tsgd_afirmar( lista_vacia(lista) == NO_ESTA_VACIA , "Obtengo que la lista no está vacía porque contiene elementos.");

	lista_borrar(lista);
	tsgd_afirmar( lista_vacia(lista) == ESTA_VACIA , "Obtengo que la lista está vacía porque quedó vacía.");

	lista_destruir(lista);
}

void tests_lista_apilar_y_lista_tope(){

	lista_t* pila = NULL;
	char elementos[11] = "abcdefghi";

	tsgd_afirmar( (lista_apilar(pila, elementos) == ERROR) && (lista_tope(pila) == NULL) , "No puedo apilar en una pila nula.");

	pila = lista_crear();

	tsgd_afirmar( (lista_apilar(pila, elementos) == EXITO) && ((char*)lista_tope(pila) == elementos) , "Puedo apilar un elemento sobre una pila vacía.");
	tsgd_afirmar( (lista_apilar(pila, elementos+1) == EXITO) && ((char*)lista_tope(pila) == elementos+1) , "Puedo apilar otro elemento en la pila.");
	tsgd_afirmar( (lista_apilar(pila, elementos+2) == EXITO) && ((char*)lista_tope(pila) == elementos+2) , "Puedo apilar un tercer elemento en la pila.");

	tsgd_afirmar( fue_exitoso_apilamiento_en_masa(pila, 1000) , "Puedo apilar 1.000 elementos." );

	lista_destruir(pila);
}

void tests_lista_desapilar(){

	lista_t* pila = NULL;
	char elementos[1000];


	tsgd_afirmar( (lista_desapilar(pila) == ERROR) && (lista_tope(pila) == NULL), "No puedo desapilar de una pila nula." );

	pila = lista_crear();

	tsgd_afirmar( (lista_desapilar(pila) == ERROR) && (lista_tope(pila) == NULL), "No puedo desapilar de una pila vacía." );

	apilar_cant(pila, elementos, 4);

	tsgd_afirmar( (lista_desapilar(pila) == EXITO) && ((char*)lista_tope(pila) == elementos+2) , "Puedo desapilar un elemento." );
	tsgd_afirmar( (lista_desapilar(pila) == EXITO) && ((char*)lista_tope(pila) == elementos+1) , "Puedo desapilar otro elemento." );
	tsgd_afirmar( (lista_desapilar(pila) == EXITO) && ((char*)lista_tope(pila) == elementos) , "Puedo desapilar el último elemento de la pila." );
	tsgd_afirmar( (lista_desapilar(pila) == EXITO) && (lista_vacia(pila) == ESTA_VACIA) && (lista_tope(pila) == NULL) , "No puedo desapilar de una pila que quedó vacía." );

	tsgd_afirmar( fue_exitoso_desapilamiento_en_masa(pila, elementos, 1000) , "Puedo desapilar 1.000 elementos." );

	lista_destruir(pila);
}

void tests_lista_encolar_y_lista_primero_y_lista_desencolar(){

	lista_t* cola = NULL;
	char elementos[1000] = "abcdefghi";

	tsgd_afirmar( (lista_encolar(cola, elementos) == ERROR) && (lista_primero(cola) == NULL) , "No puedo encolar en una cola nula.");
	tsgd_afirmar( (lista_desencolar(cola) == ERROR) && (lista_primero(cola) == NULL) , "No puedo desencolar de una cola nula.");

	cola = lista_crear();

	tsgd_afirmar( (lista_desencolar(cola) == ERROR) && (lista_primero(cola) == NULL) , "No puedo desencolar de una cola vacía.");

	tsgd_afirmar( (lista_encolar(cola, elementos) == EXITO) && (lista_primero(cola) == elementos) , "Puedo encolar un elemento en una cola vacía.");
	tsgd_afirmar( (lista_encolar(cola, elementos+1) == EXITO) && (lista_primero(cola) == elementos) , "Encolo otro elemento y el primero sigue siendo correcto.");
	tsgd_afirmar( (lista_encolar(cola, elementos+2) == EXITO) && (lista_primero(cola) == elementos) , "Encolo otro elemento más y el primero sigue siendo correcto.");
	tsgd_afirmar( (lista_desencolar(cola) == EXITO) && (lista_primero(cola) == elementos+1) , "Desencolo un elemento y el nuevo primer elemento es el correcto.");
	tsgd_afirmar( (lista_desencolar(cola) == EXITO) && (lista_primero(cola) == elementos+2) , "Desencolo otro elemento y el nuevo primer elemento es el correcto.");
	tsgd_afirmar( (lista_desencolar(cola) == EXITO) && (lista_primero(cola) == NULL && (lista_vacia(cola) == ESTA_VACIA) ) , "Desencolo el último elemento y obtengo una cola vacía.");
	tsgd_afirmar( (lista_desencolar(cola) == ERROR) && (lista_primero(cola) == NULL) , "No puedo desencolar de una cola que quedó vacía.");

	tsgd_afirmar( fue_exitoso_encolado_en_masa(cola, elementos, 1000) , "Puedo encolar 1.000 elementos.");
	tsgd_afirmar( fue_exitoso_desencolado_en_masa(cola, elementos, 1000) && (lista_vacia(cola) == ESTA_VACIA) , "Puedo desencolar 1.000 elementos y la lista queda vacía.");

	lista_destruir(cola);
}

void tests_iterador_externo(){

	lista_t* lista = NULL;
	lista_iterador_t* iterador = lista_iterador_crear(lista);
	char elementos[1000];

	tsgd_afirmar( iterador == NULL, "No puedo crear un iterador con una lista nula.");
	tsgd_afirmar( lista_iterador_tiene_siguiente(iterador) == false , "Un iterador nulo no tiene siguiente.");
	tsgd_afirmar( lista_iterador_avanzar(iterador) == false , "Un iterador nulo no puede ser avanzado.");
	tsgd_afirmar( lista_iterador_elemento_actual(iterador) == NULL, "El elemento actual de un iterador nulo es nulo.");

	lista_iterador_destruir(iterador);
	lista = lista_crear();
	iterador = lista_iterador_crear(lista);

	tsgd_afirmar( iterador != NULL, "Puedo crear un iterador con una lista no nula.");
	tsgd_afirmar( lista_iterador_tiene_siguiente(iterador) == false , "Un iterador de lista vacía no tiene siguiente.");
	tsgd_afirmar( lista_iterador_avanzar(iterador) == false , "Un iterador de lista vacía no puede ser avanzado.");
	tsgd_afirmar( lista_iterador_elemento_actual(iterador) == NULL, "El elemento actual de un iterador de lista vacía es nulo.");

	lista_iterador_destruir(iterador);
	cargar_cantidad_letras(lista, 5, elementos);
	iterador = lista_iterador_crear(lista);

	tsgd_afirmar( fue_exitoso_recorrido_iterador_externo(lista, iterador) , "Puedo iterar una lista con 5 elementos y cada uno es correcto.");

	lista_iterador_destruir(iterador);
	lista_destruir(lista);
	lista = lista_crear();
	cargar_cantidad_letras(lista, 1000, elementos);
	iterador = lista_iterador_crear(lista);

	tsgd_afirmar( fue_exitoso_recorrido_iterador_externo(lista, iterador) , "Puedo iterar una lista con 1.000 elementos y cada uno es correcto.");

	lista_iterador_destruir(iterador);
	lista_destruir(lista);
}

void tests_iterador_interno(){

	lista_t* lista = NULL;
	char elementos_sin_X[11] = "ABABABABAB";
	char elementos_con_X[11] = "ABABAXABAB";
	int cant_de_letras_A = 0;

	bool (*ptr_funcion)(void*, void*) = &funcion_contadora_letra_A;

	tsgd_afirmar( lista_con_cada_elemento(lista, ptr_funcion, &cant_de_letras_A) == ERROR_ITERADOR, "No puedo iterar con una lista nula.");

	lista = lista_crear();

	tsgd_afirmar( lista_con_cada_elemento(lista, ptr_funcion, &cant_de_letras_A) == 0, "Itero cero elementos de una lista vacía.");
	tsgd_afirmar( lista_con_cada_elemento(lista, NULL, &cant_de_letras_A) == ERROR_ITERADOR, "No puedo iterar si no existe función recibida como parámetro.");

	for(int i = 0; i < 10; i++){
		lista_insertar(lista, elementos_sin_X+i);
	}

	tsgd_afirmar( (lista_con_cada_elemento(lista, ptr_funcion, &cant_de_letras_A) == 10) && (cant_de_letras_A == 5) , "Itero 10 elementos de manera correcta y cuento una cantidad condicionada arbitraria correcta.");

	lista_destruir(lista);
	lista = lista_crear();
	cant_de_letras_A = 0;
	for(int i = 0; i < 10; i++){
		lista_insertar(lista, elementos_con_X+i);
	}

	tsgd_afirmar( (lista_con_cada_elemento(lista, ptr_funcion, &cant_de_letras_A) == 6) && (cant_de_letras_A == 3) , "Itero 6 elementos de manera correcta, hasta un elemento determinado y cuento una cantidad condicionada arbitraria correcta.");

	lista_destruir(lista);
	lista = lista_crear();

	tsgd_afirmar( fue_exitosa_iteracion_interna_en_masa(lista, 1000) , "Itero 1.000 elementos de manera correcta, hasta un elemento determinado y cuento una cantidad condicionada arbitraria correcta.");

	lista_destruir(lista);
}



void probar_funciones_lista(){

	tsgd_nuevo_titulo("CORRIENDO PRUEBAS A FUNCIONES DE LISTA");

	tsgd_nuevo_grupo("TESTS A LISTA_CREAR Y LISTA_ELEMENTOS");
	tests_lista_crear_y_lista_elementos();

	tsgd_nuevo_grupo("TESTS A LISTA_INSERTAR, LISTA_ELEMENTO_EN_POSICION Y LISTA_ELEMENTOS");
	tests_lista_insertar_y_lista_elemento_posicion_y_lista_elementos();

	tsgd_nuevo_grupo("TESTS A LISTA_INSERTAR_EN_POSICION");
	tests_lista_insertar_posicion();

	tsgd_nuevo_grupo("TESTS A LISTA_BORRAR");
	tests_lista_borrar();

	tsgd_nuevo_grupo("TESTS A LISTA_BORRAR_EN_POSICION");
	tests_lista_borrar_en_posicion();

	tsgd_nuevo_grupo("TESTS A LISTA_ULTIMO");
	tests_lista_ultimo();

	tsgd_nuevo_grupo("TESTS A LISTA_VACIA");
	tests_lista_vacia();
}

void probar_funciones_especificas_pila(){

	tsgd_nuevo_titulo("\nCORRIENDO PRUEBAS A FUNCIONES ESPECÍFICAS DE PILA");

	tsgd_nuevo_grupo("TESTS A LISTA_APILAR Y LISTA_TOPE");
	tests_lista_apilar_y_lista_tope();

	tsgd_nuevo_grupo("TESTS A LISTA_DESAPILAR");
	tests_lista_desapilar();
}

void probar_funciones_especificas_cola(){

	tsgd_nuevo_titulo("\nCORRIENDO PRUEBAS A FUNCIONES ESPECÍFICAS DE COLA");

	tsgd_nuevo_grupo("TESTS A LISTA_ENCOLAR, LISTA_PRIMERO y LISTA_DESENCOLAR");
	tests_lista_encolar_y_lista_primero_y_lista_desencolar();
}

void probar_iteradores(){

	tsgd_nuevo_titulo("\nCORRIENDO PRUEBAS DE RECORRIDO CON ITERADORES");

	tsgd_nuevo_grupo("TESTS A ITERADOR EXTERNO");
	tests_iterador_externo();

	tsgd_nuevo_grupo("TESTS A ITERADOR INTERNO");
	tests_iterador_interno();
}

int main(){

	generar_semilla_aleatoria();

	probar_funciones_lista();

	probar_funciones_especificas_pila();

	probar_funciones_especificas_cola();

	probar_iteradores();

	tsgd_mostrar_reporte();

	return 0;
}