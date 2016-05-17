clear;
load q1x.dat; load q1y.dat;
q1x = q1x - mean(q1x(:));
q1x = q1x / std(q1x(:));
m = length(q1x);
x = [ones(1,m) ; q1x']'; y = q1y;% x and y are what they should be. x has a column which has all 1s
eta = 0.01;