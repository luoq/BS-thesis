#!/bin/bash
source misc.sh
d=3
gen_prog=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/cpp/genMixRegular
RNW=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/fortran/RNW
IDEM1=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/fortran/IDEM1
IDEM2=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/fortran/IDEM2
H1=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/fortran/H1
H2=/home/lq/Documents/资料/数学/科学计算/毕业设计/codes/fortran/H2
method=H1

#basic functions
function all_sizes()
{
    local size
    local sizes=""
    for file in $(find . -maxdepth 1 -type d -name "[0-9]*")
    do
		sizes="$sizes ${file##*/}"
    done
	echo "$sizes"|tr " " "\n"|sort -g|tr "\n" " "


	## fix size from 10 to 60
	#local sizes=""
	#for ((size=10;size<=60;size++));do
	#	sizes="$sizes $size"
	#done
	#echo $sizes
		
}
function num_of_matrices()
{
    size=$1
    k=$2
    echo $(ls $size/$k/matrices/*.matrix 2>/dev/null|wc -w)
}
#num of calculated matrices
function calculated()
{
	size=$1
	k=$2
	cat $size/$k/$method.time 2>/dev/null|wc -l 
}

function calcu()
{
    local size=$1
    local k=$2
    local id=$3
	sudo nice -n -10 $(eval echo \$$method) $size/$k/matrices/$id.matrix
}
function gen()
{
	local i
    size=$1
    k=$2
    [ -d $size/$k/matrices ] || mkdir -p $size/$k/matrices

    i=$(($(num_of_matrices $size $k)+1))
    $gen_prog $size $k $d $size/$k/matrices/$i.matrix > /dev/null
}
function gen_p_matrix()
{
	local n_min=$1
	local n_max=$2
	local p=$3
	local times=$4
	local i

	for ((size=$n_min;size<=$n_max;size++));do
		k=$(bc_eval "convert_to_int($size*$p)")
		if [ $k != -1 ] ;then
			for ((i=1;i<=$times;i++));do
				gen $size $k 
			done
		fi
	done
}


#function to calculate all generated matrices save information
function calcu_all()
{
	local size=$1
	local k=$2

	local start=$(calculated  $size $k)
	start=$((start+1))
	local end=$(num_of_matrices $size $k)
	local id
	for ((id=$start;id<=$end;id++));do
		echo -n -e "\r size $size k $k id $id"
		calcu $size $k $id >> $size/$k/$method.time
		calcu $size $k $id >> $size/$k/$method.m
	done
	save_mean_and_std $size $k
}
function calcu_p_all()
{
	p=$1
	for size in $(all_sizes);do
		k=$(bc_eval "convert_to_int($size*$p)")
		if [ $k != -1 ] ;then
			calcu_all $size $k
		fi
	done
}
function save_mean_and_std()
{
	size=$1
	k=$2
	if [ -f $size/$k/$method.time ] ;then
		echo $(mean_and_std_on_col $size/$k/$method.time 2) > $size/$k/$method.result
		echo $(mean_and_std_on_col $size/$k/$method.time 3) > $size/$k/$method.m.result
	fi
}


##functions to correct random errors in run time
#record of matrix with max runtime
function max_record()
{
    size=$1
    k=$2
    cat -n $size/$k/$method.time|sort -g -k3|tail -n 1
}
function auto_correct()
{
    size=$1
	k=$2
	[ -f $size/$k/$method.time ] || return
	echo size $size
	while [ 1 ];do
		old="$(max_record $size $k)"
		id=$(echo "$old"|awk '{print $1}')
		old_time=$(echo "$old"|awk '{print $3}')
		#bc_cond "$old_time<5" || return
		new="$(calcu $size $k $id)"
		new_time=$(echo "$new"|awk '{print $2}')
		if bc_cond "$new_time < $old_time"
		then
			echo id: $id old time: $old_time new time:$new_time
			change_line $size/$k/$method.time $id "$new"
			#bc_cond "($old_time-$new_time)/$old_time<1e-2" && break
		else
			break
		fi
		break
	done

	save_mean_and_std $size $k
}
function auto_correct_p()
{
	p=$1
	savefile=$method.$p.result
	for size in $(all_sizes);do
		k=$(bc_eval "convert_to_int($size*$p)")
		if [ $k != -1 ] ;then
			auto_correct $size $k
		fi
	done
	gen_p_data $p
	plot_p_data $p 0
}
function auto_correct_all()
{
	for size in $(all_sizes);do
		for ((k=0;k<=$((size-1));k++));do
			echo -n -e "size $size k $k \r"
			auto_correct $size $k
		done
	done
	interest
}

##functions to gather information and plot
function gen_p_data()
{
	local p=$1

	[ -d p/${p////_} ] || mkdir -p p/${p////_} #replace / in p with _
	local savefile1=p/${p////_}/$method.result 
	local savefile2=p/${p////_}/$method.m.result
	[ -f $savefile1 ] && rm $savefile1
	[ -f $savefile2 ] && rm $savefile2
	for size in $(all_sizes);do
		k=$(bc_eval "convert_to_int($size*$p)")
		if [ $k != -1 ] && [ -f $size/$k/$method.result ] ;then
			echo "$size $k $(cat $size/$k/$method.result)" >> $savefile1
			echo "$size $k $(cat $size/$k/$method.m.result)" >> $savefile2
		fi
	done
}
function refresh()
{
	for size in $(all_sizes);do
		for ((k=0;k<=$size;k++));do
			save_mean_and_std $size $k 2>/dev/null

			#if [ ! -f $size/$k/$method.time ] && [ -f $size/$k/$method.result ];then
			#	rm $size/$k/$method.result
			#fi

			##to add m into data
			#paste $size/$k/$method.time $size/$k/$method.m > tmp
			#mv tmp $size/$k/$method.time
			#rm tmp
			#rm  $size/$k/$method.m
		done
	done
}
function gen_n_data()
{
    size=$1
	savefile1=$size/$method.result
    savefile2=$size/$method.m.result
    [ -f $savefile1 ] && rm $savefile1
    [ -f $savefile2 ] && rm $savefile2
    for ((k=0;k<=$size;k++));do
		if [ -f $size/$k/$method.result ] ;then
			echo "$k $(cat $size/$k/$method.result)" >> $savefile1 
			echo "$k $(cat $size/$k/$method.m.result)" >> $savefile2 
		fi
    done
}

function plot_n_data()
{
    size=$1
	[ -f $size/$method.result ] || return
	plot_range $size/$method.result 1 3 5 6 "k" "n=$size" $size/$method.eps
    plot_range $size/$method.m.result 1 3 5 6 "k" "n=$size" $size/$method.m.eps
}
function plot_p_data()
{
	p=$1
	log=$2
	plot p/${p////_}/$method.result 1 4 "n" "time/s" "p=$p" p/${p////_}/$method.eps $log
}
function plot_comparison()
{
	gnuplot comparison.gp
}

function interest()
{
	for p in 0 0.2 0.4 0.5 0.6 0.8 1 ;do
		gen_p_data $p
		plot_p_data $p
	done
	for size in $(all_sizes);do
		gen_n_data $size
		plot_n_data $size
	done
	#plot_comparison 
}

##plot distribution of runtimes for a size and k
plot_distribution()
{
    size=$1
    k=$2
    octave -q<<plot_distribution
	data=load('$size/$k/run.time');
	hist(data(:,3),30);
	xlabel('run time');
	print -deps -color '$size/$k/distribution.eps';
plot_distribution
    gnuplot <<plot_P-t
	set term post eps color solid enh
	set output '$size/$k/P-t.eps'
	set xlabel 'runtime/s'
	set ylabel 'permanent'
	set pointsize .3
	plot '$size/$k/run.time' u 3:2 w p pt 7 title ''
plot_P-t
}
function prompt()
{
    echo '#:number of matrices for a size and k'
    echo 'g:generate more matrices'
    echo 'p:calculate all matrices'
    echo 'c:correct records for given size'
    echo 'q:quit'
    read choice
    case $choice in
	'#')
	    echo -n 'which size:'
	    read size
	    echo -n 'which k:'
	    read k
		echo size $size k $k : $(num_of_matrices $size $k)
	    ;;
	g)
		local i
	    echo 'Input range of size:'
	    echo -n 'start with[5]:'
	    read low_size
	    low_size=${low_size:=5}
	    echo -n 'end with:'
	    read high_size
	    echo -n 'how many more:'
	    read n
		for ((size=$low_size;size<=$high_size;size++));do
			for ((k=0;k<=$size;k++));do
				echo -n -e "\rgenerating size $size k $k"
				for ((i=1;i<=$n;i++));do
					gen $size $k
				done
			done
		done
	    ;;
	p)
	    #echo -n 'which size:'
	    #read size
	    #echo -n 'which k:'
	    #read k
	    echo 'Input range of size:'
	    echo -n 'start with[10]:'
	    read low_size
	    low_size=${low_size:=10}
	    echo -n 'end with[60]:'
	    read high_size
	    high_size=${high_size:=60}
		for ((size=$low_size;size<=$high_size;size++));do
			for ((k=0;k<=$((size-1));k++));do
				calcu_all $size $k
			done
		done
		#interest 
		;;
	q)
	    break
	    ;;
	*)
	    echo 'bad command'
	    ;;
    esac
}
function welcome()
{
    echo 'welcome'
}
function main()
{
    welcome
    while [ 1 ]
    do
	prompt
    done
    echo Good bye
}
main
