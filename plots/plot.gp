# Script de gnuplot para los resultados de benchmarks.

set datafile separator ","
set terminal pdfcairo noenhanced font "Arial,16" size 16cm,9cm
set xlabel 'Cantidad de deportistas (n)'
set ylabel 'Tiempo promedio (s)'
set grid
set key outside
set border linewidth 2
set grid linewidth 1.5
set key font ",14"
set xlabel font ",16"
set ylabel font ",16"
set tics font ",13"


set style data lines
set key autotitle columnhead

searchFile = 'db/search_benchmark.csv'
sortFile   = 'db/sort_benchmark.csv'
selectionFile = 'db/selection_benchmark.csv'

searchWorstOut    = 'plots/search_worst_benchmark.pdf'
searchAverageOut  = 'plots/search_average_benchmark.pdf'
sortBestOut       = 'plots/sort_best_benchmark.pdf'
sortAverageOut    = 'plots/sort_average_benchmark.pdf'
sortWorstOut      = 'plots/sort_worst_benchmark.pdf'
selectionOut      = 'plots/selection_benchmark.pdf'

hasSearch = int(system(sprintf("test -f '%s' && echo 1 || echo 0", searchFile)))
hasSort   = int(system(sprintf("test -f '%s' && echo 1 || echo 0", sortFile)))
hasSelection = int(system(sprintf("test -f '%s' && echo 1 || echo 0", selectionFile)))

if (!hasSearch) {
	print sprintf("WARN: no existe %s (se saltará)", searchFile)
}
if (!hasSort) {
	print sprintf("WARN: no existe %s (se saltará)", sortFile)
}
if (!hasSelection) {
	print sprintf("WARN: no existe %s (se saltará)", selectionFile)
}

set autoscale
set xtics
set ytics
set key outside

##########################
# Search benchmark: caso promedio y peor (busca columnas con prefijos)
if (hasSearch) {
	# Promedio (prefijo 'promedio_')
	set output searchAverageOut
	set title 'Search benchmark - caso promedio'
	avgNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^promedio_/) printf \"%%s \", $i; exit}' \"%s\"", searchFile))
	navg = words(avgNames)
	if (navg < 1) {
		print sprintf("WARN: no se encontraron columnas 'promedio_' en %s", searchFile)
	} else {
		plot for [j=1:navg] searchFile using 1:(column(word(avgNames,j))) lw 4 title word(avgNames,j)
	}
	unset output

	# Peor (prefijo 'peor_')
	set output searchWorstOut
	set title 'Search benchmark - peor caso'
	worstNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^peor_/) printf \"%%s \", $i; exit}' \"%s\"", searchFile))
	nworst = words(worstNames)
	if (nworst < 1) {
		print sprintf("WARN: no se encontraron columnas 'peor_' en %s", searchFile)
	} else {
		plot for [j=1:nworst] searchFile using 1:(column(word(worstNames,j))) lw 4 title word(worstNames,j)
	}
	unset output
}


# Sort benchmark: mejor / promedio / peor
if (hasSort) {
	# Mejor (prefijo "mejor_")
	set output sortBestOut
	set title 'Sort benchmark - mejor caso'
	bestNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^mejor_/) printf \"%%s \", $i; exit}' \"%s\"", sortFile))
	nbest = words(bestNames)
	if (nbest < 1) {
		print sprintf("WARN: no se encontraron columnas 'mejor_' en %s", sortFile)
	} else {
		plot for [j=1:nbest] sortFile using 1:(column(word(bestNames,j))) lw 4 title word(bestNames,j)
	}
	unset output

	# Promedio (prefijo 'promedio_')
	set output sortAverageOut
	set title 'Sort benchmark - caso promedio'
	avgNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^promedio_/) printf \"%%s \", $i; exit}' \"%s\"", sortFile))
	navg = words(avgNames)
	if (navg < 1) {
		print sprintf("WARN: no se encontraron columnas 'promedio_' en %s", sortFile)
	} else {
		plot for [j=1:navg] sortFile using 1:(column(word(avgNames,j))) lw 4 title word(avgNames,j)
	}
	unset output

	# Peor (prefijo 'peor_')
	set output sortWorstOut
	set title 'Sort benchmark - peor caso'
	worstNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^peor_/) printf \"%%s \", $i; exit}' \"%s\"", sortFile))
	nworst = words(worstNames)
	if (nworst < 1) {
		print sprintf("WARN: no se encontraron columnas 'peor_' en %s", sortFile)
	} else {
		plot for [j=1:nworst] sortFile using 1:(column(word(worstNames,j))) lw 4 title word(worstNames,j)
	}
	unset output
}

# Selection benchmark: busca 'mejor_' y 'peor_'''
if (hasSelection) {
	set output selectionOut
	set title 'Selection benchmark'
	# Buscar columnas con prefijos relevantes
	selNames = system(sprintf("awk -F, 'NR==1{for(i=1;i<=NF;i++) if ($i ~ /^(mejor_|peor_|promedio_)/) printf \"%%s \", $i; exit}' \"%s\"", selectionFile))
	nsel = words(selNames)
	if (nsel < 1) {
		# fallback: todas menos la primera
		selNames = system(sprintf("awk -F, 'NR==1{for(i=2;i<=NF;i++) printf \"%%s \", $i; exit}' \"%s\"", selectionFile))
		nsel = words(selNames)
	}
	if (nsel < 1) {
		print sprintf("WARN: no hay columnas para plotear en %s", selectionFile)
	} else {
		plot for [j=1:nsel] selectionFile using 1:(column(word(selNames,j))) lw 4 title word(selNames,j)
	}
	unset output
}
