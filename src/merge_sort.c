#include "sorting.h"

#include <stdlib.h>

static int should_precede(Deportista left, Deportista right, SortCriteria criteria, SortOrder order){
    int cmp = compare_by_criteria(left, right, criteria);
    if(order == ASCENDING){
        return cmp < 0;
    }
    return cmp > 0;    
}

//fusiona dos mitades contiguas y ordenadas del arreglo 
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

    //copia las mitades a los buffers temporales
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++) 
        R[j] = arr[mid + 1 + j];

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

//Recursion interna
static void merge_sort_recursive(Deportista *arr, int left, int right, SortCriteria criteria, SortOrder order)
{
    if(left >= right) {
        return; //un solo elemento, ya esta ordenado 
    }

    int mid = left + (right - left) / 2;

    merge_sort_recursive(arr, left, mid, criteria, order);
    merge_sort_recursive(arr, mid + 1, right, criteria, order);
    merge(arr, left, mid, right, criteria, order);
}

void merge_sort_deportistas(Deportista *arr, int count, SortCriteria criteria, SortOrder order)
{
    if(arr == NULL || count < 2) {
        return;
    }

    merge_sort_recursive(arr, 0, count - 1, criteria, order);
}