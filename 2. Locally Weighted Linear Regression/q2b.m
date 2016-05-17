function wightedRegression(tau)
    load q3x.dat; load q3y.dat;
    m = length(q3x);
    x = [ones(1,m) ; q3x']'; y = q3y;
    xn = (-6:0.1:12.5)';
    for i = 1:length(xn)
        yn(i,1) = predictUsingWeights(x,y,tau,xn(i,1));
    end
    figure;
    hold on;
    plot(x(:,2),y ,'xr');
    plot(xn, yn, '--.b');
end

function yn = predictUsingWeights(x,y,tau,x0)
    for i = 1:length(x)
        w(i,i) = exp(-((x0-x(i,2))^2)/(2*tau*tau));
    end
    theta = (inv(x' * w * x)) * x' * w * y;
    yn = theta' * [1 ; x0];
end

