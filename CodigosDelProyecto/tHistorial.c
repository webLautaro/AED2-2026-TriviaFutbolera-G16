/*
 * ============================================================================
 * TAD: tHistorial
 * Archivo: tHistorial.c
 * Descripcion: Implementacion de las operaciones de la Pila LIFO para la
 *              revision de respuestas al finalizar una partida.
 * ============================================================================
 */

#include "tHistorial.h"

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
