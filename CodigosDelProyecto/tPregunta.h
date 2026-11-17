/*
 * ============================================================================
 * TAD: tPregunta
 * Archivo: tPregunta.h
 * Descripcion: Modulo encargado de modelar las preguntas de la trivia y
 *              gestionar la Lista Enlazada dinamica filtrada por categoria
 *              y dificultad.
 * ============================================================================
 */

#ifndef TPREGUNTA_H
#define TPREGUNTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEXTO 150
#define CAN_OPCIONES 4

typedef char tString[MAX_TEXTO];

      /* Estructura que representa una pregunta individual del banco */
typedef struct {
    int id;                         /* Identificador unico de la pregunta */
    int categoria;                  /* 1: Balon de Oro, 2: Mundial, 3: Champions, 4: Libertadores */
    int dificultad;                 /* 1: Facil, 2: Medio, 3: Dificil*/
    tString enunciado;              /* Texto de la pregunta */
    tString opciones[CAN_OPCIONES]; /* Arreglo con las 4 opciones posibles */
    int opcionCorrecta;             /* Numero de la opcion correcta (1 a 4) */
} tPregunta;

/* Nodo de la Lista Enlazada dinamica de preguntas */
typedef struct NodoPregunta {
    tPregunta dato;
    struct NodoPregunta *siguiente;
} tNodoPregunta;

/* Puntero al primer nodo de la lista */
typedef tNodoPregunta* tListaPreguntas;

      /* --- Prototipos de operaciones del TAD --- */

/* Inicializa la lista de preguntas en NULL */
void iniciarListaPreguntas(tListaPreguntas *lista);

/* Lee el archivo de texto y carga en la lista solo las preguntas que coinciden
   con la categoria y dificultad solicitadas */
bool cargarPreguntasPorCategoriaYDificultad(tListaPreguntas *lista, char nombreArchivo[], int categoriaElegida, int dificultadElegida);

/* Devuelve la cantidad total de nodos actualmente en la lista */
int contarPreguntas(tListaPreguntas lista);

/* Busca y copia en 'preguntaExtraida' el nodo ubicado en 'posicion' (1..N) */
bool obtenerPreguntaPorPosicion(tListaPreguntas lista, int posicion, tPregunta *preguntaExtraida);

/* Elimina de la lista el nodo cuyo id coincida con 'idPregunta' */
bool eliminarPregunta(tListaPreguntas *lista, int idPregunta);

/* Libera toda la memoria dinamica ocupada por la lista */
void liberarListaPreguntas(tListaPreguntas *lista);

#endif
