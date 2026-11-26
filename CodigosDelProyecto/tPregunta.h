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

/* --- Implementacion de operaciones del TAD --- */

/*
 * Inicializa la lista enlazada vacia.
 */
void iniciarListaPreguntas(tListaPreguntas *lista) {
    *lista = NULL;
}

/*
 * Recorre el enunciado buscando 4 digitos seguidos que formen un anio valido
 * (entre 1930 y 2030) y asigna automaticamente el nivel de dificultad:
 *   - Anio >= 2000 (o sin anio): Nivel 1 (Facil)
 *   - Anio entre 1980 y 1999:    Nivel 2 (Medio)
 *   - Anio entre 1930 y 1979:    Nivel 3 (Dificil / Historico)
 */
int calcularDificultadPorAnio(tString enunciado) {
    int i, anio = 0, val;
    int longitud = strlen(enunciado);

    for (i = 0; i <= longitud - 4; i++) {
        if ((enunciado[i] >= '0' && enunciado[i] <= '9') &&
            (enunciado[i + 1] >= '0' && enunciado[i + 1] <= '9') &&
            (enunciado[i + 2] >= '0' && enunciado[i + 2] <= '9') &&
            (enunciado[i + 3] >= '0' && enunciado[i + 3] <= '9')) {

            /* Convierte los 4 caracteres ASCII a un numero entero */
            val = (enunciado[i] - '0') * 1000 +
                  (enunciado[i + 1] - '0') * 100 +
                  (enunciado[i + 2] - '0') * 10 +
                  (enunciado[i + 3] - '0');

            if (val >= 1930 && val <= 2030) {
                anio = val;
                break;
            }
        }
    }

    if (anio == 0) return 1;
    if (anio >= 2000) return 1;
    else if (anio >= 1980) return 2;
    else return 3;
}

/*
 * Abre el archivo de texto (formato de 8 lineas por pregunta), calcula la
 * dificultad de cada registro e inserta al inicio de la lista enlazada
 * aquellas preguntas que pertenezcan a 'categoriaElegida' y 'dificultadElegida'.
 */
bool cargarPreguntasPorCategoriaYDificultad(tListaPreguntas *lista, char nombreArchivo[], int categoriaElegida, int dificultadElegida) {
    FILE *arch = fopen(nombreArchivo, "r");
    if (arch == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo de preguntas '%s'.\n", nombreArchivo);
        return false;
    }

    tPregunta pregTemp;
    int i, len;

    while (fscanf(arch, "%d\n", &pregTemp.id) == 1) {
        fscanf(arch, "%d\n", &pregTemp.categoria);

        fgets(pregTemp.enunciado, MAX_TEXTO, arch);
        len = strlen(pregTemp.enunciado);
        while (len > 0 && (pregTemp.enunciado[len - 1] == '\n' || pregTemp.enunciado[len - 1] == '\r')) {
            pregTemp.enunciado[len - 1] = '\0';
            len--;
        }

        for (i = 0; i < CAN_OPCIONES; i++) {
            fgets(pregTemp.opciones[i], MAX_TEXTO, arch);
            len = strlen(pregTemp.opciones[i]);
            while (len > 0 && (pregTemp.opciones[i][len - 1] == '\n' || pregTemp.opciones[i][len - 1] == '\r')) {
                pregTemp.opciones[i][len - 1] = '\0';
                len--;
            }
        }

        fscanf(arch, "%d\n", &pregTemp.opcionCorrecta);

        /* Clasificacion automatica de dificultad */
        pregTemp.dificultad = calcularDificultadPorAnio(pregTemp.enunciado);

        /* Insercion dinamica al frente de la lista si cumple ambos filtros */
        if (pregTemp.categoria == categoriaElegida && pregTemp.dificultad == dificultadElegida) {
            tNodoPregunta *nuevoNodo = (tNodoPregunta*) malloc(sizeof(tNodoPregunta));
            if (nuevoNodo != NULL) {
                nuevoNodo->dato = pregTemp;
                nuevoNodo->siguiente = *lista;
                *lista = nuevoNodo;
            }
        }
    }

    fclose(arch);
    return true;
}

/*
 * Cuenta y retorna cuantos nodos existen actualmente en la lista.
 */
int contarPreguntas(tListaPreguntas lista) {
    int cont = 0;
    tNodoPregunta *aux = lista;
    while (aux != NULL) {
        cont++;
        aux = aux->siguiente;
    }
    return cont;
}

/*
 * Recorre la lista hasta alcanzar la posicion indicada (base 1) y copia
 * el contenido del nodo en 'preguntaExtraida'. Retorna true si la encontro.
 */
bool obtenerPreguntaPorPosicion(tListaPreguntas lista, int posicion, tPregunta *preguntaExtraida) {
    int posActual = 1;
    tNodoPregunta *aux = lista;

    while (aux != NULL) {
        if (posActual == posicion) {
            *preguntaExtraida = aux->dato;
            return true;
        }
        posActual++;
        aux = aux->siguiente;
    }
    return false;
}

/*
 * Busca un nodo por su 'idPregunta', lo desenlaza de la lista y libera
 * su memoria. Se utiliza tras cada respuesta para evitar preguntas repetidas
 * dentro de una misma partida.
 */
bool eliminarPregunta(tListaPreguntas *lista, int idPregunta) {
    tNodoPregunta *actual = *lista;
    tNodoPregunta *anterior = NULL;

    while (actual != NULL && actual->dato.id != idPregunta) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) return false;

    if (anterior == NULL) {
        *lista = actual->siguiente; /* El nodo a eliminar es el primero */
    } else {
        anterior->siguiente = actual->siguiente; /* El nodo esta en el medio o al final */
    }

    free(actual);
    return true;
}

/*
 * Libera todos los nodos restantes de la lista al finalizar el nivel.
 */
void liberarListaPreguntas(tListaPreguntas *lista) {
    tNodoPregunta *aux;
    while (*lista != NULL) {
        aux = *lista;
        *lista = (*lista)->siguiente;
        free(aux);
    }
}

#endif
