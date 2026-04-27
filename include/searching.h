/**
 * @file searching.h
 * @brief Algoritmos de busqueda sobre arreglos de deportistas.
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include "deportista.h"

/**
 * @brief Algoritmos de busqueda disponibles.
 */
typedef enum {
    SEQUENTIAL_SEARCH = 1, /**< Busqueda secuencial. */
    BINARY_SEARCH = 2,      /**< Busqueda binaria (requiere arreglo ordenado). */
    RECURSIVE_BINARY_SEARCH = 3, /**< Busqueda binaria recursiva (requiere arreglo ordenado). */
    RANGE_BINARY_SEARCH = 4, /**< Busqueda binaria por rango (requiere arreglo ordenado). */
    EXPONENCIAL_SEARCH = 5, /**< Busqueda exponencial (requiere arreglo ordenado). */
    INTERPOLATION_SEARCH = 6 /**< Busqueda por interpolacion (requiere arreglo ordenado). */
} SearchAlgorithm;

typedef enum {
    SEARCH_BY_ID = 1,             /**< Buscar por ID. */
    SEARCH_BY_PUNTAJE = 2,        /**< Buscar por puntaje. */
    SEARCH_BY_COMPETENCIAS = 3,   /**< Buscar por competencias. */
} SearchCriteria;

/**
 * @brief Busca secuencialmente un deportista por ID.
 *
 * @param deportistas Arreglo de deportistas.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int sequential_search(Deportista *deportistas, int length, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda binaria.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int binary_search(Deportista *deportistas, int length, int targetId);


int recursive_binary_search(Deportista *deportistas, int left, int right, int targetId);

int range_binary_search(Deportista *deportistas, int length, int targetId);

int exponencial_search(Deportista *deportistas, int length, int targetId);

int interpolation_search(Deportista *deportistas, int length, int targetId);

#endif
