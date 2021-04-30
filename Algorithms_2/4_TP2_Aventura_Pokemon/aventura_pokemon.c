#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "batallas.h"
#include "estructuras.h"
#include "heap.h"

#define MAX_RUTA 400
const char CARACTER_PUNTO = '.';
const char EXTENSION_FILE_TEXTO[] = ".txt";
const bool EXTENSION_VALIDA = true, EXTENSION_INVALIDA = false;

/*
 * Secuencias de escape ANSI que serán utilizados como colores para decorar el TP y hacerlo más legible
 */
#define BLANCO "\x1b[37;1m"
#define VERDE "\x1b[32;1m"
#define ROJO "\x1b[31;1m"
#define AMARILLO "\x1b[33;1m"
#define CYAN "\x1b[36;1m"
#define AZUL "\x1b[34;1m"
#define NORMAL "\x1b[0m"
#define FONDO_BLANCO "\x1b[47m"
const char COLOR_AMARILLO = 'A', COLOR_CYAN = 'C', COLOR_VERDE = 'V', COLOR_AZUL = 'Z', COLOR_ROJO = 'R';

const int POSIBLES_ID_FUNCIONES[] = {1, 2, 3, 4, 5};
const int MAX_PUNTEROS_FUNCION = 5;
const int MAX_BONUS = 63;

const int EXITO = 0, ERROR = -1;
const double TIEMPO_PAUSA = 1, TIEMPO_PELEA_EN_CURSO = 0.5;

const char LETRA_EXIT = 'X';

const int ERROR_RECUPERABLE = -2, ERROR_ARCHIVO = -3, ERROR_SIN_POKEMON = -4, ERROR_FORMATO = -5;
const int REINTENTAR_GIMNASIO = 1, FINALIZAR_PARTIDA = 2, CAMBIO_DE_GIMNASIO = 3, FIN_DEL_JUEGO_EXITOSO = 4, FIN_DEL_JUEGO_DERROTA = 5;
const int DESTRUCCION_NORMAL = 0, DESTRUCCION_RESCATE = 1;

const char IDENTIFICADOR_GIMNASIO = 'G', IDENTIFICADOR_LIDER = 'L', IDENTIFICADOR_ENTRENADOR = 'E', IDENTIFICADOR_POKEMON = 'P';
const char IDENTIFICADOR_ERROR_LECTURA = 'R';
const char NOMBRE_INICIALIZADO[] = "UNNAMED";
const int DIFICULTAD_INICIALIZADA = -1, ID_PUNTERO_FUNCION_INICIALIZADO = -1;

const char PERTENECE_A_PARTY[] = "(P)";
const int MAX_ACLARACION_PARTY = 5;

const int ID_MENU_INICIO = 1, ID_MENU_GIMNASIO = 2, ID_EXIT = 3, ID_MENU_BATALLA = 4, ID_MENU_DERROTA = 5, ID_MENU_VICTORIA = 6;
const char LETRA_INGRESAR_PERSONAJE = 'E', LETRA_AGREGAR_GIMNASIO = 'A', LETRA_COMENZAR_PARTIDA = 'I', LETRA_SIMULAR_PARTIDA = 'S';
const char LETRA_MOSTRAR_PERSONAJE = 'E', LETRA_MOSTRAR_GIMNASIO = 'G', LETRA_CAMBIAR_PARTY = 'C', LETRA_REALIZAR_BATALLA = 'B';
const char LETRA_PROXIMO_COMBATE = 'N', LETRA_TOMAR_POKEMON = 'T', LETRA_PROXIMO_GIMNASIO = 'N';
const char LETRA_REINTENTAR_GIMNASIO = 'R', LETRA_FINALIZAR_PARTIDA = 'F';

/* 
 * Pre-condiciones: stdlib.h y stdio.h incluida, se debe correr en un sistema Linux donde funcione 'system("sleep x")' con x un número
 * Post-condiciones: pausa la ejecución del programa en una cant_segundos de segundos
 */
void pausar_juego(double cant_segundos) {
	char linea_ejecucion_sleep[20];
	snprintf(linea_ejecucion_sleep, 20, "sleep %lf", cant_segundos);
	system(linea_ejecucion_sleep);
}

/* 
 * Pre-condiciones: ambos punteros a gimnasios deben ser no nulos
 * Post-condiciones: devuelve PRIMERO_MENOR si gimnasio_1 posee una dificultad menor que gimnasio_2, o SEGUNDO_MENOR de otra manera
 */
int comparador_gimnasios(void* gimnasio_1, void* gimnasio_2) {
	gimnasio_t gimnasio1 = *(gimnasio_t*)gimnasio_1;
	gimnasio_t gimnasio2 = *(gimnasio_t*)gimnasio_2;

	if (gimnasio1.dificultad < gimnasio2.dificultad)
		return PRIMERO_MENOR;
	else
		return SEGUNDO_MENOR;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si opcion corresponda a una de las opciones del menu inicio, o false de otra manera
 */
bool es_opcion_menu_inicio_correcta(char opcion) {
	return ((opcion == LETRA_INGRESAR_PERSONAJE) || (opcion == LETRA_AGREGAR_GIMNASIO) || (opcion == LETRA_COMENZAR_PARTIDA) || (opcion == LETRA_SIMULAR_PARTIDA) || (opcion == LETRA_EXIT));
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si opcion corresponda a una de las opciones del menu de gimnasio, o false de otra manera
 */
bool es_opcion_menu_gimnasio_correcta(char opcion) {
	return ((opcion == LETRA_MOSTRAR_PERSONAJE) || (opcion == LETRA_MOSTRAR_GIMNASIO) || (opcion == LETRA_CAMBIAR_PARTY) || (opcion == LETRA_REALIZAR_BATALLA) || (opcion == LETRA_EXIT));
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si opcion corresponda a una de las opciones del menu de batalla, o false de otra manera
 */
bool es_opcion_menu_batalla_correcta(char opcion) {
	return (opcion == LETRA_PROXIMO_COMBATE);
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si opcion corresponda a una de las opciones del menu de derrota, o false de otra manera
 */
bool es_opcion_menu_derrota_correcta(char opcion) {
	return ((opcion == LETRA_CAMBIAR_PARTY) || (opcion == LETRA_REINTENTAR_GIMNASIO) || (opcion == LETRA_FINALIZAR_PARTIDA));
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si opcion corresponda a una de las opciones del menu de victoria, o false de otra manera
 */
bool es_opcion_menu_victoria_correcta(char opcion) {
	return ((opcion == LETRA_TOMAR_POKEMON) || (opcion == LETRA_CAMBIAR_PARTY) || (opcion == LETRA_PROXIMO_GIMNASIO));
}

/* 
 * Pre-condiciones: string.h y stdbool.h incluida
 * Post-condiciones: devuelve EXTENSION_VALIDA si la ruta contiene ".txt" al final, o EXTENSION_INVALIDA de otra manera
 */
bool es_extension_valida(char ruta_file[MAX_RUTA]) {
	char* extension = strrchr(ruta_file, CARACTER_PUNTO);

	if ((extension == NULL) || (strlen(extension) == 1))
		return EXTENSION_INVALIDA;
	else if (strcmp(extension, EXTENSION_FILE_TEXTO) == 0)
		return EXTENSION_VALIDA;
	else
		return EXTENSION_INVALIDA;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si identificador es IDENTIFICADOR_LIDER o si es IDENTIFICADOR_ENTRENADOR, o false de otra manera
 */
bool es_identificador_de_peleador(char identificador) {
	return ((identificador == IDENTIFICADOR_LIDER) || (identificador == IDENTIFICADOR_ENTRENADOR));
}

/* 
 * Pre-condiciones: stdbool.h incluida, pila_entrenadores debe ser no nula
 * Post-condiciones: devuelve true si la cantidad de elementeos de pila_entrenadores es cero y al mismo tiempo identificador_peleador
 * 		es IDENTIFICADOR_LIDER, o false de otra manera
 */
bool es_valido_cargar_lider(lista_t* pila_entrenadores, char identificador_peleador) {
	return (lista_elementos(pila_entrenadores) == 0) && (identificador_peleador == IDENTIFICADOR_LIDER);
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si lider es no nulo y al mismo tiempo identificador_peleador es IDENTIFICADOR_ENTRENADOR y 
 * 		lider->cantidad_en_party es distinto de cero, devuelve false de otra manera
 */
bool es_valido_cargar_entrenador(entrenador_t* lider, char identificador_peleador) {
	bool es_valido = false;

	if ((lider != NULL) && (identificador_peleador == IDENTIFICADOR_ENTRENADOR))
		if (lider->cantidad_en_party != 0)
			es_valido = true;

	return es_valido;
}

/* 
 * Pre-condiciones: stdbool.h incluida, pila_entrenadores debe ser no nula
 * Post-condiciones: devuelve true si la función es_valido_cargar_lider devuelve true ó si la función es_valido_cargar_entrenador devuelve
 * 		true, devuelve false de otra manera
 */
bool es_valido_cargar_peleador(lista_t* pila_entrenadores, entrenador_t* lider, char identificador_peleador) {
	return es_valido_cargar_lider(pila_entrenadores, identificador_peleador) || es_valido_cargar_entrenador(lider, identificador_peleador);
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve false si cero elementos del vector pos_ingresadas son negativas, de otra manera devuelve true
 */
bool hay_posicion_negativa(int pos_ingresadas[], int tope) {
	bool hay_negativa = false;

	for (int i = 0; i < tope; i++) {
		if (pos_ingresadas[i] < 0)
			hay_negativa = true;
	}

	return hay_negativa;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve false si todos los elementos del vector pos_ingresadas son coherentes respecto a la máxima cantidad
 * 		cant_total_pokemones, de otra manera devuelve true
 */
bool hay_posicion_inexistente(int pos_ingresadas[], int tope, size_t cant_total_pokemones) {
	bool hay_inexistente = false;

	for (int i = 0; i < tope; i++) {
		if (pos_ingresadas[i] >= cant_total_pokemones)
			hay_inexistente = true;
	}

	return hay_inexistente;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve false si cero elementos del vector pos_ingresadas están repetidos en el mismo vector, de otra manera
 * 		devuelve true
 */
bool hay_posicion_repetida(int pos_ingresadas[], int tope) {
	bool hay_repetida = false;

	for (int i = 0; i < tope; i++) {
		int una_posicion = pos_ingresadas[i];

		for (int j = 0; j < tope; j++) {
			if ((una_posicion == pos_ingresadas[j]) && (i != j))
				hay_repetida = true;
		}
	}

	return hay_repetida;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: devuelve true si id_puntero_funcion está incluido en el vector constante POSIBLES_ID_FUNCIONES, de otra manera
 * 		devuelve false
 */
bool es_id_puntero_funcion_valido(int id_puntero_funcion) {
	bool es_valido = false;

	for (int i = 0; i < MAX_PUNTEROS_FUNCION; i++) {
		if (id_puntero_funcion == POSIBLES_ID_FUNCIONES[i])
			es_valido = true;
	}

	return es_valido;
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime una fila de cuadraditos continua
 */
void imprimir_borde_horizontal() {
	printf(CYAN "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime los cuadraditos topes
 */
void imprimir_segmento_vertical() {
	printf(CYAN "  ██                                                                                                       ██" NORMAL);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime una apertura de un bloque decorativo
 */
void imprimir_comienzo_bloque(char color) {
	if (color == COLOR_CYAN) {
		printf(CYAN "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
		printf(CYAN "  ██                                                                                                       ██\n");

	} else if (color == COLOR_AMARILLO) {
		printf(AMARILLO "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
		printf(AMARILLO "  ██                                                                                                       ██\n");

	} else if (color == COLOR_VERDE) {
		printf(VERDE "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
		printf(VERDE "  ██                                                                                                       ██\n");

	} else if (color == COLOR_AZUL) {
		printf(AZUL "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
		printf(AZUL "  ██                                                                                                       ██\n");

	} else if (color == COLOR_ROJO) {
		printf(ROJO "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
		printf(ROJO "  ██                                                                                                       ██\n");
	}
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime un fin de un bloque decorativo
 */
void imprimir_fin_bloque(char color) {
	if (color == COLOR_CYAN) {
		printf(CYAN "  ██                                                                                                       ██");
		printf(CYAN "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);

	} else if (color == COLOR_AMARILLO) {
		printf(AMARILLO "  ██                                                                                                       ██");
		printf(AMARILLO "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);

	} else if (color == COLOR_VERDE) {
		printf(VERDE "  ██                                                                                                       ██");
		printf(VERDE "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);

	} else if (color == COLOR_AZUL) {
		printf(AZUL "  ██                                                                                                       ██");
		printf(AZUL "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);

	} else if (color == COLOR_ROJO) {
		printf(ROJO "  ██                                                                                                       ██");
		printf(ROJO "\n  ███████████████████████████████████████████████████████████████████████████████████████████████████████████\n" NORMAL);
	}
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: se imprime por pantalla todo el menu inicial del juego (bordes, opciones, saltos de línea, etc)
 */
void imprimir_ui_inicio() {
	system("clear");
	imprimir_borde_horizontal();
	imprimir_segmento_vertical();
	printf(CYAN "\n  ██ " NORMAL "                    BIENVENIDO A " ROJO FONDO_BLANCO "AVENTURA POKEMON" NORMAL ", EL PRIMER JUEGO POKEMON EVER                       " CYAN "██\n");
	imprimir_segmento_vertical();
	imprimir_borde_horizontal();

	imprimir_comienzo_bloque(COLOR_VERDE);

	printf(BLANCO "\n          Ingresa una de las siguientes opciones para continuar:\n\n");
	printf("          » %c :  Ingresar el archivo del entrenador principal\n", LETRA_INGRESAR_PERSONAJE);
	printf("          » %c :  Agregar un archivo con un/unos gimnasio/s\n", LETRA_AGREGAR_GIMNASIO);
	printf("          » %c :  Comenzar la partida\n", LETRA_COMENZAR_PARTIDA);
	printf("          » %c :  Simular la partida\n", LETRA_SIMULAR_PARTIDA);
	printf("          » %c :  Finalizar la ejecución del programa\n\n" NORMAL, LETRA_EXIT);

	imprimir_fin_bloque(COLOR_VERDE);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla las opciones que el usuario tiene para ingresar en el menu dentro del gimnasio
 */
void imprimir_ui_gimnasio() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_AMARILLO);
	printf(BLANCO "             »»   En: '%c :  Comenzar la partida'\n" NORMAL, LETRA_COMENZAR_PARTIDA);
	imprimir_fin_bloque(COLOR_AMARILLO);

	imprimir_comienzo_bloque(COLOR_AZUL);

	printf(BLANCO "\n          Ingresa una de las siguientes opciones para continuar:\n\n");
	printf("          » %c :  Mostrar al entrenador principal junto a sus Pokemones\n", LETRA_MOSTRAR_PERSONAJE);
	printf("          » %c :  Mostrar información del gimnasio actual\n", LETRA_MOSTRAR_GIMNASIO);
	printf("          » %c :  Cambiar los Pokemones de batalla (party)\n", LETRA_CAMBIAR_PARTY);
	printf("          » %c :  Realizar la siguiente batalla planificada\n", LETRA_REALIZAR_BATALLA);
	printf("          » %c :  Salir al menú inicial\n\n" NORMAL, LETRA_EXIT);

	imprimir_fin_bloque(COLOR_AZUL);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla un encabezado informando que se está simulando la partida
 */
void imprimir_ui_simulacion() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_CYAN);
	printf(BLANCO "                                       »»   Simulando partida...   ««\n" NORMAL);
	imprimir_fin_bloque(COLOR_CYAN);
	fflush(stdout);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: se imprime un encabezado que indica al ususario que está en el apartado para ingresar un personaje principal
 */
void imprimir_aviso_entrenador_principal() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_AMARILLO);
	printf(BLANCO "             »»   En: '%c :  Ingresar el archivo del entrenador principal'\n" NORMAL, LETRA_INGRESAR_PERSONAJE);
	imprimir_fin_bloque(COLOR_AMARILLO);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: se imprime un encabezado que indica al ususario que está en el apartado para ingresar un archivo de gimnasios
 */
void imprimir_aviso_agregar_gimnasios() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_AMARILLO);
	printf(BLANCO "             »»   En: '%c :  Agregar un archivo con un/unos gimnasio/s'\n" NORMAL, LETRA_AGREGAR_GIMNASIO);
	imprimir_fin_bloque(COLOR_AMARILLO);
}

/* 
 * Pre-condiciones: stdio.h incluida, opcion debe ser no nulo
 * Post-condiciones: opcion contiene el caracter ingresado por el usuario
 */
void obtener_exit(char* opcion) {
	printf(CYAN "\n  █ " AMARILLO "Ingrese '%c' para volver al menú anterior: " BLANCO, LETRA_EXIT);
	scanf(" %c", opcion);
}

/* 
 * Pre-condiciones: stdio.h
 * Post-condiciones: imprime por pantalla un mensaje para que se ingrese una opción, se guarda el caracter ingresado en opcion
 */
void obtener_opcion(char* opcion) {
	printf(CYAN "\n  █ " AMARILLO "Ingrese la opción deseada: " BLANCO);
	scanf(" %c", opcion);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla que se debe ingresar de nuevo la opción y guarda la respuesta en opcion
 */
void solicitar_nueva_opcion(char* opcion) {
	printf(ROJO "\n  █ " AMARILLO "Opción ingresada inexistente. Ingrese una opción correcta: " BLANCO);
	scanf(" %c", opcion);
}

/* 
 * Pre-condiciones: stdio.h y stdbool.h incluida, id_menu debe ser unas de las id's válidas
 * Post-condiciones: dependiendo del menu se encuentra una selección de opciones disponibles para ingresar, si el usuario ingresó un caracter
 * 		inválido se le pide de nuevo hasta que ingrese el correcto. Imprime por pantalla informándole al usuario sobre su elección
 */
void comprobar_opcion(char* opcion, int id_menu) {
	if (id_menu == ID_MENU_INICIO)
		while (!es_opcion_menu_inicio_correcta(*opcion)) {
			solicitar_nueva_opcion(opcion);
		}

	else if (id_menu == ID_MENU_GIMNASIO)
		while (!es_opcion_menu_gimnasio_correcta(*opcion)) {
			solicitar_nueva_opcion(opcion);
		}

	else if (id_menu == ID_EXIT)
		while (*opcion != LETRA_EXIT) {
			solicitar_nueva_opcion(opcion);
		}

	else if (id_menu == ID_MENU_BATALLA)
		while (!es_opcion_menu_batalla_correcta(*opcion)) {
			solicitar_nueva_opcion(opcion);
		}

	else if (id_menu == ID_MENU_DERROTA)
		while (!es_opcion_menu_derrota_correcta(*opcion)) {
			solicitar_nueva_opcion(opcion);
		}

	else if (id_menu == ID_MENU_VICTORIA)
		while (!es_opcion_menu_victoria_correcta(*opcion)) {
			solicitar_nueva_opcion(opcion);
		}

	printf(VERDE "\n  █ " AMARILLO "Opción ingresada '%c' es correcta.\n" BLANCO, *opcion);
}

/* 
 * Pre-condiciones: stdio.h incluida, pokemon debe contener campos válidos, indice debe ser un número de pokemon existente
 * Post-condiciones: imprime toda la informacion del pokemon recibido (nombre, velocidad actual, ataque actual y defensa actual), además de una insigna
 * 		que marca si pertenece a la party actualmente
 */
void imprimir_pokemon_personaje(pokemon_t pokemon, size_t indice) {
	char aclaracion_party[MAX_ACLARACION_PARTY];
	int velocidad = pokemon.velocidad + pokemon.bonus;
	int ataque = pokemon.ataque + pokemon.bonus;
	int defensa = pokemon.defensa + pokemon.bonus;

	if ((indice - 1) < MAX_PARTY)
		strcpy(aclaracion_party, PERTENECE_A_PARTY);
	else
		strcpy(aclaracion_party, "");

	if (indice < 10) {
		if (strcmp(aclaracion_party, PERTENECE_A_PARTY) == 0)
			printf("          %zu. " CYAN "%s" BLANCO " NOMBRE: %s    VEL: %i    ATQ: %i    DEF: %i\n", indice, aclaracion_party, pokemon.nombre, velocidad, ataque, defensa);
		else
			printf("          %zu.     NOMBRE: %s    VEL: %i    ATQ: %i    DEF: %i\n", indice, pokemon.nombre, velocidad, ataque, defensa);

	} else {
		if (strcmp(aclaracion_party, PERTENECE_A_PARTY) == 0)
			printf("         %zu. " CYAN "%s" BLANCO " NOMBRE: %s    VEL: %i,   ATQ: %i    DEF: %i\n", indice, aclaracion_party, pokemon.nombre, velocidad, ataque, defensa);
		else
			printf("         %zu.     NOMBRE: %s    VEL: %i    ATQ: %i    DEF: %i\n", indice, pokemon.nombre, velocidad, ataque, defensa);
	}
}

/* 
 * Pre-condiciones: stdio.h incluida, personaje debe ser no nulo
 * Post-condiciones: si personaje->fue_cargado es false sólo se imprime que no es posible mostrar el entrenador. Si es true se imprimen: el nombre del
 * 		personaje, las medallas que tiene ganadas actualmente, todos los pokemones que tiene en la caja de pokemones en orden, marcando cuáles son los
 * 		pertenecientes a la party
 */
void imprimir_info_personaje(personaje_t* personaje) {
	char ingresado;

	if (!(personaje->fue_cargado)) {
		printf(ROJO "\n  █ " AMARILLO "Imposible mostrar entrenador. Ningún entrenador cargado en el sistema.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return;
	}

	system("clear");
	imprimir_comienzo_bloque(COLOR_AZUL);

	size_t cant_total_pokemones = lista_elementos(personaje->lista_caja_pokemones);
	printf(VERDE "\n    █ " BLANCO "Entrenador principal:" VERDE " %s\n" BLANCO, personaje->nombre);
	printf("\n     » Medallas ganadas: %zu\n", personaje->cantidad_medallas);

	if (cant_total_pokemones == 0)
		printf(ROJO "\n     » No contiene ningún Pokemon cargado.\n" BLANCO);
	else
		printf("     » Pokemones para batalla (party): %zu\n     » Pokemones en total: %zu\n\n", personaje->cantidad_en_party, cant_total_pokemones);

	for (size_t i = 0; i < cant_total_pokemones; i++) {
		pokemon_t pokemon = *((pokemon_t*)lista_elemento_en_posicion(personaje->lista_caja_pokemones, i));
		imprimir_pokemon_personaje(pokemon, i + 1);
	}

	printf("\n\n     Nota: El símbolo " CYAN "%s" BLANCO " representa que el Pokemon forma parte de la party del entrenador.\n\n", PERTENECE_A_PARTY);

	imprimir_fin_bloque(COLOR_AZUL);

	obtener_exit(&ingresado);
	comprobar_opcion(&ingresado, ID_EXIT);
}

/* 
 * Pre-condiciones: stdio.h incluida, pokemon debe ser un puntero válido con datos correctos
 * Post-condiciones: imprime todas las estadísticas del pokemon
 */
void imprimir_pokemon_entrenador(pokemon_t* pokemon, size_t indice) {
	if (indice < 10)
		printf("\n           %zu. NOMBRE: %s    VEL: %i    ATQ: %i    DEF: %i", indice, pokemon->nombre, pokemon->velocidad, pokemon->ataque, pokemon->defensa);
	else
		printf("\n          %zu. NOMBRE: %s    VEL: %i    ATQ: %i    DEF: %i", indice, pokemon->nombre, pokemon->velocidad, pokemon->ataque, pokemon->defensa);
}

/* 
 * Pre-condiciones: stdio.h incluida, entrenador debe ser un puntero válido y al menos haber sido inicializado
 * Post-condiciones: imprime el nombre del entrenador, la cantidad de pokemones que tiene e imprime todos sus pokemones en orden
 */
void imprimir_entrenador(entrenador_t* entrenador) {
	printf("   %s\n" BLANCO, entrenador->nombre);
	printf("        » Pokemones en total: %zu\n", entrenador->cantidad_en_party);

	for (size_t i = 0; i < entrenador->cantidad_en_party; i++) {
		imprimir_pokemon_entrenador(entrenador->party[i], i + 1);
	}
}

/* 
 * Pre-condiciones: stdio.h incluida, el gimnasio debe ser válido, no nulo y debió haber sido inicializado por lo menos
 * Post-condiciones: imprime todos los entrenadores de la pila con sus pokemones, en un orden correcto, no imprime el líder
 * 		si todo fue bien devuelve EXITO, si hubo un error devuelve ERROR
 */
int imprimir_entrenadores_gimnasio(gimnasio_t* gimnasio) {
	lista_t* pila = lista_crear(NULL);
	if (!pila)
		return ERROR;

	int resultado = lista_hacer_copia_invertida(pila, gimnasio->pila_entrenadores);
	if (resultado == ERROR)
		return ERROR;

	size_t indice = 2;

	while ((lista_elementos(pila) > 0) && (resultado == EXITO)) {
		if (indice < 10)
			printf("\n\n     %zu°) Entrenador:" CYAN, indice);
		else
			printf("\n\n    %zu°) Entrenador:" CYAN, indice);

		entrenador_t* entrenador = (entrenador_t*)lista_tope(pila);
		imprimir_entrenador(entrenador);
		indice++;
		resultado = lista_desapilar(pila);
	}

	lista_destruir(pila);

	return EXITO;
}

/* 
 * Pre-condiciones: stdio.h incluida, heap_gimnasios debe ser un heap válido
 * Post-condiciones: Si no existe ningún gimnasio insertado en el heap, se imprime que no se puede mostrar un gimnasio porque no hay ninguno cargado.
 * 		Si sí existen gimnasios se imprime toda la información del gimnasio presente en la raíz del heap_gimnasios, es decir, el nombre, la dificultad
 * 		el líder y sus pokemones, y los entrenadores y todos sus pokemones.
 * 		La impresión resultante es como una escalera, el personaje deberá derrotar a los entrenadores desde el último impreso hasta el primero impreso,
 * 		"desde abajo hacia arriba" deberá ir ganando.
 * 		Devuelve EXITO si todo fue bien o ERROR si ocurrió algún error
 */
int imprimir_info_gimnasio_actual(heap_t* heap_gimnasios) {
	char ingresado;

	if (heap_cantidad(heap_gimnasios) == 0) {
		printf(ROJO "\n  █ " AMARILLO "Imposible mostrar gimnasio. Ningún gimnasio cargado en el sistema.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	gimnasio_t* gimnasio = (gimnasio_t*)heap_elemento_raiz(heap_gimnasios);

	system("clear");
	imprimir_comienzo_bloque(COLOR_ROJO);

	printf(ROJO "\n    █ " BLANCO "Estás en el" ROJO " %s\n" BLANCO, gimnasio->nombre);
	printf("\n     » Dificultad del gimnasio: %i\n", gimnasio->dificultad);
	printf("\n\n     1°) Líder:" ROJO);
	imprimir_entrenador(gimnasio->lider);

	int resultado = imprimir_entrenadores_gimnasio(gimnasio);
	if (resultado == ERROR)
		return ERROR;

	printf("\n\n");
	imprimir_fin_bloque(COLOR_ROJO);

	obtener_exit(&ingresado);
	comprobar_opcion(&ingresado, ID_EXIT);

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Post-condiciones: libera de la memoria dinámica al pokemon recibido
 */
void destructor_pokemon(void* pokemon) {
	pokemon_t* pokemon_a_destruir = (pokemon_t*)pokemon;
	free(pokemon_a_destruir);
}

/* 
 * Pre-condiciones: stdlib.h, stdbool.h y lista.h incluida
 * Post-condiciones: campos cantidad_medallas y cantidad_en_party inicializados en cero, party es un vector dinámico de pokemon_t* inicializado con
 * 		una cantidad MAX_PARTY de elementos, campo nombre incializado con NOMBRE_INICIALIZADO, lista_caja_pokemones posee una lista válida con un
 * 		destructor de pokemones incluido, campo fue_cargado inicializado con false. Devuelve EXITO si todo fue bien o ERROR de otra manera
 */
int inicializar_personaje(personaje_t* personaje) {
	personaje->cantidad_medallas = 0;
	personaje->cantidad_en_party = 0;

	personaje->party = calloc(1, MAX_PARTY * sizeof(pokemon_t*));
	if (!(personaje->party))
		return ERROR;

	strcpy(personaje->nombre, NOMBRE_INICIALIZADO);

	void (*destructor_pokemon_ptr)(void*) = &destructor_pokemon;

	lista_t* lista = lista_crear(destructor_pokemon_ptr);
	if (!lista)
		return ERROR;

	personaje->lista_caja_pokemones = lista;
	personaje->fue_cargado = false;

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h y lista.h incluida
 * Post-condiciones: destruye lista_caja_pokemones de personaje, si tipo_destruccion es DESTRUCCION_NORMAL también libera la party de la memoria
 * 		dinámica (el vector, no los elementos)
 */
void destructor_personaje(personaje_t* personaje, int tipo_destruccion) {
	if (personaje) {
		lista_destruir(personaje->lista_caja_pokemones);

		if (tipo_destruccion == DESTRUCCION_NORMAL)
			free(personaje->party);
	}
}

/* 
 * Pre-condiciones: stdlib.h y sting.h incluidas ,personaje y personaje_auxiliar deben ser no nulos
 * Post-condiciones: "inicializa" a personaje con los datos de personaje_auxiliar
 */
void inicializar_personaje_rescate(personaje_t* personaje, personaje_t* personaje_auxiliar) {
	personaje->cantidad_medallas = personaje_auxiliar->cantidad_medallas;
	personaje->cantidad_en_party = personaje_auxiliar->cantidad_en_party;

	for (int i = 0; i < personaje_auxiliar->cantidad_en_party; i++) {
		personaje->party[i] = personaje_auxiliar->party[i];
	}

	if (personaje->party != personaje_auxiliar->party) {
		free(personaje_auxiliar->party);
		personaje_auxiliar->party = NULL;
	}

	strcpy(personaje->nombre, personaje_auxiliar->nombre);

	personaje->lista_caja_pokemones = personaje_auxiliar->lista_caja_pokemones;
	personaje->fue_cargado = personaje_auxiliar->fue_cargado;
}

/* 
 * Pre-condiciones: string.h incluida, entrenador debe ser no nulo
 * Post-condiciones: devuelve EXITO si los campos de entrenados tienen como nombre a NOMBRE_INICIALIZADO, cantidad_en_party en cero, la party como
 *  	vector dinámico de máximo MAX_PARTY y cada elemento del vector party como NULL, devuelve ERRRO de otra manera
 */
int inicializar_entrenador(entrenador_t* entrenador) {
	strcpy(entrenador->nombre, NOMBRE_INICIALIZADO);
	entrenador->cantidad_en_party = 0;

	entrenador->party = calloc(1, MAX_PARTY * sizeof(pokemon_t*));
	if (!(entrenador->party))
		return ERROR;

	for (int i = 0; i < MAX_PARTY; i++) {
		entrenador->party[i] = NULL;
	}

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Post-condiciones: destruye todos los pokemones que el entrenador posee en la party y libera la party y el entrenador de la memoria dinámica
 */
void destructor_entrenador(void* entrenador) {
	entrenador_t* entrenador_a_destruir = (entrenador_t*)entrenador;

	if ((entrenador_a_destruir) && (entrenador_a_destruir->party)) {
		for (int i = 0; i < entrenador_a_destruir->cantidad_en_party; i++) {
			destructor_pokemon(entrenador_a_destruir->party[i]);
		}
		free(entrenador_a_destruir->party);
	}
	free(entrenador_a_destruir);
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Post-condiciones: destruye todos los pokemones que el entrenador posee en la party y libera la party (NO libera el entrenador)
 */
void destructor_entrenador_stack(entrenador_t entrenador) {
	if (entrenador.party) {
		for (int i = 0; i < entrenador.cantidad_en_party; i++) {
			destructor_pokemon(entrenador.party[i]);
		}
		free(entrenador.party);
	}
}

/* 
 * Pre-condiciones: stdlib.h y string.h incluidas, gimnasio debe ser no nulo
 * Post-condiciones: devuelve EXITO si gimnasio posee los campos nombre como NOMBRE_INICIALIZADO, dificultad como DIFICULTAD_INICIALIZADA, 
 * 		id_puntero_funcion como ID_PUNTERO_FUNCION_INICIALIZADO, líder como NULL y para la pila_entrenadores tiene una pila creada correctamente,
 * 		de otra manera devuelve ERROR
 */
int inicializar_gimnasio(gimnasio_t* gimnasio) {
	strcpy(gimnasio->nombre, NOMBRE_INICIALIZADO);

	gimnasio->dificultad = DIFICULTAD_INICIALIZADA;
	gimnasio->id_puntero_funcion = ID_PUNTERO_FUNCION_INICIALIZADO;
	gimnasio->lider = NULL;

	void (*destructor_entrenador_ptr)(void*) = &destructor_entrenador;
	lista_t* pila = lista_crear(destructor_entrenador_ptr);
	if (!pila)
		return ERROR;

	gimnasio->pila_entrenadores = pila;

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Post-condiciones: destruye al líder y destruye la pila_entrenadores, esta invocando el destructor de personajes en cada personaje de la pila
 * 		finalmente libera el gimnasio de la memoria dinámica
 */
void destructor_gimnasio(void* gimnasio) {
	gimnasio_t* gimnasio_a_destruir = (gimnasio_t*)gimnasio;

	if (gimnasio_a_destruir) {
		destructor_entrenador(gimnasio_a_destruir->lider);
		lista_destruir(gimnasio_a_destruir->pila_entrenadores);
	}
	free(gimnasio_a_destruir);
}

/* 
 * Pre-condiciones: stdlib.h incluida
 * Post-condiciones: destruye al líder y destruye la pila_entrenadores, esta invocando el destructor de personajes en cada personaje de la pila,
 * 		(NO libera el gimnasio)
 */
void destructor_gimnasio_stack(gimnasio_t gimnasio) {
	if (gimnasio.lider)
		destructor_entrenador(gimnasio.lider);

	lista_destruir(gimnasio.pila_entrenadores);
}

/* 
 * Pre-condiciones: stdio.h y string.h incluidas, se debe haber avisado que se solicita una ruta
 * Post-condiciones: solicita la ruta de un archivo que queda almacenada en ruta_file hasta que ingrese una ruta de un archivo finalizado con la
 * 		extensión "".txt"
 */
void solicitar_ruta(char ruta_file[MAX_RUTA]) {
	char ruta_file_temp[MAX_RUTA];

	printf(CYAN "\n  █ " AMARILLO "Ingrese la ruta: " BLANCO);
	scanf(" %[^\n]", ruta_file_temp);

	while (!es_extension_valida(ruta_file_temp)) {
		printf(ROJO "\n  █ " AMARILLO "Ruta de archivo ingresada con extensión inválida, se necesita un archivo de texto con extensión '.txt'.\n    Por favor ingrese de nuevo: " BLANCO);
		scanf(" %[^\n]", ruta_file_temp);
	}

	strcpy(ruta_file, ruta_file_temp);
	printf(VERDE "\n  █ " AMARILLO "Ruta de archivo '%s' ingresada correctamente.\n" BLANCO, ruta_file);
	pausar_juego(TIEMPO_PAUSA);
}

/* 
 * Pre-condiciones: stdlib.h incluida, pokemon_leido debe contener todos sus válidos y completos
 * Post-condiciones: devuelve un puntero a un pokemon alojado en la memoria dinámica cargado con los datos de pokemon_dato, o NULL si hubo un error
 */
pokemon_t* crear_pokemon_mem_dinamica(pokemon_t pokemon_dato) {
	pokemon_t* pokemon_definitivo = calloc(1, sizeof(pokemon_t));
	if (!pokemon_definitivo)
		return NULL;

	*pokemon_definitivo = pokemon_dato;
	pokemon_definitivo->bonus = 0;

	return pokemon_definitivo;
}

/* 
 * Pre-condiciones: stdlib.h incluida, party debe ser un vector válido, cantidad_en_party debe estar no nulo, pokemon_leido debe contener datos
 * 		válidos de un pokemon
 * Post-condiciones: devuelve EXITO si party contiene a un pokemon alojado en la memoria dinámica en la primera posición libre del vector, si 
 * 		cantidad_en_party es igual a MAX_PARTY no carga el pokemon, devuelve ERROR de otra manera
 */
int cargar_pokemon_en_party(pokemon_t** party, size_t* cantidad_en_party, pokemon_t pokemon_leido) {
	pokemon_t* pokemon_a_agregar = crear_pokemon_mem_dinamica(pokemon_leido);
	if (!pokemon_a_agregar)
		return ERROR;

	if (*cantidad_en_party < MAX_PARTY) {
		party[*cantidad_en_party] = pokemon_a_agregar;
		(*cantidad_en_party)++;

	} else
		destructor_pokemon(pokemon_a_agregar);

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida, caja_pokemon debe ser no nulo, pokemon_leido debe contener todos sus válidos y completos
 * Post-condiciones: devuelve EXITO si un pokemon fue creado en la memoria dinamica con éxito y con los datos de pokemon_leido, e insertado con
 * 		éxito en la lista caja_pokemon, de otra manera devuelve ERROR
 */
int cargar_pokemon_en_caja(lista_t* caja_pokemon, pokemon_t pokemon_leido) {
	pokemon_t* pokemon_a_agregar = crear_pokemon_mem_dinamica(pokemon_leido);
	if (!pokemon_a_agregar)
		return ERROR;

	int resultado_operacion = lista_insertar(caja_pokemon, pokemon_a_agregar);
	if (resultado_operacion == ERROR) {
		destructor_pokemon(pokemon_a_agregar);
		return ERROR;
	}

	return resultado_operacion;
}

/* 
 * Pre-condiciones: personaje debe ser no nulo
 * Post-condiciones: coloca un puntero al pokemon correspondiente en la primera posición libre del vector personaje->party. Ejemplo: con
 * 		MAX_PARTY = 6, si la primera posición libre es 4 (o sea, elemento número 5) se le asignará el puntero de personaje->lista_caja_pokemones
 * 		correspondiente también al elemento en posición 4 (elemento número 5)
 */
void vincular_party_con_caja(personaje_t* personaje) {
	size_t posicion_libre = personaje->cantidad_en_party - 1;

	personaje->party[posicion_libre] = (pokemon_t*)lista_elemento_en_posicion(personaje->lista_caja_pokemones, posicion_libre);
}

/* 
 * Pre-condiciones: stdlib.h incluida, party debe ser no nulo y cantidad_en_party debe ser la cantidad definitiva de pokemones
 * Post-condiciones: devuelve EXITO si se redimensiona exitosamente el vector personaje->party a su número definitivo de pokemones, de otra manera
 * 		devuelve ERROR
 */
int redimensionar_party(pokemon_t*** party, size_t cantidad_en_party) {
	pokemon_t** party_aux = realloc(*party, cantidad_en_party * sizeof(pokemon_t*));
	if (!party_aux)
		return ERROR;

	*party = party_aux;

	return EXITO;
}

/* 
 * Pre-condiciones: stdio.h y stdlib.h incluidas, personaje y file_entrenador_principal deben ser no nulos
 * Post-condiciones: Si es la primera vez que es llamada esta función, los resultados pueden ser los siguientes: se devuelve éxito y personaje posee
 * 		el nombre encontrado en el archivo en su campo nombre, su campo fue_cargado ahora es true, en la lista_caja_pokemones se encuentran
 * 		todos los pokemones alojados en memoria dinámica y el vector party posee los punteros a los pokemones de los primeros MAX_PARTY pokemones de
 * 		la lista, se devuelve ERROR si alguna operación falló fatalmente, se devuelve ERROR_RECUPERABLE si hubo un posible error de formato o de lectura
 * 		de los datos, ó se devuelve ERROR_SIN_POKEMON si luega de la carga de datos el personaje no posee ningún pokemon.
 * 		Si no es la primera vez que se llama a esta función, se intentará sobreescribir el personaje, es decir, se inicializará otro personaje utilizando
 * 		el puntero personaje recibido por parámetro y se esperarán los resultados listados arriba.
 */
int cargar_entrenador_principal(personaje_t* personaje, FILE* file_entrenador_principal) {
	char identificador;
	int resultado_operacion = EXITO;
	pokemon_t pokemon_leido;

	if (personaje->fue_cargado) {
		resultado_operacion = inicializar_personaje(personaje);
		if (resultado_operacion == ERROR)
			return ERROR;
	}

	int leidos = fscanf(file_entrenador_principal, "%c;%[^\n]\n", &identificador, personaje->nombre);
	if ((leidos != 2) || (identificador != IDENTIFICADOR_ENTRENADOR))
		return ERROR_RECUPERABLE;

	personaje->fue_cargado = true;
	leidos = fscanf(file_entrenador_principal, "%c;%[^;];%i;%i;%i\n", &identificador, pokemon_leido.nombre, &(pokemon_leido.velocidad), &(pokemon_leido.ataque), &(pokemon_leido.defensa));

	while ((leidos == 5) && (identificador == IDENTIFICADOR_POKEMON) && (resultado_operacion == EXITO)) {
		resultado_operacion = cargar_pokemon_en_caja(personaje->lista_caja_pokemones, pokemon_leido);

		if ((resultado_operacion == EXITO) && (personaje->cantidad_en_party < MAX_PARTY)) {
			(personaje->cantidad_en_party)++;
			vincular_party_con_caja(personaje);
		}

		if (resultado_operacion == EXITO)
			leidos = fscanf(file_entrenador_principal, "%c;%[^;];%i;%i;%i\n", &identificador, pokemon_leido.nombre, &(pokemon_leido.velocidad), &(pokemon_leido.ataque), &(pokemon_leido.defensa));
	}

	if (personaje->cantidad_en_party == 0)
		return ERROR_SIN_POKEMON;

	if (personaje->cantidad_en_party < MAX_PARTY)
		resultado_operacion = redimensionar_party(&(personaje->party), personaje->cantidad_en_party);

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdio.h, string.h y stdlib incluidas, personaje y personaje_auxiliar deben ser no nulos
 * Post-condiciones: pide la ruta del personaje, si es correcta abre el archivo, si la apertura es exitosa se almacena la direccion de memoria de
 * 		personaje en personaje_auxilicar y carga los datos encontrados en el archivo en personaje. Si la llamada a esta función no es la primera,
 * 		se reemplaza el personaje anterior por el nuevo ingresado.
 * 		Devuelve ERROR_ARCHIVO si no fue posible abrir el archivo, devuelve ERROR_RECUPERABLE si sucedió un error de formato o de carga de datos
 * 		de la que se puede recuperar y volver a un estado correcto del programa, devuelve ERROR_SIN_POKEMON si el personaje del archivo no posee
 * 		ningún pokemon cargado, devuelve ERROR si se sufrió un error irrecuperable o fatal, y se devuelve EXITO si todo fue como esperado
 */
int ingresar_entrenador_principal(personaje_t* personaje, personaje_t* personaje_auxiliar) {
	char ruta_file[MAX_RUTA];
	printf(CYAN "\n  █ " AMARILLO "Se pide la ruta del archivo de texto (o nombre si está en esta misma carpeta) que contiene al\n    entrenador principal.\n" BLANCO);
	solicitar_ruta(ruta_file);

	FILE* file_entrenador_principal = fopen(ruta_file, "r");
	if (!file_entrenador_principal) {
		printf(ROJO "\n  █ Error: Imposible abrir el archivo solicitado.\n" NORMAL);
		return ERROR_ARCHIVO;
	}

	*personaje_auxiliar = *personaje;

	int resultado = cargar_entrenador_principal(personaje, file_entrenador_principal);
	if ((resultado == ERROR_RECUPERABLE) || (resultado == ERROR_SIN_POKEMON)) {
		printf(ROJO "\n  █ " AMARILLO "Error de Lectura: Datos no legibles en el archivo. Archivo no cargado.\n" NORMAL);
		fclose(file_entrenador_principal);
		return resultado;

	} else if (resultado == ERROR) {
		printf(ROJO "\n  █ Error Fatal: Corrupción, fallo o falta en la memoria.\n\n" NORMAL);
		fclose(file_entrenador_principal);
		return ERROR;
	}

	if (personaje_auxiliar->fue_cargado) {
		printf(VERDE "\n  █ " AMARILLO "Sustituyendo personaje '%s' por nuevo personaje '%s'.\n" BLANCO, personaje_auxiliar->nombre, personaje->nombre);
		pausar_juego(TIEMPO_PAUSA);
		destructor_personaje(personaje_auxiliar, DESTRUCCION_NORMAL);
	}

	printf(VERDE "\n  █ " AMARILLO "Datos del archivo '%s' leidos correctamente.\n" BLANCO, ruta_file);
	pausar_juego(TIEMPO_PAUSA);
	fclose(file_entrenador_principal);

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida, entrenador_dato debe contener datos válidos de un entrenador o líder
 * Post-condiciones: devuelve el puntero a un entrenador alojado en la memoria dinámica con los datos de entrenador_dato, o NULL de otra manera
 */
entrenador_t* crear_entrenador_mem_dinamica(entrenador_t entrenador_dato) {
	entrenador_t* entrenador_definitivo = calloc(1, sizeof(entrenador_t));
	if (!entrenador_definitivo)
		return NULL;

	*entrenador_definitivo = entrenador_dato;

	return entrenador_definitivo;
}

/* 
 * Pre-condiciones: stdlib.h incluida, gimnasio_dato debe tener datos válidos de un gimnasio
 * Post-condiciones: devuelve el puntero a un gimnasio alojado en la memoria dinámica que contiene los mismo datos que gimnasio_dato, si hubo un
 * 		error se devuelve NULL
 */
gimnasio_t* crear_gimnasio_mem_dinamica(gimnasio_t gimnasio_dato) {
	gimnasio_t* gimnasio_definitivo = calloc(1, sizeof(gimnasio_t));
	if (!gimnasio_definitivo)
		return NULL;

	*gimnasio_definitivo = gimnasio_dato;

	return gimnasio_definitivo;
}

/* 
 * Pre-condiciones: stdlib.h incluida, gimnasio debe haber sido inicializado y ser válido, lider_leido debe contener datos válidos de un entrenador
 *  	que debe ser el líder
 * Post-condiciones: devuelve EXITO si se creó un entrenador líder en la memoria dinámica siendo asignado al campo líder del gimnasio recibido, 
 * 		devuelve ERROR_ARCHIVO si el líder no contiene ningún pokemon, o devuelve ERROR de otra manera
 */
int cargar_lider(gimnasio_t* gimnasio, entrenador_t lider_leido) {
	if (lider_leido.cantidad_en_party == 0)
		return ERROR_ARCHIVO;

	entrenador_t* lider_definitivo = crear_entrenador_mem_dinamica(lider_leido);
	if (!lider_definitivo)
		return ERROR;

	gimnasio->lider = lider_definitivo;

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida, pila_entrenadores debe ser válida y creada correctamente, entrenador_leido debe contener datos de un entrenador
 * Post-condiciones: devuelve EXITO si un entrenador alojado en la memoria dinámica es creado con los datos de entrenador_leido e insertado en la
 * 		 pila_entrenadores, devuelve ERROR de otra manera
 */
int cargar_entrenador_en_pila(lista_t* pila_entrenadores, entrenador_t entrenador_leido) {
	entrenador_t* entrenador_definitivo = crear_entrenador_mem_dinamica(entrenador_leido);
	if (!entrenador_definitivo)
		return ERROR;

	int resultado_operacion = lista_apilar(pila_entrenadores, entrenador_definitivo);
	if (resultado_operacion == ERROR) {
		destructor_entrenador(entrenador_definitivo);
		return ERROR;
	}

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h incluida, heap_gimnasios debe ser un heap válido, gimnasio_leido debe contener datos válidos de un gimnasio
 * Post-condiciones: devuelve EXITO si pudo crear un gimnasio en la memoria dinámica con los datos de gimnasio_leido y si pudo insertarlo en el
 * 		heap_gimnasios, de otra manera devuelve ERROR
 */
int cargar_gimnasio_en_heap(heap_t* heap_gimnasios, gimnasio_t gimnasio_leido) {
	gimnasio_t* gimnasio_definitivo = crear_gimnasio_mem_dinamica(gimnasio_leido);
	if (!gimnasio_definitivo)
		return ERROR;

	int resultado_operacion = heap_insertar(heap_gimnasios, gimnasio_definitivo);
	if (resultado_operacion == ERROR) {
		destructor_gimnasio(gimnasio_definitivo);
		return ERROR;
	}

	return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h, stdbool.h y stdio.h incluidas, entrenador_leido, file_gimnasios e identificador_general deben ser no nulos y válidos,
 * 		además entrenador_leido debe haber sido inicializado
 * Post-condiciones: devuelve EXITO si pudo cargar 1 o más pokemones (hasta un máximo de MAX_PARTY descartando los siguientes) en la party de
 * 		entrenador_leido exitosamente alojándolos en la memoria dinámica redimensionando el tamaño del vector party acorde al tamaño definitivo
 * 		del mismo con cantidad_en_party, identificador_general posee el úlimo caracter leído de file_gimnasios.
 * 		Devuelve ERROR_SIN_POKEMON si después de intentar cargar los pokemones el único problema es que ningún pokemon fue cargado al entrenador.
 * 		Devuelve ERROR_FORMATO si no pudo leer un IDENTIFICADOR_POKEMON al principio
 * 		Devuelve ERROR de otra manera si hubo un error fatal
 */
int cargar_pokemones(entrenador_t* entrenador_leido, FILE* file_gimnasios, char* identificador_general) {
	pokemon_t pokemon_leido;
	int resultado_operacion = EXITO;
	bool debe_parar_lectura = false;

	int leidos = fscanf(file_gimnasios, "%c;", identificador_general);
	if ((leidos != 1) || (*identificador_general != IDENTIFICADOR_POKEMON))
		return ERROR_FORMATO;

	leidos = fscanf(file_gimnasios, "%[^;]; %i; %i; %i\n", pokemon_leido.nombre, &(pokemon_leido.velocidad), &(pokemon_leido.ataque), &(pokemon_leido.defensa));

	while ((leidos == 4) && (resultado_operacion == EXITO) && (*identificador_general == IDENTIFICADOR_POKEMON) && (!debe_parar_lectura)) {
		if (entrenador_leido->cantidad_en_party < MAX_PARTY)
			resultado_operacion = cargar_pokemon_en_party(entrenador_leido->party, &(entrenador_leido->cantidad_en_party), pokemon_leido);

		if (resultado_operacion == EXITO) {
			leidos = fscanf(file_gimnasios, "%c;", identificador_general);

			if ((leidos != 1) || (*identificador_general != IDENTIFICADOR_POKEMON))
				debe_parar_lectura = true;
			else
				leidos = fscanf(file_gimnasios, "%[^;]; %i; %i; %i\n", pokemon_leido.nombre, &(pokemon_leido.velocidad), &(pokemon_leido.ataque), &(pokemon_leido.defensa));
		}
	}

	if (entrenador_leido->cantidad_en_party == 0)
		return ERROR_SIN_POKEMON;

	resultado_operacion = redimensionar_party(&(entrenador_leido->party), entrenador_leido->cantidad_en_party);

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdbool.h incluida
 * Post-condiciones: le asigna identificador_general a identificador_peleador si identificador_general es de un peleador
 */
void actualizar_identificadores(char identificador_general, char* identificador_peleador) {
	if (es_identificador_de_peleador(identificador_general))
		*identificador_peleador = identificador_general;
}

/* 
 * Pre-condiciones: stdlib.h incluida, los datos recibidos recibidos como parámetros deben haber sido cargados por lo menos una vez, 
 * 		file_gimnasios debe ser un stream de archivo válido
 * Post-condiciones: Si resultado_operacion ingresó como ERROR_ARCHIVO no se realiza ningún cambio
 * 		Si resultado_carga ingresó como ERROR_RECUPERABLE, se destruye el entrenador recibido y resultado_operacion termina siendo EXITO
 * 		Si resultado_carga ingresó como ERROR, se destruye el entrenador recibido y resultado_operacion termina siendo ERROR
 * 		Si resultado_carga ingresó como ERROR_SIN_POKEMON o como ERROR_FORMATO, se destruye el entrenador recibido y resultado_operacion
 * 		 termina siendo EXITO, además, si cant_entrenadores es mayor estricto a cero, se lee el siguiente caracter de file_gimnasios dejándolo 
 * 		 guardado en identificador_general
 */
void manejar_errores_cargar_peleador(int* resultado_operacion, int resultado_carga, entrenador_t entrenador, size_t cant_entrenadores, int* leidos_recibido, FILE* file_gimnasios, char* identificador_general) {
	if (*resultado_operacion == ERROR_ARCHIVO){
		destructor_entrenador_stack(entrenador);
		return;
	}

	if (resultado_carga == ERROR_RECUPERABLE) {
		destructor_entrenador_stack(entrenador);
		*resultado_operacion = EXITO;

	} else if (resultado_carga == ERROR) {
		destructor_entrenador_stack(entrenador);
		*resultado_operacion = ERROR;

	} else if ((resultado_carga == ERROR_SIN_POKEMON) || (resultado_carga == ERROR_FORMATO)) {
		destructor_entrenador_stack(entrenador);
		if (cant_entrenadores > 0)
			*leidos_recibido = fscanf(file_gimnasios, "%c;", identificador_general);
		*resultado_operacion = EXITO;
	}
}

/* 
 * Pre-condiciones: stdlib.h, stdbool.h y stdio.h incluidas, los punteros recibidos deben ser no nulos y válidos, file_gimnasios debe ser un stream de
 * 		archivo válido, gimnasio_leido tuvo que haber sido inicializado
 * Post-condiciones: devuelve EXITO si encontró un entrenador en el punto de lectura establecido en file_gimnasios, si leyó y cargó los datos de sus
 * 		campos correctamente, cargándolo en la pila de entrenadores o como líder del gimnasio exitosamente dependiendo de lo que corresponda.
 * 		Devuelve ERROR_ARCHIVO si hubo un error con el formato o la lectura del archivo.
 * 		Devuelve ERROR_RECUPERABLE si sucedió un error del cual se puede recuperar luego.
 * 		Devuelve ERROR si hubo un error fatal.
 */
int cargar_peleador(gimnasio_t* gimnasio_leido, FILE* file_gimnasios, char* identificador_peleador, char* identificador_general, int* leidos_recibido) {
	int resultado_operacion = EXITO, resultado_carga = EXITO;
	int leidos;
	size_t cant_entrenadores = lista_elementos(gimnasio_leido->pila_entrenadores);

	entrenador_t entrenador_leido;
	resultado_operacion = inicializar_entrenador(&entrenador_leido);
	if (resultado_operacion == ERROR)
		return ERROR;

	actualizar_identificadores(*identificador_general, identificador_peleador);

	if (es_valido_cargar_peleador(gimnasio_leido->pila_entrenadores, gimnasio_leido->lider, *identificador_peleador)) {
		leidos = fscanf(file_gimnasios, "%[^\n]\n", entrenador_leido.nombre);
		if (leidos != 1)
			resultado_operacion = ERROR_RECUPERABLE;

		if (resultado_operacion == EXITO)
			resultado_carga = cargar_pokemones(&entrenador_leido, file_gimnasios, identificador_general);

		if ((resultado_carga == ERROR_FORMATO) && es_valido_cargar_lider(gimnasio_leido->pila_entrenadores, *identificador_peleador))
			resultado_operacion = ERROR_ARCHIVO;

		if ((resultado_carga == EXITO) && es_valido_cargar_lider(gimnasio_leido->pila_entrenadores, *identificador_peleador))
			resultado_operacion = cargar_lider(gimnasio_leido, entrenador_leido);

		else if ((resultado_carga == EXITO) && es_valido_cargar_entrenador(gimnasio_leido->lider, *identificador_peleador))
			resultado_operacion = cargar_entrenador_en_pila(gimnasio_leido->pila_entrenadores, entrenador_leido);

	} else
		return ERROR_RECUPERABLE;

	manejar_errores_cargar_peleador(&resultado_operacion, resultado_carga, entrenador_leido, cant_entrenadores, leidos_recibido, file_gimnasios, identificador_general);

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdio.h y stdbool.h incluidas, file_gimnasios debe ser un stream de archivo válido, los punteros deben ser no nulos y válidos
 * Post-condiciones: devuelve EXITO si se encontró otro gimnasio, guardando el último caracter leído en identificador (que debería ser igual a 
 * 		IDENTIFICADOR_GIMNASIO)
 * 		Devuelve ERROR_ARCHIVO si no se leyó nada más o si se llegó al final del archivo
 */
int leer_hasta_siguiente_gimnasio(FILE* file_gimnasios, int* leidos, char* identificador) {
	bool debo_parar = false;

	while (debo_parar == false) {
		*leidos = fscanf(file_gimnasios, "%c", identificador);
		if ((*leidos == EOF) || (*leidos == 0))
			debo_parar = true;

		else if (*identificador == IDENTIFICADOR_GIMNASIO) {
			char caracter_leido;
			int leidos_especial = fscanf(file_gimnasios, "%c", &caracter_leido);

			if ((leidos_especial == 1) && (caracter_leido == ';'))
				debo_parar = true;
		}
	}

	if ((*leidos == EOF) || (*leidos == 0))
		return ERROR_ARCHIVO;
	else
		return EXITO;
}

/* 
 * Pre-condiciones: stdlib.h, stdbool.h y stdio.h incluidas, los punteros recibidos deben ser no nulos y válidos, file_gimnasios debe ser un stream de
 * 		archivo válido, gimnasio_leido tuvo que haber sido inicializado
 * Post-condiciones: Si no se tiene líder y el identificador es IDENTIFICADOR_ENTRENADOR se lee hasta (si es que hay) el siguiente gimnasio,
 * 		resultado_operacion contiene EXITO si se encontró otro gimnasio o ERROR_ARCHIVO si no.
 * 		Si solamente no se tiene líder, se destruye el gimnasio y se lo inicializa como nuevo, resultado_operacion conteniendo el resultado de la
 * 		inicialización
 * 		Si se encontró otro gimnasio se leen sus características y se guardan en el gimnasio, y si no se intenta leer la siguiente línea en una
 * 		última búsqueda por un nuevo gimnasios. Si no se cumplieron ninguna de estas cosas debe_parar_lectura queda en true
 */
int rescatar_de_error_gimnasio(gimnasio_t* gimnasio, FILE* file_gimnasios, char nombre_leido[MAX_NOMBRE], int* leidos, char* identificador, bool* debe_parar_lectura) {
	int resultado_operacion = ERROR;
	if (gimnasio->lider == NULL && *identificador == IDENTIFICADOR_ENTRENADOR) {
		resultado_operacion = leer_hasta_siguiente_gimnasio(file_gimnasios, leidos, identificador);

	} else if (gimnasio->lider == NULL) {
		destructor_gimnasio_stack(*gimnasio);
		resultado_operacion = inicializar_gimnasio(gimnasio);
	}

	if ((resultado_operacion == EXITO) && (*identificador == IDENTIFICADOR_GIMNASIO))
		*leidos = fscanf(file_gimnasios, "%[^;]; %i; %i\n", nombre_leido, &(gimnasio->dificultad), &(gimnasio->id_puntero_funcion));

	else if (resultado_operacion == EXITO) {
		*leidos = fscanf(file_gimnasios, "%c;", identificador);

		if ((*leidos == 1) && (*identificador == IDENTIFICADOR_GIMNASIO))
			*leidos = fscanf(file_gimnasios, "%[^;]; %i; %i\n", nombre_leido, &(gimnasio->dificultad), &(gimnasio->id_puntero_funcion));

	} else
		*debe_parar_lectura = true;

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdlib.h, stdio.h y stdbool.h incluidas, file_gimnasios debe ser un stream de archivo válido, gimnasio_leido debió haber sido
 * 		inicializado
 * Post-condiciones: devuelve EXITO si pudo cargar de manera exitosa y válida los campos de gimnasio_leido, con al menos un líder.
 * 		Devuelve ERROR si hubo un error fatal. Devuelve ERROR_RECUPERABLE si se puede recuperar del error luego
 * 		Devuelve ERROR_ARCHIVO si hubo un problema con la lectura o la interpretación del formato de file_gimnasios
 */
int leer_gimnasio(gimnasio_t* gimnasio_leido, FILE* file_gimnasios, char nombre_leido[MAX_NOMBRE], int* leidos_gimnasio, bool* debe_parar_lectura_gims) {
	char identificador_peleador = IDENTIFICADOR_ERROR_LECTURA, identificador_general = IDENTIFICADOR_ERROR_LECTURA;
	int resultado_operacion = EXITO;
	bool debe_parar_lectura = false;

	strcpy(gimnasio_leido->nombre, nombre_leido);

	int leidos = fscanf(file_gimnasios, "%c;", &identificador_peleador);
	if ((leidos != 1) || !es_identificador_de_peleador(identificador_peleador))
		return ERROR_RECUPERABLE;

	resultado_operacion = cargar_peleador(gimnasio_leido, file_gimnasios, &identificador_peleador, &identificador_general, &leidos);

	if ((gimnasio_leido->lider == NULL) && (resultado_operacion != ERROR_ARCHIVO)) {
		resultado_operacion = rescatar_de_error_gimnasio(gimnasio_leido, file_gimnasios, nombre_leido, leidos_gimnasio, &identificador_peleador, debe_parar_lectura_gims);
		return resultado_operacion;
	}

	while ((identificador_general == IDENTIFICADOR_ENTRENADOR) && (resultado_operacion == EXITO) && (!debe_parar_lectura)) {
		if ((leidos != 1) || (!es_identificador_de_peleador(identificador_peleador)))
			debe_parar_lectura = true;

		if (!debe_parar_lectura)
			resultado_operacion = cargar_peleador(gimnasio_leido, file_gimnasios, &identificador_peleador, &identificador_general, &leidos);
	}
	return resultado_operacion;
}

/* 
 * Pre-condiciones: todos los punteros recibidos deben ser válidos, file_gimnasios debe ser un stream de archivo válido
 * Post-condiciones: devuelve EXITO si pudo inicializar el gimnasio_leido, asigna true a fue_reinicializado, si identificador_general no es
 * 		IDENTIFICADOR_GIMNASIO asigna true a debe_parar_lectura, y si sí es, lee la siguiente línea de file_gimnasios guardando los datos en
 * 		gimnasio_leido. Devuelve ERROR si no pudo inicializar el gimnasio
 */
int preparar_siguiente_carga_gimnasio(gimnasio_t* gimnasio_leido, FILE* file_gimnasios, char nombre_leido[MAX_NOMBRE], char* identificador_general, int* leidos, bool* debe_parar_lectura, bool* fue_reinicializado) {
	int resultado_operacion = inicializar_gimnasio(gimnasio_leido);

	if (resultado_operacion == EXITO) {
		*fue_reinicializado = true;

		if (*identificador_general != IDENTIFICADOR_GIMNASIO)
			*debe_parar_lectura = true;
		else
			*leidos = fscanf(file_gimnasios, "%[^;]; %i; %i\n", nombre_leido, &(gimnasio_leido->dificultad), &(gimnasio_leido->id_puntero_funcion));
	}

	return resultado_operacion;
}

/* 
 * Pre-condiciones: todos los datos recibidos deben haber sido inicializados y válidos
 * Post-condiciones: destruye el gimnasio_leido si ciclo_una_vez es false, ó fue_reinicializado es true, ó debe_parar_lectura es true
 */
void destruir_gimnasio_condicionalmente(gimnasio_t gimnasio_leido, int resultado_operacion, bool fue_reinicializado, bool ciclo_una_vez, bool debe_parar_lectura) {
	if ((!ciclo_una_vez) || fue_reinicializado || debe_parar_lectura)
		destructor_gimnasio_stack(gimnasio_leido);
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h incluidas, heap_gimnasios debe ser un heap válido, file_gimnasios debe ser un stream de archivo válido
 * Post-condiciones: devuelve EXITO si cargó de manera exitosa todos los gimnasios correctos del archivo, agregándolos al heap de manera correcta,
 * 		cargando los campos de cada gimnasio leido de manera exitosa.
 * 		Devuelve ERROR_ARCHIVO si hubo un error con el formato o la lectura del archivo.
 * 		Devuelve ERROR_RECUPERABLE si sucedió un error del cual se puede recuperar luego.
 * 		Devuelve ERROR si hubo un error fatal.
 */
int cargar_gimnasios(heap_t* heap_gimnasios, FILE* file_gimnasios) {
	char identificador_general;
	int resultado_operacion = EXITO;
	char nombre_leido[MAX_NOMBRE];
	bool debe_parar_lectura = false, fue_reinicializado = false, ciclo_una_vez = false;

	gimnasio_t gimnasio_leido;
	resultado_operacion = inicializar_gimnasio(&gimnasio_leido);
	if (resultado_operacion == ERROR)
		return ERROR;

	int leidos = fscanf(file_gimnasios, "%c;", &identificador_general);
	if ((leidos != 1) || (identificador_general != IDENTIFICADOR_GIMNASIO)) {
		destructor_gimnasio_stack(gimnasio_leido);
		return ERROR_RECUPERABLE;
	}

	leidos = fscanf(file_gimnasios, "%[^;]; %i; %i\n", nombre_leido, &(gimnasio_leido.dificultad), &(gimnasio_leido.id_puntero_funcion));
	if (leidos != 3) {
		destructor_gimnasio_stack(gimnasio_leido);
		return ERROR_ARCHIVO;
	}

	while ((leidos == 3) && (identificador_general == IDENTIFICADOR_GIMNASIO) && (resultado_operacion == EXITO) && (!debe_parar_lectura)) {
		resultado_operacion = leer_gimnasio(&gimnasio_leido, file_gimnasios, nombre_leido, &leidos, &debe_parar_lectura);

		if (resultado_operacion == EXITO) {
			ciclo_una_vez = true;
			resultado_operacion = cargar_gimnasio_en_heap(heap_gimnasios, gimnasio_leido);
		}

		if (resultado_operacion == EXITO)
			resultado_operacion = preparar_siguiente_carga_gimnasio(&gimnasio_leido, file_gimnasios, nombre_leido, &identificador_general, &leidos, &debe_parar_lectura, &fue_reinicializado);
	}

	destruir_gimnasio_condicionalmente(gimnasio_leido, resultado_operacion, fue_reinicializado, ciclo_una_vez, debe_parar_lectura);

	if ((resultado_operacion == EXITO) && (!ciclo_una_vez))
		resultado_operacion = ERROR;

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdlib.h, stdio.h y stdbool.h incluidas, heap_gimnasios debe ser un heap creado válidamente y no nulo
 * Post-condiciones: Se imprime y pide por pantalla la ruta del archivo desde donde desea cargar el/los gimnasio/s. Si es una ruta imposible de abrir
 * 		se devulve ERROR_ARCHIVO. Si es correcta se procede a cargar los gimnasios presentes en el archivo, si hubieros errores de lectura se informa
 * 		por pantalla y se devuelve EXITO, si hubo un error fatal se devuelve ERROR. Si todo fue de manera exitosa en el heap_gimnasios ingresado se
 * 		tienen cargados los gimnasios minimalmente por dificultad, con datos válidos y correctos, almacenados en la memoria dinámica y se devuelve
 * 		EXITO
 */
int agregar_gimnasios(heap_t* heap_gimnasios) {
	char ruta_file[MAX_RUTA];
	printf(CYAN "\n  █ " AMARILLO "Se pide la ruta del archivo de texto (o nombre si está en esta misma carpeta) que contiene a\n    un o a varios gimnasios.\n" BLANCO);
	solicitar_ruta(ruta_file);

	FILE* file_gimnasios = fopen(ruta_file, "r");
	if (!file_gimnasios) {
		printf(ROJO "\n  █ Error: Imposible abrir el archivo solicitado.\n" NORMAL);
		pausar_juego(TIEMPO_PAUSA);
		return ERROR_ARCHIVO;
	}

	int resultado = cargar_gimnasios(heap_gimnasios, file_gimnasios);
	if ((resultado == ERROR_RECUPERABLE) || (resultado == ERROR_ARCHIVO)) {
		printf(ROJO "\n  █ " AMARILLO "Error de Lectura: Datos no legibles en el archivo. Archivo no cargado.\n" NORMAL);
		pausar_juego(TIEMPO_PAUSA);
		fclose(file_gimnasios);
		return EXITO;

	} else if (resultado == ERROR) {
		printf(ROJO "\n  █ Error Fatal: Corrupción, fallo o falta en la memoria.\n\n" NORMAL);
		pausar_juego(TIEMPO_PAUSA);
		fclose(file_gimnasios);
		return ERROR;
	}

	printf(VERDE "\n  █ " AMARILLO "Datos del archivo '%s' leidos correctamente.\n" BLANCO, ruta_file);
	pausar_juego(TIEMPO_PAUSA);

	fclose(file_gimnasios);
	return EXITO;
}

/* 
 * Pre-condiciones: stdio.h incluida, el personaje debe ser válido y debe estar cargado
 * Post-condiciones: imprime por pantalla todos los pokemones del personaje junto con sus estadísticas, mostrando cuáles son de la party
 */
void imprimir_total_pokemones_personaje(personaje_t* personaje, size_t cant_total_pokemones) {
	system("clear");
	imprimir_comienzo_bloque(COLOR_AZUL);
	printf(BLANCO "\n     » Pokemones para batalla (party): %zu\n     » Pokemones en total: %zu\n\n", personaje->cantidad_en_party, cant_total_pokemones);

	for (size_t i = 0; i < cant_total_pokemones; i++) {
		pokemon_t pokemon = *((pokemon_t*)lista_elemento_en_posicion(personaje->lista_caja_pokemones, i));
		imprimir_pokemon_personaje(pokemon, i + 1);
	}

	printf("\n\n     Nota: El símbolo " CYAN "%s" BLANCO " representa que el Pokemon forma parte de la party del entrenador.\n\n", PERTENECE_A_PARTY);
	imprimir_fin_bloque(COLOR_AZUL);
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: resta 1 a cada elemento del vector
 */
void pasar_a_posiciones(int vector[], int tope) {
	for (int i = 0; i < tope; i++) {
		vector[i] -= 1;
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: asigna cero a cada elemento del vector
 */
void inicializar_posiciones(int vector[], int tope) {
	for (int i = 0; i < tope; i++) {
		vector[i] = 0;
	}
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla las instrucciones a seguir para introducir la nueva configuración de la party
 */
void imprimir_instrucciones_cambio_party() {
	printf(CYAN "\n  █ " AMARILLO "Para ingresar la nueva composición de la party, ingrese por favor una cadena de números enteros positivos\n");
	printf("    con el siguiente formato con números de ejemplo: '2,3,4,5,6,7'. Significando esto que los pokemones que \n");
	printf("    visualiza en las posiciones 2,3,4,5,6,7 serán los nuevos miembros 1,2,3,4,5,6 de la party. Debe colocar\n");
	printf("    la misma cantidad de números que de pokemones en la party existen (se reemplaza la party entera). La \n");
	printf("    cantidad máxima que pueden entrar en una party es de: %i. El formato debe ser el indicado (separado por \n", MAX_PARTY);
	printf("    comas sin dejar espacios entre medio y sin las comillas). De ser incorrecto, no se efectuarán cambios.\n");
}

/* 
 * Pre-condiciones: stdio.h incluida, los parámetros deben ser válidos
 * Post-condiciones: pide un set de posiciones para la nueva party, si se ingresa un set erróneo o inválido se pide de nuevo hasta que el ingreso
 * 		sea correcto, el set final se guearda en el vector nuevos_pokemones_party
 */
void obtener_nuevas_posiciones_party(size_t nuevos_pokemones_party[MAX_PARTY], size_t cant_total_pokemones) {
	int pos_ingresadas[MAX_PARTY] = {0};
	int resultado_obtencion = ERROR;

	printf("\n    Ingrese su nueva party:  " BLANCO);
	int leidos = scanf("%i,%i,%i,%i,%i,%i", &pos_ingresadas[0], &pos_ingresadas[1], &pos_ingresadas[2], &pos_ingresadas[3], &pos_ingresadas[4], &pos_ingresadas[5]);

	pasar_a_posiciones(pos_ingresadas, MAX_PARTY);

	while (resultado_obtencion == ERROR) {
		if (leidos != 6)
			printf(ROJO "\n  █ " AMARILLO "Cantidad de posiciones ingresadas inválida o formato inválido.\n" BLANCO);
		else if (hay_posicion_negativa(pos_ingresadas, MAX_PARTY))
			printf(ROJO "\n  █ " AMARILLO "Una o varias de las posiciones ingresadas son negativas o cero (se comienza desde 1).\n" BLANCO);
		else if (hay_posicion_inexistente(pos_ingresadas, MAX_PARTY, cant_total_pokemones))
			printf(ROJO "\n  █ " AMARILLO "Una o varias de las posiciones ingresadas son inexistentes (no corresponden a un pokemon).\n" BLANCO);
		else if (hay_posicion_repetida(pos_ingresadas, MAX_PARTY))
			printf(ROJO "\n  █ " AMARILLO "Una o varias de las posiciones ingresadas están repetidas. Cada posición debe ser única.\n" BLANCO);
		else
			resultado_obtencion = EXITO;

		if (resultado_obtencion == ERROR) {
			inicializar_posiciones(pos_ingresadas, MAX_PARTY);

			printf(CYAN "\n  █ " AMARILLO "Por favor ingrese de nuevo su nueva party con formato correcto:  " BLANCO);
			leidos = scanf("%i,%i,%i,%i,%i,%i", &pos_ingresadas[0], &pos_ingresadas[1], &pos_ingresadas[2], &pos_ingresadas[3], &pos_ingresadas[4], &pos_ingresadas[5]);

			pasar_a_posiciones(pos_ingresadas, MAX_PARTY);
		}
	}

	for (int i = 0; i < MAX_PARTY; i++) {
		nuevos_pokemones_party[i] = (size_t)pos_ingresadas[i];
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: posiciones_para_caja posee las posiciones del vector nuevos_pokemones_party que cumplen la condición para ser almacenados
 *  	en la caja y no reaparecer en la party
 */
void obtener_posiciones_para_caja(size_t cantidad_party, size_t nuevos_pokemones_party[MAX_PARTY], size_t posiciones_para_caja[MAX_PARTY], int* tope_para_caja) {
	bool debo_moverlo_a_caja = true;
	size_t pos_a_mover;

	for (size_t i = 0; i < cantidad_party; i++) {
		for (size_t j = 0; j < cantidad_party; j++) {
			pos_a_mover = nuevos_pokemones_party[j];
			if (i == pos_a_mover)
				debo_moverlo_a_caja = false;
		}

		if (debo_moverlo_a_caja == true) {
			posiciones_para_caja[*tope_para_caja] = i;
			(*tope_para_caja)++;
		}
		debo_moverlo_a_caja = true;
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: posiciones_para_eliminar posee las posiciones del vector nuevos_pokemones_party que cumplen la condición para ser eliminados de la
 * 		caja y reinsertados formando parte de la nueva party
 */
void obtener_posiciones_para_eliminar(size_t cantidad_party, size_t nuevos_pokemones_party[MAX_PARTY], size_t posiciones_para_eliminar[MAX_PARTY], int* tope_para_eliminar) {
	bool debo_eliminarlo = true;
	size_t pos_a_mover;

	for (size_t k = 0; k < cantidad_party; k++) {
		pos_a_mover = nuevos_pokemones_party[k];

		for (size_t l = 0; l < cantidad_party; l++) {
			if (pos_a_mover == l)
				debo_eliminarlo = false;
		}

		if (debo_eliminarlo == true) {
			posiciones_para_eliminar[*tope_para_eliminar] = pos_a_mover;
			(*tope_para_eliminar)++;
		}

		debo_eliminarlo = true;
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: party_original_dato posee los pokemones (no punteros) de party_original
 */
void cargar_party_original_dato(pokemon_t party_original_dato[], size_t cantidad_party, pokemon_t* party_original[]) {
	for (size_t i = 0; i < cantidad_party; i++) {
		party_original_dato[i] = *party_original[i];
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos, lista_caja_pokemones debe ser una lista válida
 * Post-condiciones: party_nueva_dato posee los pokemones (no punteros) que representa a la party nueva elegida por el usuario
 */
void cargar_party_nueva_dato(pokemon_t party_nueva_dato[], size_t cantidad_party, lista_t* lista_caja_pokemones, size_t nuevos_pokemones_party[]) {
	for (size_t i = 0; i < cantidad_party; i++) {
		pokemon_t* pokemon = (pokemon_t*)lista_elemento_en_posicion(lista_caja_pokemones, nuevos_pokemones_party[i]);
		party_nueva_dato[i] = *pokemon;
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos, lista_caja_pokemones debe ser una lista válida
 * Post-condiciones: devuelve EXITO si inserta en lista_caja_pokemones (al final) todos los pokemones de posiciones posiciones_para_caja, devuelve
 * 		ERROR si ocurrió algún error
 */
int insertar_en_caja(size_t posiciones_para_caja[], int tope_para_caja, pokemon_t party_original_dato[], lista_t* lista_caja_pokemones) {
	int resultado = EXITO;
	pokemon_t* pokemon = NULL;

	for (size_t i = 0; i < tope_para_caja; i++) {
		if (resultado == EXITO) {
			size_t posicion = posiciones_para_caja[i];

			pokemon = crear_pokemon_mem_dinamica(party_original_dato[posicion]);

			if (pokemon)
				resultado = lista_insertar(lista_caja_pokemones, pokemon);
			else
				resultado = ERROR;
		}
	}

	if ((resultado == ERROR) && pokemon)
		destructor_pokemon(pokemon);

	return resultado;
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos, lista_caja_pokemones debe ser una lista válida
 * Post-condiciones: devuelve EXITO si inserta en lista_caja_pokemones (al inicio) todos los pokemones de datos party_nueva_dato, devuelve
 * 		ERROR si ocurrió algún error
 */
int insertar_nueva_party(pokemon_t party_nueva_dato[], int cantidad_party, lista_t* lista_caja_pokemones) {
	int resultado = EXITO;

	for (int i = (cantidad_party - 1); i >= 0; i--) {
		if (resultado == EXITO) {
			pokemon_t* pokemon = crear_pokemon_mem_dinamica(party_nueva_dato[i]);
			if (pokemon)
				resultado = lista_insertar_en_posicion(lista_caja_pokemones, pokemon, 0);
			else
				resultado = ERROR;
		}
	}

	return resultado;
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos
 * Post-condiciones: ordena el vector con sus elementos de tipo size_t de mayor a menor
 */
void ordenar_vector_mayor_a_menor(size_t vector[], int tope) {
	size_t i, j, aux;

	for (i = 1; i < tope; i++) {
		j = i;
		aux = vector[i];

		while ((j > 0) && (aux > vector[j - 1])) {
			vector[j] = vector[j - 1];
			j--;
		}

		vector[j] = aux;
	}
}

/* 
 * Pre-condiciones: los parámetros recibidos deben ser válidos, lista_caja_pokemones debe ser una lista válida
 * Post-condiciones: devuelve EXITO si se eliminaron todas las posiciones de posiciones_para_eliminar de lista_caja_pokemones exitosamente, de otra
 * 		manera se devuelve ERROR
 */
int eliminar_posiciones_pendientes(size_t posiciones_para_eliminar[], int tope_para_eliminar, lista_t* lista_caja_pokemones) {
	int resultado = EXITO;

	//Se ordena de esta manera para no modificar los valores de las posiciones relativas
	ordenar_vector_mayor_a_menor(posiciones_para_eliminar, tope_para_eliminar);

	for (size_t i = 0; i < tope_para_eliminar; i++) {
		if (resultado == EXITO)
			resultado = lista_borrar_de_posicion(lista_caja_pokemones, posiciones_para_eliminar[i]);
	}

	return resultado;
}

/* 
 * Pre-condiciones: personaje debe ser válido y no nulo
 * Post-condiciones: la party de personaje tiene reasignados los primeros cantidad_en_party elementos de la lista_caja_pokemones
 */
void actualizar_party_en_personaje(personaje_t* personaje) {
	size_t cantidad_party = personaje->cantidad_en_party;

	for (size_t i = 0; i < cantidad_party; i++) {
		personaje->party[i] = (pokemon_t*)lista_elemento_en_posicion(personaje->lista_caja_pokemones, i);
	}
}

/* 
 * Pre-condiciones: personaje debe ser válido y no nulo, nuevos_pokemones_party ya debe contener los datos cargados correctos obtenidos por el usuario
 * Post-condiciones: la party del personaje contiene los punteros a los pokemones acorde a las posiciones que se ingresaron en nuevos_pokemones_party
 * 		habiéndose realizado los movimientos, eliminaciones y reestructuraciones correspondientes
 */
int realizar_movimiento_de_party(personaje_t* personaje, size_t nuevos_pokemones_party[MAX_PARTY]) {
	int resultado;
	size_t cantidad_party = personaje->cantidad_en_party;

	size_t posiciones_para_caja[MAX_PARTY] = {0};
	int tope_para_caja = 0;

	size_t posiciones_para_eliminar[MAX_PARTY] = {0};
	int tope_para_eliminar = 0;

	obtener_posiciones_para_caja(cantidad_party, nuevos_pokemones_party, posiciones_para_caja, &tope_para_caja);
	obtener_posiciones_para_eliminar(cantidad_party, nuevos_pokemones_party, posiciones_para_eliminar, &tope_para_eliminar);

	pokemon_t party_original_dato[MAX_PARTY];
	cargar_party_original_dato(party_original_dato, cantidad_party, personaje->party);

	pokemon_t party_nueva_dato[MAX_PARTY];
	cargar_party_nueva_dato(party_nueva_dato, cantidad_party, personaje->lista_caja_pokemones, nuevos_pokemones_party);

	resultado = insertar_en_caja(posiciones_para_caja, tope_para_caja, party_original_dato, personaje->lista_caja_pokemones);
	if (resultado == ERROR)
		return ERROR;

	for (size_t i = 0; i < cantidad_party; i++) {
		if (resultado == EXITO)
			resultado = lista_borrar_de_posicion(personaje->lista_caja_pokemones, 0);
	}
	if (resultado == ERROR)
		return ERROR;

	resultado = insertar_nueva_party(party_nueva_dato, (int)cantidad_party, personaje->lista_caja_pokemones);
	if (resultado == ERROR)
		return ERROR;

	resultado = eliminar_posiciones_pendientes(posiciones_para_eliminar, tope_para_eliminar, personaje->lista_caja_pokemones);
	if (resultado == ERROR)
		return ERROR;

	actualizar_party_en_personaje(personaje);

	return resultado;
}

/* 
 * Pre-condiciones: stdlib.h, stdbool.h y stdio.h incluidas, personaje debe ser no nulo y haber sido inicializado al menos
 * Post-condiciones: Si el personaje fue cargado, se le pide al usuario que ingrese una configuración de números que sea del tipo "1,2,3,4,5,6"
 * 		como ejemplo. Una vez que ingrese una configuración se realizan los cambios extrayendo y reinsertando los elementos de manera correcta
 * 		para que los primeros MAX_PARTY elementos de la lista sean los selecciones en dicho orden y representen la nueva party elegida
 */
int cambiar_party_personaje(personaje_t* personaje) {
	if (!(personaje->fue_cargado)) {
		printf(ROJO "\n  █ " AMARILLO "Imposible cambiar party de entrenador. Ningún entrenador cargado en el sistema.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}
	if (personaje->cantidad_en_party < MAX_PARTY) {
		printf(ROJO "\n  █ " AMARILLO "Imposible cambiar party de entrenador. No se tiene la party del entrenador en su capacidad máxima.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	size_t cant_total_pokemones = lista_elementos(personaje->lista_caja_pokemones);
	if (cant_total_pokemones == 0) {
		printf(ROJO "\n  █ " AMARILLO "Imposible cambiar party de entrenador porque el entrenador no posee Pokemones.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	imprimir_total_pokemones_personaje(personaje, cant_total_pokemones);

	size_t nuevos_pokemones_party[MAX_PARTY];
	imprimir_instrucciones_cambio_party();
	obtener_nuevas_posiciones_party(nuevos_pokemones_party, cant_total_pokemones);

	printf(VERDE "\n  █ " AMARILLO "Nueva party '%zu,%zu,%zu,%zu,%zu,%zu' seleccionada correctamente, mostrando pokemones actualizados.\n" BLANCO, nuevos_pokemones_party[0] + 1, nuevos_pokemones_party[1] + 1, nuevos_pokemones_party[2] + 1, nuevos_pokemones_party[3] + 1, nuevos_pokemones_party[4] + 1, nuevos_pokemones_party[5] + 1);
	pausar_juego(3 * TIEMPO_PAUSA);

	int resultado = realizar_movimiento_de_party(personaje, nuevos_pokemones_party);
	imprimir_info_personaje(personaje);

	return resultado;
}

/* 
 * Pre-condiciones: stdbool.h incluida, los punteros recibidos deben ser válidos
 * Post-condiciones: los punteros recibidos están inicializados con los valores correctos: cantidad_disponibles_personaje con cant_party_personaje, 
 * 		debe_cambiar_retador con false, y pos_pokemon_personaje y pos_pokemon_retador con cero
 */
void inicializar_datos_de_ciclo_batalla(size_t* cantidad_disponibles_personaje, size_t cant_party_personaje, bool* debe_cambiar_retador, size_t* pos_pokemon_personaje, size_t* pos_pokemon_retador) {
	*cantidad_disponibles_personaje = cant_party_personaje;
	*debe_cambiar_retador = false;
	*pos_pokemon_personaje = 0;
	*pos_pokemon_retador = 0;
}

/* 
 * Pre-condiciones: stdio.h inclida, los datos recibidos son válidos
 * Post-condiciones: imprime por pantalla el nombre del personaje, junto con la cantidad de pokemones que tiene disponibles para luchar en la party,
 * 		 además imprime el pokemon que está luchando junto a sus estadísticas actuales
 */
void imprimir_info_batalla_personaje(char nombre_personaje[], size_t cant_disponibles_personaje, pokemon_t* pokemon_personaje) {
	int velocidad_pers = pokemon_personaje->velocidad + pokemon_personaje->bonus;
	int ataque_pers = pokemon_personaje->ataque + pokemon_personaje->bonus;
	int defensa_pers = pokemon_personaje->defensa + pokemon_personaje->bonus;

	printf("\n     " VERDE "─┬─" BLANCO " Pokemones restantes del " VERDE "entrenador principal " AMARILLO "%s" BLANCO ":  ", nombre_personaje);
	printf("%zu", cant_disponibles_personaje);

	if (cant_disponibles_personaje == 1)
		printf(" (último pokemon)\n");
	else
		printf("\n");

	printf("      " VERDE "└───" BLANCO " Pokemon peleando: %s,   VEL: %i,   ATQ: %i,   DEF: %i\n\n", pokemon_personaje->nombre, velocidad_pers, ataque_pers, defensa_pers);
}

/* 
 * Pre-condiciones: stdio.h inclida, los datos recibidos son válidos
 * Post-condiciones: imprime por pantalla el nombre del retador (que puede ser un entrenador o el líder), junto con la cantidad de pokemones que
 * 		tiene disponibles para luchar en la party, además imprime el pokemon que está luchando junto a sus estadísticas
 */
void imprimir_info_batalla_retador(char nombre_retador[], size_t cant_disponibles_retador, pokemon_t* pokemon_retador) {
	printf("\n     " ROJO "─┬─" BLANCO " Pokemones restantes del " ROJO "retador " AMARILLO "%s" BLANCO ":  ", nombre_retador);
	printf("%zu", cant_disponibles_retador);

	if (cant_disponibles_retador == 1)
		printf(" (último pokemon)\n");
	else
		printf("\n");

	printf("      " ROJO "└───" BLANCO " Pokemon peleando: %s,   VEL: %i,   ATQ: %i,   DEF: %i\n", pokemon_retador->nombre, pokemon_retador->velocidad, pokemon_retador->ataque, pokemon_retador->defensa);
}

/* 
 * Pre-condiciones: stdio.h incluida, los datos recibidos son válidos
 * Post-condiciones: imprime por pantalla el nombre, la cantidad de pokemones disponibles para luchar en la party y el pokemon que lucha en el momento
 * 		de tanto el personaje como el retador, además imprime el número de batalla en el que se está dentro del gimnasio
 */
void imprimir_informacion_batalla(int num_batalla, char nombre_personaje[], size_t cant_disponible_personaje, pokemon_t* pokemon_personaje, char nombre_retador[], size_t cant_disponible_retador, pokemon_t* pokemon_retador) {
	system("clear");
	imprimir_comienzo_bloque(COLOR_ROJO);

	printf(AMARILLO "\n                                                   BATALLA %i\n", num_batalla);
	printf("                                                  ¯¯¯¯¯¯¯¯¯¯¯¯\n" BLANCO);

	imprimir_info_batalla_personaje(nombre_personaje, cant_disponible_personaje, pokemon_personaje);
	imprimir_info_batalla_retador(nombre_retador, cant_disponible_retador, pokemon_retador);
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla un mensaje de pelea en curso además de una "barra de carga" formada por tres puntitos separados temporalmente
 * 		por un tiempo de pausa TIEMPO_PELEA_EN_CURSO
 */
void imprimir_pelea_en_curso() {
	printf(AMARILLO "\n\n      Pelea en curso ");
	fflush(stdout);
	pausar_juego(TIEMPO_PELEA_EN_CURSO);

	printf(". ");
	fflush(stdout);
	pausar_juego(TIEMPO_PELEA_EN_CURSO);

	printf(". ");
	fflush(stdout);
	pausar_juego(TIEMPO_PELEA_EN_CURSO);

	printf(".");
	fflush(stdout);
	pausar_juego(TIEMPO_PELEA_EN_CURSO);
}

/* 
 * Pre-condiciones: stdio.h incluida, los datos recibidos deben ser válidos, la función debe ser llamada después de haber decidido un ganador y después
 * 		de haber mostrado la información de batalla
 * Post-condiciones: imprime por pantalla el resultado de la pelea recién realizada, siendo ganador o perdedor el pokemon del personaje, e informando
 * 		el premio de 1 punto si corresponde
 */
void imprimir_resultado_pelea(int ganador, char nombre_personaje[], char nombre_pokemon_personaje[], int bonus_pokemon_personaje) {
	printf(CYAN "\n\n\n\n      Resultado de la batalla:\n");
	printf("      ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" BLANCO);

	if (ganador == GANO_PRIMERO) {
		printf(BLANCO "\n      ¡¡Pokemon %s del entrenador principal " AMARILLO "%s" BLANCO " ha " VERDE "ganado" BLANCO " el combate!!" BLANCO, nombre_pokemon_personaje, nombre_personaje);
		if(bonus_pokemon_personaje < MAX_BONUS)
			printf("\n      Gana un aumento de un punto en todas sus estadísticas.");
		printf("\n\n");

	} else
		printf(BLANCO "\n      Pokemon %s del entrenador principal " AMARILLO "%s" BLANCO " ha " ROJO "perdido" BLANCO " el combate.\n\n" BLANCO, nombre_pokemon_personaje, nombre_personaje);
	
	imprimir_fin_bloque(COLOR_ROJO);
}

/* 
 * Pre-condiciones: los datos recibidos deben ser correctos, el retador todavía debe tener pokemones disponibles en su party
 * Post-condiciones: se le suma 1 a pos_pokemon_retador y se le resta 1 a cantidad_disponibles_retador
 */
void actualizar_victoria_personaje_sigue_retador(size_t* pos_pokemon_retador, size_t* cantidad_disponibles_retador) {
	(*pos_pokemon_retador)++;
	(*cantidad_disponibles_retador)--;
}

/* 
 * Pre-condiciones: stdbool.h incluida, los datos recibidos deben ser correctos, el retador no debe tener pokemones disponibles en su party,
 * 		pila_entrenadores debe ser una pila válida
 * Post-condiciones: se le asigna true a debe_cambiar_retador. Si está peleando el líder se le asgina true a esta_ganado_gimnasio, si no está
 * 		peleando el líder se desapila el entrenador tope de la pila_entrenadores asignándole el resultado del desapilar a resultado
 */
void actualizar_victoria_personaje_cambio_retador(bool* debe_cambiar_retador, bool esta_peleando_lider, bool* esta_ganado_gimnasio, int* resultado, lista_t* pila_entrenadores) {
	*debe_cambiar_retador = true;

	if (esta_peleando_lider)
		*esta_ganado_gimnasio = true;
	else
		*resultado = lista_desapilar(pila_entrenadores);
}

/* 
 * Pre-condiciones: stdbool.h incluida, los datos recibidos deben ser correctos
 * Post-condiciones: si el personaje todavía posee al menos un pokemon disponible en su party se le suma 1 a pos_pokemon_personaje y se le resta 1 a 
 * 		cantidad_disponibles_personaje. Si el personaje no posee más pokemones disponibles se le asigna true a esta_perdido_gimnasio
 */
void actualizar_estado_derrota_personaje(size_t* pos_pokemon_personaje, size_t cant_party_personaje, size_t* cantidad_disponibles_personaje, bool* esta_perdido_gimnasio) {
	if (*pos_pokemon_personaje < (cant_party_personaje - 1)) {
		(*pos_pokemon_personaje)++;
		(*cantidad_disponibles_personaje)--;

	} else
		*esta_perdido_gimnasio = true;
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla las opciones disponibles para el usuario del menu de derrota
 */
void imprimir_ui_menu_derrota() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_ROJO);
	printf(BLANCO "                                   »»   Acabas de ser " ROJO "DERROTADO" BLANCO "   ««\n" NORMAL);
	imprimir_fin_bloque(COLOR_ROJO);

	imprimir_comienzo_bloque(COLOR_ROJO);
	printf(BLANCO "\n          Ingresa una de las siguientes opciones para continuar:\n\n");
	printf("          » %c :  Cambiar los Pokemones de batalla (party)\n", LETRA_CAMBIAR_PARTY);
	printf("          » %c :  Reintentar el gimnasio desde el entrenador que te derrotó\n", LETRA_REINTENTAR_GIMNASIO);
	printf("          » %c :  Finalizar partida\n\n", LETRA_FINALIZAR_PARTIDA);
	imprimir_fin_bloque(COLOR_ROJO);
}

/* 
 * Pre-condiciones: stdio.h incluida, los parámetros recibidos deben ser válidos, esta función es llamada para la derrota definitiva de la simulación
 * Post-condiciones: imprime por pantalla un mensaje de derrota, mostrando las medallas ganadas y los gimnasios que no llegó a batallar
 */
void imprimir_derrota_simulacion(personaje_t* personaje, gimnasio_t* gimnasio, size_t cant_gimnasios_restantes) {
	imprimir_comienzo_bloque(COLOR_ROJO);
	printf(BLANCO "                      »»   Acabas de ser " ROJO "DERROTADO" BLANCO " en %s   ««\n" NORMAL, gimnasio->nombre);
	printf("\n      " AMARILLO "»" BLANCO " Gimnasios sin batallar: " AMARILLO "%zu" BLANCO ".", cant_gimnasios_restantes);
	printf(BLANCO "\n      " CYAN "»" BLANCO " Has ganado un total de " CYAN "%zu" BLANCO " medallas de gimnasio.", personaje->cantidad_medallas);

	if (cant_gimnasios_restantes == 0)
		printf(" Perdiste en el último gimnasio.");

	printf("\n\n" NORMAL);
	imprimir_fin_bloque(COLOR_ROJO);
}

/* 
 * Pre-condiciones: stdio.h incluida, gimnasio debe ser un puntero válido
 * Post-condiciones: imprime por pantalla un mensaje final y libera el gimnasio de la memoria, finalizando la ejecución del programa, se devuelve
 * 		FINALIZAR_PARTIDA
 */
int finalizar_partida(gimnasio_t* gimnasio) {
	printf(CYAN "\n  █ GG WP. Finalizando partida.\n\n" BLANCO);
	fflush(stdout);
	pausar_juego(TIEMPO_PAUSA * 2);

	destructor_gimnasio(gimnasio);

	return FINALIZAR_PARTIDA;
}

/* 
 * Pre-condiciones: stdio.h y stdbool.h incluidas, los parámetros recibidos deben ser válidos
 * Post-condiciones: Si se_debe_simular es true, imprime por pantalla un mensaje de derrota, mostrando las medallas ganadas y los gimnasios que no
 * 		 llegó a batallar, asignando true a esta_gimnasio_perdido y se devuelve FIN_DEL_JUEGO_DERROTA.
 * 		 Si se_debe_simular es false, se imprime un menu de opciones de derrota y se pide que el usuario ingrese una opción hasta que ingrese una
 * 		 correcta. Si se ingresó LETRA_CAMBIAR_PARTY se muestra la funcionalidad para cambiar party y se vuelve al menu de derrota. Si  se ingresa
 * 		 LETRA_REINTENTAR_GIMNASIO se devuelve REINTENTAR_GIMNASIO para reintentar el gimnasio, y si se ingresa LETRA_FINALIZAR_PARTIDA se ejecuta
 * 		 finalizar_partida
 */
int ejecutar_menu_derrota(personaje_t* personaje, gimnasio_t* gimnasio, bool* esta_gimnasio_perdido, bool se_debe_simular, size_t cant_gimnasios_restantes) {
	int resultado = EXITO;
	pausar_juego(TIEMPO_PAUSA * 2);

	if (!se_debe_simular) {
		while (resultado == EXITO) {
			imprimir_ui_menu_derrota();
			char opcion_ingresada;
			obtener_opcion(&opcion_ingresada);
			comprobar_opcion(&opcion_ingresada, ID_MENU_DERROTA);

			if (opcion_ingresada == LETRA_CAMBIAR_PARTY)
				resultado = cambiar_party_personaje(personaje);

			else if (opcion_ingresada == LETRA_REINTENTAR_GIMNASIO)
				resultado = REINTENTAR_GIMNASIO;

			else
				resultado = finalizar_partida(gimnasio);
		}

	} else {
		imprimir_derrota_simulacion(personaje, gimnasio, cant_gimnasios_restantes);
		*esta_gimnasio_perdido = true;
		resultado = FIN_DEL_JUEGO_DERROTA;
	}

	return resultado;
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla las opciones disponibles para el usuario del menu de victoria
 */
void imprimir_ui_menu_victoria() {
	system("clear");
	imprimir_comienzo_bloque(COLOR_VERDE);
	printf(BLANCO "                                »»   Acabas obtener una " VERDE "VICTORIA" BLANCO "   ««\n" NORMAL);

	imprimir_fin_bloque(COLOR_VERDE);

	imprimir_comienzo_bloque(COLOR_VERDE);
	printf(BLANCO "\n          Ingresa una de las siguientes opciones para continuar:\n\n");
	printf("          » %c :  Tomar un Pokemon del líder que acabas de vencer. Todo tuyo!\n", LETRA_TOMAR_POKEMON);
	printf("          » %c :  Cambiar los Pokemones de batalla (party)\n", LETRA_CAMBIAR_PARTY);
	printf("          » %c :  Avanzar al siguiente gimnasio\n\n", LETRA_PROXIMO_GIMNASIO);
	imprimir_fin_bloque(COLOR_VERDE);
}

/* 
 * Pre-condiciones: stdlib.h incluida, personaje es válido
 * Post-condiciones: redimensiona el tamaño de la party si es necesario porque se tenían menos pokemones que MAX_PARTY
 */
int ajustar_party_si_necesario(personaje_t* personaje) {
	int resultado = EXITO;

	if (personaje->cantidad_en_party < MAX_PARTY) {
		(personaje->cantidad_en_party)++;
		resultado = redimensionar_party(&(personaje->party), personaje->cantidad_en_party);
		if (resultado == EXITO)
			vincular_party_con_caja(personaje);
	}

	return resultado;
}

/* 
 * Pre-condiciones: el puntero debe ser válido
 * Post-condiciones: suma 1 a bonus si bonus es menor que MAX_BONUS
 */
void actualizar_bonus(int* bonus) {
	if (*bonus < MAX_BONUS)
		(*bonus)++;
}

/* 
 * Pre-condiciones: stdio.h incluida, el lider recibido debe ser válido
 * Post-condiciones: Muestra todos los pokemones que posee el líder en su party junto con un número identificador
 */
void imprimir_aviso_tomar_lider(entrenador_t* lider) {
	system("clear");

	imprimir_comienzo_bloque(COLOR_AZUL);
	printf(BLANCO "            »»   En: '%c :  Tomar un Pokemon del líder que acabas de vencer'\n" NORMAL, LETRA_TOMAR_POKEMON);
	imprimir_fin_bloque(COLOR_AZUL);

	imprimir_comienzo_bloque(COLOR_AZUL);
	printf(BLANCO "      Líder: " ROJO);
	imprimir_entrenador(lider);
	printf("\n\n");
	imprimir_fin_bloque(COLOR_AZUL);
}

/* 
 * Pre-condiciones: stdio.h incluida, se deben haber mostrado todos los pokemones del líder, los parámetros recibidos deben ser válidos
 * Post-condiciones: solicita una posición válida al usuario hasta que la ingrese correctamente
 */
void solicitar_posicion_pokemon_a_tomar(int* pos_ingresada, size_t cant_party_lider) {
	int resultado = ERROR;

	printf(CYAN "\n  █ " AMARILLO "Ingrese la posición del Pokemon deseado: " BLANCO);
	int leidos = scanf("%i", pos_ingresada);
	(*pos_ingresada)--;

	while (resultado == ERROR) {
		if (leidos != 1)
			printf(ROJO "\n  █ " AMARILLO "No se leyó ninguna posición.\n" BLANCO);
		else if (*pos_ingresada < 0)
			printf(ROJO "\n  █ " AMARILLO "La posición ingresada es negativa o cero (se comienza desde 1).\n" BLANCO);
		else if (*pos_ingresada > cant_party_lider)
			printf(ROJO "\n  █ " AMARILLO "La posición ingresada es inexistentee (no corresponden a un Pokemon).\n" BLANCO);
		else
			resultado = EXITO;

		if (resultado == ERROR) {
			*pos_ingresada = 0;
			printf(CYAN "\n  █ " AMARILLO "Por favor ingrese de nuevo la posición del Pokemon que desea tomar:  " BLANCO);
			leidos = scanf("%i", pos_ingresada);
			(*pos_ingresada)--;
		}
	}
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h, los parámetros recibidos deben ser no nulos y válidos
 * Post-condiciones: muestra por pantalla los pokemones que posee el líder derrotado, pide al usuario que seleccione uno y carga en la caja de
 * 		pokemones el personaje el pokemones recién seleccionado.
 * 		Devuelve EXITO si fue todo bien o ERROR de otra manera
 */
int tomar_pokemon_del_lider(personaje_t* personaje, entrenador_t* lider, bool* fue_tomado) {
	int resultado = EXITO;

	if (!(*fue_tomado)) {
		imprimir_aviso_tomar_lider(lider);

		int pos_ingresada = 0;
		solicitar_posicion_pokemon_a_tomar(&pos_ingresada, lider->cantidad_en_party);

		pokemon_t pokemon_solicitado = *(lider->party[pos_ingresada]);

		resultado = cargar_pokemon_en_caja(personaje->lista_caja_pokemones, pokemon_solicitado);
		if (resultado == ERROR)
			return ERROR;

		resultado = ajustar_party_si_necesario(personaje);
		*fue_tomado = true;

	} else {
		printf(ROJO "\n  █ " AMARILLO "Error: Ya fue tomado un Pokemon del líder. Esta opción no se puede volver a utilizar.\n" BLANCO);
		pausar_juego(TIEMPO_PAUSA * 3);
	}

	return resultado;
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib incluidas , se debe haber ganado un gimnasio, los parámetros recibidos son válidos
 * Post-condiciones: Se imprime por pantalla las opiones que puede ingresar el usuario pidiéndole una opción hasta que ingresa la correcta.
 * 		Si se ingresa LETRA_TOMAR_POKEMON se muestra por pantalla los pokemones que posee el líder derrotado, pide al usuario que seleccione uno
 * 		y carga en la caja de pokemones el personaje el pokemones recién seleccionado. Devuelve EXITO si fue todo bien o ERROR de otra manera y se
 * 		vuelve al menu victoria.
 * 		Si se ingresa LETRA_CAMBIAR_PARTY se le permite al usuario cambiar personaje y se retorna a este menu. Esto devuelve EXITO si todo fue bien
 * 		o error si ocurrió un ERROR
 * 		Si se ingresa LETRA_PROXIMO_GIMNASIO se retorna CAMBIO_DE_GIMNASIO
 */
int ejecutar_menu_victoria(personaje_t* personaje, gimnasio_t* gimnasio) {
	int resultado = EXITO;
	pausar_juego(TIEMPO_PAUSA * 2);
	bool fue_ya_tomado_de_lider = false;

	while (resultado == EXITO) {
		imprimir_ui_menu_victoria();
		char opcion_ingresada;
		obtener_opcion(&opcion_ingresada);
		comprobar_opcion(&opcion_ingresada, ID_MENU_VICTORIA);

		if (opcion_ingresada == LETRA_TOMAR_POKEMON)
			resultado = tomar_pokemon_del_lider(personaje, gimnasio->lider, &fue_ya_tomado_de_lider);

		else if (opcion_ingresada == LETRA_CAMBIAR_PARTY)
			resultado = cambiar_party_personaje(personaje);

		else
			resultado = CAMBIO_DE_GIMNASIO;
	}

	return resultado;
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: imprime por pantalla y solicita la opción correcta al usuario para avanzar al siguiente combate hasta que ingresa el caracter
 * 		correcto
 */
void ingresar_para_proximo_combate() {
	char opcion_ingresada;

	printf(CYAN "\n  █ " AMARILLO "Ingrese '%c' para ir al próximo combate: " BLANCO, LETRA_PROXIMO_COMBATE);
	scanf(" %c", &opcion_ingresada);

	comprobar_opcion(&opcion_ingresada, ID_MENU_BATALLA);
}

/* 
 * Pre-condiciones: stdbool.h incluida, los parámetros recibidos deben ser válidos
 * Post-condiciones: se le asignan a los parámetros los valores necesarios para reintentar el gimnasio
 */
int actualizar_datos_reintentar_gimnasio(size_t* pos_pokemon_personaje, size_t* pos_pokemon_retador, size_t* cantidad_disponibles_personaje, size_t* cantidad_disponibles_retador, size_t cant_party_personaje, size_t cant_party_retador, bool* esta_perdido_gimnasio, bool* debe_cambiar_retador) {
	*esta_perdido_gimnasio = false;
	*pos_pokemon_personaje = 0;
	*pos_pokemon_retador = 0;
	*debe_cambiar_retador = false;
	*cantidad_disponibles_personaje = cant_party_personaje;
	*cantidad_disponibles_retador = cant_party_retador;

	return EXITO;
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h incluidas ,los parámetros recibidos deben ser válidos
 * Post-condiciones: si se_debe_simular es false: se ejecuta todo el proceso de batalla, se muestran los pokemones batallando, se decide el
 * 		pokemon ganador de la ronda, se ejecutan las derrotas junto con su menu y sus opciones, se ejecutan las victorias con la opción de
 * 		tomar el pokemon del lider, informando al usuario de lo que se está haciendo y los resultados en cada paso. Una vez decidido el resultado,
 * 		que puede ser la victoria del gimnasio o la finalización del juego, se devuelve CAMBIO_DE_GIMNASIO o FINALIZAR_PARTIDA
 * 		Si se_debe_simular es true se ejecutará la simulación, en donde se ejecuta el juego y la batalla sin mostrar nada por pantalla ni pedir
 * 		ingreso de datos, terminando en resultado que puede ser derrota o victoria del gimnasio
 */
int batallar_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio, bool se_debe_simular, size_t cant_gimnasios_restantes) {
	if (!es_id_puntero_funcion_valido(gimnasio->id_puntero_funcion)) {
		printf(ROJO "\n  █ Error Fatal: El ID para determinar la función de batalla es inválido. Terminando ejecución.\n" NORMAL);
		return ERROR;
	}

	int resultado = EXITO, ganador, num_batalla = 0;
	size_t pos_pokemon_personaje = 0, pos_pokemon_retador = 0;
	bool debe_cambiar_retador = false, esta_peleando_lider = false, esta_ganado_gimnasio = false, esta_perdido_gimnasio = false;
	int (*array_funciones_batalla[])(void*, void*) = {funcion_batalla_1, funcion_batalla_2, funcion_batalla_3, funcion_batalla_4, funcion_batalla_5};
	int pos_puntero_funcion = gimnasio->id_puntero_funcion - 1;

	while ((resultado == EXITO) && (!esta_peleando_lider) && (!esta_perdido_gimnasio) && (!esta_ganado_gimnasio)) {
		entrenador_t* retador = lista_tope(gimnasio->pila_entrenadores);
		if (!retador) {
			retador = gimnasio->lider;
			esta_peleando_lider = true;
		}

		size_t cantidad_disponibles_personaje = personaje->cantidad_en_party;
		size_t cantidad_disponibles_retador = retador->cantidad_en_party;
		inicializar_datos_de_ciclo_batalla(&cantidad_disponibles_personaje, personaje->cantidad_en_party, &debe_cambiar_retador, &pos_pokemon_personaje, &pos_pokemon_retador);

		while ((resultado == EXITO) && (!debe_cambiar_retador) && (!esta_perdido_gimnasio) && (!esta_ganado_gimnasio)) {
			pokemon_t* pokemon_retador = retador->party[pos_pokemon_retador];
			pokemon_t* pokemon_personaje = personaje->party[pos_pokemon_personaje];
			num_batalla++;

			ganador = array_funciones_batalla[pos_puntero_funcion]((void*)pokemon_personaje, (void*)pokemon_retador);

			if (!se_debe_simular) {
				imprimir_informacion_batalla(num_batalla, personaje->nombre, cantidad_disponibles_personaje, pokemon_personaje, retador->nombre, cantidad_disponibles_retador, pokemon_retador);
				imprimir_pelea_en_curso();
				imprimir_resultado_pelea(ganador, personaje->nombre, pokemon_personaje->nombre, pokemon_personaje->bonus);
			}

			if (ganador == GANO_PRIMERO) {
				actualizar_bonus(&(pokemon_personaje->bonus));
				if (pos_pokemon_retador < (retador->cantidad_en_party - 1))
					actualizar_victoria_personaje_sigue_retador(&pos_pokemon_retador, &cantidad_disponibles_retador);
				else
					actualizar_victoria_personaje_cambio_retador(&debe_cambiar_retador, esta_peleando_lider, &esta_ganado_gimnasio, &resultado, gimnasio->pila_entrenadores);
			} else
				actualizar_estado_derrota_personaje(&pos_pokemon_personaje, personaje->cantidad_en_party, &cantidad_disponibles_personaje, &esta_perdido_gimnasio);

			if ((!esta_ganado_gimnasio) && (!se_debe_simular))
				ingresar_para_proximo_combate();

			if (esta_perdido_gimnasio) {
				resultado = ejecutar_menu_derrota(personaje, gimnasio, &esta_perdido_gimnasio, se_debe_simular, cant_gimnasios_restantes);
				if (resultado == REINTENTAR_GIMNASIO)
					resultado = actualizar_datos_reintentar_gimnasio(&pos_pokemon_personaje, &pos_pokemon_retador, &cantidad_disponibles_personaje, &cantidad_disponibles_retador, personaje->cantidad_en_party, retador->cantidad_en_party, &esta_perdido_gimnasio, &debe_cambiar_retador);
			}
		}
	}

	if (esta_ganado_gimnasio && (!se_debe_simular))
		resultado = ejecutar_menu_victoria(personaje, gimnasio);
	else if (esta_ganado_gimnasio && se_debe_simular)
		resultado = CAMBIO_DE_GIMNASIO;

	return resultado;
}

/* 
 * Pre-condiciones: stdio.h incluida, personaje debe ser válido
 * Post-condiciones: se imprime por pantalla que el personaje se convirtió en maestro pokemon y la cantidad de medallas que ganó
 */
void mostrar_victoria_maestro_pokemon(personaje_t* personaje) {
	imprimir_comienzo_bloque(COLOR_AMARILLO);
	printf(BLANCO "                 ¡" VERDE "%s" BLANCO " TE HAS CONVERTIDO EN MAESTRO POKEMON!¡FELICITACIONES!\n" NORMAL, personaje->nombre);
	printf(BLANCO "\n      " CYAN "»»" BLANCO " Has ganado un total de " CYAN "%zu" BLANCO " medallas de gimnasio.\n\n" NORMAL, personaje->cantidad_medallas);
	imprimir_fin_bloque(COLOR_AMARILLO);
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h incluidas ,los parámetros recibidos deben ser válidos
 * Post-condiciones: si el personaje no está cargado o no hay gimnasios en el heap se retorna EXITO imprimiendo por pantalla el motivo.
 * 		Se ejecuta todo el proceso de batalla, se muestran los pokemones batallando, se decide el
 * 		pokemon ganador de la ronda, se ejecutan las derrotas junto con su menu y sus opciones, se ejecutan las victorias con la opción de
 * 		tomar el pokemon del lider, informando al usuario de lo que se está haciendo y los resultados en cada paso. Una vez decidido el resultado,
 * 		que puede ser la victoria del gimnasio o la finalización del juego, si se ganó el gimnasio se cicla a lo largo de cada gimnasio hasta que
 * 		se ganen todos o se decida finalizar el programa en uno de ellos. Si se ganó se imprime un menú de victoria.
 * 		Se devuelve FIN_DEL_JUEGO_EXITOSO si fue todo bien o ERROR si hubo un error
 */
int batallar(personaje_t* personaje, heap_t* heap_gimnasios) {
	if (!personaje->fue_cargado) {
		printf(ROJO "\n  █ " AMARILLO "Imposible realizar próxima batalla planificada. Ningún entrenador cargado en el sistema.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	if (heap_cantidad(heap_gimnasios) == 0) {
		printf(ROJO "\n  █ " AMARILLO "Imposible realizar próxima batalla planificada. Ningún gimnasio cargado.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	gimnasio_t* gimnasio_actual = heap_extraer_elemento_raiz(heap_gimnasios);
	if (!gimnasio_actual)
		return ERROR;

	bool se_debe_simular = false;
	int resultado = batallar_gimnasio(personaje, gimnasio_actual, se_debe_simular, heap_cantidad(heap_gimnasios));

	while (resultado == CAMBIO_DE_GIMNASIO) {
		(personaje->cantidad_medallas)++;
		destructor_gimnasio(gimnasio_actual);

		if (heap_cantidad(heap_gimnasios) == 0)
			resultado = EXITO;
		else {
			gimnasio_actual = heap_extraer_elemento_raiz(heap_gimnasios);
			if (!gimnasio_actual)
				resultado = ERROR;
			else
				resultado = batallar_gimnasio(personaje, gimnasio_actual, se_debe_simular, heap_cantidad(heap_gimnasios));
		}
	}

	if (resultado == EXITO) {
		system("clear");
		mostrar_victoria_maestro_pokemon(personaje);

	} else if (resultado == ERROR)
		return ERROR;

	return FIN_DEL_JUEGO_EXITOSO;
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h incluidas ,los parámetros recibidos deben ser válidos
 * Post-condiciones: ejecutará la simulación, en donde se ejecuta el juego y la batalla sin mostrar nada por pantalla ni pedir
 * 		ingreso de datos, terminando en resultado que puede ser derrota o victoria del gimnasio, así ciclará por cada gimnasio hasta que se pierda
 * 		en uno o se ganen todos, imprimiendo por pantalla el resultado final del juego. Se devuelve FIN_DEL_JUEGO_EXITOSO si todo fue bien o ERROR
 * 		si ocurrió algún error.
 */
int batallar_simulacion(personaje_t* personaje, heap_t* heap_gimnasios) {
	gimnasio_t* gimnasio_actual = heap_extraer_elemento_raiz(heap_gimnasios);
	if (!gimnasio_actual)
		return ERROR;

	bool se_debe_simular = true;
	int resultado = batallar_gimnasio(personaje, gimnasio_actual, se_debe_simular, heap_cantidad(heap_gimnasios));

	while (resultado == CAMBIO_DE_GIMNASIO) {
		(personaje->cantidad_medallas)++;
		destructor_gimnasio(gimnasio_actual);

		if (heap_cantidad(heap_gimnasios) == 0)
			resultado = EXITO;
		else {
			gimnasio_actual = heap_extraer_elemento_raiz(heap_gimnasios);
			if (!gimnasio_actual)
				resultado = ERROR;
			else
				resultado = batallar_gimnasio(personaje, gimnasio_actual, se_debe_simular, heap_cantidad(heap_gimnasios));
		}
	}

	if (resultado == FIN_DEL_JUEGO_DERROTA)
		destructor_gimnasio(gimnasio_actual);
	else if (resultado == EXITO)
		mostrar_victoria_maestro_pokemon(personaje);
	else if (resultado == ERROR)
		return ERROR;

	return FIN_DEL_JUEGO_EXITOSO;
}

/* 
 * Pre-condiciones: stdbool.h, stdlib.h y stdio.h incluidas, personaje debió haber sido inicialiado y no nulo, heap_gimnasios debe ser un heap válido
 * 		y no nulo
 * Post-condiciones: se imprime la interfaz del menu_gimnasio, se guarda la opcion ingresada y se ejecuta cada opción acorde a lo ingresado.
 * 		Si es LETRA_MOSTRAR_PERSONAJE se muestra toda la información del personaje cargado.
 * 		Si es LETRA_MOSTRAR_GIMNASIO se muestra toda la información del gimnasio actual (el siguiente a batallar).
 * 		Si es LETRA_CAMBIAR_PARTY se permite modificar la party del personaje
 * 		Si es LETRA_REALIZAR_BATALLA se procede a batallar al siguiente gimnasio
 * 		Si no es ninguna de las anteriores se sale del menu (si es X)
 */
int comenzar_partida(personaje_t* personaje, heap_t* heap_gimnasios) {
	char opcion_ingresada;
	bool sigo_ejecutando_menu = true;
	int resultado_operacion = EXITO;

	while ((sigo_ejecutando_menu == true) && (resultado_operacion == EXITO)) {
		imprimir_ui_gimnasio();
		obtener_opcion(&opcion_ingresada);
		comprobar_opcion(&opcion_ingresada, ID_MENU_GIMNASIO);
		pausar_juego(TIEMPO_PAUSA / 2);

		if (opcion_ingresada == LETRA_MOSTRAR_PERSONAJE)
			imprimir_info_personaje(personaje);

		else if (opcion_ingresada == LETRA_MOSTRAR_GIMNASIO)
			resultado_operacion = imprimir_info_gimnasio_actual(heap_gimnasios);

		else if (opcion_ingresada == LETRA_CAMBIAR_PARTY)
			resultado_operacion = cambiar_party_personaje(personaje);

		else if (opcion_ingresada == LETRA_REALIZAR_BATALLA)
			resultado_operacion = batallar(personaje, heap_gimnasios);

		else /* opcion_ingresada == LETRA_EXIT */
			sigo_ejecutando_menu = false;
	}

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdio.h, stdbool.h y stdlib.h incluidas ,los parámetros recibidos deben ser válidos
 * Post-condiciones:  si el personaje no está cargado o no hay gimnasios en el heap se retorna EXITO imprimiendo por pantalla el motivo.
 * 		Ejecutará la simulación, en donde se ejecuta el juego y la batalla sin mostrar nada por pantalla ni pedir
 * 		ingreso de datos, terminando en resultado que puede ser derrota o victoria del gimnasio, así ciclará por cada gimnasio hasta que se pierda
 * 		en uno o se ganen todos, imprimiendo por pantalla el resultado final del juego. Se devuelve FIN_DEL_JUEGO_EXITOSO si todo fue bien o ERROR
 * 		si ocurrió algún error.
 */
int simular_partida(personaje_t* personaje, heap_t* heap_gimnasios) {
	if (!personaje->fue_cargado) {
		printf(ROJO "\n  █ " AMARILLO "Imposible realizar simulación. Ningún entrenador cargado en el sistema.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	if (heap_cantidad(heap_gimnasios) == 0) {
		printf(ROJO "\n  █ " AMARILLO "Imposible realizar simulación. Ningún gimnasio cargado.\n" BLANCO);
		pausar_juego(3 * TIEMPO_PAUSA);
		return EXITO;
	}

	imprimir_ui_simulacion();
	fflush(stdout);
	pausar_juego(TIEMPO_PAUSA);

	int resultado_operacion = batallar_simulacion(personaje, heap_gimnasios);

	return resultado_operacion;
}

/* 
 * Pre-condiciones: stdio.h y stdbool.h incluida
 * Post-condiciones: imprime por pantalla toda la información relacionada con el menú inicial, opcion contiene un caracter válido que represente
 * 		una de las opciones del menu
 */
void ejecutar_menu_inicio(char* opcion) {
	imprimir_ui_inicio();
	obtener_opcion(opcion);
	comprobar_opcion(opcion, ID_MENU_INICIO);
}

/* 
 * Pre-condiciones: stdio.h y stdlib.h incluidas, personaje debe ser no nulo
 * Post-condiciones: Si se detecta que resultado es ERROR_RECUPARABLE se intenta recuperar de uno de dos problemas: 1) hubo un problema de lectura
 * 		o con el formato al leer el entrenador con lo cual se mantiene el entrenador original si había alguno y se sigue con la ejecución del menu
 * 		principal, o 2) se intentó sobreescribir un entrenador cargado pero hubo un error recuperable, por lo cual se destruye el entrenador que
 * 		se intentó cargar y se deja el entrenador original.
 * 		Si se detecta que resultado es ERROR_SIN_POKEMON se sabe que el último personaje que quiso ser cargado no poseía pokemones válidos por lo
 * 		que se intenta recuperar de uno de dos problemas: 1) es la primera vez que se carga un personaje y el mismo no tiene pokemones, por lo que
 * 		destruye el existente y se inicializa uno nuevo, o 2) se intentó sobreescribir un personaje que al leerlo no poseía pokemones, por lo cual
 * 		se deja el entrenador original cargado.
 */
void rescatar_de_error_recuperable_personaje(int* resultado, personaje_t* personaje, personaje_t personaje_auxiliar) {
	if (*resultado == ERROR_RECUPERABLE) {
		pausar_juego(TIEMPO_PAUSA * 2);

		if (!(personaje_auxiliar.fue_cargado)) {
			inicializar_personaje_rescate(personaje, &personaje_auxiliar);
			*resultado = EXITO;
			printf(ROJO "\n  █ " AMARILLO "El entrenador ingresado con formato de lectura incorrecto. Ningún personaje cargado.\n\n" NORMAL);

		} else {
			destructor_personaje(personaje, DESTRUCCION_RESCATE);
			inicializar_personaje_rescate(personaje, &personaje_auxiliar);
			*resultado = EXITO;
			printf(VERDE "\n  █ " AMARILLO "Entrenador original sigue cargado.\n\n" NORMAL);
		}
		pausar_juego(TIEMPO_PAUSA * 2);

	} else if (*resultado == ERROR_SIN_POKEMON) {
		pausar_juego(TIEMPO_PAUSA * 2);
		printf(ROJO "\n  █ " AMARILLO "Entrenador sin Pokemones o con error en el formato para lectura.\n" NORMAL);

		if (!(personaje_auxiliar.fue_cargado)) {
			destructor_personaje(personaje, DESTRUCCION_NORMAL);
			*resultado = inicializar_personaje(personaje);

		} else {
			destructor_personaje(personaje, DESTRUCCION_RESCATE);
			inicializar_personaje_rescate(personaje, &personaje_auxiliar);
			*resultado = EXITO;
			pausar_juego(TIEMPO_PAUSA);
			printf(VERDE "\n  █ " AMARILLO "Entrenador original sigue cargado.\n\n" NORMAL);
		}
		pausar_juego(TIEMPO_PAUSA * 2);
	}
}

/* 
 * Pre-condiciones: stdio.h incluida
 * Post-condiciones: si resultado es ERROR_ARCHIVO se informa al usuario que el error que ocurrió fue por un error al intentar abrir el archivo
 * 		resultado termina conteniendo EXITO
 */
void rescatar_de_error_archivo(int* resultado) {
	if (*resultado == ERROR_ARCHIVO) {
		pausar_juego(TIEMPO_PAUSA * 2);
		printf(ROJO "\n  █ " AMARILLO "El archivo no se puede abrir o no existe. Intente ingresando otro archivo.\n\n" NORMAL);
		*resultado = EXITO;
		pausar_juego(TIEMPO_PAUSA * 2);
	}
}

int main() {
	char opcion_ingresada;
	bool sigo_ejecutando_menu = true;

	personaje_t personaje, personaje_auxiliar;

	heap_t* heap_gimnasios;
	int (*comparador_gimnasios_ptr)(void*, void*) = &comparador_gimnasios;
	void (*destructor_gimnasio_ptr)(void*) = &destructor_gimnasio;

	int resultado_operacion = inicializar_personaje(&personaje);
	if (resultado_operacion == ERROR)
		return -1;

	heap_gimnasios = heap_crear(comparador_gimnasios_ptr, destructor_gimnasio_ptr);
	if (!heap_gimnasios) {
		destructor_personaje(&personaje, DESTRUCCION_NORMAL);
		return -1;
	}

	while ((sigo_ejecutando_menu == true) && (resultado_operacion == EXITO)) {
		ejecutar_menu_inicio(&opcion_ingresada);

		if (opcion_ingresada == LETRA_INGRESAR_PERSONAJE) {
			imprimir_aviso_entrenador_principal();
			resultado_operacion = ingresar_entrenador_principal(&personaje, &personaje_auxiliar);
			rescatar_de_error_recuperable_personaje(&resultado_operacion, &personaje, personaje_auxiliar);

		} else if (opcion_ingresada == LETRA_AGREGAR_GIMNASIO) {
			imprimir_aviso_agregar_gimnasios();
			resultado_operacion = agregar_gimnasios(heap_gimnasios);

		} else if (opcion_ingresada == LETRA_COMENZAR_PARTIDA) {
			resultado_operacion = comenzar_partida(&personaje, heap_gimnasios);

		} else if (opcion_ingresada == LETRA_SIMULAR_PARTIDA) {
			resultado_operacion = simular_partida(&personaje, heap_gimnasios);

		} else /* opcion_ingresada == LETRA_EXIT */ {
			sigo_ejecutando_menu = false;
		}

		rescatar_de_error_archivo(&resultado_operacion);
	}

	destructor_personaje(&personaje, DESTRUCCION_NORMAL);
	heap_destruir(heap_gimnasios);

	return 0;
}