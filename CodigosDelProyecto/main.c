/*
 * ============================================================================
 * PROYECTO INTEGRADOR - ALGORITMOS Y ESTRUCTURAS DE DATOS II
 * Archivo: main.c
 * Tema: Juego de Trivia de Futbol con Dificultad Progresiva y Rankings
 *
 * Estructuras y TADs integrados:
 *   1. tPregunta  -> Lista Enlazada dinamica + lectura de archivo de texto
 *   2. tHistorial -> Pila dinamica (LIFO) para revision de respuestas
 *   3. tRanking   -> Archivo binario (ranking.dat) + ordenamiento Burbuja
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "tPregunta.h"
#include "tRanking.h"
#include "tHistorial.h"

#define ARCHIVO_PREGUNTAS "preguntas.txt"
#define ARCHIVO_RANKING "ranking.dat"

/* --- Prototipos de funciones del programa principal --- */
void jugarPartida();
void limpiarBuffer();
void esperarEnter(char mensaje[]);

int main() {
    int opcion;

    /* Inicializa la semilla de tiempo para la seleccion aleatoria */
    srand(time(NULL));

    do {
        system("cls");
        printf("\n\t\t============================================\n");
        printf(" \t\t\tTRIVIA DE FUTBOL INTERACTIVA  \n");
        printf("\t\t============================================\n");
        printf(" 1. JUGAR\n");
        printf(" 2. RANKING\n");
        printf(" 0. Salir\n");
        printf("-----------------------------------------\n");
        printf(" Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            opcion = -1;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                jugarPartida();
                break;

            case 2: {
                int catRanking;
                system("cls");
                printf("\n\t\t============================================\n");
                printf("\t\t\tSELECCIONAR RANKING A CONSULTAR\n");
                printf("\t\t============================================\n");
                printf(" 1. Balon de Oro\n");
                printf(" 2. Mundial de Futbol\n");
                printf(" 3. UEFA Champions League\n");
                printf(" 4. Copa Libertadores\n");
                printf("-----------------------------------------\n");
                printf(" Seleccione la categoria que desea consultar (1-4): ");

                if (scanf("%d", &catRanking) != 1) {
                    catRanking = -1;
                }
                limpiarBuffer();

                if (catRanking >= 1 && catRanking <= 4) {
                    system("cls");
                    mostrarRanking(ARCHIVO_RANKING, catRanking);
                } else {
                    printf("\n [ERROR] Categoria invalida.\n");
                }
                esperarEnter("Presione ENTER para volver al menu...");
                break;
            }

            case 0:
                system("cls");
                printf("\n Gracias por jugar! Hasta la proxima.\n");
                break;

            default:
                printf("\n [ERROR] Opcion invalida. Reintente.\n");
                esperarEnter("Presione ENTER para continuar...");
                break;
        }

    } while (opcion != 0);

    return 0;
}

/*
 * Lee y descarta los caracteres sobrantes del teclado hasta el salto de linea.
 */
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Muestra un mensaje en pantalla y espera hasta que el usuario presione ENTER.
 */
void esperarEnter(char mensaje[]) {
    int c;
    printf("\n %s", mensaje);
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Controla el flujo completo de una partida:
 *   1. Permite elegir una de las 4 categorias futbolisticas.
 *   2. Ejecuta 3 etapas progresivas (Facil: 8 preg, Medio: 6 preg, Dificil: 4 preg).
 *   3. Selecciona preguntas al azar sin repetir (eliminandolas de la lista).
 *   4. Gestiona el sistema de pistas (50/50) y el calculo dinamico de puntaje.
 *   5. Apila cada resultado en tPilaHistorial y al final muestra el resumen y
 *      guarda el puntaje en el ranking binario.
 */
void jugarPartida() {
    int categoria, i, nivel;
    float puntajeTotal = 0.0;
    int respuestasCorrectasTotal = 0;
    int totalPreguntasJugadas = 0;

    tPilaHistorial pilaHistorial;
    tStringNombre nombreJugador;

    iniciarPila(&pilaHistorial);

    system("cls");
    printf("\n\t\t============================================\n");
    printf("\t\t\tSELECCION DE CATEGORIA          \n");
    printf("\t\t============================================\n");
    printf(" 1. Balon de Oro\n");
    printf(" 2. Mundial de Futbol\n");
    printf(" 3. UEFA Champions League\n");
    printf(" 4. Copa Libertadores\n");
    printf("-----------------------------------------\n");
    printf(" Elija la categoria que desea jugar (1-4): ");

    if (scanf("%d", &categoria) != 1) {
        categoria = -1;
    }
    limpiarBuffer();

    if (categoria < 1 || categoria > 4) {
        printf("\n [ERROR] Categoria no valida.\n");
        esperarEnter("Presione ENTER para regresar...");
        return;
    }

    /* Configuracion de las 3 etapas de dificultad progresiva */
    int preguntasPorNivel[3] = {8, 6, 4};
    int maxPistasPorNivel[3]  = {2, 1, 0};
    char *nombresNivel[3]    = {"FACIL", "MEDIO", "DIFICIL"};

    /* Bucle progresivo: Nivel 1 (Facil) -> Nivel 2 (Medio) -> Nivel 3 (Dificil) */
    for (nivel = 1; nivel <= 3; nivel++) {
        tListaPreguntas listaPreguntas;
        iniciarListaPreguntas(&listaPreguntas);

        /* Carga dinamica desde preguntas.txt filtrando por categoria y nivel */
        cargarPreguntasPorCategoriaYDificultad(&listaPreguntas, ARCHIVO_PREGUNTAS, categoria, nivel);

        int disponibles = contarPreguntas(listaPreguntas);
        if (disponibles == 0) {
            printf("\n [ERROR] No hay preguntas disponibles para esta categoria y nivel.\n");
            printf(" Verifique que '%s' se encuentre en el directorio del programa.\n", ARCHIVO_PREGUNTAS);
            liberarListaPreguntas(&listaPreguntas);
            liberarPila(&pilaHistorial);
            esperarEnter("Presione ENTER para volver al menu...");
            return;
        }

        int aJugar = preguntasPorNivel[nivel - 1];
        if (disponibles < aJugar) {
            aJugar = disponibles;
        }

        int maxPistas = maxPistasPorNivel[nivel - 1];

        for (i = 0; i < aJugar; i++) {
            totalPreguntasJugadas++;
            int posDisponibles = contarPreguntas(listaPreguntas);
            int posAleatoria = (rand() % posDisponibles) + 1;
            tPregunta pregActual;

            /* Obtiene una pregunta aleatoria de la Lista Enlazada */
            obtenerPreguntaPorPosicion(listaPreguntas, posAleatoria, &pregActual);

            bool opEliminada[CAN_OPCIONES] = {false, false, false, false};
            int pistasUsadas = 0;
            int respuestaUsuario = 0;
            bool respondio = false;
            char mensajeNotificacion[200] = "";

            while (!respondio) {
                system("cls");
                printf("\n\t\t============================================\n");
                printf("\t\t ETAPA %d: NIVEL %s (Pregunta %d/%d)\n", nivel, nombresNivel[nivel - 1], i + 1, aJugar);
                printf("\t\t============================================\n");

                if (strlen(mensajeNotificacion) > 0) {
                    printf("%s\n", mensajeNotificacion);
                }

                printf("\n-----------------------------------------\n");
                printf(" Pregunta #%d (Puntaje acumulado: %.2f pts):\n %s\n", totalPreguntasJugadas, puntajeTotal, pregActual.enunciado);
                printf("-----------------------------------------\n");

                int j;
                for (j = 0; j < CAN_OPCIONES; j++) {
                    if (opEliminada[j]) {
                        printf("  %d) [ OPCION ELIMINADA POR PISTA ]\n", j + 1);
                    } else {
                        printf("  %d) %s\n", j + 1, pregActual.opciones[j]);
                    }
                }

                if (maxPistas > 0 && pistasUsadas < maxPistas) {
                    printf("  5) *** PEDIR PISTA *** (Pistas usadas: %d/%d)\n", pistasUsadas, maxPistas);
                }

                printf("\n Ingrese su opcion (1-4)%s: ", (maxPistas > 0 && pistasUsadas < maxPistas) ? " o 5 para pedir pista" : "");
                if (scanf("%d", &respuestaUsuario) != 1) {
                    respuestaUsuario = -1;
                }
                limpiarBuffer();

                if (respuestaUsuario == 5 && maxPistas > 0 && pistasUsadas < maxPistas) {
                    int seleccionada;
                    do {
                        seleccionada = rand() % CAN_OPCIONES;
                    } while (seleccionada == (pregActual.opcionCorrecta - 1) || opEliminada[seleccionada]);

                    opEliminada[seleccionada] = true;
                    pistasUsadas++;
                    strcpy(mensajeNotificacion, " [PISTA ACTIVADA] Se descarto una opcion incorrecta.");
                } else if (respuestaUsuario >= 1 && respuestaUsuario <= 4) {
                    if (opEliminada[respuestaUsuario - 1]) {
                        strcpy(mensajeNotificacion, " [AVISO] Esa opcion ya fue descartada. Seleccione otra.");
                    } else {
                        respondio = true;
                    }
                } else {
                    strcpy(mensajeNotificacion, " [ERROR] Opcion invalida. Reintente.");
                }
            }

            /* Calculo de puntaje segun nivel de dificultad y pistas utilizadas */
            float puntosEstaPregunta = 0.0;
            if (nivel == 1) {        /* FACIL */
                if (pistasUsadas == 0) puntosEstaPregunta = 10.0;
                else if (pistasUsadas == 1) puntosEstaPregunta = 6.0;
                else if (pistasUsadas == 2) puntosEstaPregunta = 3.0;
            } else if (nivel == 2) { /* MEDIO */
                if (pistasUsadas == 0) puntosEstaPregunta = 15.0;
                else if (pistasUsadas == 1) puntosEstaPregunta = 7.0;
            } else if (nivel == 3) { /* DIFICIL (sin pistas) */
                puntosEstaPregunta = 20.0;
            }

            /* Prepara el registro para apilar en el historial LIFO */
            tRespuestaHistorial resHist;
            strcpy(resHist.enunciado, pregActual.enunciado);
            strcpy(resHist.respuestaUsuario, pregActual.opciones[respuestaUsuario - 1]);
            strcpy(resHist.respuestaCorrecta, pregActual.opciones[pregActual.opcionCorrecta - 1]);

            if (respuestaUsuario == pregActual.opcionCorrecta) {
                printf("\n CORRECTO! (+%.2f puntos)\n", puntosEstaPregunta);
                puntajeTotal += puntosEstaPregunta;
                respuestasCorrectasTotal++;
                resHist.esCorrecta = true;
                resHist.puntosObtenidos = puntosEstaPregunta;
            } else {
                printf("\n INCORRECTO. La respuesta correcta era: %s\n", pregActual.opciones[pregActual.opcionCorrecta - 1]);
                resHist.esCorrecta = false;
                resHist.puntosObtenidos = 0.0;
            }

            /* Apila el resultado en la estructura LIFO */
            apilarRespuesta(&pilaHistorial, resHist);

            /* Elimina la pregunta respondida de la lista para evitar repeticiones */
            eliminarPregunta(&listaPreguntas, pregActual.id);

            esperarEnter("Presione ENTER para continuar...");
        }

        /* Libera la lista enlazada del nivel completado */
        liberarListaPreguntas(&listaPreguntas);

        if (nivel < 3) {
            system("cls");
            printf("\n\t\t============================================\n");
            printf("\t\t FELICITACIONES! COMPLETASTE EL NIVEL %s\n", nombresNivel[nivel - 1]);
            printf("\t\t============================================\n");
            printf(" Puntaje acumulado hasta ahora: %.2f puntos\n", puntajeTotal);
            esperarEnter("Presione ENTER para pasar al siguiente nivel...");
        }
    }

    /* Resumen final, revision LIFO y persistencia binaria */
    system("cls");
    printf("\n\t\t============================================\n");
    printf("\t\t\tFIN DE LA TRIVIA COMPLETA     \n");
    printf("\t\t============================================\n");
    printf(" Respuestas correctas: %d / %d\n", respuestasCorrectasTotal, totalPreguntasJugadas);
    printf(" Puntaje final:        %.2f puntos\n", puntajeTotal);

    /* Muestra el historial en orden inverso (Pila LIFO) */
    mostrarHistorial(pilaHistorial);

    printf("\n Ingrese su Nickname (sin espacios) para guardar su puntaje en el Ranking: ");
    scanf("%s", nombreJugador);
    limpiarBuffer();

    /* Guarda el registro en ranking.dat */
    guardarPuntaje(ARCHIVO_RANKING, nombreJugador, puntajeTotal, categoria);

    esperarEnter("Presione ENTER para volver al menu principal...");

    /* Libera toda la memoria de la pila antes de salir */
    liberarPila(&pilaHistorial);
}
