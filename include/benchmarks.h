#ifndef BENCHMARKS_H
#define BENCHMARKS_H

#include "time.h"
#include "deportista.h"
#include "searching.h"
#include "sorting.h"
#include "print_format.h"

#define SEARCH_BENCH_ALGO_COUNT 6
#define SORT_BENCH_ALGO_COUNT 10

/**
 * @brief Tipo para mascaras de seleccion de benchmarks (bit i => opcion i).
 */
typedef unsigned int BenchmarkMask;

/**
 * @brief Estructura para definir un algoritmo de búsqueda para benchmarks.
 */
typedef struct {
    const char *label;  /**< Etiqueta descriptiva del algoritmo. */
    const char *csvCol; /**< Nombre de la columna CSV para este algoritmo. */
    const char *stage;  /**< Nombre de la etapa para mostrar en progreso. */
} SearchBenchAlgoDef;

/**
 * @brief Estructura para definir un algoritmo de ordenamiento para benchmarks.
 */
typedef struct {
    const char *label;  /**< Etiqueta descriptiva del algoritmo. */
    const char *csvName; /**< Nombre de la columna CSV para este algoritmo. */
    const char *stage;  /**< Nombre de la etapa para mostrar en progreso. */
} SortBenchAlgoDef;

/* Definiciones de los algoritmos disponibles para benchmarks */
const SearchBenchAlgoDef SEARCH_BENCH_ALGOS[SEARCH_BENCH_ALGO_COUNT] = {
    {"Secuencial (ID)", "peor_secuencial_s", "peor secuencial"},
    {"Binaria (ID)", "peor_binaria_s", "peor binaria"},
    {"Binaria recursiva (ID)", "peor_binaria_rec_s", "peor binaria rec"},
    {"Exponencial (ID)", "peor_exponencial_s", "peor exponencial"},
    {"Interpolacion (ID)", "peor_interpolacion_s", "peor interpolacion"},
    {"Binaria por rango (PUNTAJE)", "peor_rango_puntaje_s", "peor rango puntaje"}
};

/* Definiciones de los algoritmos disponibles para benchmarks */
const SortBenchAlgoDef SORT_BENCH_ALGOS[SORT_BENCH_ALGO_COUNT] = {
    {"Insertion", "insertion", "insertion"},
    {"Bubble (opt)", "bubble", "bubble_opt"},
    {"Selection (opt)", "selection", "selection_opt"},
    {"Cocktail shaker", "cocktail", "cocktail"},
    {"Merge", "merge", "merge"},
    {"Merge optimizado", "merge_opt", "merge_opt"},
    {"Quick (pivote primero)", "quick_first", "quick_first"},
    {"Quick (pivote ultimo)", "quick_last", "quick_last"},
    {"Quick (pivote aleatorio)", "quick_random", "quick_random"},
    {"Quick (pivote mediana)", "quick_median", "quick_median"}
};

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
int build_selected_indices(BenchmarkMask mask, int optionCount, int *indices, int maxIndices);

/**
 * @brief Genera una mascara de bits que selecciona todas las opciones.
 *
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con todas las opciones seleccionadas.
 */
BenchmarkMask benchmark_all_mask(int optionCount);

/**
 * @brief Parsea una cadena de texto y genera una mascara de bits con las opciones seleccionadas.
 *
 * @param line Cadena de texto con las opciones seleccionadas.
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con las opciones seleccionadas.
 */
BenchmarkMask parse_benchmark_selection(const char *line, int optionCount);

/**
 * @brief Solicita al usuario que seleccione opciones para el benchmark.
 *
 * @param title Titulo de la seleccion.
 * @param labels Arreglo de etiquetas para las opciones.
 * @param optionCount Cantidad total de opciones disponibles.
 * @return BenchmarkMask Mascara de bits con las opciones seleccionadas.
 */
BenchmarkMask ask_benchmark_selection(const char *title, const char *const *labels, int optionCount);

/**
 * @brief Convierte el tiempo medido en ticks a segundos.
 *
 * @param start Tiempo de inicio en ticks.
 * @param end Tiempo de finalización en ticks.
 * @return double Tiempo en segundos.
 */
double clock_to_seconds(clock_t start, clock_t end);

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
double run_search_worst_case_once(int algoIndex, Deportista *baseArray, int baseCount, int n, FILE *out, int intervalIndex, int intervalCount, int repeatIndex, int repeatCount);

/**
 * @brief Ejecuta una iteración del benchmark de ordenamiento.
 * 
 * @param algoIndex Indice del algoritmo a ejecutar.
 * @param work Arreglo de deportistas a ordenar (debe tener `n` elementos).
 * @param n Cantidad de elementos en el arreglo.
 */
void run_sort_algorithm_once(int algoIndex, Deportista *work, int n);

/**
 * @brief Prepara un arreglo para medir un caso experimental de ordenamiento.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param benchmarkCase Caso experimental requerido.
 */
void prepare_sort_case(Deportista *deportistas, int count, BenchmarkCase benchmarkCase);

/**
 * @brief Busca un ID inexistente para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int ID que no esta presente en el arreglo.
 */
int get_missing_target_id(Deportista *deportistas, int count);

/**
 * @brief Busca un puntaje inexistente (en int) para medir el peor caso de busqueda por rango.
 *
 * @param deportistas Arreglo de deportistas.
 * @param count Cantidad de elementos.
 * @return int Puntaje (entero) que no esta presente en el arreglo.
 */
int get_missing_target_score(Deportista *deportistas, int count);

/**
 * @brief Prepara el arreglo para medir el peor caso de busqueda.
 *
 * @param deportistas Arreglo a preparar.
 * @param count Cantidad de elementos.
 * @param algorithm Algoritmo de busqueda a ejecutar.
 * @return int ID ausente para provocar el peor caso.
 */
int prepare_search_worst_case(Deportista *deportistas, int count, SearchAlgorithm algorithm);

/**
 * @brief Duplica una cadena para los benchmarks.
 *
 * @param srcString String a duplicar.
 * @return char* Cadena duplicada o NULL si hubo un error.
 */
char *duplicate_string_bench(const char *srcString);

/**
 * @brief Maneja un error de memoria durante la ejecucion de benchmarks.
 *
 * @param baseArray Arreglo base cargado desde el CSV.
 * @param count Cantidad de deportistas del arreglo base.
 * @param out Archivo de salida del benchmark.
 */
void handle_benchmark_memory_error(Deportista *baseArray, int count, FILE *out);

/**
 * @brief Clona un arreglo de deportistas para ejecutar benchmarks.
 *
 * @param srcArray Arreglo a clonar.
 * @param count Cantidad de elementos a clonar
 * @return Deportista* Arreglo de deportistas clonados o NULL si hubo un error.
 */
Deportista *clone_deportistas_array(Deportista *srcArray, int count);

/**
 * @brief Ejecuta el benchmark de busqueda y guarda sus resultados en CSV.
 */
void run_search_benchmark();

/**
 * @brief Ejecuta el benchmark de ordenamiento y guarda sus resultados en CSV.
 */
void run_sort_benchmark();




#endif