#include <stdbool.h>
#include <stddef.h>

#include "lista.h"

#define MAX_NOMBRE 200
#define MAX_PARTY 6

typedef struct pokemon {
	char nombre[MAX_NOMBRE];
	int velocidad;
	int defensa;
	int ataque;
	int bonus;
} pokemon_t;

typedef struct personaje {
	char nombre[MAX_NOMBRE];
	size_t cantidad_medallas;
	pokemon_t** party;
	size_t cantidad_en_party;
	lista_t* lista_caja_pokemones;
	bool fue_cargado;
} personaje_t;

typedef struct entrenador {
	char nombre[MAX_NOMBRE];
	pokemon_t** party;
	size_t cantidad_en_party;
} entrenador_t;

typedef struct gimnasio {
	char nombre[MAX_NOMBRE];
	int dificultad;
	int id_puntero_funcion;
	entrenador_t* lider;
	lista_t* pila_entrenadores;
} gimnasio_t;