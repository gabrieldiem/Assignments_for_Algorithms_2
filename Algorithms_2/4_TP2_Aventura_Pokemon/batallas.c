#include "batallas.h"

#include "estructuras.h"

/* 
 *                     "El más veloz"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más velocidad que pkm_1.
 * Si pkm_1 tiene más velocidad o la misma se devuelve GANO_PRIMERO
 */
int funcion_batalla_1(void* pkm_1, void* pkm_2) {
	pokemon_t pokemon_1 = *((pokemon_t*)pkm_1);
	pokemon_t pokemon_2 = *((pokemon_t*)pkm_2);

	int velocidad_1 = pokemon_1.velocidad + pokemon_1.bonus;
	int velocidad_2 = pokemon_2.velocidad + pokemon_2.bonus;

	if (velocidad_2 > velocidad_1)
		return GANO_SEGUNDO;
	else
		return GANO_PRIMERO;
}

/* 
 *                     "El más fuerte"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más ataque que pkm_1.
 * Si pkm_1 tiene más ataque o el mismo se devuelve GANO_PRIMERO
 */
int funcion_batalla_2(void* pkm_1, void* pkm_2) {
	pokemon_t pokemon_1 = *((pokemon_t*)pkm_1);
	pokemon_t pokemon_2 = *((pokemon_t*)pkm_2);

	int ataque_1 = pokemon_1.ataque + pokemon_1.bonus;
	int ataque_2 = pokemon_2.ataque + pokemon_2.bonus;

	if (ataque_2 > ataque_1)
		return GANO_SEGUNDO;
	else
		return GANO_PRIMERO;
}

/* 
 *                     "El más resistente"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más defensa que pkm_1.
 * Si pkm_1 tiene más defensa o la misma se devuelve GANO_PRIMERO
 */
int funcion_batalla_3(void* pkm_1, void* pkm_2) {
	pokemon_t pokemon_1 = *((pokemon_t*)pkm_1);
	pokemon_t pokemon_2 = *((pokemon_t*)pkm_2);

	int defensa_1 = pokemon_1.defensa + pokemon_1.bonus;
	int defensa_2 = pokemon_2.defensa + pokemon_2.bonus;

	if (defensa_2 > defensa_1)
		return GANO_SEGUNDO;
	else
		return GANO_PRIMERO;
}

/* 
 *                     "El más equilibrado"
 * Por cada pokemon se crea un valor "promedio" de sus 3 características
 * Devuelve GANO_SEGUNDO si pkm_2 posee un promedio más alto que pkm_1.
 * Si pkm_1 tiene un promedio más alto o el mismo se devuelve GANO_PRIMERO
 */
int funcion_batalla_4(void* pkm_1, void* pkm_2) {
	pokemon_t pokemon_1 = *((pokemon_t*)pkm_1);
	pokemon_t pokemon_2 = *((pokemon_t*)pkm_2);

	int velocidad_1 = pokemon_1.velocidad + pokemon_1.bonus;
	int ataque_1 = pokemon_1.ataque + pokemon_1.bonus;
	int defensa_1 = pokemon_1.defensa + pokemon_1.bonus;
	int promedio_1 = (velocidad_1 + ataque_1 + defensa_1) / 3;

	int velocidad_2 = pokemon_2.velocidad + pokemon_2.bonus;
	int ataque_2 = pokemon_2.ataque + pokemon_2.bonus;
	int defensa_2 = pokemon_2.defensa + pokemon_2.bonus;
	int promedio_2 = (velocidad_2 + ataque_2 + defensa_2) / 3;

	if (promedio_2 > promedio_1)
		return GANO_SEGUNDO;
	else
		return GANO_PRIMERO;
}

/*
 * Se suma 1 a contador por cada estadística que sea par
 */
void contar_pares(int* contador, int velocidad, int ataque, int defensa) {
	if (velocidad % 2 == 0)
		(*contador)++;

	if (ataque % 2 == 0)
		(*contador)++;

	if (defensa % 2 == 0)
		(*contador)++;
}

/* 
 *                     "El más suertudo"
 * El criterio para determinar la "suerte" es contar la cantidad de estadísticas que son pares.
 * Devuelve GANO_SEGUNDO si pkm_2 posee más estadísticas pares que pkm_1.
 * Si pkm_1 tiene más estadísticas pares o la misma cantidad se devuelve GANO_PRIMERO
 */
int funcion_batalla_5(void* pkm_1, void* pkm_2) {
	pokemon_t pokemon_1 = *((pokemon_t*)pkm_1);
	pokemon_t pokemon_2 = *((pokemon_t*)pkm_2);

	int velocidad_1 = pokemon_1.velocidad + pokemon_1.bonus;
	int ataque_1 = pokemon_1.ataque + pokemon_1.bonus;
	int defensa_1 = pokemon_1.defensa + pokemon_1.bonus;
	int cantidad_pares_1 = 0;

	int velocidad_2 = pokemon_2.velocidad + pokemon_2.bonus;
	int ataque_2 = pokemon_2.ataque + pokemon_2.bonus;
	int defensa_2 = pokemon_2.defensa + pokemon_2.bonus;
	int cantidad_pares_2 = 0;

	contar_pares(&cantidad_pares_1, velocidad_1, ataque_1, defensa_1);
	contar_pares(&cantidad_pares_2, velocidad_2, ataque_2, defensa_2);

	if (cantidad_pares_2 > cantidad_pares_1)
		return GANO_SEGUNDO;
	else
		return GANO_PRIMERO;
}