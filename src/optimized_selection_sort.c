/**
 * @file selection_sort.c
 * @brief Implementacion de selection sort para deportistas.
 */

#include "sorting.h"

/**
 * @brief Ordena un arreglo de deportistas usando selection sort.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Orden de ordenamiento (ascendente o descendente).
 */
void optimized_selection_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order)
{
    if(deportistas == NULL || length < 2) {
        return;
    }

    for(int left = 0, right = length - 1; left < right; left++, right--) {
        int minIndex = left;
        int maxIndex = left;

        for(int i = left + 1; i <= right; i++) {
            if(compare_by_criteria(deportistas[i], deportistas[minIndex], criteria) < 0) {
                minIndex = i;
            }

            if(compare_by_criteria(deportistas[i], deportistas[maxIndex], criteria) > 0) {
                maxIndex = i;
            }
        }

        if(order == ASCENDING) {
            if(minIndex != left) {
                swap_deportistas(&deportistas[left], &deportistas[minIndex]);
            }

            if(maxIndex == left) {
                maxIndex = minIndex;
            }

            if(maxIndex != right) {
                swap_deportistas(&deportistas[right], &deportistas[maxIndex]);
            }
        } else {
            if(maxIndex != left) {
                swap_deportistas(&deportistas[left], &deportistas[maxIndex]);
            }

            if(minIndex == left) {
                minIndex = maxIndex;
            }

            if(minIndex != right) {
                swap_deportistas(&deportistas[right], &deportistas[minIndex]);
            }
        }
    }
}
