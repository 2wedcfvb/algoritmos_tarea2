# Algorithm Analyzer V2

![C](https://img.shields.io/badge/C-C11-blue) ![Build](https://img.shields.io/badge/build-make-success) ![LaTeX](https://img.shields.io/badge/docs-LaTeX-9cf) ![Doxygen](https://img.shields.io/badge/docs-Doxygen-blue) ![Gnuplot](https://img.shields.io/badge/plots-gnuplot-orange)

Proyecto en C para generar datos de deportistas, ejecutar busquedas y ordenamientos sobre esos datos, y medir el tiempo de ejecucion de cada algoritmo mediante benchmarks. Hecho para la asignatura de Diseño de Algoritmos 2026/1.

## Funcionalidad

- Generacion de un CSV con deportistas aleatorios.
- Busqueda por ID con busqueda secuencial, binaria, binaria recursiva, exponencial e interpolacion.
- Busqueda de deportistas por rango de puntaje.
- Ordenamiento interactivo por ID, puntaje, competencias, nombre o equipo.
- Ranking de los mejores deportistas por puntaje.
- Consulta del k-esimo deportista por puntaje.
- Benchmarks de busqueda y ordenamiento con exportacion a CSV.
- Benchmark de seleccion.
- Generacion de graficos a partir de los resultados con `gnuplot`.

## Requisitos

- `gcc` y `make`.
- `gnuplot` para generar graficos.
- (Opcional) `doxygen` para generar documentacion del codigo.
- (Opcional) TeX Live + `latexmk` + `biber` para compilar el informe.

## Compilacion

```bash
make
```

El ejecutable queda en `build/algorithm_analyzer.out`.

## Uso

```bash
./build/algorithm_analyzer.out -h
./build/algorithm_analyzer.out -g 1000
./build/algorithm_analyzer.out -t
./build/algorithm_analyzer.out -i 42
./build/algorithm_analyzer.out -r 10
./build/algorithm_analyzer.out -k 3
./build/algorithm_analyzer.out -p 50 80
./build/algorithm_analyzer.out -b
./build/algorithm_analyzer.out -s
./build/algorithm_analyzer.out -n
make plot
```

Los parametros `-p <min> <max>` muestran los deportistas con puntaje dentro de ese rango.

Comandos disponibles:

- `-h`: muestra la ayuda.
- `-g <cantidad>`: genera datos aleatorios y los guarda en el CSV.
- `-t`: ejecuta el flujo interactivo de ordenamiento.
- `-i <id>`: busca un deportista por ID.
- `-r <cantidad>`: muestra el top N por puntaje.
- `-k <k>`: muestra el k-esimo deportista por puntaje.
- `-p <min> <max>`: muestra deportistas con puntaje dentro de ese rango.
- `-b`: ejecuta benchmark de busqueda.
- `-s`: ejecuta benchmark de ordenamiento.
- `-n`: ejecuta benchmark de seleccion.

## Estructura

- `src/`: implementaciones del programa, algoritmos y benchmarks.
- `include/`: cabeceras publicas, estructuras, enums y prototipos.
- `db/`: datos CSV de entrada y resultados de benchmarks.
- `plots/`: script de `gnuplot` y graficos generados.
- `docs/`: informe del proyecto en LaTeX.

## Documentacion (Doxygen)

Para generar la documentacion del codigo con Doxygen:

```bash
doxygen Doxyfile
```

La salida HTML queda en `docs/doxygen/html/index.html`.

## Codigos de error

Los mensajes de error visibles para el usuario se centralizan en `print_error(...)` mediante el enum `ErrorCode`:

| Codigo | Significado |
| --- | --- |
| `ERROR_INVALID_DATA_AMOUNT` | La cantidad de datos solicitada esta fuera del rango permitido. |
| `ERROR_INVALID_RANKING_AMOUNT` | El ranking solicitado es menor o igual a cero. |
| `ERROR_NO_DATA_LOADED` | No se pudieron cargar deportistas desde el CSV principal. |
| `ERROR_FILE_CREATE_FAILED` | No se pudo crear un archivo de salida. |
| `ERROR_MEMORY_ALLOCATION_FAILED` | Fallo una reserva de memoria dinamica. |
| `ERROR_BENCHMARK_DATA_LOAD_FAILED` | No se pudieron cargar datos para ejecutar un benchmark. |
| `ERROR_UNKNOWN_OPTION` | Se recibio una opcion invalida por linea de comandos. |
| `ERROR_NOT_IMPLEMENTED` | Se intento usar una opcion no implementada. |
| `ERROR_DEPORTISTA_NOT_FOUND` | No existe un deportista para el ID solicitado. |

## Modulos principales

- `csv.c`: genera y carga archivos CSV de deportistas.
- `deportista.c`: administra la estructura `Deportista`.
- `generator.c`: produce nombres, equipos, puntajes y competencias aleatorias.
- `run_experiment.c`: contiene el flujo interactivo para ordenar, buscar y mostrar rankings.
- `benchmarks.c`: mide tiempos de ejecucion y exporta resultados.
- `sorting.c` y algoritmos asociados: encapsulan comparacion y ordenamiento.
- `sequential_search.c`, `binary_search.c`, `recursive_binary_search.c`, `range_binary_search.c`, `exponencial_search.c` e `interpolation_search.c`: implementan las busquedas del programa.

## Avisos

- Los archivos del informe en LaTeX se conservan aparte del flujo principal del programa.
- Los CSV y PNG generados pueden cambiar al ejecutar benchmarks o generar nuevos datos.
