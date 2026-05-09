#include "sorting.h"

static int should_precede(Deportista left, Deportista right, SortCriteria criteria, SortOrder order){
    int cmp = compare_by_criteria(left, right, criteria);
    if(order == ASCENDING){
        return cmp < 0;
    }
    return cmp > 0;    
}

static void merge(Deportista *arr, int left, int mid, int right, SortCriteria criteria, SortOrder order){
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int i, j, k;

    Deportista *L = malloc(n1 * sizeof(Deportista));
    Deportista *R = malloc(n2 * sizeof(Deportista));

    if(L == NULL || R == NULL){ //si no hay memoria suficiente para el buffer auxiliar aborta
        free(L);
        free(R);
        return;
    }

    //Copia las mitades a los buffers temporales
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++) 
        R[j] = arr[mid + 1 + j];

    //intercala de vuelta sobre arr
    i = 0;
    j = 0;
    k = left;

    while(i < n1 && j < n2){
        if(should_precede(L[i], R[j], criteria, order)){
            arr[k++] = L[i++];    
        } else {
            arr[k++] = R[j++];
        }
    }

    //copia los elementos restantes de cada mitad
    while(i < n1)
        arr[k++] = L[i++];
    while(j < n2)
        arr[k++] = R[j++];
        
    free(L);
    free(R);    
}

static void insertion_sort_range(Deportista *arr, int left, int right, SortCriteria criteria, SortOrder order)
{
    int i, j;

    for(i = left + 1; i <= right; i++) {
        Deportista key = arr[i];
        j = i - 1;

        //desplaza hacia la derecha mientras key deba preceder a arr[j]
        while(j >= left && should_precede(key, arr[j], criteria, order)) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

//recursion interna de la versión optimizada
static void merge_sort_optimized_recursive(Deportista *arr, int left, int right, SortCriteria criteria, SortOrder order)
{
    if(left >= right) {
        return;
    }

    //sub arreglo insertion Sort
    if(right - left + 1 <= MERGE_SORT_THRESHOLD) {
        insertion_sort_range(arr, left, right, criteria, order);
        return;
    }

    int mid = left + (right - left) / 2;

    merge_sort_optimized_recursive(arr, left, mid, criteria, order);
    merge_sort_optimized_recursive(arr, mid + 1, right, criteria, order);
    merge(arr, left, mid, right, criteria, order);
}

void merge_sort_optimized_deportistas(Deportista *arr, int count, SortCriteria criteria, SortOrder order)
{
    if(arr == NULL || count < 2) {
        return;
    }

    merge_sort_optimized_recursive(arr, 0, count - 1, criteria, order);
}