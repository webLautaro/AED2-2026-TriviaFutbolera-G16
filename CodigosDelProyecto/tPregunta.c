/*
 * ============================================================================
 * TAD: tPregunta
 * Archivo: tPregunta.c
 * Descripcion: Implementacion de la Lista Enlazada de preguntas, lectura desde
 *              archivo de texto plano, clasificacion automatica de dificultad
 *              por anio y eliminacion de preguntas ya jugadas.
 * ============================================================================
 */

#include "tPregunta.h"

/* Prototipo de funcion auxiliar usada dentro de este modulo */
int calcularDificultadPorAnio(tString enunciado);

/*
 * Inicializa la lista enlazada vacia.
 */
void iniciarListaPreguntas(tListaPreguntas *lista) {
    *lista = NULL;
}

/*
 * Recorre el enunciado buscando 4 digitos seguidos que formen un anño valido
 * (entre 1930 y 2030) y asigna automaticamente el nivel de dificultad:
 *   - Año >= 2000 (o sin año): Nivel 1 (Facil)
 *   - Año entre 1980 y 1999:    Nivel 2 (Medio)
 *   - Año entre 1930 y 1979:    Nivel 3 (Dificil / Historico)
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
