#ifndef __BATALLAS_H__
#define __BATALLAS_H__

#define GANO_PRIMERO 1
#define GANO_SEGUNDO -1

/*
 * Se deberan implementar las 5 funciones de batalla.
 * Las reglas para decidir al ganador las inventan ustedes
 * 
 * Si el ganador es el primer pokemon, se devolvera 1
 * Si el ganador es el segundo pokemon, se devolvera -1
 * 
 * No habran empates.
 */

/* 
 *                     "El más veloz"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más velocidad que pkm_1.
 * Si pkm_1 tiene más velocidad o la misma se devuelve GANO_PRIMERO
 */
int funcion_batalla_1(void* pkm_1, void* pkm_2);

/* 
 *                     "El más fuerte"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más ataque que pkm_1.
 * Si pkm_1 tiene más ataque o el mismo se devuelve GANO_PRIMERO
 */
int funcion_batalla_2(void* pkm_1, void* pkm_2);

/* 
 *                     "El más resistente"
 * Devuelve GANO_SEGUNDO si pkm_2 posee más defensa que pkm_1.
 * Si pkm_1 tiene más defensa o la misma se devuelve GANO_PRIMERO
 */
int funcion_batalla_3(void* pkm_1, void* pkm_2);

/* 
 *                     "El más equilibrado"
 * Por cada pokemon se crea un valor "promedio" de sus 3 características
 * Devuelve GANO_SEGUNDO si pkm_2 posee un promedio más alto que pkm_1.
 * Si pkm_1 tiene un promedio más alto o el mismo se devuelve GANO_PRIMERO
 */
int funcion_batalla_4(void* pkm_1, void* pkm_2);

/* 
 *                     "El más suertudo"
 * El criterio para determinar la "suerte" es contar la cantidad de estadísticas que son pares.
 * Devuelve GANO_SEGUNDO si pkm_2 posee más estadísticas pares que pkm_1.
 * Si pkm_1 tiene más estadísticas pares o la misma cantidad se devuelve GANO_PRIMERO
 */
int funcion_batalla_5(void* pkm_1, void* pkm_2);

#endif /* __BATALLAS_H__ */