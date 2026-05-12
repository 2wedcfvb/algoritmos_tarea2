/**
 * @file main.c
 * @brief Punto de entrada y parseo de argumentos del programa.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "base.h"
#include "csv.h"
#include "errors.h"
#include "print_format.h"
#include "kth_athlete.h"
#include "benchmarks.h"

/**
 * @brief Ejecuta el programa a partir de los argumentos entregados.
 *
 * @param argc Cantidad de argumentos.
 * @param argv Arreglo de argumentos.
 * @return int EXIT_SUCCESS si finaliza correctamente, EXIT_FAILURE si falla.
 */
int main(int argc, char **argv)
{
    int opt;

    srand(time(NULL));

    if(argc == 1) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    while((opt = getopt(argc, argv, "hg:tr:bsi:k:np:")) != -1) {
        switch(opt) {
            /* ------------ ayuda -----------------*/
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;

            /* ------------ generacion de datos -----------------*/
            case 'g':
                if(MIN_DATA > atoi(optarg) || atoi(optarg) > MAX_DATA) {
                    char detail[128];

                    snprintf(detail, sizeof(detail), "Debe estar entre %d y %d", MIN_DATA, MAX_DATA);
                    print_error(ERROR_INVALID_DATA_AMOUNT, detail);
                    return EXIT_FAILURE;
                }

                printf("\nGenerando" BOLD ORANGE " %d " RESET "datos...\n\n", atoi(optarg));
                create_deportistas_csv(atoi(optarg));
                break;
            
            /* ------------ flujo interactivo ordenamiento -----------------*/
            case 't':
                run_experiment();
                break;

            /** ----------- flujo interactivo busqueda --------------------- */
            case 'i':
                search(atoi(optarg));
                break;

            /** ----------- mostrar ranking --------------------- */
            case 'r':

                if(atoi(optarg) <= 0) {
                    print_error(ERROR_INVALID_RANKING_AMOUNT, NULL);
                    return EXIT_FAILURE;
                }

                show_ranking(atoi(optarg));
                break;
            
            /** ----------- benchmark de busqueda --------------------- */
            case 'b':
                run_search_benchmark();
                break;
            
            /** ----------- benchmark de ordenamiento --------------------- */
            case 's':  
                run_sort_benchmark();
                break;

            /** ----------- benchmark de selección ----------------------- */
            case 'n':
                run_selection_benchmark();
                break;
            
            /** ---------- k-ésimo deportista --------------------- */
            case 'k':
                show_kth_athlete(atoi(optarg));
                break;
            
            /** ---------- puntaje entre rangos -------------------- */
            case 'p':
            {
                char *endFirst = NULL;
                char *endSecond = NULL;
                long puntajeInit;
                long puntajeEnd;

                if(optind >= argc || argv[optind] == NULL) {
                    print_error(ERROR_UNKNOWN_OPTION, "-p requiere 2 parametros");
                    return EXIT_FAILURE;
                }

                puntajeInit = strtol(optarg, &endFirst, 10);
                puntajeEnd = strtol(argv[optind], &endSecond, 10);

                if((endFirst != NULL && *endFirst != '\0') || (endSecond != NULL && *endSecond != '\0')) {
                    print_error(ERROR_UNKNOWN_OPTION, "-p requiere 2 parametros enteros");
                    return EXIT_FAILURE;
                }

                puntaje_range((int)puntajeInit, (int)puntajeEnd);
                optind++;
                break;
            }
            /** ----------- opción desconocida --------------------- */
            case '?':
            {
                char detail[32];
                const char *detailPtr = NULL;

                if(optopt != 0) {
                    snprintf(detail, sizeof(detail), "-%c", optopt);
                    detailPtr = detail;
                } else if(optind > 0 && optind <= argc) {
                    detailPtr = argv[optind - 1];
                }

                print_error(ERROR_UNKNOWN_OPTION, detailPtr);
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}
