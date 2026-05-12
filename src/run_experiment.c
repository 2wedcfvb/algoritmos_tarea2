/**
 * @file run_experiment.c
 * @brief Flujos interactivos de ordenamiento, busqueda y ranking.
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "base.h"
#include "csv.h"
#include "errors.h"
#include "print_format.h"
#include "searching.h"
#include "sorting.h"
#include "kth_athlete.h"

/**
 * @brief Carga el CSV actual en un arreglo.
 *
 * @param deportistas Direccion donde se entrega el arreglo creado.
 * @param count Direccion donde se entrega la cantidad de elementos.
 * @return int 1 si la carga fue exitosa, 0 en caso contrario.
 */
static int load_data(Deportista **deportistas, int *count)
{
    if(deportistas == NULL) {
        return 0;
    }
    if(count == NULL) {
        return 0;
    }

    *deportistas = load_deportistas_array(count);
    if(*deportistas == NULL) {
        print_error(ERROR_NO_DATA_LOADED, NULL);
        return 0;
    }

    if(*count == 0) {
        free_deportistas_array(*deportistas, *count);
        print_error(ERROR_NO_DATA_LOADED, NULL);
        return 0;
    }

    return 1;
}

/**
 * @brief Verifica si una opcion de algoritmo de ordenamiento es valida.
 *
 * @param value Opcion a verificar.
 * @return int 1 si es valida, 0 en caso contrario.
 */
static int is_valid_sort_algorithm_option(int value)
{
    return (value >= INSERTION_SORT && value <= QUICK_SORT_MEDIAN);
}

/**
 * @brief Pregunta que ordenamiento usar.
 *
 * @return SortAlgorithm Opcion seleccionada.
 */
static SortAlgorithm ask_sort_algorithm(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Algoritmo de ordenamiento ===\n" RESET);
        printf(DIM GRAY "(Los algoritmos en gris estan pendientes)\n\n" RESET);

        printf("  1) Insertion sort\n");
        printf("  2) Bubble sort                 " DIM GRAY "[optimizado]" RESET "\n");
        printf("  3) Selection sort              " DIM GRAY "[optimizado]" RESET "\n");
        printf("  4) Cocktail shaker sort\n");
        printf("  5) Merge sort\n");
        printf("  6) Merge sort optimizado       " DIM GRAY "[optimizado]" RESET "\n");
        printf("  7) Quick sort                  " DIM GRAY "[pivote primero]" RESET "\n");
        printf("  8) Quick sort                  " DIM GRAY "[pivote ultimo]" RESET "\n");
        printf("  9) Quick sort                  " DIM GRAY "[pivote aleatorio]" RESET "\n");
        printf(" 10) Quick sort                  " DIM GRAY "[pivote mediana]" RESET "\n\n");

        printf(BOLD "Opcion: " RESET);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(!is_valid_sort_algorithm_option(selected));

    return (SortAlgorithm)selected;
}

/**
 * @brief Pregunta que busqueda usar.
 *
 * @return SearchAlgorithm Opcion seleccionada.
 */
static SearchAlgorithm ask_search_algorithm(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Algoritmo de busqueda ===\n" RESET);
        printf(DIM GRAY "(Algunas busquedas requieren arreglo ordenado)\n\n" RESET);

        printf("  1) Busqueda secuencial            " DIM GRAY "[ID]" RESET "\n");
        printf("  2) Busqueda binaria               " DIM GRAY "[ID]" RESET "\n");
        printf("  3) Busqueda binaria recursiva     " DIM GRAY "[ID]" RESET "\n");
        printf("  4) Busqueda binaria por rango     " DIM GRAY "[PUNTAJE]" RESET "\n");
        printf("  5) Busqueda exponencial           " DIM GRAY "[ID]" RESET "\n");
        printf("  6) Busqueda por interpolacion     " DIM GRAY "[ID]" RESET "\n\n");

        printf(BOLD "Opcion: " RESET);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(selected < SEQUENTIAL_SEARCH || selected > INTERPOLATION_SEARCH);

    return selected;
}

/**
 * @brief Ejecuta una operacion de ordenamiento o ranking.
 *
 * @param criteria Campo por el cual se ordena.
 * @param rankingAmount Cantidad de elementos a mostrar.
 * @param order Sentido de ordenamiento.
 */
static double run_sort_operation(SortCriteria criteria, int rankingAmount, SortOrder order)
{
    Deportista *deportistas = NULL;
    SortAlgorithm algorithmOption = ask_sort_algorithm();
    int count = 0;

    struct timespec startTime;
    struct timespec endTime;
    double elapsedSeconds = 0.0;
    int hasElapsed = 0;

    if(rankingAmount < 0) {
        rankingAmount = 0;
    }

    if(load_data(&deportistas, &count) == 0) {
        return 0.0;
    }

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    switch(algorithmOption) {
        case INSERTION_SORT:
            insertion_sort_deportistas(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;
        case BUBBLE_SORT:
            optimized_bubble_sort(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;
        case SELECTION_SORT:
            optimized_selection_sort(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;
        case COCKTAIL_SHAKER_SORT:
            cocktail_shaker_sort(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;
        case QUICK_SORT_FIRST:
            quick_sort_first(deportistas, 0, count - 1, criteria, order);
            hasElapsed = 1;
            break;
        case QUICK_SORT_LAST:
            quick_sort_last(deportistas, 0, count - 1, criteria, order);
            hasElapsed = 1;
            break;
        case QUICK_SORT_RANDOM:
            quick_sort_random(deportistas, 0, count - 1, criteria, order);
            hasElapsed = 1;
            break;
        case QUICK_SORT_MEDIAN:
            quick_sort_median(deportistas, 0, count - 1, criteria, order);
            hasElapsed = 1;
            break;
         case MERGE_SORT:
            merge_sort_deportistas(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;
        case OPTIMIZED_MERGE_SORT:
            merge_sort_optimized_deportistas(deportistas, count, criteria, order);
            hasElapsed = 1;
            break;   
        default:
            print_error(ERROR_NOT_IMPLEMENTED, NULL);
            break;
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    if(hasElapsed) {
        elapsedSeconds = (double)(endTime.tv_sec - startTime.tv_sec)
            + (double)(endTime.tv_nsec - startTime.tv_nsec) / 1e9;
        if(elapsedSeconds < 0) {
            elapsedSeconds = 0;
        }
    }

    if(rankingAmount > count) {
        rankingAmount = count;
    }

    print_sort_result_header(
        get_sort_algorithm_name(algorithmOption),
        get_sort_criteria_name(criteria),
        get_sort_order_name(order),
        rankingAmount,
        count,
        elapsedSeconds
    );
    print_deportistas_array(deportistas, rankingAmount);
    print_sort_result_footer();
    free_deportistas_array(deportistas, count);

    return elapsedSeconds;
}

/**
 * @brief Busca un deportista e imprime el resultado.
 *
 * @param targetId ID buscado.
 */
void search(int targetId)
{
    Deportista *deportistas = NULL;
    SearchAlgorithm algorithmOption = ask_search_algorithm();
    int count = 0;
    int index = -1;
    Range range = {-1,-1 };
    int isRangeResult = 0;
    int matches = 1;

    const char *algorithmLabel = NULL;
    const char *fieldLabel = NULL;
    char detail[32];

    struct timespec startTime;
    struct timespec endTime;
    double elapsedSeconds = 0;
    int hasElapsed = 0;

    if(load_data(&deportistas, &count) == 0) {
        return;
    }
    switch(algorithmOption){
        case SEQUENTIAL_SEARCH:
            algorithmLabel = "Secuencial";
            fieldLabel = "ID";
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            index = sequential_search(deportistas, count, SEARCH_BY_ID, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            break;
        case BINARY_SEARCH:
            algorithmLabel = "Binaria";
            fieldLabel = "ID";
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            index = binary_search(deportistas, count, SEARCH_BY_ID, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            break;
        case EXPONENCIAL_SEARCH:
            algorithmLabel = "Exponencial";
            fieldLabel = "ID";
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            index = exponencial_search(deportistas, count, SEARCH_BY_ID, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            break;
        case RANGE_BINARY_SEARCH:
            algorithmLabel = "Binaria por rango";
            fieldLabel = "PUNTAJE";
            isRangeResult = 1;
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_PUNTAJE, ASCENDING);
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            range = range_binary_search(deportistas, count, SEARCH_BY_PUNTAJE, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            if(range.start >= 0 && range.end >= range.start) {
                matches = (range.end - range.start + 1);
            }
            break;
        case INTERPOLATION_SEARCH:
            algorithmLabel = "Interpolacion";
            fieldLabel = "ID";
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            index = interpolation_search(deportistas, count, SEARCH_BY_ID, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            break;
        case RECURSIVE_BINARY_SEARCH:
            algorithmLabel = "Binaria recursiva";
            fieldLabel = "ID";
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
            clock_gettime(CLOCK_MONOTONIC, &startTime);
            index = recursive_binary_search(deportistas, 0, count - 1, SEARCH_BY_ID, targetId);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            hasElapsed = 1;
            break;
        default:
             print_error(ERROR_NOT_IMPLEMENTED, NULL);
             free_deportistas_array(deportistas, count);
             return;
    }   

    if(!isRangeResult) {
        if(index < 0) {
            snprintf(detail, sizeof(detail), "ID %d", targetId);
            print_error(ERROR_DEPORTISTA_NOT_FOUND, detail);
            free_deportistas_array(deportistas, count);
            return;
        }

        print_search_result_header(algorithmLabel, fieldLabel, targetId, 1);
        print_deportista(deportistas[index]);
        print_search_result_footer();
    }
    else {
        if(range.start < 0 || range.end < 0) {
            snprintf(detail, sizeof(detail), "Puntaje %d", targetId);
            print_error(ERROR_DEPORTISTA_NOT_FOUND, detail);
            free_deportistas_array(deportistas, count);
            return;
        }

        print_search_result_header(algorithmLabel, fieldLabel, targetId, matches);
        for(int i = range.start; i <= range.end; i++) {
            print_deportista(deportistas[i]);
        }
        print_search_result_footer();
    }

    if(hasElapsed) {
        elapsedSeconds = (double)(endTime.tv_sec - startTime.tv_sec)
            + (double)(endTime.tv_nsec - startTime.tv_nsec) / 1e9;
        if(elapsedSeconds < 0) {
            elapsedSeconds = 0;
        }
        printf(DIM GRAY "Tiempo total: %f s\n" RESET, elapsedSeconds);
    }

    free_deportistas_array(deportistas, count);
}

/**
 * @brief Obtiene los mejores N deportistas por puntaje usando Quick Select.
 *
 * Usa Quick Select para dejar los mejores N deportistas en el prefijo del
 * arreglo y luego ordena solo ese tramo para mostrar un ranking correcto.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad total de deportistas.
 * @param n Cantidad de puestos a obtener.
 * @return Deportista* Arreglo con los N mejores (debe liberarse con free), o NULL si falla la reserva de memoria.
 */
static Deportista *get_top_n_athletes(Deportista *deportistas, int count, int n)
{
    Deportista *ranking = NULL;

    if (deportistas == NULL || count <= 0 || n <= 0) {
        return NULL;
    }

    get_kth_element(deportistas, count, n - 1, SORT_BY_PUNTAJE, DESCENDING);

    if (n > 1) {
        quick_sort_median(deportistas, 0, n - 1, SORT_BY_PUNTAJE, DESCENDING);
    }

    ranking = malloc((size_t)n * sizeof(Deportista));
    if (ranking == NULL) {
        return NULL;
    }

    memcpy(ranking, deportistas, (size_t)n * sizeof(Deportista));

    return ranking;
}

/**
 * @brief Muestra el ranking de los mejores N deportistas por puntaje.
 *
 * @param rankingAmount Cantidad de deportistas a mostrar.
 */
void show_ranking(int rankingAmount)
{
    int count = 0;
    Deportista *deportistas = load_deportistas_array(&count);

    if (deportistas == NULL) {
        print_error(ERROR_MEMORY_ALLOCATION_FAILED, "No se pudo cargar CSV");
        return;
    }
    
    if (count == 0) {
        print_error(ERROR_NO_DATA_LOADED, NULL);
        free_deportistas_array(deportistas, count);
        return;
    }

    if (rankingAmount > count) {
        rankingAmount = count;
    }

    struct timespec startTime, endTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    Deportista *ranking = get_top_n_athletes(deportistas, count, rankingAmount);
    clock_gettime(CLOCK_MONOTONIC, &endTime);

    if (ranking == NULL) {
        print_error(ERROR_MEMORY_ALLOCATION_FAILED, "No se pudo reservar memoria para el ranking");
        free_deportistas_array(deportistas, count);
        return;
    }

    double elapsed = (double)(endTime.tv_sec - startTime.tv_sec)
                   + (double)(endTime.tv_nsec - startTime.tv_nsec) / 1e9;

    print_sort_result_header("Quick Select", "Puntaje", "Descendente", rankingAmount, count, elapsed);
    print_deportistas_array(ranking, rankingAmount);
    print_sort_result_footer();

    free(ranking);
    free_deportistas_array(deportistas, count);
}

/**
 * @brief Pregunta por el campo de ordenamiento.
 *
 * @return SortCriteria Opcion seleccionada.
 */
static SortCriteria ask_sort_criteria(void)
{
    char option[16];
    SortCriteria criteria;

    do {
        system("clear");

        printf(BOLD BLUE "=== Campo de ordenamiento ===\n" NORMAL);
        printf("  1) ID\n");
        printf("  2) Puntaje\n");
        printf("  3) Competencias\n");
        printf("  4) Nombre\n");
        printf("  5) Equipo\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        criteria = (SortCriteria)atoi(option);
    }
    while(criteria < SORT_BY_ID || criteria > SORT_BY_EQUIPO);

    return criteria;
}

/**
 * @brief Pregunta por el sentido del ordenamiento.
 *
 * @return SortOrder Opcion seleccionada.
 */
static SortOrder ask_sort_order(void)
{
    char option[16];
    int selected;

    do {
        system("clear");

        printf(BOLD BLUE "=== Orden ===\n" NORMAL);
        printf("  1) Descendente\n");
        printf("  2) Ascendente\n\n");
        printf(BOLD "Opcion: " NORMAL);

        if(fgets(option, sizeof(option), stdin) == NULL) {
            return 0;
        }

        selected = atoi(option);
    }
    while(selected < 1 || selected > 2);

    return (selected == 1) ? DESCENDING : ASCENDING;
}

/**
 * @brief Ejecuta el flujo interactivo de ordenamiento.
 */
void run_experiment(void)
{
    SortCriteria criteria = ask_sort_criteria();
    SortOrder order = ask_sort_order();

    (void)run_sort_operation(criteria, MAX_DATA, order);
}
