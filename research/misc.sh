#!/bin/bash
##misc helper funcitons 
function bc_cond()
{
	#NOTE: bc doesn't surpport scientific notation
	local result=$(echo "$@"|sed 's/\([+-]\{0,1\}[0-9]\{1,\}\(\.\{0,1\}[0-9]\{1,\}\)\{0,1\}\)[eE]\([+-]\{0,1\}[0-9]\{1,\}\)/\1*10^\3/g'| bc -q functions.bc)
	[ $result -eq 1 ] && return 0
	return 1
}
function bc_eval()
{
	#NOTE: bc doesn't surpport scientific notation
	echo "$@"|sed 's/\([+-]\{0,1\}[0-9]\{1,\}\(\.\{0,1\}[0-9]\{1,\}\)\{0,1\}\)[eE]\([+-]\{0,1\}[0-9]\{1,\}\)/\1*10^\3/g'| bc -q functions.bc
}
function file_lines()
{
	if [ -f $1 ]
	then
		line=$(cat $1|wc -l)
	else
		line=0
	fi
	echo $line
}
function remove_line()
{
	file=$1
	line=$2
	sed -i "${line}d" $file
}
function change_line()
{
	file=$1
	line=$2
	new=$3
	#,* instead of ^$ is used
	sed -i "${line}s/.*/$new/" $file
}
function read_line()
{
	file=$1
	line=$2
	sed -n "${line}p" $file
}
function mean_and_std_on_col()
{
	path=$1
	c=$2
	echo $(awk -f mean_and_std_on_col.awk -v c=$c $path)
}
function plot()
{
    file=$1
	n_x=$2
	n_y=$3
	xlabel=$4
	ylabel=$5
    title=$6
    savefile=$7
	log=$8
	if [ $log == 0 ];then
		gnuplot<<plot.plot
		set term post eps color solid enh
		set output '$savefile'
		set mxtics 5
		set grid xtics mxtics ytics

		set xlabel '$xlabel'
		set ylabel 'relative range'
		#set ytic 0.1
		plot '$file' u $n_x:$n_y w l lw 2 title ''
plot.plot
	elif [ $log == 1 ];then
		gnuplot<<plot.plot
		set term post eps color solid enh
		set output '$savefile'
		set mxtics 5
		set grid xtics mxtics ytics

		set xlabel '$xlabel'
		set ylabel 'relative range'
		#set ytic 0.1
		set logscale y
		plot '$file' u $n_x:$n_y w l lw 2 title ''
plot.plot
	elif [ $log == 2 ];then
		gnuplot<<plot.plot
		set term post eps color solid enh
		set output '$savefile'
		set mxtics 5
		set grid xtics mxtics ytics

		set xlabel '$xlabel'
		set ylabel 'relative range'
		#set ytic 0.1
		set logscale x
		set logscale y
		plot '$file' u $n_x:$n_y w l lw 2 title ''
plot.plot
	fi
}
function plot_range()
{
    file=$1
	n_x=$2
	n_mean=$3
	n_min=$4
	n_max=$5
	xlabel=$6
    title=$7
    savefile=$8
    gnuplot<<plot_range.plot
	set term post eps color solid enh
	set output '$savefile'
	set multiplot
	set xtic 5
	set mxtics 5
	set lmargin 9
	set rmargin 2
	set grid xtics mxtics ytics

	set origin 0,0
	set size 1,0.3
	set bmargin 3
	set tmargin 0
	set xlabel '$xlabel'
	set ylabel 'relative range'
	set xtic 5
	set mxtics 5
	#set ytic 0.1
	set grid xtics mxtics ytics
	#plot '$file' u $n_x:(\$$n_std/\$$n_mean) w l lw 2 title ''
	plot '$file' u $n_x:((\$$n_max-\$$n_min)/\$$n_mean) w l lw 2 title ''

	set origin 0.0,0.3
	set size 1,0.7
	set bmargin 0
	set tmargin 3
	#set ytic 0.0001,0.1,10
	set format x ""
	set xlabel ""
	set ylabel 'run time/s'
	set title '$title'
	set logscale y
	#plot '$file' u $n_x:$n_mean:$n_min:$n_max w errorlines lw 2 title ''
	plot '$file' u $n_x:$n_mean w l lw 2 title ''

	set nomultiplot
plot_range.plot
}
function plot_range2()
{
    file=$1
	n_x=$2
	n_mean=$3
	n_min=$4
	n_max=$5
	xlabel=$6
    title=$7
    savefile=$8
    gnuplot<<plot_range.plot
	set term post eps color solid enh
	set output '$savefile'
	set multiplot
	set xtic 5
	set mxtics 5
	set lmargin 9
	set rmargin 2
	set grid xtics mxtics ytics

	set origin 0,0
	set size 1,0.3
	set bmargin 3
	set tmargin 0
	set xlabel '$xlabel'
	set ylabel 'relative range'
	set xtic 5
	set mxtics 5
	#set ytic 0.1
	set grid xtics mxtics ytics
	#plot '$file' u $n_x:(\$$n_std/\$$n_mean) w l lw 2 title ''
	plot '$file' u $n_x:((\$$n_max-\$$n_min)/\$$n_mean) w l lw 2 title ''

	set origin 0.0,0.3
	set size 1,0.7
	set bmargin 0
	set tmargin 3
	#set ytic 0.0001,0.1,10
	set format x ""
	set xlabel ""
	set ylabel 'run time/s'
	set title '$title'
	#set logscale y
	#plot '$file' u $n_x:$n_mean:$n_min:$n_max w errorlines lw 2 title ''
	plot '$file' u $n_x:$n_mean w l lw 2 title ''

	set nomultiplot
plot_range.plot
}
