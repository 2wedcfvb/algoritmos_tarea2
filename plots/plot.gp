# =========================================
# Script de gnuplot para benchmarks
# =========================================

set datafile separator ","

set terminal pdfcairo enhanced color font "Arial,14" size 18cm,10cm

set xlabel "Cantidad de deportistas (n)"
set ylabel "Tiempo (s)"

set grid
set border linewidth 1.5

set key outside right
set key font ",12"

set xlabel font ",15"
set ylabel font ",15"
set title font ",18"

set tics font ",12"

# Escala logaritmica
set logscale y

# SOLO PUNTOS (sin lineas)
set style data lines
set pointsize 1.6

# =========================================
# Archivos
# =========================================

searchFile     = "db/search_benchmark.csv"
sortFile       = "db/sort_benchmark.csv"
selectionFile  = "db/selection_benchmark.csv"

# =========================================
# SEARCH - PROMEDIO
# =========================================

set output "plots/search_average_benchmark.pdf"
set title "Benchmark de busqueda - Caso promedio"

plot \
searchFile using 1:2 title "Secuencial", \
searchFile using 1:3 title "Binaria", \
searchFile using 1:4 title "Binaria recursiva", \
searchFile using 1:5 title "Exponencial", \
searchFile using 1:6 title "Interpolacion", \
searchFile using 1:7 title "Binaria rango"

unset output

# =========================================
# SEARCH - PEOR CASO
# =========================================

set output "plots/search_worst_benchmark.pdf"
set title "Benchmark de busqueda - Peor caso"

plot \
searchFile using 1:8  title "Secuencial", \
searchFile using 1:9  title "Binaria", \
searchFile using 1:10 title "Binaria recursiva", \
searchFile using 1:11 title "Exponencial", \
searchFile using 1:12 title "Interpolacion", \
searchFile using 1:13 title "Binaria rango"

unset output

# =========================================
# SORT - MEJOR CASO
# =========================================

set output "plots/sort_best_benchmark.pdf"
set title "Benchmark de ordenamiento - Mejor caso"

plot \
sortFile using 1:2 title "Insertion", \
sortFile using 1:3 title "Bubble", \
sortFile using 1:4 title "Selection", \
sortFile using 1:5 title "Cocktail", \
sortFile using 1:6 title "Merge", \
sortFile using 1:7 title "Merge opt", \
sortFile using 1:8 title "Quick first", \
sortFile using 1:9 title "Quick last", \
sortFile using 1:10 title "Quick random", \
sortFile using 1:11 title "Quick median"

unset output

# =========================================
# SORT - PROMEDIO
# =========================================

set output "plots/sort_average_benchmark.pdf"
set title "Benchmark de ordenamiento - Caso promedio"

plot \
sortFile using 1:12 title "Insertion", \
sortFile using 1:13 title "Bubble", \
sortFile using 1:14 title "Selection", \
sortFile using 1:15 title "Cocktail", \
sortFile using 1:16 title "Merge", \
sortFile using 1:17 title "Merge opt", \
sortFile using 1:18 title "Quick first", \
sortFile using 1:19 title "Quick last", \
sortFile using 1:20 title "Quick random", \
sortFile using 1:21 title "Quick median"

unset output

# =========================================
# SORT - PEOR CASO
# =========================================

set output "plots/sort_worst_benchmark.pdf"
set title "Benchmark de ordenamiento - Peor caso"

plot \
sortFile using 1:22 title "Insertion", \
sortFile using 1:23 title "Bubble", \
sortFile using 1:24 title "Selection", \
sortFile using 1:25 title "Cocktail", \
sortFile using 1:26 title "Merge", \
sortFile using 1:27 title "Merge opt", \
sortFile using 1:28 title "Quick first", \
sortFile using 1:29 title "Quick last", \
sortFile using 1:30 title "Quick random", \
sortFile using 1:31 title "Quick median"

unset output

# =========================================
# QUICKSELECT
# =========================================

set output "plots/selection_benchmark.pdf"
set title "Benchmark Quickselect"

plot \
selectionFile using 1:2 title "Mejor caso", \
selectionFile using 1:3 title "Peor caso"

unset output