# Algorithm Analyzer

![C](https://img.shields.io/badge/C-C11-blue) ![Build](https://img.shields.io/badge/build-make-success) ![LaTeX](https://img.shields.io/badge/docs-LaTeX-9cf) ![Doxygen](https://img.shields.io/badge/docs-Doxygen-blue) ![Gnuplot](https://img.shields.io/badge/plots-gnuplot-orange)

Proyecto en C para generar datos de deportistas, ejecutar busquedas y ordenamientos sobre esos datos, y medir el tiempo de ejecucion de cada algoritmo mediante benchmarks. Hecho para la asignatura de Diseño de Algoritmos 2026/1.

## Funcionalidad

- Generacion de un CSV con deportistas aleatorios.
- Busqueda por ID con busqueda secuencial o binaria.
- Ordenamiento interactivo por ID, puntaje, competencias, nombre o equipo.
- Ranking de los mejores deportistas por puntaje.
- Benchmarks de busqueda y ordenamiento con exportacion a CSV.
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
./build/algorithm_analyzer.out -id 42
./build/algorithm_analyzer.out -r 10
./build/algorithm_analyzer.out -b
./build/algorithm_analyzer.out -s
make plot
```

Tambien se mantiene el alias `-id [valor]` por compatibilidad con el flujo previo del proyecto.

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


## Criterios de codificacion aplicados

El codigo del repositorio fue alineado con las normas adjuntas:

- Variables en `camelCase`.
- Funciones en `snake_case`.
- Constantes y macros en `SCREAMING_SNAKE_CASE`.
- Llave de apertura de funciones en la linea inferior al prototipo.
- Llaves en bloques de control siempre presentes, incluso si hay una sola sentencia.
- Errores centralizados mediante `print_error(...)` y un conjunto documentado de codigos.
- Comentarios y textos del proyecto en ASCII para evitar problemas de compatibilidad por tildes o `enie`.

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
- `sequential_search.c` y `binary_search.c`: implementan las busquedas por ID.

## Avisos

- Los archivos del informe en LaTeX se conservan aparte del flujo principal del programa.
- Los CSV y PNG generados pueden cambiar al ejecutar benchmarks o generar nuevos datos.
- Se recomienda realizar una generación de no más de 10.000 datos, para apreciar correctamente los gráficos, ya que este es el punto ideal para ver diferencias y a la vez que no se demore en realizar los benchmarks (el programa fue probado con hasta 100.000 datos generados aleatoriamente, pero para conseguir resultados hay que esperar un tiempo de más de 30 minutos o 1 hora dependiendo del equipo donde se ejecute el programa, y se comprobó que la diferencia con 10.000 datos al momento de analizar los gráficos era mínima).
