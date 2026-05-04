/**
 * @file quicksort.c
 * @brief Implementacion de quick sort con particion de Lomuto
 *        y cuatro variantes de seleccion de pivote.
 */

#include "sorting.h"
#include <stdlib.h>

/* Funcion de particion de Lomuto.
 * Toma el ultimo elemento como pivote, lo coloca en su posicion correcta
 * y deja los menores a la izquierda y los mayores a la derecha. */
static int partition(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    Deportista pivot = deportistas[high];
    int i = low - 1;

    for(int j = low; j <= high - 1; j++) {
        int cmp = compare_by_criteria(deportistas[j], pivot, criteria);

        /* En ascendente queremos los menores al pivote a la izquierda,
         * en descendente queremos los mayores a la izquierda */
        int goes_left;
        if(order == ASCENDING) {
            goes_left = (cmp < 0);
        } 
        else {
            goes_left = (cmp > 0);
        }

        if(goes_left) {
            i++;
            swap_deportistas(&deportistas[i], &deportistas[j]);
        }
    }

    /* Colocar el pivote en su posicion correcta */
    swap_deportistas(&deportistas[i + 1], &deportistas[high]);
    return i + 1;
}

/* Mueve el pivote elegido al final antes de particionar */

static void pivot_last(Deportista *deportistas, int low, int high)
{
    /* El ultimo elemento ya esta al final, no hay nada que hacer */
    (void)deportistas;
    (void)low;
    (void)high;
}

static void pivot_first(Deportista *deportistas, int low, int high)
{
    /* Mover el primer elemento al final */
    swap_deportistas(&deportistas[low], &deportistas[high]);
}

static void pivot_random(Deportista *deportistas, int low, int high)
{
    /* Elegir un indice aleatorio y moverlo al final */
    int r = low + rand() % (high - low + 1);
    swap_deportistas(&deportistas[r], &deportistas[high]);
}

static void pivot_median(Deportista *deportistas, int low, int high, SortCriteria criteria)
{
    int mid = low + (high - low) / 2;

    /* Ordenar los tres candidatos entre si para encontrar la mediana */
    if(compare_by_criteria(deportistas[low], deportistas[mid], criteria) > 0) {
        swap_deportistas(&deportistas[low], &deportistas[mid]);
    }
    if(compare_by_criteria(deportistas[low], deportistas[high], criteria) > 0) {
        swap_deportistas(&deportistas[low], &deportistas[high]);
    }
    if(compare_by_criteria(deportistas[mid], deportistas[high], criteria) > 0) {
        swap_deportistas(&deportistas[mid], &deportistas[high]);
    }

    /* La mediana esta en mid, moverla al final */
    swap_deportistas(&deportistas[mid], &deportistas[high]);
}

/* Funciones publicas */

void quick_sort_last(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    if(low < high) {
        pivot_last(deportistas, low, high);
        int pi = partition(deportistas, low, high, criteria, order);
        quick_sort_last(deportistas, low, pi - 1, criteria, order);
        quick_sort_last(deportistas, pi + 1, high, criteria, order);
    }
}

void quick_sort_first(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    if(low < high) {
        pivot_first(deportistas, low, high);
        int pi = partition(deportistas, low, high, criteria, order);
        quick_sort_first(deportistas, low, pi - 1, criteria, order);
        quick_sort_first(deportistas, pi + 1, high, criteria, order);
    }
}

void quick_sort_random(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    if(low < high) {
        pivot_random(deportistas, low, high);
        int pi = partition(deportistas, low, high, criteria, order);
        quick_sort_random(deportistas, low, pi - 1, criteria, order);
        quick_sort_random(deportistas, pi + 1, high, criteria, order);
    }
}

void quick_sort_median(Deportista *deportistas, int low, int high, SortCriteria criteria, SortOrder order)
{
    if(low < high) {
        pivot_median(deportistas, low, high, criteria);
        int pi = partition(deportistas, low, high, criteria, order);
        quick_sort_median(deportistas, low, pi - 1, criteria, order);
        quick_sort_median(deportistas, pi + 1, high, criteria, order);
    }
}