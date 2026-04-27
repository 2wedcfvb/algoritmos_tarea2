/**
 * @file sequential_search.c
 * @brief Implementacion de busqueda secuencial.
 */

#include "searching.h"

/**
 * @brief Busca secuencialmente un deportista por ID.
 *
 * @param deportistas Arreglo de deportistas.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID a buscar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int sequential_search_by_id(Deportista *deportistas, int length, int targetId)
{
    int i;

    if(deportistas == NULL) {
        return -1;
    }

    for(i = 0; i < length; i++) {
        if(deportistas[i] == NULL) {
            continue;
        }

        if(deportistas[i]->id == targetId) {
            return i;
        }
    }

    return -1;
}
