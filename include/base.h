/**
 * @file base.h
 * @brief Constantes globales y prototipos del flujo principal del programa.
 */

#ifndef BASE_H
#define BASE_H

#include "config.h"


/**
 * @brief Ejecuta el flujo interactivo de ordenamiento.
 */
void run_experiment(void);

/**
 * @brief Ejecuta el benchmark de algoritmos de busqueda.
 */
void run_search_benchmark(void);

/**
 * @brief Ejecuta el benchmark de algoritmos de ordenamiento.
 */
void run_sort_benchmark(void);

/**
 * @brief Busca un deportista por su ID.
 *
 * @param targetId ID a buscar.
 */
void search(int targetId);

/**
 * @brief Muestra el ranking con los mejores deportistas por puntaje.
 *
 * @param rankingAmount Cantidad de deportistas a mostrar.
 */
void show_ranking(int rankingAmount);

#endif
