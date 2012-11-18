#mean and std of relative range for 40<=k<=60
BEGIN{
	n=0
	sum=0
	sum2=0
	min=1e20
	max=0
}
$1<=20{
	n+=1
	sum+=$4/$3
	sum2+=($4/$3)^2
	if($c<min)
		min=$c
	if($c>max)
		max=$c
}
END{
	print n,sum/n,sqrt((sum2-sum^2/n)/(n-1)),min,max
}
