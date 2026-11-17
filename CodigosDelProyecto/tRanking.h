/*
 * ============================================================================
 * TAD: tRanking
 * Archivo: tRanking.h
 * Descripcion: Modulo encargado de persistir los puntajes obtenidos en un
 *              archivo binario (ranking.dat) y mostrar la tabla de posiciones
 *              ordenada por categoria.
 * ============================================================================
 */

#ifndef TRANKING_H
#define TRANKING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_JUGADORES 100

typedef char tStringNombre[50];

      /* Registro almacenado en el archivo binario ranking.dat */
typedef struct {
    tStringNombre nombre; /* Nickname del jugador */
    float puntaje;        /* Puntaje total acumulado en la partida */
    int categoria;        /* 1: Balon de Oro, 2: Mundial, 3: Champions, 4: Libertadores */
} tRegRanking;

       /* --- Prototipos de operaciones del TAD --- */

/* Agrega un nuevo registro de puntaje al final del archivo binario */
void guardarPuntaje(char nombreArchivo[], tStringNombre nombre, float puntaje, int categoria);

/* Lee el archivo binario, filtra por categoria, ordena de mayor a menor
   mediante Burbuja y muestra la tabla de posiciones */
void mostrarRanking(char nombreArchivo[], int categoriaElegida);

#endif
