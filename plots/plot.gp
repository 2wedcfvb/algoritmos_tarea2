
# Script de gnuplot para benchmarks


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


# Archivos


searchFile     = "db/search_benchmark.csv"
sortFile       = "db/sort_benchmark.csv"
selectionFile  = "db/selection_benchmark.csv"
thresholdFile  = "db/threshold_benchmark.csv"


# SEARCH - PROMEDIO


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


# SEARCH - PEOR CASO


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


# SORT - MEJOR CASO


set output "plots/sort_best_benchmark.pdf"
set title "Benchmark de ordenamiento - Mejor caso"

plot \
sortFile using 1:2 title "Merge", \
sortFile using 1:3 title "Merge opt", \
sortFile using 1:4 title "Quick first", \
sortFile using 1:5 title "Quick last", \
sortFile using 1:6 title "Quick random", \
sortFile using 1:7 title "Quick median"

unset output


# SORT - PROMEDIO


set output "plots/sort_average_benchmark.pdf"
set title "Benchmark de ordenamiento - Caso promedio"

plot \
sortFile using 1:8 title "Merge", \
sortFile using 1:9 title "Merge opt", \
sortFile using 1:10 title "Quick first", \
sortFile using 1:11 title "Quick last", \
sortFile using 1:12 title "Quick random", \
sortFile using 1:13 title "Quick median"

unset output


# SORT - PEOR CASO


set output "plots/sort_worst_benchmark.pdf"
set title "Benchmark de ordenamiento - Peor caso"

plot \
sortFile using 1:14 title "Merge", \
sortFile using 1:15 title "Merge opt", \
sortFile using 1:16 title "Quick first", \
sortFile using 1:17 title "Quick last", \
sortFile using 1:18 title "Quick random", \
sortFile using 1:19 title "Quick median"

unset output


# QUICKSELECT


set output "plots/selection_benchmark.pdf"
set title "Benchmark Quickselect"

plot \
selectionFile using 1:2 title "Mejor caso", \
selectionFile using 1:3 title "Peor caso"

unset output


# THRESHOLD

set output "plots/threshold_benchmark.pdf"
set title "Benchmark Threshold"

plot \
thresholdFile using 1:2 title "Tiempo"

unset output