/**
 * @file range_binary_search.c
 * @author Iván Mansilla
 * @brief Implementación de la función de búsqueda binaria que devuelve el rango de índices donde se encuentra lo buscado.
 */
#include "searching.h"

/**
 * @brief Busqueda binaria que devuelve el rango de índices donde se encuentra el puntaje buscado.
 * 
 * @param deportistas 
 * @param length 
 * @param criteria 
 * @param targetId 
 * @return Range 
 */
Range range_binary_search(Deportista *deportistas, int length, SearchCriteria criteria, int targetId){

    Range result = {-1, -1};

    if(deportistas == NULL || length <= 0) {
        return result;
    }

    if(criteria != SEARCH_BY_ID && criteria != SEARCH_BY_PUNTAJE && criteria != SEARCH_BY_COMPETENCIAS) {
        return result;
    }

    int left = 0;
    int right = length - 1;

    while(left <= right) {
        int mid = left + (right - left) / 2;

        if(deportistas[mid] == NULL) {
            return result;
        }

        int midValue;
        if(criteria == SEARCH_BY_ID) {
            midValue = deportistas[mid]->id;
        } else if(criteria == SEARCH_BY_PUNTAJE) {
            midValue = (int)deportistas[mid]->puntaje;
        } else {
            midValue = deportistas[mid]->competencias;
        }

        if(midValue == targetId) {
            result.start = mid;
            result.end = mid;

            while(result.start > left) {
                int prevValue;
                if(deportistas[result.start - 1] == NULL) {
                    break;
                }
                if(criteria == SEARCH_BY_ID) {
                    prevValue = deportistas[result.start - 1]->id;
                } else if(criteria == SEARCH_BY_PUNTAJE) {
                    prevValue = (int)deportistas[result.start - 1]->puntaje;
                } else {
                    prevValue = deportistas[result.start - 1]->competencias;
                }
                if(prevValue != targetId) {
                    break;
                }
                result.start--;
            }

            while(result.end < right) {
                int nextValue;
                if(deportistas[result.end + 1] == NULL) {
                    break;
                }
                if(criteria == SEARCH_BY_ID) {
                    nextValue = deportistas[result.end + 1]->id;
                } else if(criteria == SEARCH_BY_PUNTAJE) {
                    nextValue = (int)deportistas[result.end + 1]->puntaje;
                } else {
                    nextValue = deportistas[result.end + 1]->competencias;
                }
                if(nextValue != targetId) {
                    break;
                }
                result.end++;
            }

            return result;
        }

        if(midValue < targetId) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

int range_binary_search_first_at_least(Deportista *deportistas, int length, int targetScore)
{
    int left = 0;
    int right = length - 1;
    int result = -1;

    if(deportistas == NULL || length <= 0) {
        return -1;
    }

    while(left <= right) {
        int mid = left + (right - left) / 2;

        if(deportistas[mid] == NULL) {
            return -1;
        }

        if((int)deportistas[mid]->puntaje >= targetScore) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return result;
}

int range_binary_search_last_at_most(Deportista *deportistas, int length, int targetScore)
{
    int left = 0;
    int right = length - 1;
    int result = -1;

    if(deportistas == NULL || length <= 0) {
        return -1;
    }

    while(left <= right) {
        int mid = left + (right - left) / 2;

        if(deportistas[mid] == NULL) {
            return -1;
        }

        if((int)deportistas[mid]->puntaje <= targetScore) {
            result = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}