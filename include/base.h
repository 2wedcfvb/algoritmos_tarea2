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

/**
 * @brief Muestra los deportistas dentro de un rango de puntaje.
 *
 * @param puntaje_init Puntaje inicial.
 * @param puntaje_end Puntaje final.
 */
void puntaje_range(int puntaje_init, int puntaje_end);

#endif
