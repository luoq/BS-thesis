load('benchmark.data');
time=[benchmark(:,1) mean(benchmark(:,2:end),2)];
plot(time(:,1),time(:,2),'-')
