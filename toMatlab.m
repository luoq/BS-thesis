%%% toMatlab.m --- 
%% 
%% Filename: toMatlab.m
%% Description: load the sparse matrix saved by C++ program
%% Author: Luo Qiang
%% Created: 03/18/2010 08:47:14
%% Version: 
%% Last-Updated: 03/18/2010 08:51:10
%%           By: Luo Qiang
%%     Update #: 5
%% Keywords: 
%% 
%%% Commentary: 
%% 
%%% Change log:
%% 
%%% Code:
function sm=toMatlab(data)
data(:,1)+=1;
data(:,2)+=1;
sm=spconvert(data);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% toMatlab.m ends here
