/**
 * @file insertion_sort.c
 * @brief Implementacion de insertion sort para deportistas.
 */

#include "sorting.h"

/**
 * @brief Ordena un arreglo de deportistas usando insertion sort.
 *
 * @param deportistas Arreglo de deportistas a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio a utilizar para la comparacion.
 * @param order Orden de ordenamiento (ascendente o descendente).
 */
void insertion_sort_deportistas(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order)
{
    if(deportistas == NULL || length < 2) {
        return;
    }

    for(int i = 1; i < length; i++) {
        Deportista key = deportistas[i];
        int j = i - 1;

        while(j >= 0) {
            int cmp = compare_by_criteria(deportistas[j], key, criteria);
            int shouldShift;

            if(order == ASCENDING) {
                shouldShift = (cmp > 0);
            } else {
                shouldShift = (cmp < 0);
            }

            if(!shouldShift) {
                break;
            }

            deportistas[j + 1] = deportistas[j];
            j--;
        }

        deportistas[j + 1] = key;
    }
}
