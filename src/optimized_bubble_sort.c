/**
 * @file optimized_bubble_sort.c
 * @brief Implementacion de bubble sort optimizado para deportistas.
 */

#include "sorting.h"

/**
 * @brief Ordena un arreglo de deportistas usando bubble sort optimizado.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Orden de ordenamiento (ascendente o descendente).
 */
void optimized_bubble_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order)
{
    int swapped;

    if(deportistas == NULL || length < 2) {
        return;
    }

    for(int i = 0; i < length - 1; i++) {
        swapped = 0;

        for(int j = 0; j < length - i - 1; j++) {
            int cmp = compare_by_criteria(deportistas[j], deportistas[j + 1], criteria);
            int shouldSwap;

            if(order == ASCENDING) {
                shouldSwap = (cmp > 0);
            } else {
                shouldSwap = (cmp < 0);
            }

            if(shouldSwap) {
                swap_deportistas(&deportistas[j], &deportistas[j + 1]);
                swapped = 1;
            }
        }

        if(!swapped) {
            break;
        }
    }
}
