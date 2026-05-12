/**
 * @file print_format.c
 * @brief Implementacion de utilidades de formato.
 */

#include "print_format.h"

static void print_result_header_base(const char *title)
{
    const char *safeTitle = (title != NULL && title[0] != '\0') ? title : "=== Resultado ===";

    system("clear");

    printf(BOLD TEAL "%s\n" RESET, safeTitle);
    printf(DIM GRAY "%s\n" RESET, ASCII_HR);
}

static void print_result_footer_base(void)
{
    printf(DIM GRAY "%s\n" RESET, ASCII_HR);
}

/**
 * @brief Imprime una lista de deportistas.
 * 
 * @param deportistas Array de deportistas a imprimir.
 * @param rankingAmount Número de deportistas a imprimir.
 */
void print_deportistas_array(Deportista *deportistas, int rankingAmount)
{
    if(deportistas == NULL || rankingAmount <= 0) {
        return;
    }

    const int pageSize = (DEPORTISTAS_PAGE_SIZE > 0) ? DEPORTISTAS_PAGE_SIZE : rankingAmount;
    int totalPages = (rankingAmount + pageSize - 1) / pageSize;
    int currentPage = 1;

    for(int i = 0; i < rankingAmount; i++) {
        print_deportista(deportistas[i]);

        int printedInPage = (i + 1) % pageSize;
        int hasMore = (i + 1) < rankingAmount;
        if(printedInPage == 0 && hasMore) {
            char input[16];
            printf(DIM GRAY "-- Pagina %d/%d (Enter=continuar, q=salir) --" RESET "\n", currentPage, totalPages);
            fflush(stdout);

            if(fgets(input, sizeof(input), stdin) == NULL) {
                return;
            }
            if(input[0] == 'q' || input[0] == 'Q') {
                return;
            }

            currentPage++;
        }
    }
}

/**
 * @brief Limpia la linea actual en la consola.
 */
void progress_clear_line(void)
{
    printf(CURSOR_HOME CLEAR_LINE);
    fflush(stdout);
}

/**
 * @brief Actualiza la barra de progreso de un benchmark.
 *
 * @param benchmarkName Nombre del benchmark (ej: "search" o "sort")
 * @param intervalIndex Indice del intervalo actual
 * @param intervalCount Cantidad de intervalos
 * @param n Tamano de los datos
 * @param repeatIndex Indice de la repeticion actual
 * @param repeatCount Cantidad de repeticiones
 * @param stage Etapa del proceso
 */
void progress_update_line(const char *benchmarkName, int intervalIndex, int intervalCount, int n, int repeatIndex, int repeatCount, const char *stage)
{
    printf(
        CURSOR_HOME CLEAR_LINE DIM"%s | %d/%d | n=%d | rep %d/%d | %s" RESET,
        benchmarkName,
        intervalIndex,
        intervalCount,
        n,
        repeatIndex,
        repeatCount,
        stage
    );
    fflush(stdout);
}

/**
 * @brief Devuelve el nombre legible de un caso experimental.
 *
 * @param benchmarkCase Caso experimental.
 * @return const char* Nombre asociado.
 */
const char *get_case_name(BenchmarkCase benchmarkCase)
{
    switch(benchmarkCase) {
        case BENCHMARK_CASE_BEST:
            return "mejor";
        case BENCHMARK_CASE_AVERAGE:
            return "promedio";
        case BENCHMARK_CASE_WORST:
            return "peor";
        default:
            return "desconocido";
    }
}

/**
 * @brief Imprime la ayuda de uso del programa.
 *
 * @param programName Nombre del ejecutable.
 */
void print_help(const char *programName)
{
    const char *name = (programName != NULL && programName[0] != '\0') ? programName : "programa";

    printf(BOLD BLUE "Uso:" RESET " %s [opciones]\n\n", name);
    printf(BOLD "Opciones:\n" RESET);
    printf("  %-14s %s\n", "-h", "Muestra esta ayuda");
    printf("  %-14s %s\n", "-g <cantidad>", "Genera datos aleatorios y los guarda en el CSV");
    printf("  %-14s %s\n", "-t", "Ejecuta el flujo interactivo de ordenamiento");
    printf("  %-14s %s\n", "-i <id>", "Busca un deportista por ID");
    printf("  %-14s %s\n", "-r <cantidad>", "Muestra el top N por puntaje");
    printf("  %-14s %s\n", "-p <min> <max>", "Muestra deportistas con puntaje en ese rango");
    printf("  %-14s %s\n", "-b", "Ejecuta benchmark de busqueda");
    printf("  %-14s %s\n", "-s", "Ejecuta benchmark de ordenamiento");
    printf("\n");
}

void print_search_result_header(const char *algorithmName, const char *fieldName, int value, int matches)
{
    const char *safeAlgorithm = (algorithmName != NULL && algorithmName[0] != '\0') ? algorithmName : "(desconocido)";
    const char *safeField = (fieldName != NULL && fieldName[0] != '\0') ? fieldName : "(desconocido)";

    print_result_header_base("=== Resultado de busqueda ===");
    printf(BOLD "Algoritmo: " RESET "%s\n", safeAlgorithm);
    printf(BOLD "Criterio:  " RESET "%s\n", safeField);
    printf(BOLD "Valor:     " RESET "%d\n", value);
    printf(BOLD "Hallados:  " RESET "%d\n", matches);
    printf(DIM GRAY "%s\n" RESET, ASCII_HR);
}

void print_search_result_footer(void)
{
    print_result_footer_base();
}

void print_sort_result_header(const char *algorithmName, const char *fieldName, const char *orderName, int shown, int total, double sortSeconds)
{
    const char *safeAlgorithm = (algorithmName != NULL && algorithmName[0] != '\0') ? algorithmName : "(desconocido)";
    const char *safeField = (fieldName != NULL && fieldName[0] != '\0') ? fieldName : "(desconocido)";
    const char *safeOrder = (orderName != NULL && orderName[0] != '\0') ? orderName : "(desconocido)";

    print_result_header_base("=== Resultado de ordenamiento ===");
    printf(BOLD "Algoritmo: " RESET "%s\n", safeAlgorithm);
    printf(BOLD "Criterio:  " RESET "%s\n", safeField);
    printf(BOLD "Orden:     " RESET "%s\n", safeOrder);
    if(shown > DEPORTISTAS_PAGE_SIZE && DEPORTISTAS_PAGE_SIZE > 0) {
        printf(BOLD "Mostrando: " RESET "%d/%d\n", DEPORTISTAS_PAGE_SIZE, total);
    }
    else{
        printf(BOLD "Mostrando: " RESET "%d/%d\n", shown, total);
    }
    if(sortSeconds >= 0) {
        printf(BOLD "Tiempo:    " RESET "%.6f s\n", sortSeconds);
    }
    printf(DIM GRAY "%s\n" RESET, ASCII_HR);
}

void print_sort_result_footer(void)
{
    print_result_footer_base();
}

void print_time(time_t start, time_t end)
{
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf(BOLD "Tiempo de ejecucion: " RESET "%.6f segundos\n", seconds);
}