/**
 * @file benchmarks.c
 * @brief Implementacion de benchmarks de busqueda y ordenamiento.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "benchmarks.h"
#include "csv.h"
#include "errors.h"
#include "print_format.h"
#include "searching.h"
#include "sorting.h"

/**
 * @brief Convierte una mascara de bits en una lista de indices seleccionados.
 *
 * Recorre las opciones desde 0 hasta (optionCount - 1) y, por cada bit i en 1,
 * agrega i al arreglo `indices` hasta completar `maxIndices`.
 *
 * @param mask Mascara de bits con las opciones seleccionadas (bit i => opcion i).
 * @param optionCount Cantidad total de opciones disponibles.
 * @param indices Arreglo de salida donde se escriben los indices seleccionados.
 * @param maxIndices Capacidad maxima del arreglo `indices`.
 * @return int Cantidad de indices escritos en `indices` (0 si parametros invalidos).
 */
int build_selected_indices(BenchmarkMask mask, int optionCount, int *indices, int maxIndices){
    int count = 0;

    if(indices == NULL || optionCount <= 0 || maxIndices <= 0) {
        return 0;
    }
    for(int i = 0; i < optionCount; i++) {
        if(mask & (1u << i)) {
            if(count < maxIndices) {
                indices[count++] = i;
            }
        }
    }
    return count;
}

/**
 * @brief Genera una mascara de bits que selecciona todas las opciones.
 *
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con todas las opciones seleccionadas.
 */
BenchmarkMask benchmark_all_mask(int optionCount){
    BenchmarkMask mask = 0;
    if(optionCount <= 0) {
        return 0;
    }
    for(int i = 0; i < optionCount; i++) {
        mask |= (BenchmarkMask)(1u << i);
    }
    return mask;
}

/**
 * @brief Parsea una cadena de texto y genera una mascara de bits con las opciones seleccionadas.
 *
 * @param line Cadena de texto con las opciones seleccionadas.
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con las opciones seleccionadas.
 */
BenchmarkMask parse_benchmark_selection(const char *line, int optionCount){
    const char *p = line;
    BenchmarkMask mask = 0;

    if(line == NULL || optionCount <= 0) {
        return 0;
    }
    while(*p != '\0') {
        while(*p == ' ' || *p == '\t' || *p == ',' || *p == '\n' || *p == '\r') {
            p++;
        }
        if(*p == '\0') {
            break;
        }

        if(*p == 'a' || *p == 'A') {
            return benchmark_all_mask(optionCount);
        }
        char *endPtr = NULL;
        long value = strtol(p, &endPtr, 10);
        if(endPtr == p) {
            p++;
            continue;
        }
        if(value >= 1 && value <= optionCount) {
            mask |= (BenchmarkMask)(1u << (value - 1));
        }
        p = endPtr;
    }
    return mask;
}

/**
 * @brief Solicita al usuario que seleccione opciones para el benchmark.
 *
 * @param title Titulo de la seleccion.
 * @param labels Arreglo de etiquetas para las opciones.
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con las opciones seleccionadas.
 */
BenchmarkMask ask_benchmark_selection(const char *title, const char *const *labels, int optionCount){
    char line[256];
    BenchmarkMask mask;
    if(labels == NULL || optionCount <= 0) {
        return 0;
    }

    printf(BOLD_BLUE "\n=== %s ===\n" RESET, (title != NULL) ? title : "Seleccion de algoritmos");
    printf(DIM "Elige opciones (1-%d) separadas por espacios o comas.\n" RESET, optionCount);
    printf(DIM "Enter = todos | 'a' = todos\n\n" RESET);

    for(int i = 0; i < optionCount; i++) {
        printf(" %2d) %s\n", i + 1, labels[i]);
    }
    printf("\n> ");

    if(fgets(line, sizeof(line), stdin) == NULL) {
        return benchmark_all_mask(optionCount);
    }
    if(line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
        return benchmark_all_mask(optionCount);
    }
    mask = parse_benchmark_selection(line, optionCount);
    if(mask == 0) {
        mask = benchmark_all_mask(optionCount);
    }
    return mask;
}

/**
 * @brief Convierte el tiempo medido en ticks a segundos.
 *
 * @param start Tiempo de inicio en ticks.
 * @param end Tiempo de finalización en ticks.
 * @return double Tiempo en segundos.
 */
double clock_to_seconds(clock_t start, clock_t end){
    return (double)(end - start) / CLOCKS_PER_SEC;
}

/**
 * @brief Ejecuta una iteración del benchmark de búsqueda en el peor caso.
 *
 * @param algoIndex Índice del algoritmo a ejecutar.
 * @param baseArray Arreglo base de deportistas.
 * @param baseCount Cantidad de elementos en el arreglo base.
 * @param n Cantidad de elementos a buscar.
 * @param out Archivo de salida para escribir resultados.
 * @param intervalIndex Índice del intervalo actual.
 * @param intervalCount Cantidad total de intervalos.
 * @param repeatIndex Índice de la repetición actual.
 * @param repeatCount Cantidad total de repeticiones.
 * @return double Tiempo en segundos.
 */
double run_search_worst_case_once(int algoIndex, Deportista *baseArray, int baseCount, int n, FILE *out, int intervalIndex, int intervalCount, int repeatIndex, int repeatCount){
    Deportista *work;
    int target;
    clock_t start;
    clock_t end;

    if(algoIndex < 0 || algoIndex >= SEARCH_BENCH_ALGO_COUNT) {
        return 0.0;
    }

    progress_update_line("search", intervalIndex, intervalCount, n, repeatIndex, repeatCount, SEARCH_BENCH_ALGOS[algoIndex].stage);

    work = clone_deportistas_array(baseArray, n);
    if(work == NULL) {
        handle_benchmark_memory_error(baseArray, baseCount, out);
    }

    switch(algoIndex) {
        case 0: // secuencial
            target = prepare_search_worst_case(work, n, SEQUENTIAL_SEARCH);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            sequential_search(work, n, SEARCH_BY_ID, target);
            end = clock();
            break;

        case 1: // binaria
            target = prepare_search_worst_case(work, n, BINARY_SEARCH);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            binary_search(work, n, SEARCH_BY_ID, target);
            end = clock();
            break;

        case 2: // binaria rec
            target = prepare_search_worst_case(work, n, RECURSIVE_BINARY_SEARCH);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            recursive_binary_search(work, 0, n - 1, SEARCH_BY_ID, target);
            end = clock();
            break;

        case 3: // exponencial
            target = prepare_search_worst_case(work, n, EXPONENCIAL_SEARCH);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            exponencial_search(work, n, SEARCH_BY_ID, target);
            end = clock();
            break;

        case 4: // interpolacion
            target = prepare_search_worst_case(work, n, INTERPOLATION_SEARCH);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            interpolation_search(work, n, SEARCH_BY_ID, target);
            end = clock();
            break;

        case 5: // rango puntaje
            quick_sort_median(work, 0, n - 1, SORT_BY_PUNTAJE, ASCENDING);
            target = get_missing_target_score(work, n);
            if(target < 0) {
                free_deportistas_array(work, n);
                handle_benchmark_memory_error(baseArray, baseCount, out);
            }
            start = clock();
            (void)range_binary_search(work, n, SEARCH_BY_PUNTAJE, target);
            end = clock();
            break;

        default:
            start = 0;
            end = 0;
            break;
    }

    free_deportistas_array(work, n);
    return clock_to_seconds(start, end);
}

/**
 * @brief Ejecuta una iteración del benchmark de ordenamiento.
 * 
 * @param algoIndex Indice del algoritmo a ejecutar.
 * @param work Arreglo de deportistas a ordenar (debe tener `n` elementos).
 * @param n Cantidad de elementos en el arreglo.
 */
void run_sort_algorithm_once(int algoIndex, Deportista *work, int n){
    switch(algoIndex) {
        case 0:
            insertion_sort_deportistas(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 1:
            optimized_bubble_sort(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 2:
            optimized_selection_sort(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 3:
            cocktail_shaker_sort(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 4:
            merge_sort_deportistas(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 5:
            merge_sort_optimized_deportistas(work, n, SORT_BY_ID, ASCENDING);
            break;
        case 6:
            quick_sort_first(work, 0, n - 1, SORT_BY_ID, ASCENDING);
            break;
        case 7:
            quick_sort_last(work, 0, n - 1, SORT_BY_ID, ASCENDING);
            break;
        case 8:
            quick_sort_random(work, 0, n - 1, SORT_BY_ID, ASCENDING);
            break;
        case 9:
            quick_sort_median(work, 0, n - 1, SORT_BY_ID, ASCENDING);
            break;
        default:
            break;
    }
}

/**
 * @brief Prepara un arreglo para medir un caso experimental de ordenamiento.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param benchmarkCase Caso experimental requerido.
 */
void prepare_sort_case(Deportista *deportistas, int count, BenchmarkCase benchmarkCase)
{
    if(deportistas == NULL || count < 2) {
        return;
    }
    switch(benchmarkCase) {
        case BENCHMARK_CASE_BEST:
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
            break;
        case BENCHMARK_CASE_AVERAGE:
            shuffle_deportistas_array(deportistas, count);
            break;
        case BENCHMARK_CASE_WORST:
            quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, DESCENDING);
            break;
        default:
            break;
    }
}

/**
 * @brief Busca un ID inexistente para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int ID que no esta presente en el arreglo.
 */
int get_missing_target_id(Deportista *deportistas, int count)
{
    int maxId;
    if(deportistas == NULL || count <= 0 || deportistas[0] == NULL) {
        return -1;
    }
    maxId = deportistas[0]->id;
    for(int i = 1; i < count; i++) {
        if(deportistas[i] != NULL && deportistas[i]->id > maxId) {
            maxId = deportistas[i]->id;
        }
    }
    return maxId + 1;
}

/**
 * @brief Busca un puntaje inexistente (en int) para medir el peor caso de busqueda por rango.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int Puntaje (entero) que no esta presente en el arreglo.
 */
int get_missing_target_score(Deportista *deportistas, int count)
{
    int maxScore;
    if(deportistas == NULL || count <= 0 || deportistas[0] == NULL) {
        return -1;
    }
    maxScore = (int)deportistas[0]->puntaje;
    for(int i = 1; i < count; i++) {
        if(deportistas[i] != NULL) {
            int score = (int)deportistas[i]->puntaje;
            if(score > maxScore) {
                maxScore = score;
            }
        }
    }
    return maxScore + 1;
}

/**
 * @brief Prepara el arreglo para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param algorithm Algoritmo de busqueda a ejecutar.
 * @return int ID ausente para provocar el peor caso.
 */
int prepare_search_worst_case(Deportista *deportistas, int count, SearchAlgorithm algorithm)
{
    if(deportistas == NULL || count <= 0) {
        return -1;
    }

    if(algorithm != SEQUENTIAL_SEARCH) {
        quick_sort_median(deportistas, 0, count - 1, SORT_BY_ID, ASCENDING);
    }

    return get_missing_target_id(deportistas, count);
}

/**
 * @brief Duplica una cadena para los benchmarks.
 *
 * @param srcString String a duplicar.
 * @return char* Cadena duplicada o NULL si hubo un error.
 */
char *duplicate_string_bench(const char *srcString)
{
    size_t len;
    char *copy;

    if(srcString == NULL) {
        return NULL;
    }

    len = strlen(srcString);
    copy = malloc(len + 1);
    if(copy == NULL) {
        return NULL;
    }

    memcpy(copy, srcString, len + 1);
    return copy;
}

/**
 * @brief Maneja un error de memoria durante la ejecucion de benchmarks.
 *
 * @param baseArray Arreglo base cargado desde el CSV.
 * @param count Cantidad de deportistas del arreglo base.
 * @param out Archivo de salida del benchmark.
 */
void handle_benchmark_memory_error(Deportista *baseArray, int count, FILE *out)
{
    progress_clear_line();
    printf(SHOW_CURSOR);

    if(out != NULL) {
        fclose(out);
    }

    free_deportistas_array(baseArray, count);
    print_error(ERROR_MEMORY_ALLOCATION_FAILED, "durante el benchmark");
    exit(EXIT_FAILURE);
}

/**
 * @brief Clona un arreglo de deportistas para ejecutar benchmarks.
 *
 * @param srcArray Arreglo a clonar.
 * @param count Cantidad de elementos a clonar
 * @return Deportista* Arreglo de deportistas clonados o NULL si hubo un error.
 */
Deportista *clone_deportistas_array(Deportista *srcArray, int count)
{
    Deportista *deportistas;

    if(srcArray == NULL || count < 0) {
        return NULL;
    }

    deportistas = malloc(sizeof(Deportista) * count);
    if(deportistas == NULL) {
        return NULL;
    }

    for(int i = 0; i < count; i++) {
        char *nombre;
        char *equipo;

        if(srcArray[i] == NULL) {
            deportistas[i] = NULL;
            continue;
        }

        nombre = duplicate_string_bench(srcArray[i]->nombre);
        equipo = duplicate_string_bench(srcArray[i]->equipo);

        if(nombre == NULL || equipo == NULL) {
            free(nombre);
            free(equipo);

            for(int j = 0; j < i; j++) {
                if(deportistas[j] != NULL) {
                    delete_deportista(deportistas[j]);
                }
            }

            free(deportistas);
            return NULL;
        }

        deportistas[i] = create_deportista(
            srcArray[i]->id,
            nombre,
            equipo,
            srcArray[i]->puntaje,
            srcArray[i]->competencias
        );

        if(deportistas[i] == NULL) {
            free(nombre);
            free(equipo);

            for(int j = 0; j < i; j++) {
                if(deportistas[j] != NULL) {
                    delete_deportista(deportistas[j]);
                }
            }

            free(deportistas);
            return NULL;
        }
    }

    return deportistas;
}

/**
 * @brief Ejecuta el benchmark de busqueda y guarda sus resultados en CSV.
 */
void run_search_benchmark(){
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);
    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }
        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(SEARCH_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, SEARCH_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    // Solicitar al usuario que seleccione los algoritmos de busqueda a evaluar
    const char *searchLabels[SEARCH_BENCH_ALGO_COUNT];
    for(int i = 0; i < SEARCH_BENCH_ALGO_COUNT; i++) {
        searchLabels[i] = SEARCH_BENCH_ALGOS[i].label;
    }
    BenchmarkMask selectedSearch = ask_benchmark_selection("Selecciona busquedas a evaluar", searchLabels, SEARCH_BENCH_ALGO_COUNT);
    int selectedSearchIdx[SEARCH_BENCH_ALGO_COUNT];
    int selectedSearchCount = build_selected_indices(selectedSearch, SEARCH_BENCH_ALGO_COUNT, selectedSearchIdx, SEARCH_BENCH_ALGO_COUNT);

    // Escribir encabezado CSV (solo lo seleccionado)
    fprintf(out, "n");
    for(int k = 0; k < selectedSearchCount; k++) {
        int idx = selectedSearchIdx[k];
        fprintf(out, ",%s", SEARCH_BENCH_ALGOS[idx].csvCol);
    }
    fprintf(out, "\n");

    // Imprimir en consola el encabezado del benchmark
    printf(BOLD_BLUE "\n=== Search benchmark (peor caso) ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SEARCH_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);
    printf("n");
    for(int k = 0; k < selectedSearchCount; k++) {
        int idx = selectedSearchIdx[k];
        printf(" \t | \t %s(s)", searchLabels[idx]);
    }
    printf("\n");
    printf(ASCII_HR "\n");
    printf(HIDE_CURSOR);

    // Inicio del benchmark
    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double totals[SEARCH_BENCH_ALGO_COUNT] = {0.0};
        double averages[SEARCH_BENCH_ALGO_COUNT] = {0.0};

        for(int j = 0; j < EXPERIMENT_REPEATS; j++) {
            for(int k = 0; k < selectedSearchCount; k++) {
                int algoIdx = selectedSearchIdx[k];
                totals[algoIdx] += run_search_worst_case_once(algoIdx, baseArray, count, n, out, i + 1, intervals, j + 1, EXPERIMENT_REPEATS);
            }
        }

        for(int k = 0; k < selectedSearchCount; k++) {
            int algoIdx = selectedSearchIdx[k];
            averages[algoIdx] = totals[algoIdx] / EXPERIMENT_REPEATS;
        }

        fprintf(out, "%d", n);
        for(int k = 0; k < selectedSearchCount; k++) {
            int idx = selectedSearchIdx[k];
            fprintf(out, ",%.10f", averages[idx]);
        }
        fprintf(out, "\n");

        progress_clear_line();
        printf("%d", n);
        for(int k = 0; k < selectedSearchCount; k++) {
            int idx = selectedSearchIdx[k];
            printf(" \t | \t %.10f", averages[idx]);
        }
        printf("\n");
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);
    printf(BG_GREEN "\nBenchmark guardado correctamente en %s\n" RESET, SEARCH_BENCHMARK_ROUTE);
}

/**
 * @brief Ejecuta el benchmark de ordenamiento y guarda sus resultados en CSV.
 */
void run_sort_benchmark(){
    int count = 0;
    Deportista *baseArray = load_deportistas_array(&count);

    if(baseArray == NULL || count <= 0) {
        if(baseArray != NULL) {
            free_deportistas_array(baseArray, count);
        }

        print_error(ERROR_BENCHMARK_DATA_LOAD_FAILED, NULL);
        return;
    }

    int intervals = (count < INTERVAL_SIZE) ? count : INTERVAL_SIZE;
    int stepSize = count / intervals;

    if(stepSize <= 0) {
        stepSize = 1;
    }

    FILE *out = fopen(SORT_BENCHMARK_ROUTE, "w");

    if(out == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, SORT_BENCHMARK_ROUTE);
        free_deportistas_array(baseArray, count);
        return;
    }

    printf(BOLD_BLUE "\n=== Sort benchmark ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SORT_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    // Solicitar al usuario que seleccione los algoritmos de ordenamiento a evaluar
    const char *sortLabels[SORT_BENCH_ALGO_COUNT];
    for(int i = 0; i < SORT_BENCH_ALGO_COUNT; i++) {
        sortLabels[i] = SORT_BENCH_ALGOS[i].label;
    }
    BenchmarkMask selectedSort = ask_benchmark_selection("Selecciona ordenamientos a evaluar", sortLabels, SORT_BENCH_ALGO_COUNT);
    int selectedSortIdx[SORT_BENCH_ALGO_COUNT];
    int selectedSortCount = build_selected_indices(selectedSort, SORT_BENCH_ALGO_COUNT, selectedSortIdx, SORT_BENCH_ALGO_COUNT);

    // Escribir encabezado CSV (solo lo seleccionado)
    fprintf(out, "n");
    for(int k = 0; k < selectedSortCount; k++) {
        int idx = selectedSortIdx[k];
        fprintf(out, ",mejor_%s_s", SORT_BENCH_ALGOS[idx].csvName);
    }
    for(int k = 0; k < selectedSortCount; k++) {
        int idx = selectedSortIdx[k];
        fprintf(out, ",promedio_%s_s", SORT_BENCH_ALGOS[idx].csvName);
    }
    for(int k = 0; k < selectedSortCount; k++) {
        int idx = selectedSortIdx[k];
        fprintf(out, ",peor_%s_s", SORT_BENCH_ALGOS[idx].csvName);
    }
    fprintf(out, "\n");

    printf("n \t | caso");
    for(int k = 0; k < selectedSortCount; k++) {
        int idx = selectedSortIdx[k];
        printf(" \t | %s(s)", sortLabels[idx]);
    }
    printf("\n");
    printf(ASCII_HR_WIDE "\n");

    printf(HIDE_CURSOR);

    // Inicio del benchmark
    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double caseTotals[3][10] = {{0.0}};

        for(int caseIndex = BENCHMARK_CASE_BEST; caseIndex <= BENCHMARK_CASE_WORST; caseIndex++) {
            BenchmarkCase benchmarkCase = (BenchmarkCase)caseIndex;

            for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
                const char *caseName = get_case_name(benchmarkCase);
                char stage[96];

                for(int k = 0; k < selectedSortCount; k++) {
                    int algoIdx = selectedSortIdx[k];
                    Deportista *work;
                    clock_t start;
                    clock_t end;

                    snprintf(stage, sizeof(stage), "%s - %s", caseName, SORT_BENCH_ALGOS[algoIdx].stage);
                    progress_update_line("sort", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, stage);

                    work = clone_deportistas_array(baseArray, n);
                    if(work == NULL) {
                        handle_benchmark_memory_error(baseArray, count, out);
                    }
                    prepare_sort_case(work, n, benchmarkCase);

                    start = clock();
                    run_sort_algorithm_once(algoIdx, work, n);
                    end = clock();

                    caseTotals[caseIndex][algoIdx] += clock_to_seconds(start, end);
                    free_deportistas_array(work, n);
                }
            }

            for(int algorithmIndex = 0; algorithmIndex < 10; algorithmIndex++) {
                if(selectedSort & (1u << algorithmIndex)) {
                    caseTotals[caseIndex][algorithmIndex] /= EXPERIMENT_REPEATS;
                }
            }
        }

        fprintf(out, "%d", n);
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            fprintf(out, ",%.10f", caseTotals[BENCHMARK_CASE_BEST][idx]);
        }
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            fprintf(out, ",%.10f", caseTotals[BENCHMARK_CASE_AVERAGE][idx]);
        }
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            fprintf(out, ",%.10f", caseTotals[BENCHMARK_CASE_WORST][idx]);
        }
        fprintf(out, "\n");

        progress_clear_line();

        printf("%d \t | %s", n, get_case_name(BENCHMARK_CASE_BEST));
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            printf(" \t | %.10f", caseTotals[BENCHMARK_CASE_BEST][idx]);
        }
        printf("\n");

        printf("%d \t | %s", n, get_case_name(BENCHMARK_CASE_AVERAGE));
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            printf(" \t | %.10f", caseTotals[BENCHMARK_CASE_AVERAGE][idx]);
        }
        printf("\n");

        printf("%d \t | %s", n, get_case_name(BENCHMARK_CASE_WORST));
        for(int k = 0; k < selectedSortCount; k++) {
            int idx = selectedSortIdx[k];
            printf(" \t | %.10f", caseTotals[BENCHMARK_CASE_WORST][idx]);
        }
        printf("\n");
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);
    printf(BG_GREEN "\nBenchmark de ordenamiento guardado con exito en %s\n" RESET, SORT_BENCHMARK_ROUTE);
}
