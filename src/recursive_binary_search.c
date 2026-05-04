/**
 * @file recursive_binary_search.c
 * @brief Implementacion de busqueda binaria recursiva para deportistas.
 *
 * Busca un deportista por ID dividiendo repetidamente el arreglo a la mitad
 * hasta encontrar el elemento o agotar el rango de busqueda.
 */

#include "searching.h"

int recursive_binary_search(Deportista *deportistas, int left, int right, SearchCriteria criteria, int targetId)
{
    (void)criteria;

    if(left > right) {
        return -1;
    } else {
        int mid = left + (right - left) / 2;

        if(deportistas[mid]->id == targetId) {
            return mid;
        }

        if(deportistas[mid]->id > targetId) {
            return recursive_binary_search(deportistas, left, mid - 1, criteria, targetId);
        } else {
            return recursive_binary_search(deportistas, mid + 1, right, criteria, targetId);
        }
    }
}