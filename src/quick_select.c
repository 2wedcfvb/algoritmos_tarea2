#include "sorting.h"

static int partition(Deportista *arr, int left, int right, SortCriteria criteria, SortOrder order) {
    Deportista pivot = arr[right];
    int i = left;

    for (int j = left; j < right; j++) {

        int cmp = compare_by_criteria(arr[j], pivot, criteria);
        int shouldSwap = (order == ASCENDING) ? (cmp <= 0) : (cmp >= 0);

        if (shouldSwap) {
            swap_deportistas(&arr[i], &arr[j]);
            i++;
        }
    }
    swap_deportistas(&arr[i], &arr[right]);
    return i;
}

static Deportista quick_select_recursive(Deportista *arr, int left, int right, int k, SortCriteria criteria, SortOrder order) {
    if (left == right) {
        return arr[left];
    }

    int pivotIndex = partition(arr, left, right, criteria, order);

    if (k == pivotIndex) {
        return arr[k];
    } else if (k < pivotIndex) {
        return quick_select_recursive(arr, left, pivotIndex - 1, k, criteria, order);
    } else {
        return quick_select_recursive(arr, pivotIndex + 1, right, k, criteria, order);
    }
}

//funcion principal para obtener el k-esimo deportista

Deportista get_kth_element(Deportista *arr, int length, int k, SortCriteria criteria, SortOrder order) {
    if (arr == NULL || k < 0 || k >= length) {
        return NULL;
    }
    return quick_select_recursive(arr, 0, length - 1, k, criteria, order);
}