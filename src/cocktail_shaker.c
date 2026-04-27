/**
 * @file cocktail_shaker.c
 * @brief Implementacion del algoritmo cocktail shaker sort.
 */

#include "sorting.h"

/**
 * @brief Determina si dos deportistas deben intercambiarse.
 *
 * @param left Deportista izquierdo.
 * @param right Deportista derecho.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Orden de ordenamiento (ascendente o descendente).
 * @return int 1 si se deben intercambiar, 0 en caso contrario.
 */
static int should_swap(Deportista left, Deportista right, SortCriteria criteria, SortOrder order)
{
    int cmp = compare_by_criteria(left, right, criteria);

    if(order == ASCENDING) {
        return cmp > 0;
    }

    return cmp < 0;
}

/**
 * @brief Ordena un arreglo de deportistas con cocktail shaker sort.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param length Largo del arreglo de deportistas.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Sentido de ordenamiento (ascendente o descendente).
 */
void cocktail_shaker_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order)
{
    int left = 0;
    int right = length - 1;
    int swapped = 1;

    if(deportistas == NULL || length < 2) {
        return;
    }

    while(swapped) {
        swapped = 0;

        for(int i = left; i < right; i++) {
            if(should_swap(deportistas[i], deportistas[i + 1], criteria, order)) {
                swap_deportistas(&deportistas[i], &deportistas[i + 1]);
                swapped = 1;
            }
        }

        if(!swapped) {
            break;
        }

        right--;
        swapped = 0;

        for(int i = right; i > left; i--) {
            if(should_swap(deportistas[i - 1], deportistas[i], criteria, order)) {
                swap_deportistas(&deportistas[i - 1], &deportistas[i]);
                swapped = 1;
            }
        }

        left++;
    }
}
