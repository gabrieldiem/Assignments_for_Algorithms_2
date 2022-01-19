#ifndef __TEST_SUITE_GD_H_
#define __TEST_SUITE_GD_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define BLANCO "\x1b[37;1m"
#define VERDE "\x1b[32;1m"
#define ROJO "\x1b[31;1m"
#define AMARILLO "\x1b[33;1m"
#define CYAN "\x1b[36;1m"
#define NORMAL "\x1b[0m"

#define TILDE "✓"
#define CRUZ "✗"

int __tsgd_cantidad_de_pruebas_corridas = 0;
int __tsgd_cantidad_de_pruebas_fallidas = 0;
const char* __tsgd_prueba_actual = NULL;

void __tsgd_atajarse(void (*handler)(int)){
  signal(SIGABRT, handler);
  signal(SIGSEGV, handler);
  signal(SIGBUS, handler);
  signal(SIGILL, handler);
  signal(SIGFPE, handler);
}

void __tsgd_morir(int signum){
  if(__tsgd_prueba_actual)
    printf(ROJO "\n\nERROR MIENTRAS SE EJECUTABA LA PRUEBA: " AMARILLO "'%s'\n\n" BLANCO, __tsgd_prueba_actual);
  else printf(ROJO "\n\nFINALIZACION ANORMAL DE LAS PRUEBAS\n\n"BLANCO);
  fflush(stdout);
  __tsgd_atajarse(SIG_DFL);
}

#define tsgd_afirmar(afirmacion, descripcion) do {      \
    __tsgd_prueba_actual = descripcion;                 \
    __tsgd_atajarse(__tsgd_morir);                      \
    if (afirmacion) {                                   \
      printf( VERDE TILDE " ");                         \
    } else {                                            \
      __tsgd_cantidad_de_pruebas_fallidas++;            \
      printf( ROJO CRUZ " ");                           \
    }                                                   \
    printf(BLANCO "%s\n", __tsgd_prueba_actual);        \
    fflush(stdout);                                     \
    __tsgd_prueba_actual = NULL;                        \
    __tsgd_cantidad_de_pruebas_corridas++;              \
  }while(0);

void tsgd_nuevo_grupo(const char* descripcion){
  printf(AMARILLO "\n%s\n", descripcion);
  while(*(descripcion++)) printf("=");
  printf(BLANCO "\n");
}

void tsgd_nuevo_titulo(const char* descripcion){
  printf(CYAN "\n%s\n", descripcion);
  while(*(descripcion++)) printf("=");
  printf(BLANCO "\n");
}

int tsgd_mostrar_reporte() {
  printf(BLANCO "\n------------------------------------\n"
         "%i pruebas corridas, %i errores - %s\n" NORMAL,
         __tsgd_cantidad_de_pruebas_corridas,
         __tsgd_cantidad_de_pruebas_fallidas,
         __tsgd_cantidad_de_pruebas_fallidas == 0 ? "OK" : "D:");
  return __tsgd_cantidad_de_pruebas_fallidas;
}

#endif /* __TEST_SUITE_GD_H_ */