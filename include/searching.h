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

/**
 * @brief Criterios de busqueda disponibles. Se puede escoger solo si se utiliza el algoritmo de range_binary_search.
 */
typedef enum {
    SEARCH_BY_ID = 1,             /**< Buscar por ID. */
    SEARCH_BY_PUNTAJE = 2,        /**< Buscar por puntaje. */
    SEARCH_BY_COMPETENCIAS = 3,   /**< Buscar por competencias. */
} SearchCriteria;

/**
 * @brief Definición de un rango
 */
typedef struct {
    int start; /**< Indice inicial del rango. */
    int end;   /**< Indice final del rango. */
} Range;

/**
 * @brief Busca secuencialmente un deportista
 *
 * @param deportistas Arreglo de deportistas.
 * @param length Cantidad de elementos del arreglo.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int sequential_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda binaria.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int binary_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda binaria recursiva.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param left Indice izquierdo del rango de busqueda.
 * @param right Indice derecho del rango de busqueda.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int recursive_binary_search(Deportista *deportistas, int left, int right, SearchCriteria criteria, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda binaria devolviendo el rango de indices donde se encuentra el ID buscado.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetId ID que se desea encontrar.
 * @return Range Estructura que contiene los indices del rango donde se encuentra el ID buscado.
 */
Range range_binary_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetId);

int range_binary_search_first_at_least(Deportista *deportistas, int length, int targetScore);

int range_binary_search_last_at_most(Deportista *deportistas, int length, int targetScore);

/**
 * @brief Busca un deportista por ID usando busqueda exponencial.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetId ID que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int exponencial_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetId);

/**
 * @brief Busca un deportista por ID usando busqueda por interpolacion.
 * @param deportistas Arreglo de deportistas ordenado por ID ascendente.
 * @param length Cantidad de elementos del arreglo.
 * @param criteria Criterio de busqueda (ID, puntaje o competencias).
 * @param targetValue Valor que se desea encontrar.
 * @return int Indice del elemento encontrado o -1 si no existe.
 */
int interpolation_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetValue)

;
#endif
