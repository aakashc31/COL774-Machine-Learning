load q3x.dat; load q3y.dat;
m = length(q3x);
x = [ones(1,m) ; q3x']'; y = q3y;
theta = (inv(x'*x)) * (x' * y)
yn = x*theta;
q1b_plot(x,y,theta);
