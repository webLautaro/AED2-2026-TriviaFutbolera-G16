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


/* --- Implementacion de operaciones del TAD --- */

/*
 * Inicializa la pila dejando su puntero tope en NULL.
 */
void iniciarPila(tPilaHistorial *pila) {
    *pila = NULL;
}

/*
 * Reserva memoria dinamica para un nuevo nodo y lo inserta en el tope
 * de la pila.
 */
void apilarRespuesta(tPilaHistorial *pila, tRespuestaHistorial dato) {
    tNodoPila *nuevo = (tNodoPila*) malloc(sizeof(tNodoPila));
    if (nuevo != NULL) {
        nuevo->dato = dato;
        nuevo->siguiente = *pila;
        *pila = nuevo;
    } else {
        printf("\n[ERROR] Memoria insuficiente para registrar el historial.\n");
    }
}

/*
 * Recorre la pila desde el tope hasta NULL mostrando cada pregunta respondida,
 * la eleccion del usuario, la respuesta correcta y los puntos obtenidos.
 */
void mostrarHistorial(tPilaHistorial pila) {
    if (pila == NULL) {
        printf("\nNo hay respuestas registradas en esta partida.\n");
        return;
    }

    printf("\n\t\t============================================\n");
    printf("\t\t\tREVISION DE PARTIDA (ESTRUCTURA PILA)  \n");
    printf("\t\t============================================\n");

    tNodoPila *aux = pila;
    int num = 1;

    while (aux != NULL) {
        printf("\nPregunta %d (orden inverso): %s\n", num, aux->dato.enunciado);
        printf("  Tu respuesta:       %s\n", aux->dato.respuestaUsuario);
        printf("  Respuesta correcta: %s\n", aux->dato.respuestaCorrecta);

        if (aux->dato.esCorrecta) {
            printf("  Resultado:          [ CORRECTO (+%.2f pts) ]\n", aux->dato.puntosObtenidos);
        } else {
            printf("  Resultado:          [ INCORRECTO (0.00 pts) ]\n");
        }
        printf("-----------------------------------------\n");

        aux = aux->siguiente;
        num++;
    }
}

/*
 * Recorre toda la pila liberando nodo por nodo mediante free() para evitar
 * fugas de memoria al terminar la partida.
 */
void liberarPila(tPilaHistorial *pila) {
    tNodoPila *aux;
    while (*pila != NULL) {
        aux = *pila;
        *pila = (*pila)->siguiente;
        free(aux);
    }
}
#endif
