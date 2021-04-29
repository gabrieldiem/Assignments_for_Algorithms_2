#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "evento_pesca.h"

#define MAX_RUTA 200

const static char MODO_LECTURA[] = "r", MODO_ESCRITURA[] = "w";
const static char EXTENSION_TXT[] = "txt";
const static int TOTAL_LEIDO_CORRECTAMENTE = 4;
const static int ERROR = -1, EXITO = 0;

const static bool NO_HAY_SUFICIENTES = false, SI_HAY_SUFICIENTES = true;
const static bool ENCONTRADO = true, NO_ENCONTRADO = false;
const static bool ES_CORRECTA = true, NO_ES_CORRECTA = false;
const static bool NO_HAY_PUNTO = false, HAY_PUNTO = true;
const static bool NO_HAY_LETRA = false, HAY_LETRA = true;


/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, arrecife debe contener datos validos al igual que los pokemones alojados en el heap,
 	la funcion por parametro debe respetar el prototipo señalado
 * Post-condiciones: devuelve true si existe una cantidad de pokemones en el arrecife igual a cant_seleccion que cumpla con las condiciones
 	 dictadas por la funcion recibida como parametro o false si no es asi
 */
bool hay_suficientes_para_traslado(arrecife_t* arrecife, int cant_seleccion, bool (*seleccionar_pokemon) (pokemon_t*)){

	bool hay_suficientes = NO_HAY_SUFICIENTES;
	int cant_apta = 0;

	int i = 0;

	if( (arrecife == NULL) && ((*arrecife).cantidad_pokemon == 0) )
		return NO_HAY_SUFICIENTES;

	if( ((*arrecife).cantidad_pokemon > 0) && ((*arrecife).cantidad_pokemon >= cant_seleccion) ){
		while( (i < (*arrecife).cantidad_pokemon) && (hay_suficientes == NO_HAY_SUFICIENTES) ){
			if(seleccionar_pokemon((*arrecife).pokemon + i)){
				cant_apta++;
			}

			if(cant_apta >= cant_seleccion){
				hay_suficientes = SI_HAY_SUFICIENTES;
			}

			i++;
		}
	}
	
	return hay_suficientes;
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, la ruta_file debe ser un string de caractere
 * Post-condiciones: devuelve true si encuentra un caracter distinto de '\0' luego de un punto '.' o false si no es asi
 */
bool hay_punto_despues_letra(char const * ruta_file){

	bool hay_punto = NO_HAY_PUNTO;
	bool hay_letra_siguiendo_punto = NO_HAY_LETRA;
	int pos_punto;

	int i = 0;
	char caracter_leido = ruta_file[0];
	while( (caracter_leido != '\0') && (i < MAX_RUTA) ){
		if(caracter_leido == '.'){
			hay_punto = HAY_PUNTO;
			pos_punto = i;
		}

		i++;
		caracter_leido = ruta_file[i];
	}

	if( (hay_punto) && (ruta_file[pos_punto+1] != '\0') && ((pos_punto+1) < MAX_RUTA) )
		hay_letra_siguiendo_punto = HAY_LETRA;


	return (hay_punto && hay_letra_siguiendo_punto);
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h y stdio.h incluida, la ruta_file debe ser un string de caracteres
 * Post-condiciones: devuelve true si en el parámetro string existe la extensión ".txt" o false si no es asi
 */
bool revisar_extension_file(char const * ruta_file){

	bool es_correcta_extension = NO_ES_CORRECTA;

	char ruta_file_temp[MAX_RUTA];
	char extension[MAX_RUTA];
	char* token_extension;

	if(hay_punto_despues_letra(ruta_file)){
		strcpy(ruta_file_temp, ruta_file);
		strtok(ruta_file_temp, ".");
		token_extension = strtok(NULL, ".");
		strcpy(extension, token_extension);

		if(strcmp(extension, EXTENSION_TXT) == 0){
			es_correcta_extension = ES_CORRECTA;
			printf("Archivo con formato de texto detectado para lectura.\n");
		} else {
			printf("Archivo sin formato de texto para lectura.\n");
		}
	} else {
		printf("Archivo sin formato de texto para lectura.\n");
	}

	return es_correcta_extension;
}

/* 
 * Pre-condiciones: el parámetro puntero a un puntero debe estar inicializado como un puntero a un arrecife, biblioteca estandar stdlib.h incluida
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria, se posee
  	el campo-puntero "pokemon" apuntando a un bloque en el heap de tipo pokemon_t y el puntero arrecife a un bloque en el heap de tipo arrecife_t
 */
int reservar_memoria_pokemones(arrecife_t** arrecife){

	*arrecife = (arrecife_t*)malloc( sizeof(arrecife_t) );
	if(*arrecife == NULL)
		return ERROR;

	(**arrecife).pokemon = (pokemon_t*)malloc( sizeof(pokemon_t) );
	if( (**arrecife).pokemon == NULL ){
		free(*arrecife);
		return ERROR;
	}
	(**arrecife).cantidad_pokemon = 0;

	return EXITO;
}

/* 
 * Pre-condiciones: el parametro arrecife debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
  	y stdio.h incluida, cant_necesaria debe ser mayor a cero
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria, se posee 
 	el campo-puntero "pokemon" apuntando a un bloque en el heap de tipo pokemon_t con una cantidad de elementos cant_necesaria
 */
int realocar_memoria_pokemones(arrecife_t* arrecife, int cant_necesaria){

	pokemon_t* array_pokemones_aux;

	array_pokemones_aux = realloc( (*arrecife).pokemon, (size_t)(cant_necesaria) * sizeof(pokemon_t) );
	if(array_pokemones_aux == NULL){
		printf("Error Fatal de Memoria: Imposible modificar el tamaño del bloque de memoria dinámico. Finalizando ejecución.\n");
		free((*arrecife).pokemon);
		free(arrecife);
		return ERROR;

	} else if(array_pokemones_aux != (*arrecife).pokemon){
		(*arrecife).pokemon = array_pokemones_aux;
	}

	return EXITO;
}

/* 
 * Pre-condiciones: el archivo file_arrecife debe estar abierto correctamente para lectura, el puntero a un arrecife debe ingresar 
 	inicializado en NULL, biblioteca estandar stdlib.h y stdio.h incluida
 * Post-condiciones: devuelve un puntero a un arrecife que contiene su campo-vector "pokemon" cargado con todos los pokemones leidos correctamente
 	del archivo file_arrecife con el formato "%s;%i;%i;s\n", alojado en el heap teniendo reservada la cantidad justa de memoria, devuelve NULL si
 	hubo un error en algunas de las operaciones con memoria dinamica
 */
arrecife_t* cargar_pokemones(FILE * file_arrecife, arrecife_t* arrecife){

	arrecife_t** arrecife_reservado = &arrecife;
	pokemon_t pokemon_leido;
	int resultado = EXITO;
	int i = 0;
	int leidos;

	leidos = fscanf(file_arrecife, "%[^;];%i;%i;%[^\n]\n", pokemon_leido.especie, &(pokemon_leido.velocidad), &(pokemon_leido.peso), pokemon_leido.color);
	while((leidos == TOTAL_LEIDO_CORRECTAMENTE) && (resultado == EXITO)){

		if(i == 0){
			resultado = reservar_memoria_pokemones(arrecife_reservado);
		} else
			resultado = realocar_memoria_pokemones(arrecife, (i+1));

		if(resultado == EXITO){
			((*arrecife).pokemon)[i] = pokemon_leido;
			((*arrecife).cantidad_pokemon)++;
			i++;
		} 
		leidos = fscanf(file_arrecife, "%[^;];%i;%i;%[^\n]\n", pokemon_leido.especie, &(pokemon_leido.velocidad), &(pokemon_leido.peso), pokemon_leido.color);
	}

	if(resultado == ERROR)
		return NULL;
	else
		return arrecife;
}

/*
 * Función que dado un archivo carga los pokémon que viven en el arrecife
 * reservando la memoria necesaria para el mismo. Se debe intentar leer la mayor
 * cantidad posible de registros del archivo. Al encontrar el primer registro
 * erróneo (o al llegar al final del archivo) se deben finalizar la lectura y
 * crear el arrecife con los pokémon leídos exitosamente. En caso de no
 * encontrar ningún registro con el formato correcto, se debe devolver error.
 * Devuelve un puntero a un arrecife válido o NULL en caso de error.
 */
arrecife_t* crear_arrecife(const char* ruta_archivo){

	bool tiene_extension_correcta = revisar_extension_file(ruta_archivo);
	arrecife_t* arrecife_creado = NULL;

	if(tiene_extension_correcta){

		FILE * file_arrecife = fopen(ruta_archivo, MODO_LECTURA);
		if(!file_arrecife){
			printf("Error Fatal: Imposible abrir el archivo '%s'.\n", ruta_archivo);
			return NULL;
		}

		arrecife_creado = cargar_pokemones(file_arrecife, arrecife_creado);

		fclose(file_arrecife);
	}
	
	return arrecife_creado;
}

/*
 * Función que crea un acuario vacío reservando la memoria necesaria para el mismo.
 * Devuelve el acuario o NULL en caso de error.
 */
acuario_t* crear_acuario(){

	acuario_t* acuario;

	acuario = (acuario_t*)malloc(sizeof(acuario_t));
	if(acuario == NULL){
		return NULL;
	}

	(*acuario).pokemon = (pokemon_t*)malloc(sizeof(pokemon_t));
	if((*acuario).pokemon == NULL){
		free(acuario);
		return NULL;
	}

	(*acuario).cantidad_pokemon = 0;

	return acuario;
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, arrecife debe contener datos validos al igual que los pokemones alojados en el heap,
 	la funcion por parametro debe respetar el prototipo señalado, cant_trasladada no puede contener basura, no puede haber cero pokemones que cumplan
 	con las condiciones dictadas por la funcion recibida como parametro
 * Post-condiciones: devuelve la posicion del vector de pokemones de arrecife del primer pokemon contado por primera vez que cumple con 
 	las condiciones dictadas por la funcion recibida como parametro
 */
int busqueda_posicion(arrecife_t* arrecife, int cant_trasladada, bool (*seleccionar_pokemon) (pokemon_t*)){

	bool encontrado = NO_ENCONTRADO;
	int cant_para_ultimo = cant_trasladada;
	int posicion;
	int i = 0;
	while( (i < (*arrecife).cantidad_pokemon) && (encontrado == NO_ENCONTRADO) && (cant_para_ultimo >= 0) ){

		if( seleccionar_pokemon((*arrecife).pokemon + i) && (cant_para_ultimo == 0) ){
			posicion = i;
			encontrado = ENCONTRADO;

		} else if(seleccionar_pokemon((*arrecife).pokemon + i)){
			cant_para_ultimo--;
		}
		i++;
	}
	return posicion;
}

/* 
 * Pre-condiciones: el parametro acuario debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
  y stdio.h incluida, pokemon_copiar debe contener datos validos
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria, se posee 
 	el campo-puntero "pokemon" apuntando a un bloque en el heap de tipo pokemon_t con una cantidad de elementos aumentada en 1, asi como un ultimo
 	elemento de tipo pokemon_t agregado, aumentando correspondientemente la cantidad de pokemones
 */
int colocar_pokemon_acuario(acuario_t* acuario, pokemon_t pokemon_copiar){

	if((*acuario).cantidad_pokemon > 0){

		int cantidad_requerida = (*acuario).cantidad_pokemon + 1;
		pokemon_t* pokemon_aux = realloc( (*acuario).pokemon , (size_t)(cantidad_requerida) * sizeof(pokemon_t) );
		if(pokemon_aux == NULL){
			printf("Error de Memoria: Imposible modificar el tamaño del bloque de memoria dinámico. Traslado cancelado.\n");
			return ERROR;

		} else if(pokemon_aux != (*acuario).pokemon)
			(*acuario).pokemon = pokemon_aux;
	}

	int posicion = (*acuario).cantidad_pokemon;
	((*acuario).pokemon)[posicion] = pokemon_copiar;
	((*acuario).cantidad_pokemon)++;

	return EXITO;
}

/* 
 * Pre-condiciones: el parametro acuario debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
  y stdio.h incluida, nueva_cantidad debe ser mayor a cero
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria, se posee 
 	el campo-puntero "pokemon" apuntando a un bloque en el heap de tipo pokemon_t con una cantidad de elementos reducida un cierto numero de veces,
 	siendo el resultado un nuevo tamaño de nueva_cantidad veces el tamaño de pokemon_t
 */
int reducir_cantidad_arrecife(arrecife_t* arrecife, int nueva_cantidad){

	pokemon_t* pokemon_aux = realloc( (*arrecife).pokemon, (size_t)(nueva_cantidad) * sizeof(pokemon_t) );
	if(pokemon_aux == NULL){
		printf("Error de Memoria: Imposible modificar el tamaño del bloque de memoria dinámico. Traslado cancelado.\n");
		return ERROR;

	} else if(pokemon_aux != (*arrecife).pokemon){
		(*arrecife).pokemon = pokemon_aux;
	}

	return EXITO;
}

/* 
 * Pre-condiciones: el parametro arrecife debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
 	y stdio.h incluida, el campo cantidad_pokemon debe ser mayor a cero, el vector pokemon debe haber tenido los pokemones removidos de manera
 	ordenada, dejando así, una cantidad de pokemones al final del vector que pueden ser eliminados sin perder datos importantes
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria,
 	 al vector de pokemones se les son eliminados los ultimos elementos, cantidad dictada por el nuevo cantidad_pokemon
 	 en forma de perder acceso a sus direcciones de memoria, reduciendose asi tambien la cantidad de memoria reservada que posee, si la cantidad
 	 de pokemones queda en cero, el puntero pokemones apuntara a un solo bloque de memoria de tipo pokemon_t, quedando del mismo tamaño que 
 	 cuando se le reservo memoria con malloc la primera vez
 */
int reducir_pokemon_arrecife(arrecife_t* arrecife){

	int resultado_reduccion = EXITO;

	if( (*arrecife).cantidad_pokemon == 0 ){
		free((*arrecife).pokemon);
		(*arrecife).pokemon = NULL;
		printf("ATENCIÓN: El arrecife se ha quedado vacío, no existen más pokemones disponibles para el traslado.\n");
	} else {
		resultado_reduccion = reducir_cantidad_arrecife(arrecife, (*arrecife).cantidad_pokemon);
	}

	return resultado_reduccion;
}

/* 
 * Pre-condiciones: el parametro acuario debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
  y stdio.h incluida, nueva_cantidad debe ser mayor a cero
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria, se posee 
 	el campo-puntero "pokemon" apuntando a un bloque en el heap de tipo pokemon_t con una cantidad de elementos reducida un cierto numero de veces,
 	siendo el resultado un nuevo tamaño de nueva_cantidad veces el tamaño de pokemon_t
 */
int reducir_acuario(acuario_t* acuario, int nueva_cantidad){

	pokemon_t* pokemon_aux = realloc( (*acuario).pokemon , (size_t)(nueva_cantidad) * sizeof(pokemon_t) );
	if(pokemon_aux == NULL){
		printf("Error Fatal de Memoria: Imposible modificar el tamaño del bloque de memoria dinámico. Finalizando ejecución.\n");
		return ERROR;

	} else if(pokemon_aux != (*acuario).pokemon)
		(*acuario).pokemon = pokemon_aux;

	return EXITO;
}

/* 
 * Pre-condiciones: el parametro acuario debe haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.h
  y stdio.h incluida, cant_copiados debe contener datos validos
 * Post-condiciones: devuelve el valor de EXITO si no hubo ningun error o ERROR si hubo una falla en alguna reserva de memoria,
 	 al vector de pokemones se les son eliminados los ultimos cant_copiados elementos en forma de perder acceso a sus direcciones de memoria,
 	 reduciendose asi tambien la cantidad de memoria reservada que posee, si la cantidad de pokemones queda en cero, el puntero pokemones apuntara a
 	 un solo bloque de memoria de tipo pokemon_t, quedando del mismo tamaño que cuando se le reservo memoria con malloc la primera vez
 */
int revertir_copias_acuario(acuario_t* acuario, int cant_copiados){

	int resultado_reduccion;

	(*acuario).cantidad_pokemon -= cant_copiados;

	if((*acuario).cantidad_pokemon == 0){
		resultado_reduccion = reducir_acuario(acuario, 1);	
	} else {
		resultado_reduccion = reducir_acuario(acuario, (*acuario).cantidad_pokemon);
	}

	return resultado_reduccion;
}

/* 
 * Pre-condiciones: arrecife y acuario deben contener datos validos, tope_eliminar no debe contener basura, los parametro arrecife y  acuario deben
 	haber obtenido memoria reservada por un malloc previamente, biblioteca estandar stdlib.hy stdio.h incluida, necesariahabmente previamente debieron
	haber suficientes pokemones que cumplan la condicion
 * Post-condiciones: del arrecife son eliminados los pokemones que ocupan las posiciones encontradas en el vector posiciones_eliminar, devuelve EXITO si
 	la operacion se realiza exitosamente, de otra manera devuelve ERROR e intenta revertir las copias que se realizaron previamente en el acuario
 */
int eliminar_pokemon_arrecife(arrecife_t* arrecife, acuario_t* acuario, int posiciones_eliminar[], int tope_eliminar){

	int resultado_eliminacion = EXITO;
	int pos_eliminar;

	int i = 0;
	while( (i < tope_eliminar) && (resultado_eliminacion == EXITO) ){
		pos_eliminar = posiciones_eliminar[i];

		for(int j = (pos_eliminar + 1); j < (*arrecife).cantidad_pokemon; j++){
			((*arrecife).pokemon)[j-1] = ((*arrecife).pokemon)[j];
		}

		for(int k = (i+1); k < tope_eliminar; k++){
			(posiciones_eliminar[k])--;
		}

		((*arrecife).cantidad_pokemon)--;
		resultado_eliminacion = reducir_pokemon_arrecife(arrecife);

		i++;
	}

	if(resultado_eliminacion == ERROR){
		revertir_copias_acuario(acuario, tope_eliminar);
	}

	if(resultado_eliminacion == EXITO)
		return EXITO;
	else
		return ERROR;
}

/* 
 * Pre-condiciones: tope_eliminar no puede contener basura, posicion_copiada debe tener un dato valido
 * Post-condiciones: al vector posicion_eliminar se le agrega la posicion_copiada y su tope aumenta en uno
 */
void agregar_posicion_a_eliminar(int posicion_eliminar[], int* tope_eliminar, int posicion_copiada){

	posicion_eliminar[*tope_eliminar] = posicion_copiada;
	(*tope_eliminar)++;
}

/*
 * Función que deberá sacar del arrecife a todos los pokémon que satisfagan la
 * condición dada por el puntero a función (que devuelvan true) y trasladarlos
 * hacia el acuario. El parámetro cant_seleccion especifica la cantidad máxima
 * de pokémon que serán trasladados. En caso de que haya menos pokémon trasladables en el
 * arrecife que los pedidos, no se deberá mover ninguno para conservar los pocos existentes.
 * El vector de pokemones del arrecife quedará solo con aquellos que no fueron
 * trasladados (su tamaño se ajustará luego de cada traslado).
 * El vector de pokemones del acuario quedará con aquellos que fueron
 * trasladados esta vez más los que ya había en el
 * acuario (su tamaño se ajustará luego de cada traslado).
 * Devuelve -1 en caso de error o 0 en caso contrario.
 */
int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon) (pokemon_t*), int cant_seleccion){

	if(cant_seleccion < 0){
		printf("Error: La cantidad a seleccionar no puede ser negativa ni puede ser cero.\n");
		return ERROR;
	}

	if(arrecife == NULL){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo. Finalizando ejecución de función.\n");
		return ERROR;
	}

	if( ((*arrecife).pokemon == NULL) && ((*arrecife).cantidad_pokemon != 0) ){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo. Finalizando ejecución de función.\n");
		return ERROR;
	}

	if(acuario == NULL){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo. Finalizando ejecución de función.\n");
		return ERROR;
	}

	if((*acuario).pokemon == NULL){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo. Finalizando ejecución de función.\n");
		return ERROR;
	}

	bool hay_suficientes = hay_suficientes_para_traslado(arrecife, cant_seleccion, seleccionar_pokemon);
	pokemon_t pokemon_copiar;
	int cant_restantes = cant_seleccion;
	int cant_trasladada = 0;
	int posicion_copiar;
	int resultado_copia = EXITO;
	int resultado_eliminacion = EXITO;
	int resultado_reversion = EXITO;

	int posiciones_eliminar[cant_seleccion];
	int tope_eliminar = 0;

	if(hay_suficientes){
		while( (cant_restantes > 0) && (resultado_copia == EXITO) ){

			posicion_copiar = busqueda_posicion(arrecife, cant_trasladada, seleccionar_pokemon);
			pokemon_copiar = ((*arrecife).pokemon)[posicion_copiar];
			resultado_copia = colocar_pokemon_acuario(acuario, pokemon_copiar);

			if(resultado_copia == EXITO){
				agregar_posicion_a_eliminar(posiciones_eliminar, &tope_eliminar, posicion_copiar);
				cant_trasladada++;
				cant_restantes--;
			}
		}
		if( (resultado_copia == ERROR) && (tope_eliminar > 0) )
			resultado_reversion = revertir_copias_acuario(acuario, tope_eliminar);
		if(resultado_reversion == ERROR)
			return -1;

		resultado_eliminacion = eliminar_pokemon_arrecife(arrecife, acuario, posiciones_eliminar, tope_eliminar);
	} else {
		printf("No hay suficientes pokemones en el arrecife como para realizar el traslado.\n");
	}

	if( (resultado_eliminacion == EXITO) && hay_suficientes )
		return 0;
	else
		return -1;
}

/*
 * Procedimiento que dado un arrecife deberá mostrar por pantalla a todos los pokemon que contiene.
 */
void censar_arrecife(arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*)){

	if(arrecife == NULL){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo. Finalizando ejecución de función.\n");
		return;
	}

	int tope = (*arrecife).cantidad_pokemon;

	if( ((*arrecife).pokemon == NULL) && (tope != 0) ){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo.\n");
		return;
	} else if( ((*arrecife).pokemon == NULL) && (tope == 0) ){
		printf("\n    En el arrecife no queda ningún pokemon.\n\n");
		return;
	}

	printf("\n    Los pokemones presentados a continuación son los que actualmente residen en el Arrecife:\n\n");
	for(int i = 0; i < tope; i++){
		mostrar_pokemon((*arrecife).pokemon + i);
	}
}

/*
 * Función que dado un acuario guarda en un archivo de texto a los pokemones que contiene.
 * Devuelve 0 si se realizo con éxito o -1 si hubo algun problema para guardar el archivo.
 */
int guardar_datos_acuario(acuario_t* acuario, const char* nombre_archivo){

	if(acuario == NULL){
		printf("Error: Puntero que debería ser NO NULO es recibido como nulo.\n");
		return ERROR;
	}

	if((*acuario).pokemon == NULL){
		printf("Error: Puntero  que debería ser NO NULO es recibido como nulo.\n");
		return ERROR;
	}

	int tope_pokemones = (*acuario).cantidad_pokemon;
	pokemon_t pokemon_escribir;

	FILE * file_acuario = fopen(nombre_archivo, MODO_ESCRITURA);
	if(!file_acuario){
		printf("Error Fatal: Imposible sobreescribir o crear el archivo '%s'.\n", nombre_archivo);
		return -1;
	}

	for(int i = 0; i < tope_pokemones; i++){
		pokemon_escribir = ((*acuario).pokemon)[i];
		fprintf(file_acuario, "%s;%i;%i;%s\n", pokemon_escribir.especie, pokemon_escribir.velocidad, pokemon_escribir.peso, pokemon_escribir.color);
	}

	fclose(file_acuario);

	return 0;
}

/*
 * Libera la memoria que fue reservada para el acuario.
 */
void liberar_acuario(acuario_t* acuario){

	free((*acuario).pokemon);
	free(acuario);
}


/*
 * Libera la memoria que fue reservada para el arrecife.
 */
void liberar_arrecife(arrecife_t* arrecife){
	free((*arrecife).pokemon);
	free(arrecife);
}