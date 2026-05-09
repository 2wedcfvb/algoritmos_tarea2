/**
 * @file benchmarks.c
 * @brief Implementacion de benchmarks de busqueda y ordenamiento.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "base.h"
#include "csv.h"
#include "errors.h"
#include "print_format.h"
#include "searching.h"
#include "sorting.h"

/**
 * @brief Prepara un arreglo para medir un caso experimental de ordenamiento.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param benchmarkCase Caso experimental requerido.
 */
static void prepare_sort_case(Deportista *deportistas, int count, BenchmarkCase benchmarkCase)
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
static int get_missing_target_id(Deportista *deportistas, int count)
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
static int get_missing_target_score(Deportista *deportistas, int count)
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
static int prepare_search_worst_case(Deportista *deportistas, int count, SearchAlgorithm algorithm)
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
static char *duplicate_string_bench(const char *srcString)
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
static void handle_benchmark_memory_error(Deportista *baseArray, int count, FILE *out)
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
static Deportista *clone_deportistas_array(Deportista *srcArray, int count)
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
void run_search_benchmark()
{
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

    fprintf(out, "n,peor_secuencial_s,peor_binaria_s,peor_binaria_rec_s,peor_exponencial_s,peor_interpolacion_s,peor_rango_puntaje_s\n");

    printf(BOLD_BLUE "\n=== Search benchmark (peor caso) ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SEARCH_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    printf("n \t | \t peor secuencial(s) \t | \t peor binaria(s) \t | \t peor binaria rec(s) \t | \t peor exponencial(s) \t | \t peor interpolacion(s) \t | \t peor rango puntaje(s)\n");
    printf(ASCII_HR "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double totalWorstSeq = 0.0;
        double totalWorstBin = 0.0;
        double totalWorstRec = 0.0;
        double totalWorstExp = 0.0;
        double totalWorstInter = 0.0;
        double totalWorstRange = 0.0;

        for(int j = 0; j < EXPERIMENT_REPEATS; j++) {
            int targetId;
            clock_t start;
            clock_t end;
            Deportista *workSequential;
            Deportista *workBinary;
            Deportista *workRecursive;
            Deportista *workExponential;
            Deportista *workInterpolation;
            Deportista *workRange;

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor secuencial");

            workSequential = clone_deportistas_array(baseArray, n);
            if(workSequential == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workSequential, n, SEQUENTIAL_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workSequential, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            sequential_search(workSequential, n, SEARCH_BY_ID, targetId);
            end = clock();
            totalWorstSeq += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workSequential, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor binaria");

            workBinary = clone_deportistas_array(baseArray, n);
            if(workBinary == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workBinary, n, BINARY_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workBinary, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            binary_search(workBinary, n, SEARCH_BY_ID, targetId);
            end = clock();
            totalWorstBin += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workBinary, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor binaria rec");

            workRecursive = clone_deportistas_array(baseArray, n);
            if(workRecursive == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workRecursive, n, RECURSIVE_BINARY_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workRecursive, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            recursive_binary_search(workRecursive, 0, n - 1, SEARCH_BY_ID, targetId);
            end = clock();
            totalWorstRec += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workRecursive, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor exponencial");

            workExponential = clone_deportistas_array(baseArray, n);
            if(workExponential == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workExponential, n, EXPONENCIAL_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workExponential, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            exponencial_search(workExponential, n, SEARCH_BY_ID, targetId);
            end = clock();
            totalWorstExp += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workExponential, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor interpolacion");

            workInterpolation = clone_deportistas_array(baseArray, n);
            if(workInterpolation == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            targetId = prepare_search_worst_case(workInterpolation, n, INTERPOLATION_SEARCH);
            if(targetId < 0) {
                free_deportistas_array(workInterpolation, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            interpolation_search(workInterpolation, n, SEARCH_BY_ID, targetId);
            end = clock();
            totalWorstInter += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workInterpolation, n);

            progress_update_line("search", i + 1, intervals, n, j + 1, EXPERIMENT_REPEATS, "peor rango puntaje");

            workRange = clone_deportistas_array(baseArray, n);
            if(workRange == NULL) {
                handle_benchmark_memory_error(baseArray, count, out);
            }

            quick_sort_median(workRange, 0, n - 1, SORT_BY_PUNTAJE, ASCENDING);
            targetId = get_missing_target_score(workRange, n);
            if(targetId < 0) {
                free_deportistas_array(workRange, n);
                handle_benchmark_memory_error(baseArray, count, out);
            }

            start = clock();
            (void)range_binary_search(workRange, n, SEARCH_BY_PUNTAJE, targetId);
            end = clock();
            totalWorstRange += (double)(end - start) / CLOCKS_PER_SEC;
            free_deportistas_array(workRange, n);
        }

        double avgWorstSeq = totalWorstSeq / EXPERIMENT_REPEATS;
        double avgWorstBin = totalWorstBin / EXPERIMENT_REPEATS;
        double avgWorstRec = totalWorstRec / EXPERIMENT_REPEATS;
        double avgWorstExp = totalWorstExp / EXPERIMENT_REPEATS;
        double avgWorstInter = totalWorstInter / EXPERIMENT_REPEATS;
        double avgWorstRange = totalWorstRange / EXPERIMENT_REPEATS;

        fprintf(out, "%d,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n", n, avgWorstSeq, avgWorstBin, avgWorstRec, avgWorstExp, avgWorstInter, avgWorstRange);
        progress_clear_line();
        printf("%d \t | \t %.10f \t | \t %.10f \t | \t %.10f \t | \t %.10f \t | \t %.10f \t | \t %.10f\n", n, avgWorstSeq, avgWorstBin, avgWorstRec, avgWorstExp, avgWorstInter, avgWorstRange);
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
void run_sort_benchmark(void)
{
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

    fprintf(
        out,
        "n,"
        "mejor_insertion_s,mejor_bubble_s,mejor_selection_s,mejor_cocktail_s,mejor_merge_s,mejor_merge_opt_s,mejor_quick_first_s,mejor_quick_last_s,mejor_quick_random_s,mejor_quick_median_s,"
        "promedio_insertion_s,promedio_bubble_s,promedio_selection_s,promedio_cocktail_s,promedio_merge_s,promedio_merge_opt_s,promedio_quick_first_s,promedio_quick_last_s,promedio_quick_random_s,promedio_quick_median_s,"
        "peor_insertion_s,peor_bubble_s,peor_selection_s,peor_cocktail_s,peor_merge_s,peor_merge_opt_s,peor_quick_first_s,peor_quick_last_s,peor_quick_random_s,peor_quick_median_s\n"
    );

    printf(BOLD_BLUE "\n=== Sort benchmark ===\n" RESET);
    printf(DIM "Archivo: %s | intervalos: %d | repeticiones: %d\n\n" RESET, SORT_BENCHMARK_ROUTE, intervals, EXPERIMENT_REPEATS);

    printf("n \t | caso \t | insertion(s) \t | bubble(s) \t | selection(s) \t | cocktail(s) \t | merge(s) \t | merge_opt(s) \t | q_first(s) \t | q_last(s) \t | q_rand(s) \t | q_median(s)\n");
    printf(ASCII_HR_WIDE "\n");

    printf(HIDE_CURSOR);

    for(int i = 0; i < intervals; i++) {
        int n = (i == intervals - 1) ? count : (stepSize * (i + 1));
        double caseTotals[3][10] = {{0.0}};

        for(int caseIndex = BENCHMARK_CASE_BEST; caseIndex <= BENCHMARK_CASE_WORST; caseIndex++) {
            BenchmarkCase benchmarkCase = (BenchmarkCase)caseIndex;

            for(int r = 0; r < EXPERIMENT_REPEATS; r++) {
                Deportista *work;
                clock_t start;
                clock_t end;

                progress_update_line("sort", i + 1, intervals, n, r + 1, EXPERIMENT_REPEATS, get_case_name(benchmarkCase));
                work = clone_deportistas_array(baseArray, n);

                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                insertion_sort_deportistas(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][0] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                optimized_bubble_sort(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][1] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                optimized_selection_sort(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][2] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }

                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                cocktail_shaker_sort(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][3] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                merge_sort_deportistas(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][4] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                merge_sort_optimized_deportistas(work, n, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][5] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_first(work, 0, n - 1, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][6] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_last(work, 0, n - 1, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][7] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_random(work, 0, n - 1, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][8] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);

                work = clone_deportistas_array(baseArray, n);
                if(work == NULL) {
                    handle_benchmark_memory_error(baseArray, count, out);
                }
                prepare_sort_case(work, n, benchmarkCase);
                start = clock();
                quick_sort_median(work, 0, n - 1, SORT_BY_ID, ASCENDING);
                end = clock();
                caseTotals[caseIndex][9] += (double)(end - start) / CLOCKS_PER_SEC;
                free_deportistas_array(work, n);
            }

            for(int algorithmIndex = 0; algorithmIndex < 10; algorithmIndex++) {
                caseTotals[caseIndex][algorithmIndex] /= EXPERIMENT_REPEATS;
            }
        }

        fprintf(
            out,
            "%d,"
            "%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,"
            "%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,"
            "%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\n",
            n,
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3],
            caseTotals[BENCHMARK_CASE_BEST][4],
            caseTotals[BENCHMARK_CASE_BEST][5],
            caseTotals[BENCHMARK_CASE_BEST][6],
            caseTotals[BENCHMARK_CASE_BEST][7],
            caseTotals[BENCHMARK_CASE_BEST][8],
            caseTotals[BENCHMARK_CASE_BEST][9],
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3],
            caseTotals[BENCHMARK_CASE_AVERAGE][4],
            caseTotals[BENCHMARK_CASE_AVERAGE][5],
            caseTotals[BENCHMARK_CASE_AVERAGE][6],
            caseTotals[BENCHMARK_CASE_AVERAGE][7],
            caseTotals[BENCHMARK_CASE_AVERAGE][8],
            caseTotals[BENCHMARK_CASE_AVERAGE][9],
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3],
            caseTotals[BENCHMARK_CASE_WORST][4],
            caseTotals[BENCHMARK_CASE_WORST][5],
            caseTotals[BENCHMARK_CASE_WORST][6],
            caseTotals[BENCHMARK_CASE_WORST][7],
            caseTotals[BENCHMARK_CASE_WORST][8],
            caseTotals[BENCHMARK_CASE_WORST][9]
        );

        progress_clear_line();
        printf(
            "%d \t | %s \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_BEST),
            caseTotals[BENCHMARK_CASE_BEST][0],
            caseTotals[BENCHMARK_CASE_BEST][1],
            caseTotals[BENCHMARK_CASE_BEST][2],
            caseTotals[BENCHMARK_CASE_BEST][3],
            caseTotals[BENCHMARK_CASE_BEST][4],
            caseTotals[BENCHMARK_CASE_BEST][5],
            caseTotals[BENCHMARK_CASE_BEST][6],
            caseTotals[BENCHMARK_CASE_BEST][7],
            caseTotals[BENCHMARK_CASE_BEST][8],
            caseTotals[BENCHMARK_CASE_BEST][9]
        );
        printf(
            "%d \t | %s \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_AVERAGE),
            caseTotals[BENCHMARK_CASE_AVERAGE][0],
            caseTotals[BENCHMARK_CASE_AVERAGE][1],
            caseTotals[BENCHMARK_CASE_AVERAGE][2],
            caseTotals[BENCHMARK_CASE_AVERAGE][3],
            caseTotals[BENCHMARK_CASE_AVERAGE][4],
            caseTotals[BENCHMARK_CASE_AVERAGE][5],
            caseTotals[BENCHMARK_CASE_AVERAGE][6],
            caseTotals[BENCHMARK_CASE_AVERAGE][7],
            caseTotals[BENCHMARK_CASE_AVERAGE][8],
            caseTotals[BENCHMARK_CASE_AVERAGE][9]
        );
        printf(
            "%d \t | %s \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f \t | %.10f\n",
            n,
            get_case_name(BENCHMARK_CASE_WORST),
            caseTotals[BENCHMARK_CASE_WORST][0],
            caseTotals[BENCHMARK_CASE_WORST][1],
            caseTotals[BENCHMARK_CASE_WORST][2],
            caseTotals[BENCHMARK_CASE_WORST][3],
            caseTotals[BENCHMARK_CASE_WORST][4],
            caseTotals[BENCHMARK_CASE_WORST][5],
            caseTotals[BENCHMARK_CASE_WORST][6],
            caseTotals[BENCHMARK_CASE_WORST][7],
            caseTotals[BENCHMARK_CASE_WORST][8],
            caseTotals[BENCHMARK_CASE_WORST][9]
        );
    }

    progress_clear_line();
    printf(SHOW_CURSOR);

    fclose(out);
    free_deportistas_array(baseArray, count);
    printf(BG_GREEN "\nBenchmark de ordenamiento guardado con exito en %s\n" RESET, SORT_BENCHMARK_ROUTE);
}
