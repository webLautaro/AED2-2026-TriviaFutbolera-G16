/*
 * ============================================================================
 * TAD: tRanking
 * Archivo: tRanking.c
 * Descripcion: Implementacion de la lectura/escritura en archivo binario y
 *              ordenamiento descendente por puntaje para el ranking.
 * ============================================================================
 */

#include "tRanking.h"

/*
 * Abre el archivo binario en modo adicion ("ab") y graba un registro
 * tRegRanking con el nombre, puntaje y categoria del jugador.
*/
void guardarPuntaje(char nombreArchivo[], tStringNombre nombre, float puntaje, int categoria) {
    FILE *arch = fopen(nombreArchivo, "ab");
    if (arch == NULL) {
        printf("\n[ERROR] No se pudo abrir el archivo de ranking para escritura.\n");
        return;
    }

    tRegRanking reg;
    strcpy(reg.nombre, nombre);
    reg.puntaje = puntaje;
    reg.categoria = categoria;

    fwrite(&reg, sizeof(tRegRanking), 1, arch);
    fclose(arch);

    printf("\n[OK] Puntaje guardado exitosamente en el Ranking!\n");
}

/*
 * Lee secuencialmente el archivo binario filtrando los registros que
* corresponden a 'categoriaElegida', los carga en un vector auxiliar,
 * los ordena de mayor a menor puntaje (Metodo de Burbuja) y los imprime.
*/
void mostrarRanking(char nombreArchivo[], int categoriaElegida) {
    if (categoriaElegida < 1 || categoriaElegida > 4) {
        printf("\n[ERROR] Categoria invalida.\n");
        return;
    }

    FILE *arch = fopen(nombreArchivo, "rb");
    char *nombresCat[4] = {"BALON DE ORO", "MUNDIAL DE FUTBOL", "UEFA CHAMPIONS LEAGUE", "COPA LIBERTADORES"};

    if (arch == NULL) {
        printf("\n\t\t============================================\n");
        printf("\t\t RANKING - %s\n", nombresCat[categoriaElegida - 1]);
        printf("\t\t============================================\n");
        printf(" Aun no hay partidas registradas en esta categoria.\n");
        printf("=========================================\n");
        return;
    }

    tRegRanking lista[MAX_JUGADORES];
    tRegRanking reg;
    int cant = 0;
    int i, j;

    /* Lectura del archivo binario con lectura adelantada */
    fread(&reg, sizeof(tRegRanking), 1, arch);
    while (!feof(arch) && cant < MAX_JUGADORES) {
        if (reg.categoria == categoriaElegida) {
            lista[cant] = reg;
            cant++;
        }
        fread(&reg, sizeof(tRegRanking), 1, arch);
    }
    fclose(arch);

    printf("\n\t\t============================================\n");
    printf("\t\t TABLA DE POSICIONES - %s\n", nombresCat[categoriaElegida - 1]);
    printf("\t\t============================================\n");

    if (cant == 0) {
        printf(" No hay registros guardados para esta categoria.\n");
        printf("=========================================\n");
        return;
    }

    /* Ordenamiento por Burbuja (descendente por puntaje) */
    tRegRanking aux;
    for (i = 0; i < cant - 1; i++) {
        for (j = 0; j < cant - 1 - i; j++) {
            if (lista[j].puntaje < lista[j + 1].puntaje) {
                aux = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
        }
    }

    printf("%-5s %-25s %-10s\n", "POS", "JUGADOR", "PUNTAJE");
    printf("-----------------------------------------\n");

    for (i = 0; i < cant; i++) {
        printf("%-5d %-25s %-10.2f\n", i + 1, lista[i].nombre, lista[i].puntaje);
    }
    printf("=========================================\n");
}
