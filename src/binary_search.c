/**
 * @file binary_search.c
 * @brief Implementacion de busqueda binaria iterativa.
 */

#include "searching.h"

/**
 * @brief Busqueda binaria iterativa de un deportista por ID.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID en orden ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID a buscar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 * @warning El arreglo debe estar ordenado por ID en orden ascendente.
 */
int binary_search_by_id(Deportista *deportistas, int length, int targetId)
{
    int left, right;

    if(deportistas == NULL || length < 1) {
        return -1;
    }

    left = 0;
    right = length - 1;

    while(left <= right) {
        int mid = left + (right - left) / 2;

        if(deportistas[mid] == NULL) {
            return -1;
        }

        if(deportistas[mid]->id == targetId) {
            return mid;
        }

        if(deportistas[mid]->id < targetId) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}
