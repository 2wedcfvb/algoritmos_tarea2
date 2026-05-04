/**
 * @file sorting.h
 * @brief Ordenamiento para deportistas.
 */

#ifndef SORTING_H
#define SORTING_H

#include "deportista.h"
#include <string.h>

/**
 * @brief Criterios disponibles para ordenar deportistas.
 */
typedef enum {
    SORT_BY_ID = 1,             /**< Ordenar/Comparar por ID. */
    SORT_BY_PUNTAJE = 2,        /**< Ordenar/Comparar por puntaje. */
    SORT_BY_COMPETENCIAS = 3,   /**< Ordenar/Comparar por competencias. */
    SORT_BY_NOMBRE = 4,         /**< Ordenar/Comparar por nombre (lexicografico). */
    SORT_BY_EQUIPO = 5          /**< Ordenar/Comparar por equipo (lexicografico). */
} SortCriteria;

/**
 * @brief Sentidos de ordenamiento disponibles.
 */
typedef enum {
    DESCENDING = 1, /**< Mayor a menor. */
    ASCENDING = 0   /**< Menor a mayor. */
} SortOrder;

/**
 * @brief Algoritmos de ordenamiento disponibles.
 */
typedef enum {
    INSERTION_SORT = 1,        /**< Insertion sort. */
    BUBBLE_SORT = 2,           /**< Bubble sort optimizado. */
    SELECTION_SORT = 3,        /**< Selection sort optimizado. */
    COCKTAIL_SHAKER_SORT = 4,   /**< Cocktail shaker sort. */
    MERGE_SORT = 5,             /**< Merge sort. */
    OPTIMIZED_MERGE_SORT = 6,   /**< Merge sort optimizado. */
    QUICK_SORT = 7             /**< Quick sort. */
} SortAlgorithm;

/**
 * @brief Compara dos deportistas segun un criterio.
 * 
 * @param criteria 
 * @return const char* 
 */
const char *get_sort_criteria_name(SortCriteria criteria);

/**
 * @brief Obtiene el nombre de un algoritmo de ordenamiento.
 * 
 * @param algorithm 
 * @return const char* 
 */
const char *get_sort_algorithm_name(SortAlgorithm algorithm);

/**
 * @brief Obtiene el nombre de un sentido de ordenamiento.
 * 
 * @param order 
 * @return const char* 
 */
const char *get_sort_order_name(SortOrder order);

/**
 * @brief Compara dos deportistas segun un criterio.
 *
 * @param left Deportista izquierdo.
 * @param right Deportista derecho.
 * @param criteria Criterio de comparacion.
 * @return int Valor negativo si left < right, positivo si left > right y 0 si son iguales.
 */
int compare_by_criteria(Deportista left, Deportista right, SortCriteria criteria);

/**
 * @brief Ordena un arreglo con cocktail shaker sort.
 *
 * @param deportistas Arreglo a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio de comparacion.
 * @param order Sentido del ordenamiento.
 */
void cocktail_shaker_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order);

/**
 * @brief Ordena un arreglo con insertion sort.
 *
 * @param deportistas Arreglo a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio de comparacion.
 * @param order Sentido del ordenamiento.
 */
void insertion_sort_deportistas(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order);

/**
 * @brief Ordena un arreglo con bubble sort optimizado.
 *
 * @param deportistas Arreglo a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio de comparacion.
 * @param order Sentido del ordenamiento.
 */
void optimized_bubble_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order);

/**
 * @brief Ordena un arreglo con selection sort.
 *
 * @param deportistas Arreglo a ordenar.
 * @param length Largo del arreglo.
 * @param criteria Criterio de comparacion.
 * @param order Sentido del ordenamiento.
 */
void optimized_selection_sort(Deportista *deportistas, int length, SortCriteria criteria, SortOrder order);
void quick_sort_last(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order);
void quick_sort_first(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order);
void quick_sort_random(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order);
void quick_sort_median(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order);
#endif
