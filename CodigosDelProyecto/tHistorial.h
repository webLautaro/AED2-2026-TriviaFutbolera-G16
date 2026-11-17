/*
 * ============================================================================
 * TAD: tHistorial
 * Archivo: tHistorial.h
 * Descripcion: Modulo encargado de registrar el historial de respuestas dadas
 *              durante la partida actual mediante una Pila Dinamica (LIFO).
 * ============================================================================
 */

#ifndef THISTORIAL_H
#define THISTORIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TEXTO 150

typedef char tStringTexto[MAX_TEXTO];

    /* Registro que almacena el detalle de cada pregunta respondida en la partida */
typedef struct {
    tStringTexto enunciado;         //Texto de la pregunta formulada
    tStringTexto respuestaUsuario;  //Texto de la opcion elegida por el jugador
    tStringTexto respuestaCorrecta; //Texto de la opcion correcta
    bool esCorrecta;                //Indica si el jugador acerto (true/false)
    float puntosObtenidos;          //Puntaje ganado en esa pregunta
} tRespuestaHistorial;

/* Nodo de la Pila dinamica simplemente enlazada */
typedef struct NodoPila {
    tRespuestaHistorial dato;
    struct NodoPila *siguiente;
} tNodoPila;

/* Puntero al tope de la Pila (NULL cuando esta vacia) */
typedef tNodoPila* tPilaHistorial;

      /* --- Prototipos de operaciones del TAD --- */

/* Inicializa el tope de la Pila en NULL */
void iniciarPila(tPilaHistorial *pila);

/* Inserta (apila) una nueva respuesta en el tope de la Pila (politica LIFO) */
void apilarRespuesta(tPilaHistorial *pila, tRespuestaHistorial dato);

/* Recorre y muestra por pantalla el historial desde la ultima pregunta a la primera */
void mostrarHistorial(tPilaHistorial pila);

/* Libera toda la memoria dinamica reservada por los nodos de la Pila */
void liberarPila(tPilaHistorial *pila);

#endif
