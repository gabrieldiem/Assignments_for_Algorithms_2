#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evento_pesca.h"

#define MAX_RUTA 200

const int LINEA_RUTA_ARRECIFE = 1, LINEA_RUTA_ACUARIO = 2;
const char RUTA_FILE_ACUARIO_DEFAULT[] = "acuario.txt";
const bool RUTA_NO_INGRESADA = false;
const int ERROR = -1, EXITO = 0;

const bool SI_POSEE = true, NO_POSEE = false;
const int PESO_PARA_SEDENTARIO = 70;
const char MI_INICIAL = 'G';
const int MAX_VEL_RAYO_MCQUEEN = 318;
const char CELESTE_COMO_CIELO[] = "celeste";

const int CANTIDAD_SEDENTARIOS = 10;
const int CANTIDAD_CON_MI_INICIAL = 3;
const int CANTIDAD_GANA_MCQUEEN = 6;
const int CANTIDAD_CELESTES = 4;
const int CANTIDAD_PESO_PAR = 10;

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida
 * Post-condiciones: devuelve true si una ruta de arrecife es ingresado por linea de comando o false si esto no es asi
 */
bool se_ingreso_ruta_arrecife(int argc) {
	return (argc >= (LINEA_RUTA_ARRECIFE + 1));
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida
 * Post-condiciones: devuelve true si una ruta de acuario es ingresado por linea de comando o false si esto no es asi
 */
bool se_ingreso_ruta_acuario(int argc) {
	return (argc >= (LINEA_RUTA_ACUARIO + 1));
}

/* 
 * Pre-condiciones: biblioteca estandar string.h incluida, un string de caracteres debe existir en la posicion adecuada de argv, ingresado por linea
 		de comando
 * Post-condiciones: ruta_file posee la ruta adecuada al archivo correspondiente
 */
void obtener_ruta_file(char ruta_file[MAX_RUTA], int linea_ruta, char* argv[]) {
	strcpy(ruta_file, argv[linea_ruta]);
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: devuelve true si el peso del pokemon es mayor a PESO_PARA_SEDENTARIO o false si no es asi
 */
bool es_pokemon_sedentario(pokemon_t* pokemon) {
	return ((*pokemon).peso > PESO_PARA_SEDENTARIO);
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h y string.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: devuelve true si en el nombre de especie del pokemon existe un caracter igual a MI_INICIAL o false si no es asi
 */
bool posee_pokemon_mi_inicial(pokemon_t* pokemon) {
	int tope = (int)strlen((*pokemon).especie);
	bool posee = NO_POSEE;
	int i = 0;
	while ((i < tope) && (posee == NO_POSEE)) {
		if ((*pokemon).especie[i] == MI_INICIAL)
			posee = SI_POSEE;

		i++;
	}
	return posee;
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: devuelve true si la velocidad del pokemon es mayor a MAX_VEL_RAYO_MCQUEEN o false si no es asi
 */
bool gana_pokemon_a_rayo_mcqueen(pokemon_t* pokemon) {
	return ((*pokemon).velocidad > MAX_VEL_RAYO_MCQUEEN);
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h, string.h y ctpye.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: devuelve true si el color del pokemon es celeste (no importa si el color tiene letras mayusculas o minusculas) o false si
 	si no es asi
 */
bool es_pokemon_celeste_como_cielo(pokemon_t* pokemon) {
	char color[MAX_COLOR];
	char contenedor_aux;
	strcpy(color, (*pokemon).color);
	int tope = (int)strlen(color);
	for (int i = 0; i < tope; i++) {
		contenedor_aux = (char)tolower(color[i]);
		color[i] = contenedor_aux;
	}

	return (strcmp(color, CELESTE_COMO_CIELO) == 0);
}

/* 
 * Pre-condiciones: biblioteca estandar stdbool.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: devuelve true si la division del peso del pokemon por dos tiene un resto de cero o false si no es asi
 */
bool tiene_pokemon_peso_par(pokemon_t* pokemon) {
	int peso = (*pokemon).peso;
	return ((peso % 2) == 0);
}

/* 
 * Pre-condiciones: biblioteca estandar stdio.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: muestra  por pantalla los campos de un pokemon con un formato simple
 */
void mostrar_simple(pokemon_t* pokemon) {
	printf(" Especie: %s.  Velocidad: %i.  Peso: %i.  Color: %s.\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);
}

/* 
 * Pre-condiciones: biblioteca estandar stdio.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: muestra  por pantalla los campos de un pokemon con un formato csv
 */
void mostrar_formato_csv(pokemon_t* pokemon) {
	printf(" %s;%i;%i;%s\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);
}

/* 
 * Pre-condiciones: biblioteca estandar stdio.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: muestra  por pantalla los campos de un pokemon con un formato con espacios entre los campos
 */
void mostrar_espaciado(pokemon_t* pokemon) {
	printf("-----------------------------------------------------------------------------------------\n");
	printf("|| Especie: %s    ||    Velocidad: %i    ||    Peso: %i    ||    Color: %s ||\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);
}

/* 
 * Pre-condiciones: biblioteca estandar stdio.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: muestra por pantalla los campos de un pokemon con un formato parecido a una tabla
 */
void mostrar_elegante(pokemon_t* pokemon) {
	printf("═════════════════════════════════════════════════════════════════════════════════════════\n");
	printf("║ Especie: %s   ▓   Velocidad: %i   ▓   Peso: %i   ▓   Color: %s ║\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);
}

/* 
 * Pre-condiciones:  biblioteca estandar stdio.h incluida, el parametro pokemon debe contener datos validos
 * Post-condiciones: muestra por pantalla los campos de un pokemon con un formato con las palabras literales escritas una letra en minuscula y una letra en mayuscula
 */
void mostrar_raro(pokemon_t* pokemon) {
	printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
	printf("~~-- eSpEcIe~ %s  ~~~  vElOcIdAd: %i  ~~~  pEsO: %i  ~~~  cOlOr: %s --~~\n", (*pokemon).especie, (*pokemon).velocidad, (*pokemon).peso, (*pokemon).color);
}

/* 
 * Pre-condiciones: resultado_traslado debe contener el valor de EXITO u ERROR, el arrecife debe contener datos validos para mostrar o ningun pokemon en
 	el vector, la funcion que viene por parametro debe ser valida para mostrar un pokemon
 * Post-condiciones: si el valor resultado_traslado es EXITO, muestra por pantalla todo el vector de pokemones con la forma de la funcion recibida por
 	parametro, si no hay ningun pokemon, imprime por pantalla un mensaje que diga eso
 */
void censar_arrecife_si_corresponde(int resultado_traslado, arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*)) {
	if (resultado_traslado == EXITO) {
		printf("\n Este traslado se ha realizado con éxito!\n");
		censar_arrecife(arrecife, mostrar_pokemon);
	}
}

int main(int argc, char* argv[]) {
	if (!se_ingreso_ruta_arrecife(argc)) {
		printf("Error 404 Not Found: Por favor ingrese como primer parámetro la ruta del archivo .txt que contiene el arrecife.\n");
		return -1;
	}

	char ruta_file_arrecife[MAX_RUTA], ruta_file_acuario[MAX_RUTA];
	obtener_ruta_file(ruta_file_arrecife, LINEA_RUTA_ARRECIFE, argv);

	if (se_ingreso_ruta_acuario(argc)) {
		obtener_ruta_file(ruta_file_acuario, LINEA_RUTA_ACUARIO, argv);
		printf("Detección exitosa de la ruta '%s' para guardar el archivo de texto del Acuario.\n", ruta_file_acuario);
	} else {
		printf("Detección fallida de la ruta para guardar el archivo de texto del Acuario. Se utilizará la ruta default '/%s'.\n", RUTA_FILE_ACUARIO_DEFAULT);
		strcpy(ruta_file_acuario, RUTA_FILE_ACUARIO_DEFAULT);
	}

	arrecife_t* arrecife = crear_arrecife(ruta_file_arrecife);
	if (arrecife == NULL) {
		printf("Error Fatal: Hubo un error al leer y/o cargar los datos contenidos de '%s'. Finalizando ejecución.\n", ruta_file_arrecife);
		return -1;
	}

	acuario_t* acuario = crear_acuario();
	if (acuario == NULL) {
		printf("Error Fatal de Memoria: Imposible reservar memoria. Finalizando ejecución.\n");
		return -1;
	}

	int resultado_traslado, resultado_guardado;

	resultado_traslado = trasladar_pokemon(arrecife, acuario, es_pokemon_sedentario, CANTIDAD_SEDENTARIOS);
	censar_arrecife_si_corresponde(resultado_traslado, arrecife, mostrar_simple);

	resultado_traslado = trasladar_pokemon(arrecife, acuario, posee_pokemon_mi_inicial, CANTIDAD_CON_MI_INICIAL);
	censar_arrecife_si_corresponde(resultado_traslado, arrecife, mostrar_formato_csv);

	resultado_traslado = trasladar_pokemon(arrecife, acuario, gana_pokemon_a_rayo_mcqueen, CANTIDAD_GANA_MCQUEEN);
	censar_arrecife_si_corresponde(resultado_traslado, arrecife, mostrar_espaciado);

	resultado_traslado = trasladar_pokemon(arrecife, acuario, es_pokemon_celeste_como_cielo, CANTIDAD_CELESTES);
	censar_arrecife_si_corresponde(resultado_traslado, arrecife, mostrar_elegante);

	resultado_traslado = trasladar_pokemon(arrecife, acuario, tiene_pokemon_peso_par, CANTIDAD_PESO_PAR);
	censar_arrecife_si_corresponde(resultado_traslado, arrecife, mostrar_raro);

	resultado_guardado = guardar_datos_acuario(acuario, ruta_file_acuario);
	if (resultado_guardado == ERROR) {
		printf("\n El Error Fatal hizo imposible trabajar con el archivo '%s'.\n", ruta_file_acuario);
	} else {
		printf("\n El archivo de ruta '%s' fue guardado con datos de manera correcta.\n", ruta_file_acuario);
	}

	liberar_arrecife(arrecife);
	liberar_acuario(acuario);

	if (resultado_guardado == ERROR)
		return -1;
	else
		return 0;
}