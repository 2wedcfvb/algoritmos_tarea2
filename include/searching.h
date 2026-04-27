/**
 * @file searching.h
 * @brief Algoritmos de busqueda sobre arreglos de deportistas.
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include "deportista.h"

/**
 * @brief Busca secuencialmente un deportista por ID.
 *
 * @param deportistas Arreglo de deportistas.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int sequential_search_by_id(Deportista *deportistas, int length, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda binaria.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int binary_search_by_id(Deportista *deportistas, int length, int targetId);

#endif
